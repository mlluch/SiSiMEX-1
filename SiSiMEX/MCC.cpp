#include "MCC.h"
#include "Globals.h"
#include "Packets.h"
#include "Log.h"
#include <iostream>

enum State
{
	ST_INIT,
	ST_REGISTERING,
	ST_IDLE,
	ST_UNREGISTERING,
	ST_FINISHED
};

MCC::MCC(Node *node, uint16_t itemId) :
	Agent(node),
	_itemId(itemId)
{
	setState(ST_INIT);
}


MCC::~MCC()
{
}

void MCC::update()
{
	switch (state())
	{
	case ST_INIT:
		if (registerIntoYellowPages()) {
			setState(ST_REGISTERING);
		} else {
			setState(ST_FINISHED);
		}
		break;
	case ST_REGISTERING:
		waitForRegisterAck();
		break;
	case ST_IDLE:
		break;
	case ST_UNREGISTERING:
		waitForUnregisterAck();
		break;
	case ST_FINISHED:
		finish();
	}
}

void MCC::finalize()
{
	unregisterFromYellowPages();
	setState(ST_UNREGISTERING);
}


bool MCC::registerIntoYellowPages()
{
	// Create listen socket
	TCPSocketPtr agentSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
	if (agentSocket == nullptr) {
		eLog << "SocketUtil::CreateTCPSocket() failed";
		return false;
	}

	// Connect to Yellow Pages
	SocketAddress yellowPagesAddress("localhost:8000");
	int res = agentSocket->Connect(yellowPagesAddress);
	if (res != NO_ERROR) {
		eLog << "TCPSocket::Connect() failed";
		return false;
	}

	// Add socket to the network manager
	g_NetworkManager->AddSocket(agentSocket);

	// Create message header and data
	PacketHeader packetHead;
	packetHead.packetType = PacketType::RegisterMCC;
	packetHead.srcAgentId = id();
	packetHead.dstAgentId = -1;
	PacketRegisterMCC packetData;
	packetData.itemId = _itemId;

	// Serialize message
	OutputMemoryStream stream;
	packetHead.Write(stream);
	packetData.Write(stream);

	// Append data
	agentSocket->SendPacket(stream.GetBufferPtr(), stream.GetSize());

	return true;
}

void MCC::waitForRegisterAck()
{
	// Nothing to do
}

void MCC::unregisterFromYellowPages()
{
	// Create listen socket
	TCPSocketPtr agentSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
	if (agentSocket == nullptr) {
		eLog << "SocketUtil::CreateTCPSocket() failed";
		return;
	}

	// Connect to Yellow Pages
	SocketAddress yellowPagesAddress("localhost:8000");
	int res = agentSocket->Connect(yellowPagesAddress);
	if (res != NO_ERROR) {
		eLog << "TCPSocket::Connect() failed";
		return;
	}

	// Add socket to the network manager
	g_NetworkManager->AddSocket(agentSocket);

	// Create message
	PacketHeader packetHead;
	packetHead.packetType = PacketType::UnregisterMCC;
	packetHead.srcAgentId = id();
	packetHead.dstAgentId = -1;
	PacketUnregisterMCC packetData;
	packetData.itemId = _itemId;

	// Serialize message
	OutputMemoryStream stream;
	packetHead.Write(stream);
	packetData.Write(stream);

	// Append data
	agentSocket->SendPacket(stream.GetBufferPtr(), stream.GetSize());

	setState(ST_UNREGISTERING);
}

void MCC::waitForUnregisterAck()
{
	// Nothing to do
}


void MCC::OnPacketReceived(TCPSocketPtr socket, PacketType packetType, InputMemoryStream &stream)
{
	if (state() == ST_REGISTERING && packetType == PacketType::RegisterMCCAck) {
		setState(ST_IDLE);
		socket->Disconnect();
	}
	if (state() == ST_UNREGISTERING && packetType == PacketType::UnregisterMCCAck) {
		setState(ST_FINISHED);
		socket->Disconnect();
	}
}
