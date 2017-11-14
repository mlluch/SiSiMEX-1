#include "AgentManager.h"


AgentManager::AgentManager()
{
}

AgentManager::~AgentManager()
{
}

void AgentManager::addAgent(AgentPtr agent)
{
	_agents.push_back(agent);
}

AgentPtr AgentManager::getAgent(int agentId)
{
	// Agent search
	for (auto agent : _agents) {
		if (agent->id() == agentId) {
			return agent;
		}
	}

	return nullptr;
}

bool AgentManager::empty() const
{
	return _agents.empty();
}

void AgentManager::update()
{
	// Track alive agents
	std::vector<AgentPtr> agentsAlive;

	// Update all agents
	for (auto agent : _agents)
	{
		agent->update();

		// Keep track of alive agents
		if (!agent->finished()) {
			agentsAlive.push_back(agent);
		}
	}

	// Remove finished agents
	_agents.swap(agentsAlive);
}

void AgentManager::finalize()
{
	// Update all agents
	for (auto agent : _agents) {
		agent->finalize();
	}
}
