#pragma once
#include "Agent.h"

// Forward declaration
class UCP;
using UCPPtr = std::shared_ptr<UCP>;

class MCP :
	public Agent
{
public:

	MCP(Node *node, uint16_t itemId);
	~MCP();

	void update() override;
	void finalize() override;

	void OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream) override;

	bool resultsAvailable();

private:

	bool queryMCCsForItem(int itemId);

	void createChildUCP();

	void destroyChildUCP();

	uint16_t _itemId;

	int _mccRegisterIndex; /**< Iterator through _mccRegisters. */
	std::vector<MCCRegister> _mccRegisters; /**< MCCs returned by the YP. */

	UCPPtr _ucp; /**< Child UCP. */
};

