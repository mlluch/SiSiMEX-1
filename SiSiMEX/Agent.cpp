#include "Agent.h"

uint16_t g_IdCounter = 1;

Agent::Agent() :
	_finished(false),
	_id(g_IdCounter++)
{
}


Agent::~Agent()
{
}
