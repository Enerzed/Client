#pragma once

#include "ClientNetwork.hpp"
#include "Interface.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>

class Client
{
private:
	unsigned short port;
public:
	void Run();
};