#include "ClientNetwork.hpp"

ClientNetwork::ClientNetwork(std::string name)
{
    this->name = name;
    systemMessages.push_back("Client has started\n");
    std::cout << systemMessages.back() << std::endl;
    receptionThread = new std::thread(&ClientNetwork::ReceivePackets, this, &socket);
}

bool ClientNetwork::Connect(const char* address, unsigned short port)
{
    if (socket.connect(address, port) != sf::Socket::Done)
    {
        systemMessages.push_back("Could not connect to the server with address ");
        systemMessages.back().append(address).append(":").append(std::to_string(port)).append("\n");
        isSystemMessage = true;

        std::cout << systemMessages.back() << std::endl;
        return false;
    }
    else
    {
        isConnected = true;

        systemMessages.push_back("Connected to the server with address ");
        systemMessages.back().append(address).append(":").append(std::to_string(port)).append("\n");
        isSystemMessage = true;

        std::cout << systemMessages.back() << std::endl;
        return true;
    }
}

void ClientNetwork::Disconnect()
{
    systemMessages.push_back("Disconnected from the server\n");
    isSystemMessage = true;
    std::cout << systemMessages.back() << std::endl;
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
            systemMessages.push_back("From ");
            systemMessages.back().append(receivedName).append(" with address ").append(senderAddress).append(":").append(std::to_string(senderPort)).append(" - ").append(receivedString).append("\n");

            std::cout << systemMessages.back() << std::endl;
            systemMessages.pop_back();
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
        systemMessages.push_back("Could not sent packet\n");
        isSystemMessage = true;
        std::cout << systemMessages.back() << std::endl;
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
        systemMessages.push_back("Server connection is not present, message is not sent\n");
        isSystemMessage = true;
        std::cout << systemMessages.back() << std::endl;
    }
}

void ClientNetwork::Run(std::string input)
{
    ManagePackets(input);
}

void ClientNetwork::ClearSystemMessages()
{
    systemMessages.clear();
}

void ClientNetwork::SetIsPacketRecieved(bool newIsPacketRecieved)
{
    isPacketRecieved = newIsPacketRecieved;
}

void ClientNetwork::SetName(std::string newName)
{
    name = newName;
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

std::vector<std::string> ClientNetwork::GetSystemMessages()
{
    return systemMessages;
}

void ClientNetwork::SetIsSystemMessage(bool newIsSystemMessage)
{
    isSystemMessage = newIsSystemMessage;
}

std::string ClientNetwork::GetName()
{
    return name;
}