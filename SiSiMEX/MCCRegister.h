#pragma once
#include "Globals.h"
#include <Net.h>

/**
* Basic information about MCCs.
* It contains the minimum information to find an MCC agent in
* the network (complete ip address + port + agent identifier)
*/
class MCCRegister
{
public:

	std::string hostIP; /**< IP address where the agent is. */
	uint16_t hostPort; /**< Listen port of this host. */
	uint16_t agentId; /**< Identifier of the MCC agent within the host. */
	uint16_t constraintItemId; /**< Optional item ID required as constraint. */

	// The constructor sets a NULL constraint by default
	MCCRegister() : constraintItemId(NULL_ITEM_ID) { }

	void Read(InputMemoryStream &stream) {
		stream.Read(hostIP);
		stream.Read(hostPort);
		stream.Read(agentId);
		stream.Read(constraintItemId);
	}

	void Write(OutputMemoryStream &stream) {
		stream.Write(hostIP);
		stream.Write(hostPort);
		stream.Write(agentId);
		stream.Write(constraintItemId);
	}
};
