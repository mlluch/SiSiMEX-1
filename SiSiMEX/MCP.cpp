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
	ST_NEGOTIATION_FINISHED
};

MCP::MCP(Node *node, uint16_t itemId) :
	Agent(node),
	_itemId(itemId),
	_negotiationAgreement(false)
{
	setState(ST_INIT);
}

MCP::~MCP()
{
}

void MCP::update()
{
	switch (state())
	{
	case ST_INIT:
		queryMCCsForItem(_itemId);
		setState(ST_REQUESTING_MCCs);
		break;
	case ST_ITERATING_OVER_MCCs:
		if (_mccRegisterIndex < _mccRegisters.size())
		{
			const AgentLocation &mccRegister(_mccRegisters[_mccRegisterIndex]);
			sendNegotiationRequest(mccRegister);
			setState(ST_STARTING_NEGOTIATION);
		}
		else
		{
			setState(ST_NEGOTIATION_FINISHED);
		}
		break;
	case ST_NEGOTIATING:
		if (_ucp->negotiationFinished()) {
			if (_ucp->negotiationAgreement()) {
				_negotiationAgreement = true;
				setState(ST_NEGOTIATION_FINISHED);
			}
			else {
				_mccRegisterIndex++;
				setState(ST_ITERATING_OVER_MCCs);
			}
			destroyChildUCP();
		}
		break;
	default:;
	}
}

void MCP::finalize()
{
	destroyChildUCP();
	finish();
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
		PacketStartNegotiationResponse iPacketData;
		iPacketData.Read(stream);

		// Create UCP to achieve the constraint item
		AgentLocation uccLoc;
		uccLoc.hostIP = socket->RemoteAddress().GetIPString();
		uccLoc.hostPort = LISTEN_PORT_AGENTS;
		uccLoc.agentId = iPacketData.uccAgentId;
		createChildUCP(uccLoc);

		// Wait for UCP results
		setState(ST_NEGOTIATING);

		socket->Disconnect();
	}
}

bool MCP::negotiationFinished() const
{
	return state() == ST_NEGOTIATION_FINISHED;
}

bool MCP::negotiationAgreement() const
{
	return _negotiationAgreement;
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

bool MCP::sendNegotiationRequest(const AgentLocation &mccRegister)
{
	const std::string &hostIP = mccRegister.hostIP;
	const uint16_t hostPort = mccRegister.hostPort;
	const uint16_t agentId = mccRegister.agentId;

	PacketHeader packetHead;
	packetHead.packetType = PacketType::StartNegotiation;
	packetHead.srcAgentId = id();
	packetHead.dstAgentId = mccRegister.agentId;

	OutputMemoryStream stream;
	packetHead.Write(stream);

	return sendPacketToHost(hostIP, hostPort, stream);
}

void MCP::createChildUCP(const AgentLocation &uccLoc)
{
	destroyChildUCP();
	_ucp.reset(new UCP(node(), _itemId, uccLoc));
	g_AgentContainer->addAgent(_ucp);
}

void MCP::destroyChildUCP()
{
	// Make sure we call this function when the agent has
	// finished so it is destroyed by the AgentContainer
	if (_ucp.get()) {
		_ucp->finalize();
		_ucp.reset();
	}
}
