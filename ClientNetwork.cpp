#include "ClientNetwork.hpp"

ClientNetwork::ClientNetwork()
{
    // Сообщение о старте работы клиента
    systemMessages.push_back("Client has started\n");
    std::cout << systemMessages.back() << std::endl;
    systemMessages.pop_back();
    // Генерируем ключ и вектор инициализации
    aes.GenerateRandomKey();
    aes.GenerateRandomIV();
    // Поток приема сообщений
    receptionThread = new std::thread(&ClientNetwork::ReceivePackets, this, &socket);
}

bool ClientNetwork::Connect(const char* address, unsigned short port)
{
    if (socket.connect(address, port, sf::milliseconds(100)) != sf::Socket::Done)
    {
        // Сообщение о том, что были не были подключены к серверу
        systemMessages.push_back("Could not connect to the server with address ");
        systemMessages.back().append(address).append(":").append(std::to_string(port)).append("\n");
        std::cout << systemMessages.back() << std::endl;
        return false;
    }
    else
    {
        isConnected = true;
        // Сообщение о том, что были подключены к серверу
        systemMessages.push_back("Connected to the server with address ");
        systemMessages.back().append(address).append(":").append(std::to_string(port)).append("\n");
        std::cout << systemMessages.back() << std::endl;
        systemMessages.pop_back();
        return true;
    }
}

void ClientNetwork::Disconnect()
{
    // Сообщение об отключении от сервера
    systemMessages.push_back("Disconnected from the server\n");
    std::cout << systemMessages.back() << std::endl;
    // Отключаемся
    isConnected = false;
    socket.disconnect();
}

void ClientNetwork::ReceivePackets(sf::TcpSocket* socket)
{
    sf::Packet packet;
    unsigned short type;
    std::string name;
    std::string message;

    // Бесконечный цикл для получения пакетов
    while (true)
    {
        if (socket->receive(packet) == sf::Socket::Done)
        {
            packet >> type;
            switch (type)
            {
            case PACKET_TYPE_MESSAGE:
            {
                // Получаем содержимое пакета
                packet >> name >> message;
                packet.clear();
                // Пакет с расшифрованным сообщением используется для вывода на экран
                packet << type << name << aes.Decrypt(message, aes.GetIV());
                packets.push_back(packet);
                // Выводим сообщение в консоль
                systemMessages.push_back(name);
                systemMessages.back().append(": ").append(aes.Decrypt(message, aes.GetIV())).append("\n");
                std::cout << systemMessages.back() << std::endl;
                systemMessages.pop_back();
                break;
            }
            case PACKET_TYPE_CLIENT_NAME:
            {
                packet >> name;
                systemMessages.push_back(name);
                systemMessages.back().append(" has joined the server\n");
                std::cout << systemMessages.back() << std::endl;
                break;
            }
            case PACKET_TYPE_CLIENT_CONNECTED:
            {
                // Получаем адрес и порт подключившегося клиента
                std::string address;
                unsigned short port;
                packet >> address >> port;
                // Сообщение о подключении клиента с адресом и портом
                systemMessages.push_back(address);
                systemMessages.back().append(":").append(std::to_string(port)).append(" has joined the server\n");
                std::cout << systemMessages.back() << std::endl;
                systemMessages.pop_back();
                break;
            }
            case PACKET_TYPE_CLIENT_DISCONNECTED:
            {
                // Получаем имя отключившегося клиента
                packet >> name;
                // Сообщение об отключении клиента
                systemMessages.push_back(name);
                systemMessages.back().append(" disconnected from the server\n");
                std::cout << systemMessages.back();
                break;
            }
            case PACKET_TYPE_SERVER_DOWN:
            {
                // Сообщение о отключении сервера
                systemMessages.push_back("Server is down\n");
                std::cout << systemMessages.back();
                // Отключаемся
                isConnected = false;
                socket->disconnect();
                break;
            }
            case PACKET_TYPE_RSA_KEY:
            {
                // Сообщение о получении RSA ключа
                systemMessages.push_back("Got RSA key\n");
                std::cout << systemMessages.back() << std::endl;
                systemMessages.pop_back();
                // Получаем сам ключ
                std::string rsaKey;
                packet >> rsaKey;
                // Ставим объекту RSA полученный ключ
                rsa.SetPublicKey(rsaKey);
                // Отправляем зашифрованные с помощью RSA ключ и вектор инициализации
                Run(PACKET_TYPE_AES_KEY, rsa.Encrypt(aes.GetKey()));
                Run(PACKET_TYPE_AES_IV, rsa.Encrypt(aes.GetIV()));
                Run(PACKET_TYPE_CLIENT_NAME, this->name);
                break;
            }
            }
        }
    }
}

void ClientNetwork::SendPacket(sf::Packet& packet)
{
    if (packet.getDataSize() > 0 && socket.send(packet) != sf::Socket::Done)
    {
        systemMessages.push_back("Could not sent packet\n");
        std::cout << systemMessages.back() << std::endl;
    }
}

void ClientNetwork::ManagePackets(unsigned short type, std::string message)
{
    if (isConnected)
    {
        if (message.length() < 1)
            return;
        sf::Packet packet;

        if (type == PACKET_TYPE_MESSAGE)
            packet << type << aes.Encrypt(message);
        else
            packet << type  << message;
        SendPacket(packet);
    }
    else
    {
        systemMessages.push_back("Server connection is not present, message is not sent\n");
        std::cout << systemMessages.back() << std::endl;
    }
}

void ClientNetwork::Run(unsigned short type, std::string message)
{
    ManagePackets(type, message);
}

void ClientNetwork::ClearSystemMessages()
{
    systemMessages.clear();
}

void ClientNetwork::ClearPackets()
{
    packets.clear();
}

void ClientNetwork::SetName(std::string newName)
{
    name = newName;
}

std::vector<std::string> ClientNetwork::GetSystemMessages()
{
    return systemMessages;
}

std::vector<sf::Packet> ClientNetwork::GetPackets()
{
    return packets;
}

bool ClientNetwork::GetIsConnected()
{
    return isConnected;
}