#include "YellowPages.h"
#include "Globals.h"
#include "Packets.h"
#include "Agent.h"
#include <Net.h>
#include <iostream>

#if 0

#endif

YellowPages::YellowPages() :
	_networkManager(this)
{
	g_NetworkManager = &_networkManager;
}


YellowPages::~YellowPages()
{
	g_NetworkManager = nullptr;
}


bool YellowPages::initialize()
{
	std::cout << "-------------------------------------------------------------" << std::endl;
	std::cout << "                   SiSiMEX: Yellow Pages                     " << std::endl;
	std::cout << "-------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

	// Initialize sockets library
	int res;
	if (!SocketUtil::StaticInit()) {
		std::cout << "SocketUtil::StaticInit() failed" << std::endl;
		return false;
	}

	// Create listen socket
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
	if (listenSocket == nullptr) {
		std::cerr << "SocketUtil::CreateTCPSocket() failed" << std::endl;
		return false;
	}
	std::cout << " - Server Listen socket created" << std::endl;

	// Bind
	const int port = LISTEN_PORT_YP;
	SocketAddress bindAddress(port); // localhost:LISTEN_PORT_YP
	listenSocket->SetReuseAddress(true);
	res = listenSocket->Bind(bindAddress);
	if (res != NO_ERROR) { return false; }
	std::cout << " - Socket Bind to interface 127.0.0.1:" << LISTEN_PORT_YP << std::endl;

	// Listen mode
	res = listenSocket->Listen();
	if (res != NO_ERROR) { return false; }
	std::cout << " - Socket entered in Listen state..." << std::endl;

	// Add the socket to the manager
	_networkManager.AddSocket(listenSocket);

	return true;
}

void YellowPages::update()
{
	const int timeoutMillis = 100;
	_networkManager.HandleIncomingPackets(timeoutMillis);

	_networkManager.HandleOutgoingPackets();
}

void YellowPages::finalize()
{
	SocketUtil::CleanUp();
}

void YellowPages::OnAccepted(TCPSocketPtr socket)
{
	//std::cout << "OnAccepted" << std::endl;
}

void YellowPages::OnPacketReceived(TCPSocketPtr socket, InputMemoryStream &stream)
{
	std::cout << "OnPacketReceived: " << std::endl;

	// Read packet header
	PacketHeader inPacketHead;
	inPacketHead.Read(stream);

	if (inPacketHead.packetType == PacketType::RegisterMCC)
	{
		std::cout << "PacketType::RegisterMCC" << std::endl;

		// Read the packet
		PacketRegisterMCC inPacketData;
		inPacketData.Read(stream);

		// Register the MCC into the yellow pages
		MCCAddress mccAddress;
		mccAddress.hostIP = socket->RemoteAddress().GetIPString();
		mccAddress.hostPort = LISTEN_PORT_AGENTS;
		mccAddress.agentId = inPacketHead.srcAgentId;
		_mccByItem[inPacketData.itemId].push_back(mccAddress);

		// Host address
		std::string hostAddress = socket->RemoteAddress().GetString();

		std::cout << " - MCC Agent ID: " << inPacketHead.srcAgentId << std::endl;
		std::cout << " - Contributed Item ID: " << inPacketData.itemId << std::endl;
		std::cout << " - Remote host address: " << hostAddress << std::endl;

		// Send RegisterMCCAck packet
		OutputMemoryStream outStream;
		PacketHeader outPacket;
		outPacket.packetType = PacketType::RegisterMCCAck;
		outPacket.dstAgentId = inPacketHead.srcAgentId;
		outPacket.Write(outStream);
		socket->SendPacket(outStream.GetBufferPtr(), outStream.GetSize());
	}
	else if (inPacketHead.packetType == PacketType::UnregisterMCC)
	{
		std::cout << "PacketType::UnregisterMCC" << std::endl;

		// Read the packet
		PacketUnregisterMCC inPacketData;
		inPacketData.Read(stream);

		// Unregister the MCC from the yellow pages
		std::list<MCCAddress> &mccs(_mccByItem[inPacketData.itemId]);
		for (auto it = mccs.begin(); it != mccs.end();) {
			if (it->agentId == inPacketHead.srcAgentId) {
				auto oldIt = it++;
				mccs.erase(oldIt);
				break;
			} else {
				++it;
			}
		}

		std::cout << " - MCC Agent ID: " << inPacketHead.srcAgentId << std::endl;
		std::cout << " - Contributed Item ID: " << inPacketData.itemId << std::endl;

		// Send RegisterMCCAck packet
		OutputMemoryStream outStream;
		PacketHeader outPacket;
		outPacket.packetType = PacketType::UnregisterMCCAck;
		outPacket.dstAgentId = inPacketHead.srcAgentId;
		outPacket.Write(outStream);
		socket->SendPacket(outStream.GetBufferPtr(), outStream.GetSize());
	}
	else if (inPacketHead.packetType == PacketType::QueryMCCsForItem)
	{
		std::cout << "PacketType::QueryMCCsForItem" << std::endl;
		
		// Read packet
		PacketQueryMCCsForItem inPacketData;
		inPacketData.Read(stream);

		// Response packet
		PacketReturnMCCsForItem outPacketData;

		// Obtain the MCCAddresses
		auto itemId = inPacketData.itemId;
		auto &mccAddressList = _mccByItem[itemId];
		for (auto &mccAddress : mccAddressList) {
			outPacketData.mccAddresses.push_back(mccAddress);
		}

		// Send response packet
		OutputMemoryStream outStream;
		PacketHeader outPacketHead;
		outPacketHead.packetType = PacketType::ReturnMCCsForItem;
		outPacketHead.dstAgentId = inPacketHead.srcAgentId;
		outPacketHead.Write(outStream);
		outPacketData.Write(outStream);
		socket->SendPacket(outStream.GetBufferPtr(), outStream.GetSize());
	}
}

void YellowPages::OnDisconnected(TCPSocketPtr socket)
{
	//std::cout << "OnDisconnected" << std::endl;
}
