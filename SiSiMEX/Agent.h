#pragma once
#include "Item.h"
#include "Packets.h"
#include <list>
#include <memory>

class Agent
{
public:

	Agent();
	virtual ~Agent();

	virtual void update() = 0;
	virtual void finalize() = 0;

	// Finish condition (if true, will be removed from AgentManager)
	bool finished() const { return _finished; }

	// State machine
	int state() const { return _state; }
	void setState(int state) { _state = state; }

	virtual void OnPacketReceived(TCPSocketPtr socket, PacketType packetType, InputMemoryStream &stream) = 0;

protected:

	// Finish condition (if true, will be removed from AgentManager)
	// Should be called by the agent itself when it finished
	void finish() { _finished = true; }

private:

	bool _finished; /**< Whether or not the agent finished. */


public:

	// TODO: Remove all...

	uint16_t id() const { return _id; }

private:

	uint16_t _id; /**< Agent identifier. */

	int _state; /**< Current state of the agent. */
};

using AgentPtr = std::shared_ptr<Agent>;
