#include "MultiAgentApplication.h"
#include "Globals.h"
#include "Packets.h"
#include "Log.h"
#include "MCC.h"
#include "MCP.h"
#include <Windows.h>
#include <iostream>
#include <sstream>


MultiAgentApplication::MultiAgentApplication() :
	_running(true),
	_networkManager(this)
{
	g_AgentContainer = &_agentContainer;
	g_NetworkManager = &_networkManager;
}

MultiAgentApplication::~MultiAgentApplication()
{
	g_NetworkManager = nullptr;
}

bool MultiAgentApplication::initialize()
{
	iLog << "-------------------------------------------------------------";
	iLog << "                    SiSiMEX Multi-Agents                     ";
	iLog << "-------------------------------------------------------------";
	iLog << "";

	// Initialize network
	SocketUtil::StaticInit();

	// Initialize nodes
	for (int i = 0; i < MAX_NODES; ++i)
	{
		// Create and intialize nodes
		NodePtr node = std::make_shared<Node>();
		node->initialize();
		_nodes.push_back(node);
	}

	// Create MCP agents for each node
	for (int i = 0; i < MAX_NODES; ++i)
	{
		// Spawn MCC (MultiCastContributors) one for each spare item
		NodePtr node = _nodes[i];
		ItemList spareItems = node->itemList().getSpareItems();
		for (auto item : spareItems.items()) {
			AgentPtr agentPtr(new MCC(node.get(), item.id()));
			g_AgentContainer->addAgent(agentPtr);
		}
	}

	return true;
}

void MultiAgentApplication::update()
{
	const int timeoutMillis = 100;
	_networkManager.HandleIncomingPackets(timeoutMillis);

	_agentContainer.update();

	_networkManager.HandleOutgoingPackets();
}

void MultiAgentApplication::finalize()
{
	_nodes.clear();

	_agentContainer.finalize();

	// Extra updates to wait agents to finish
	for (auto i = 0; i < 10 && !_agentContainer.empty(); ++i) {
		update(); 
		Sleep(17);
	}

	_networkManager.Finalize(); // To close sockets

	SocketUtil::CleanUp();
}

// User actions
void MultiAgentApplication::listLocalNodes()
{
	std::ostringstream oss;
	for (auto i = 0; i < _nodes.size(); ++i) {
		oss << i << " ";
	}
	iLog << "List of Local Nodes: " << oss.str().c_str();
}

void MultiAgentApplication::inspectLocalNode(int nodeId)
{
	iLog << "Inpect local node " << nodeId;
	if (nodeId >= 0 && nodeId < _nodes.size()) {
		NodePtr node = _nodes[nodeId];

		{ std::ostringstream oss;
		for (auto item : node->itemList().items()) {
			oss << item.id() << " ";
		}
		iLog << " - Items: " << oss.str().c_str(); }

		{ std::ostringstream oss;
		ItemList spareItems = node->itemList().getSpareItems();
		for (auto item : spareItems.items()) {
			oss << item.id() << " ";
		}
		iLog << " - Spare Items: " << oss.str().c_str(); }
	} else {
		wLog << "Could not find node with ID " << nodeId;
	}
}

void MultiAgentApplication::spawnMCP(int nodeId, int itemId)
{
	iLog << "Spawn MCP for node " << nodeId << " petitioning item " << itemId;
	if (nodeId >= 0 && nodeId < _nodes.size()) {
		NodePtr node = _nodes[nodeId];
		AgentPtr agent(new MCP(node.get(), itemId));
		g_AgentContainer->addAgent(agent);
	}
	else {
		wLog << "Could not find node with ID " << nodeId;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// Network manager delegate
//////////////////////////////////////////////////////////////////////////////////////////

void MultiAgentApplication::OnAccepted(TCPSocketPtr socket)
{
	//iLog << "OnAccepted";
}

void MultiAgentApplication::OnPacketReceived(TCPSocketPtr socket, InputMemoryStream &stream)
{
	//iLog << "OnPacketReceived";
	
	PacketHeader packetHead;
	packetHead.Read(stream);

	// Get the agent
	auto agentPtr = _agentContainer.getAgent(packetHead.dstAgentId);
	if (agentPtr != nullptr)
	{
		agentPtr->OnPacketReceived(socket, packetHead, stream);
	}
	else
	{
		eLog << "Couldn't find agent: " << packetHead.dstAgentId;
	}
}

void MultiAgentApplication::OnDisconnected(TCPSocketPtr socket)
{
	//iLog << "OnDisconnected";
}
