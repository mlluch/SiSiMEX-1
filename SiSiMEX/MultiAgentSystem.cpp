#include "MultiAgentSystem.h"
#include "Globals.h"
#include "Packets.h"
#include <Windows.h>
#include <iostream>



MultiAgentSystem::MultiAgentSystem() :
	_running(true),
	_mustGetContributorsForItem(false),
	_itemIndex(0),
	_networkManager(this)
{
	g_AgentManager = &_agentManager;
	g_NetworkManager = &_networkManager;
}

MultiAgentSystem::~MultiAgentSystem()
{
	g_NetworkManager = nullptr;
}

//bool MultiAgentSystem::execute()
//{
//	initialize();
//
//	std::thread t(&MultiAgentSystem::guiLoop, this);
//
//	while (_running)
//	{
//		update();
//	}
//
//	t.join();
//
//	finalize();
//
//	return true;
//}

bool MultiAgentSystem::initialize()
{
	std::cout << "-------------------------------------------------------------" << std::endl;
	std::cout << "                    SiSiMEX Multi-Agents                     " << std::endl;
	std::cout << "-------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

	// Initialize network
	SocketUtil::StaticInit();

	// Initialize nodes
	for (int i = 0; i < MAX_NODES; ++i)
	{	
		NodePtr node = std::make_shared<Node>();
		node->initialize();
		_nodes.push_back(node);
	}

	return true;
}

void MultiAgentSystem::update()
{
	const int timeoutMillis = 100;
	_networkManager.HandleIncomingPackets(timeoutMillis);

	//{
	//	mutex_lock lock(_mutex);
	//	if (_mustGetContributorsForItem) {
	//		sendMsgGetContributors(_itemIndex);
	//		_mustGetContributorsForItem = false;
	//	}
	//}

	_agentManager.update();

	_networkManager.HandleOutgoingPackets();
}

void MultiAgentSystem::finalize()
{
	_nodes.clear();

	_agentManager.finalize();

	// Extra updates to wait agents to finish
	while (!_agentManager.empty()) {
		update(); 
		Sleep(17);
	}

	_networkManager.Finalize(); // To close sockets

	SocketUtil::CleanUp();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Network manager delegate
//////////////////////////////////////////////////////////////////////////////////////////

void MultiAgentSystem::OnAccepted(TCPSocketPtr socket)
{
	//std::cout << "OnAccepted" << std::endl;
}

void MultiAgentSystem::OnPacketReceived(TCPSocketPtr socket, InputMemoryStream &stream)
{
	//std::cout << "OnPacketReceived" << std::endl;
	
	PacketHeader packetHead;
	packetHead.Read(stream);

	// Get the agent
	auto agentPtr = _agentManager.getAgent(packetHead.dstAgentId);
	if (agentPtr != nullptr)
	{
		agentPtr->OnPacketReceived(socket, packetHead.packetType, stream);
	}
	else
	{
		std::cerr << "Couldn't find agent: " << packetHead.dstAgentId << std::endl;
	}
}

void MultiAgentSystem::OnDisconnected(TCPSocketPtr socket)
{
	//std::cout << "OnDisconnected" << std::endl;
}


//////////////////////////////////////////////////////////////////////////////////////////
// GUI Loop
//////////////////////////////////////////////////////////////////////////////////////////

//void MultiAgentSystem::guiLoop()
//{
//
//	while (_running)
//	{
//		std::cout << std::endl;
//		std::cout << "-------------------------------------------------------------" << std::endl;
//		std::cout << "Multi Agent System menu" << std::endl;
//		std::cout << std::endl;
//		std::cout << "1) List local nodes" << std::endl;
//		std::cout << "2) Inspect local node" << std::endl;
//		std::cout << "3) Get contributors for item" << std::endl;
//		std::cout << "4) Quit" << std::endl;
//		std::cout << "Select an option: " << std::flush;
//
//		int op;
//		std::cin >> op;
//		std::cin.ignore();
//		std::cout << std::endl;
//
//		if (op == 1)
//		{
//			std::cout << "---> List local nodes" << std::endl;
//
//			std::cout << " - Node indices: " << std::flush;
//			for (int i = 0; i < _nodes.size(); ++i) {
//				std::cout << i << " " << std::flush;
//			}
//			std::cout << std::endl;
//		}
//		else if (op == 2)
//		{
//			std::cout << "---> Inspect local node" << std::endl;
//			std::cout << "Type a node index: " << std::flush;
//			int id;
//			std::cin >> id;
//			std::cin.ignore();
//
//			if (id < _nodes.size()) {
//				auto node = _nodes[id];
//				std::cout << " - Node " << id << " has the following items: " << std::flush;
//
//				for (auto item : node->itemList().items()) {
//					std::cout << item.id() << " " << std::flush;
//				}
//				std::cout << std::endl;
//
//				std::cout << " - And these are their spare items: " << std::flush;
//				ItemList spareItems = node->itemList().getSpareItems();
//				for (auto item : spareItems.items()) {
//					std::cout << item.id() << " " << std::flush;
//				}
//				std::cout << std::endl;
//			}
//		}
//		else if (op == 3)
//		{
//			std::cout << "---> Get contributors for item" << std::endl;
//			std::cout << "Type an item id: " << std::flush;
//			int id;
//			std::cin >> id;
//			std::cin.ignore();
//
//			mutex_lock lock(_mutex);
//
//			_mustGetContributorsForItem = true;
//		}
//		else if (op == 4)
//		{
//			_running = false;
//		}
//		else
//		{
//			std::cout << "Unrecongnised option" << std::endl;
//		}
//	}
//}
