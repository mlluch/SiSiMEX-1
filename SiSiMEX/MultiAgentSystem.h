#pragma once

#include "Node.h"
#include "AgentManager.h"
#include <Net.h>
#include <thread>
#include <mutex>
#include <vector>

class MultiAgentSystem : public TCPNetworkManagerDelegate
{
public:
	MultiAgentSystem();
	~MultiAgentSystem();

	//bool execute();

	// Main methods
	bool initialize();
	void update();
	void finalize();

private:

	// TCPNetworkManagerDelegate methods
	void OnAccepted(TCPSocketPtr socket) override;
	void OnPacketReceived(TCPSocketPtr socket, InputMemoryStream &stream) override;
	void OnDisconnected(TCPSocketPtr socket) override;

	//// Send methods
	//void sendMsgGetContributors(int itemId);

	void guiLoop();

	bool _running; /**< Running flag. */

	std::vector<NodePtr> _nodes; /**< Array of nodes spawn in this host. */

	AgentManager _agentManager; /**< Manager for all agents in this host (from one or more Nodes). */

	// The _nodes attribute will be accessed from more
	// than one thread, so we will need a mutex to protect it
	// from concurrent access from several threads
	std::mutex _mutex; /**< Mutex for _localAgents. */

	// Type alias for unique_locks (typedef)
	using mutex_lock = std::unique_lock<std::mutex>;

	TCPNetworkManager _networkManager; /**< The network manager. */

	bool _mustGetContributorsForItem;
	int _itemIndex;
};
