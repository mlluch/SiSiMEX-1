#pragma once
#include "Agent.h"
#include "Item.h"

class MCP :
	public Agent
{
public:

	MCP(uint16_t itemId);
	~MCP();

	void update() override;
	void finalize() override;

	void OnPacketReceived(TCPSocketPtr socket, PacketType packetType, InputMemoryStream &stream) override;

private:

	uint16_t _itemId;
};

