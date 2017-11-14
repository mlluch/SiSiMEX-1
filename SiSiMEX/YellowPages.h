#pragma once
#include "MCCAddress.h"
#include <Net.h>
#include<map>

class YellowPages : TCPNetworkManagerDelegate
{
public:
	YellowPages();
	~YellowPages();

	bool initialize();
	void update();
	void finalize();

	// TCPNetworkManagerDelegate methods
	void OnAccepted(TCPSocketPtr socket) override;
	void OnPacketReceived(TCPSocketPtr socket, InputMemoryStream &stream) override;
	void OnDisconnected(TCPSocketPtr socket) override;

private:

	TCPNetworkManager _networkManager; /**< The network manager. */

	std::map<uint16_t, std::list<MCCAddress> > _mccByItem; /**< MCCs accessed by item id. */
};

