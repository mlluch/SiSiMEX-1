#pragma once
#include "Agent.h"

// Forward declaration
class UCC;
using UCCPtr = std::shared_ptr<UCC>;

class MCC :
	public Agent
{
public:

	MCC(Node *node, uint16_t itemId);
	~MCC();

	void update() override;
	void finalize() override;

	void OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream) override;

private:

	bool registerIntoYellowPages();
	
	void unregisterFromYellowPages();

	void createChildUCC();

	void destroyChildUCC();

	uint16_t _contributedItemId; /**< The contributed item. */
	uint16_t _constraintItemId; /**< The constraint item. */

	UCCPtr _ucc; /**< Child UCC. */
};
