#pragma once
#include "Agent.h"

// Forward declaration
class MCP;
using MCPPtr = std::shared_ptr<MCP>;

class UCP :
	public Agent
{
public:

	// Constructor and destructor
	UCP(Node *node, uint16_t requestedItemId);
	~UCP();

	void update() override;
	void finalize() override;

	void OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream) override;

private:

	void requestItem();
	void createChildMCP();
	void destroyChildMCP();

	uint16_t _requestedItemId; /**< The item to request. */

	MCPPtr _mcp; /**< The child MCP. */
};

