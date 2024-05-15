// Класс для соединения и обработки сообщений

#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#include <SFML/Network.hpp>


class ClientNetwork
{
private:
    sf::TcpSocket socket;               // Сокет для подключения к серверу
    sf::Packet lastPacket;              // Пакет с последним содержимым для вывода на экранную форму
    sf::Packet packet;                  // Пакет, в который считываются содержимое всех пакетов
    std::thread* receptionThread;       // Отдельный поток для получения сообщений от кого-либо
    std::string systemMessage;          // Различные сообщения
    std::string name;                   // Имя клиента
    bool isSystemMessage = false;       // Если хотим отправить сообщение в основную экранную форму, то меняем флаг на true
    bool isPacketRecieved = false;      // Если хотим показать сообщение от кого-либо на основной экранной форме, меняем флаг на true
    bool isConnected = false;           // Флаг присутствия подключения
public:
    ClientNetwork(std::string);
    void Connect(const char*, unsigned short);  // Подключаемся к серверу
    void Disconnect();                          // Отключаемся от сервера
    void ReceivePackets(sf::TcpSocket*);        // Получаем пакеты
    void SendPacket(sf::Packet&);               // Отправляем пакеты
    void ManagePackets(std::string);            // Формируем пакеты
    void Run(std::string input);                // Очередная итерация

    // Setters
    void SetIsPacketRecieved(bool);             
    void SetIsSystemMessage(bool);
    // Getters
    bool GetIsPacketRecieved();
    bool GetIsSystemMessage();
    std::string GetName();
    std::string GetSystemMessage();
    sf::Packet GetLastPacket();
};