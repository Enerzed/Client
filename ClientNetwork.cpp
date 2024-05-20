#include "ClientNetwork.hpp"

ClientNetwork::ClientNetwork()
{
    systemMessages.push_back("Client has started\n");
    std::cout << systemMessages.back() << std::endl;
    receptionThread = new std::thread(&ClientNetwork::ReceivePackets, this, &socket);
}

bool ClientNetwork::Connect(const char* address, unsigned short port)
{
    if (socket.connect(address, port, sf::milliseconds(100)) != sf::Socket::Done)
    {
        systemMessages.push_back("Could not connect to the server with address ");
        systemMessages.back().append(address).append(":").append(std::to_string(port)).append("\n");
        std::cout << systemMessages.back() << std::endl;

        return false;
    }
    else
    {
        isConnected = true;

        systemMessages.push_back("Connected to the server with address ");
        systemMessages.back().append(address).append(":").append(std::to_string(port)).append("\n");
        std::cout << systemMessages.back() << std::endl;

        return true;
    }
}

void ClientNetwork::Disconnect()
{
    systemMessages.push_back("Disconnected from the server\n");
    std::cout << systemMessages.back() << std::endl;

    isConnected = false;
    socket.disconnect();
}

void ClientNetwork::ReceivePackets(sf::TcpSocket* socket)
{
    sf::Packet packet;
    unsigned short type;
    std::string name;
    std::string message;
    std::string address;
    unsigned short port;

    while (true)
    {
        if (socket->receive(packet) == sf::Socket::Done)
        {
            packets.push_back(packet);
            packet >> type;
            switch (type)
            {
            case PACKET_TYPE_MESSAGE:
            {
                packet >> name >> message >> address >> port;
                systemMessages.push_back("From ");
                systemMessages.back().append(name).append(" with address ").append(address).append(":").append(std::to_string(port)).append(" - ").append(message).append("\n");

                std::cout << systemMessages.back() << std::endl;

                systemMessages.pop_back();
                break;
            }
            default:
            {
                packets.pop_back();
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