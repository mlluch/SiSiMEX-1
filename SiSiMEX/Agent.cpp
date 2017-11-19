#include "Agent.h"

uint16_t g_IdCounter = 1;

Agent::Agent(Node *node) :
	_finished(false),
	_node(node),
	_id(g_IdCounter++)
{
}


Agent::~Agent()
{
}
