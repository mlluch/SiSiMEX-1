#include "MCP.h"
#include "UCP.h"
#include "AgentContainer.h"


enum State
{
	ST_INIT,
	ST_REQUESTING_MCCs,
	ST_ITERATING_OVER_MCCs,
	ST_STARTING_NEGOTIATION,
	ST_NEGOTIATING,
	ST_PROVIDING_RESULTS,
	ST_FINISHING
};

MCP::MCP(Node *node, uint16_t itemId) :
	Agent(node),
	_itemId(itemId)
{
	setState(ST_INIT);
}

MCP::~MCP()
{
}

void MCP::update()
{
	if (state() == ST_INIT) {
		queryMCCsForItem(_itemId);
		setState(ST_REQUESTING_MCCs);
	}
	else if (state() == ST_REQUESTING_MCCs) {
		// Handled from OnPacketReceived
	}
	else if (state() == ST_ITERATING_OVER_MCCs) {
		if (_mccRegisterIndex < _mccRegisters.size())
		{
			// TODO: Send MCC a negotiation request

			setState(ST_STARTING_NEGOTIATION);
		}
		else
		{
			setState(ST_PROVIDING_RESULTS);
		}
	}
	else if (state() == ST_NEGOTIATING) {
		// if _ucp->negotiationFinished then
		//     if _ucp->agree then
		//         collect _ucc->constraint
		//         setState(ST_UNREGISTERING);
		//     else
		//         setState(ST_IDLE);
		//     endif
		//     _ucc.reset();
		// endif
	}
	else if (state() == ST_PROVIDING_RESULTS) {
		// TODO: Finish when the results are provided
	}
	else if (state() == ST_FINISHING) {
		finish();
	}
}

void MCP::finalize()
{

}

bool MCP::queryMCCsForItem(int itemId)
{
	// Create message header and data
	PacketHeader packetHead;
	packetHead.packetType = PacketType::QueryMCCsForItem;
	packetHead.srcAgentId = id();
	packetHead.dstAgentId = -1;
	PacketQueryMCCsForItem packetData;
	packetData.itemId = _itemId;

	// Serialize message
	OutputMemoryStream stream;
	packetHead.Write(stream);
	packetData.Write(stream);

	// 1) Ask YP for MCC hosting the item 'itemId'
	return sendPacketToYellowPages(stream);
}


void MCP::createChildUCP()
{
	_ucp.reset(new UCP(node(), _itemId));
	g_AgentContainer->addAgent(_ucp);
}

void MCP::destroyChildUCP()
{
	// Make sure we call this function when the agent has
	// finished so it is destroyed by the AgentContainer
	_ucp.reset();
}

void MCP::OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream)
{
	const PacketType packetType = packetHeader.packetType;
	if (state() == ST_REQUESTING_MCCs && packetType == PacketType::ReturnMCCsForItem)
	{
		iLog << "OnPacketReceived PacketType::ReturnMCCsForItem " << _itemId;

		// Read the packet
		PacketReturnMCCsForItem packetData;
		packetData.Read(stream);

		for (auto &mccdata : packetData.mccAddresses)
		{
			uint16_t agentId = mccdata.agentId;
			const std::string &hostIp = mccdata.hostIP;
			uint16_t hostPort = mccdata.hostPort;
			iLog << " - MCC: " << agentId << " - host: " << hostIp << ":" << hostPort;
		}

		// 2) Collect MCC compatible from YP
		_mccRegisters.swap(packetData.mccAddresses);

		// 3) Select MCC to negociate
		_mccRegisterIndex = 0;
		setState(ST_ITERATING_OVER_MCCs);

		socket->Disconnect();
	}
	else if (state() == ST_STARTING_NEGOTIATION && packetType == PacketType::StartNegotiationResponse)
	{
		// Create UCP to achieve the constraint item
		createChildUCP();

		// Wait for UCP results
		setState(ST_NEGOTIATING);

		socket->Disconnect();
	}
}
