#include "ClientNetwork.hpp"

ClientNetwork::ClientNetwork(std::string name)
{
    this->name = name;
    systemMessage = "Client has started\n";
    std::cout << systemMessage << std::endl;
    receptionThread = new std::thread(&ClientNetwork::ReceivePackets, this, &socket);
}

void ClientNetwork::Connect(const char* address, unsigned short port)
{
    if (socket.connect(address, port) != sf::Socket::Done)
    {
        systemMessage = "Could not connect to the server with address ";
        systemMessage.append(address).append(":").append(std::to_string(port)).append("\n");
        isSystemMessage = true;

        std::cout << systemMessage << std::endl;
    }
    else
    {
        isConnected = true;

        systemMessage = "Connected to the server with address ";
        systemMessage.append(address).append(":").append(std::to_string(port)).append("\n");
        isSystemMessage = true;

        std::cout << systemMessage << std::endl;
    }
}

void ClientNetwork::Disconnect()
{
    systemMessage = "Disconnected from the server\n";
    isSystemMessage = true;
    std::cout << systemMessage << std::endl;
    socket.disconnect();
}

void ClientNetwork::ReceivePackets(sf::TcpSocket* socket)
{
    while (true)
    {
        if (socket->receive(packet) == sf::Socket::Done)
        {
            lastPacket = packet;

            std::string receivedString;
            std::string receivedName;
            std::string senderAddress;
            unsigned short senderPort;

            packet >> receivedString >> receivedName >> senderAddress >> senderPort;
            systemMessage = "From ";
            systemMessage.assign(receivedName).append(" with address ").append(senderAddress).append(":").append(std::to_string(senderPort)).append(" - ").append(receivedString).append("\n");

            std::cout << systemMessage << std::endl;
            // Debug
            // isSystemMessage = true;
            isPacketRecieved = true;
        }
    }
}

void ClientNetwork::SendPacket(sf::Packet& replyPacket)
{
    if (replyPacket.getDataSize() > 0 && socket.send(replyPacket) != sf::Socket::Done)
    {
        systemMessage = "Could not sent packet\n";
        isSystemMessage = true;
        std::cout << systemMessage << std::endl;
    }
    else
        std::cout << "Sent packet" << std::endl;
}

void ClientNetwork::ManagePackets(std::string message)
{
    if (isConnected)
    {
        if (message.length() < 1)
            return;

        sf::Packet replyPacket;
        replyPacket << message << name;

        SendPacket(replyPacket);
    }
    else
    {
        systemMessage = "Server connection is not present, message is not sent\n";
        isSystemMessage = true;
        std::cout << systemMessage << std::endl;
    }
}

void ClientNetwork::Run(std::string input)
{
    ManagePackets(input);
}

void ClientNetwork::SetIsPacketRecieved(bool newIsPacketRecieved)
{
    isPacketRecieved = newIsPacketRecieved;
}

bool ClientNetwork::GetIsPacketRecieved()
{
    return isPacketRecieved;
}

sf::Packet ClientNetwork::GetLastPacket()
{
    return lastPacket;
}

bool ClientNetwork::GetIsSystemMessage()
{
    return isSystemMessage;
}

std::string ClientNetwork::GetSystemMessage()
{
    return systemMessage;
}

void ClientNetwork::SetIsSystemMessage(bool newIsSystemMessage)
{
    isSystemMessage = newIsSystemMessage;
}

std::string ClientNetwork::GetName()
{
    return name;
}