#pragma once

#include "ClientNetwork.hpp"
#include "Interface.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>

class Client
{
private:
	sf::RenderWindow* window;			// Основное окно программы
	sf::Time* time;						// Время SFML
	ClientNetwork* network;				// Сеть клиента
	Interface* interface;				// Интерфейс
	unsigned short port;				// Порт для подключения к серверу
public:
	Client();							// Инициализация окна времени и другого
	void Run();							// Основной цикл
	void RunNetwork();					// Обработка чата
	void RunSystemMessages();			// Обработка системных сообщений
	void RunPackets();					// Обработка полученных пакетов
	void RunConnect();					// Обработка подключения к серверу
	void RunInterface();				// Обработка интерфейса
};