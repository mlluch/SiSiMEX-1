#include "UCC.h"



enum State
{
	ST_INIT
};

UCC::UCC(Node *node, uint16_t contributedItemId, uint16_t constraintItemId) :
	Agent(node),
	_contributedItemId(contributedItemId),
	_constraintItemId(constraintItemId)
{
	setState(ST_INIT);
}

UCC::~UCC()
{
}

void UCC::update()
{
	switch (state())
	{
	case ST_INIT:
		// TODO
		break;
	default:;
	}
}

void UCC::finalize()
{

}

void UCC::OnPacketReceived(TCPSocketPtr socket, const PacketHeader &packetHeader, InputMemoryStream &stream)
{

}
