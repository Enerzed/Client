// Класс для соединения и обработки сообщений
#pragma once

#define PACKET_TYPE_MESSAGE 1
#define PACKET_TYPE_NAME 2

#include <iostream>
#include <thread>
#include <chrono>

#include <SFML/Network.hpp>


class ClientNetwork
{
private:
    sf::TcpSocket socket;                       // Сокет для подключения к серверу
    std::vector<sf::Packet> packets;            // Пакеты
    std::vector<std::string> systemMessages;    // Различные сообщения
    std::thread* receptionThread;               // Отдельный поток для получения сообщений от кого-либо
    std::string name;                           // Имя клиента
    bool isConnected = false;                   // Флаг присутствия подключения
public:
    ClientNetwork(std::string);
    bool Connect(const char*, unsigned short);  // Подключаемся к серверу
    void Disconnect();                          // Отключаемся от сервера
    void ReceivePackets(sf::TcpSocket*);        // Получаем пакеты
    void SendPacket(sf::Packet&);               // Отправляем пакеты
    void ManagePackets(size_t, std::string);    // Формируем пакеты
    void Run(size_t type, std::string input);   // Очередная итерация
    void ClearPackets();
    void ClearSystemMessages();
    // Setters
    void SetName(std::string);
    // Getters
    std::string GetName();
    std::vector<std::string> GetSystemMessages();
    std::vector<sf::Packet> GetPackets();
    bool GetIsConnected();
};