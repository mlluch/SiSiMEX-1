#include "MCC.h"
#include "UCC.h"
#include "AgentContainer.h"

enum State
{
	ST_INIT,
	ST_REGISTERING,
	ST_IDLE,
	ST_NEGOTIATING,
	ST_UNREGISTERING,
	ST_FINISHED
};

MCC::MCC(Node *node, uint16_t itemId) :
	Agent(node),
	_contributedItemId(itemId)
{
	setState(ST_INIT);
}


MCC::~MCC()
{
}

void MCC::update()
{
	switch (state())
	{
	case ST_INIT:
		if (registerIntoYellowPages()) {
			setState(ST_REGISTERING);
		} else {
			setState(ST_FINISHED);
		}
		break;
	case ST_REGISTERING:
		// See OnPacketReceived()
		break;
	case ST_NEGOTIATING:
		// if _ucc->negotiationFinished then
		//     if _ucc->agree then
		//         collect _ucc->constraint
		//         setState(ST_UNREGISTERING);
		//     else
		//         setState(ST_WAITING_NEGOTIATION_REQUEST);
		//     endif
		//     _ucc.reset();
		// endif
		break;
	case ST_UNREGISTERING:
		// See OnPacketReceived()
		break;
	case ST_FINISHED:
		finish();
	}
}

void MCC::finalize()
{
	unregisterFromYellowPages();
	setState(ST_UNREGISTERING);
}


bool MCC::registerIntoYellowPages()
{
	// Create message header and data
	PacketHeader packetHead;
	packetHead.packetType = PacketType::RegisterMCC;
	packetHead.srcAgentId = id();
	packetHead.dstAgentId = -1;
	PacketRegisterMCC packetData;
	packetData.itemId = _contributedItemId;

	// Serialize message
	OutputMemoryStream stream;
	packetHead.Write(stream);
	packetData.Write(stream);

	return sendPacketToYellowPages(stream);
}

void MCC::unregisterFromYellowPages()
{
	// Create message
	PacketHeader packetHead;
	packetHead.packetType = PacketType::UnregisterMCC;
	packetHead.srcAgentId = id();
	packetHead.dstAgentId = -1;
	PacketUnregisterMCC packetData;
	packetData.itemId = _contributedItemId;

	// Serialize message
	OutputMemoryStream stream;
	packetHead.Write(stream);
	packetData.Write(stream);

	sendPacketToYellowPages(stream);
}

void MCC::createChildUCC()
{
	uint16_t request = _contributedItemId;
	uint16_t constraint = _constraintItemId;
	_ucc.reset(new UCC(node(), request, constraint));
	g_AgentContainer->addAgent(_ucc);
}

void MCC::destroyChildUCC()
{
	// Make sure we call this function when the agent has
	// finished so it is destroyed by the AgentContainer
	_ucc.reset();
}

void MCC::OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream)
{
	const PacketType packetType = packetHeader.packetType;
	if (state() == ST_REGISTERING && packetType == PacketType::RegisterMCCAck) {
		setState(ST_IDLE);
		socket->Disconnect();
	}
	if (state() == ST_IDLE && packetType == PacketType::StartNegotiation) {
		// Create UCC
		createChildUCC();

		// Ask UCC to start a negotiation
		// TODO: Or maybe simply because it is created it works...

		// Send negotiation response
		// TODO: PacketType::MCCNegotiationResponse
		//       include UCC agent Id _ucc->id();

		setState(ST_NEGOTIATING);
	}
	if (state() == ST_UNREGISTERING && packetType == PacketType::UnregisterMCCAck) {
		setState(ST_FINISHED);
		socket->Disconnect();
	}
}
