#include "MCP.h"



MCP::MCP(uint16_t itemId) :
	_itemId(itemId)
{
}

MCP::~MCP()
{
}
void MCP::update()
{

}

void MCP::finalize()
{

}

void MCP::OnPacketReceived(TCPSocketPtr socket, PacketType packetType, InputMemoryStream &stream)
{

}
