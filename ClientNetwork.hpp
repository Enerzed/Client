// Класс для соединения и обработки сообщений
#pragma once

#define PACKET_TYPE_MESSAGE 1
#define PACKET_TYPE_INITIAL_DATA 2
#define PACKET_TYPE_CLIENT_CONNECTED 3
#define PACKET_TYPE_CLIENT_DISCONNECTED 4
#define PACKET_TYPE_SERVER_DOWN 5
#define PACKET_TYPE_RSA_KEY 6
#define PACKET_TYPE_AES_KEY 7
#define PACKET_TYPE_AES_IV 8

#include <iostream>
#include <thread>
#include <chrono>

#include <SFML/Network.hpp>
#include "Encryption.hpp"


class ClientNetwork
{
private:
    sf::TcpSocket socket;                               // Сокет для подключения к серверу
    std::vector<sf::Packet> packets;                    // Пакеты
    std::vector<std::string> systemMessages;            // Различные сообщения
    std::thread* receptionThread;                       // Отдельный поток для получения сообщений от кого-либо
    //std::string name;                                 // Имя клиента
    bool isConnected = false;                           // Флаг присутствия подключения

    RSAEncryption rsa;
    AESEncryption aes;

public:
    ClientNetwork();
    bool Connect(const char*, unsigned short);          // Подключаемся к серверу
    void Disconnect();                                  // Отключаемся от сервера
    void ReceivePackets(sf::TcpSocket*);                // Получаем пакеты
    void SendPacket(sf::Packet&);                       // Отправляем пакеты
    void ManagePackets(unsigned short, std::string);    // Формируем пакеты
    void Run(unsigned short, std::string);              // Очередная итерация
    void ClearPackets();
    void ClearSystemMessages();
    // Setters
    
    // Getters
    std::vector<std::string> GetSystemMessages();
    std::vector<sf::Packet> GetPackets();
    bool GetIsConnected();
};