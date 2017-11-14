#include "Node.h"
#include "MCC.h"
#include "Globals.h"
#include "AgentManager.h"


Node::Node()
{
}


Node::~Node()
{
}


void Node::initialize()
{
	// Initialize items
	_itemList.randomInitialization();

	// Spawn MCC (MultiCastContributors) one for each spare item
	ItemList spareItems = _itemList.getSpareItems();
	for (auto item : spareItems.items()) {
		AgentPtr agentPtr(new MCC(item.id()));
		g_AgentManager->addAgent(agentPtr);
	}
}
