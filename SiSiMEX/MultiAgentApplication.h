#pragma once

#include "Node.h"
#include "AgentContainer.h"
#include <Net.h>
#include <thread>
#include <mutex>
#include <vector>

class MultiAgentApplication : public TCPNetworkManagerDelegate
{
public:
	MultiAgentApplication();
	~MultiAgentApplication();

	// Main methods
	bool initialize();
	void update();
	void finalize();

	// User actions
	void listLocalNodes();
	void inspectLocalNode(int nodeId);
	void spawnMCP(int nodeId, int itemId);

private:

	// TCPNetworkManagerDelegate methods
	void OnAccepted(TCPSocketPtr socket) override;
	void OnPacketReceived(TCPSocketPtr socket, InputMemoryStream &stream) override;
	void OnDisconnected(TCPSocketPtr socket) override;

	bool _running; /**< Running flag. */

	std::vector<NodePtr> _nodes; /**< Array of nodes spawn in this host. */

	AgentContainer _agentContainer; /**< Manager for all agents in this host (from one or more Nodes). */

	TCPNetworkManager _networkManager; /**< The network manager. */
};
