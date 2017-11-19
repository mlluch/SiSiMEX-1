#include "MCP.h"


enum State
{
	ST_INIT,
	ST_REQUESTING_MCCs,
	ST_IDLE,
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
	else if (state() == ST_IDLE) {
		// By now, finish the agent after receiving the initial request...
		setState(ST_FINISHING);
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
	// TODO:
	// 1) Create a query packet
	// 2) Serialize it
	// 3) Send it to the yellow pages:
	//    return sendPacketToYellowPages(stream);

	return true; // remove after filling the function properly
}

void MCP::OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream)
{
	const PacketType packetType = packetHeader.packetType;
	if (state() == ST_REQUESTING_MCCs && packetType == PacketType::ReturnMCCsForItem)
	{
		iLog << "OnPacketReceived PacketType::ReturnMCCsForItem " << _itemId;

		// TODO:
		// 1) Deserialize the packet
		// 2) Log the received MCC registers
		// 3) Disconnect the socket
		// 4) Set the next state (IDLE by now)
	}
}
