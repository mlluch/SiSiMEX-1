#include "UCP.h"
#include "MCP.h"
#include "AgentContainer.h"


enum State
{
	ST_INIT,
	ST_REQUESTING_ITEM,
	ST_RESOLVING_CONSTRAINT,
	ST_NEGOTIATION_FINISHED,
};

UCP::UCP(Node *node, uint16_t requestedItemId, const AgentLocation &uccLocation) :
	Agent(node),
	_requestedItemId(requestedItemId),
	_uccLocation(uccLocation),
	_negotiationAgreement(false)
{
	setState(ST_INIT);
}

UCP::~UCP()
{
}

void UCP::update()
{
	switch (state())
	{
	case ST_INIT:
		requestItem();
		setState(ST_REQUESTING_ITEM);
		break;
	case ST_RESOLVING_CONSTRAINT:
		if (_mcp->negotiationFinished()) {
			sendConstraint(_mcp->requestedItemId());
			_negotiationAgreement = _mcp->negotiationAgreement();
			setState(ST_NEGOTIATION_FINISHED);
			destroyChildMCP();
		}
		break;
	default:;
	}
}

void UCP::finalize()
{
	destroyChildMCP();
	finish();
}

void UCP::OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream)
{
	PacketType packetType = packetHeader.packetType;
	if (state() == ST_REQUESTING_ITEM && packetType == PacketType::RequestItemResponse)
	{
		PacketRequestItemResponse packetData;
		packetData.Read(stream);
		if (packetData.constraintItemId != NULL_ITEM_ID) {
			createChildMCP(packetData.constraintItemId);
			setState(ST_RESOLVING_CONSTRAINT);
		}
		else
		{
			_negotiationAgreement = true;
			setState(ST_NEGOTIATION_FINISHED);
		}
	}
}

bool UCP::negotiationFinished() const {
	return state() == ST_NEGOTIATION_FINISHED;
}

bool UCP::negotiationAgreement() const {
	return _negotiationAgreement;
}


void UCP::requestItem()
{
	// Send PacketType::RequestItem to agent
	PacketHeader oPacketHead;
	oPacketHead.packetType = PacketType::RequestItem;
	oPacketHead.srcAgentId = id();
	oPacketHead.dstAgentId = _uccLocation.agentId;
	PacketRequestItem oPacketData;
	oPacketData.requestedItemId = _requestedItemId;

	OutputMemoryStream ostream;
	oPacketHead.Write(ostream);
	oPacketData.Write(ostream);

	const std::string &ip = _uccLocation.hostIP;
	const uint16_t port = _uccLocation.hostPort;
	sendPacketToHost(ip, port, ostream);
}

void UCP::sendConstraint(uint16_t constraintItemId)
{
	// Send PacketType::RequestItem to agent
	PacketHeader oPacketHead;
	oPacketHead.packetType = PacketType::SendConstraint;
	oPacketHead.srcAgentId = id();
	oPacketHead.dstAgentId = _uccLocation.agentId;
	PacketSendConstraint oPacketData;
	oPacketData.constraintItemId = constraintItemId;

	OutputMemoryStream ostream;
	oPacketHead.Write(ostream);
	oPacketData.Write(ostream);

	const std::string &ip = _uccLocation.hostIP;
	const uint16_t port = _uccLocation.hostPort;
	sendPacketToHost(ip, port, ostream);
}

void UCP::createChildMCP(uint16_t constraintItemId)
{
	destroyChildMCP();
	_mcp.reset(new MCP(node(), constraintItemId));
	g_AgentContainer->addAgent(_mcp);
}

void UCP::destroyChildMCP()
{
	if (_mcp.get()) {
		_mcp->finalize();
		_mcp.reset();
	}
}
