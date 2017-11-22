#include "UCP.h"
#include "MCP.h"
#include "AgentContainer.h"


enum State
{
	ST_INIT,
	ST_REQUESTING_ITEM,
};

UCP::UCP(Node *node, uint16_t requestedItemId) :
	Agent(node),
	_requestedItemId(requestedItemId)
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
	default:;
	}
}

void UCP::finalize()
{

}

void UCP::OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream)
{
	PacketType packetType = packetHeader.packetType;
	if (state() == ST_REQUESTING_ITEM && packetType == PacketType::RequestItemResponse)
	{

	}
}

void UCP::requestItem()
{
	// TODO
}

void UCP::createChildMCP()
{
	// TODO
}

void UCP::destroyChildMCP()
{
	// TODO
}
