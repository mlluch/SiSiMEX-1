#pragma once
#include "Agent.h"
#include "Item.h"

class MCC :
	public Agent
{
public:

	MCC(Node *node, uint16_t itemId);
	~MCC();

	void update() override;
	void finalize() override;

	void OnPacketReceived(TCPSocketPtr socket, PacketType packetType, InputMemoryStream &stream) override;

private:

	bool registerIntoYellowPages();

	void waitForRegisterAck();
	
	void unregisterFromYellowPages();
	
	void waitForUnregisterAck();

	uint16_t _itemId; /**< The contributed item. */
};
