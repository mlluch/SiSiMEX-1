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
		// TODO MCP stuff
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
	packetHead.packetType = PacketType::QueryMCCsForItem;
	packetHead.srcAgentId = id();
	packetHead.dstAgentId = -1;
	PacketQueryMCCsForItem packetData;
	packetData.itemId = _itemId;

	// Serialize message
	OutputMemoryStream stream;
	packetHead.Write(stream);
	packetData.Write(stream);

	// Append data
	agentSocket->SendPacket(stream.GetBufferPtr(), stream.GetSize());

	return true;
}

void MCP::OnPacketReceived(TCPSocketPtr socket, PacketType packetType, InputMemoryStream &stream)
{
	if (state() == ST_REQUESTING_MCCs && packetType == PacketType::ReturnMCCsForItem)
	{
		iLog << "OnPacketReceived PacketType::ReturnMCCsForItem " << _itemId;

		// Read the packet
		PacketReturnMCCsForItem packetData;
		packetData.Read(stream);

		for (auto &mccdata : packetData.mccAddresses)
		{
			uint16_t agentId = mccdata.agentId;
			const std::string &hostIp = mccdata.hostIP;
			uint16_t hostPort = mccdata.hostPort;
			iLog << " - MCC: " << agentId << " - host: " << hostIp << ":" << hostPort;
		}

		setState(ST_IDLE);
	}
}
