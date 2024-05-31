#pragma once

#include "ClientNetwork.hpp"
#include "Interface.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>

class Client
{
private:
	sf::RenderWindow* window;
	ClientNetwork* network;
	Interface* interface;
	unsigned short port;
public:
	Client();
	void Run();
	void RunNetwork();
	void RunSystemMessages();
	void RunPackets();
	void RunConnect();
};