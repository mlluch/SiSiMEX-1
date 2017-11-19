#pragma once
#include "Agent.h"

class MCP :
	public Agent
{
public:

	MCP(Node *node, uint16_t itemId);
	~MCP();

	void update() override;
	void finalize() override;

	void OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream) override;

private:

	bool queryMCCsForItem(int itemId);

	uint16_t _itemId;
};

