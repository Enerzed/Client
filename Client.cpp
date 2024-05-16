// Client.cpp: определяет точку входа для приложения.

#pragma execution_character_set("utf-8")

#include "Client.h"

void Client::Run()
{
    ClientNetwork clientNetwork("UNDEFINED");

    // Окно
    sf::RenderWindow window(sf::VideoMode(1280, 720), L"Мессенджер", sf::Style::Close);
    window.setFramerateLimit(120);

    Interface interface;
    interface.Init(window);
    // Время
    sf::Clock clock;
    // Основной цикл
    while (window.isOpen())
    {
        sf::Time time = clock.getElapsedTime();
        clock.restart();
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
        }
        // Сеть
        if (interface.GetIsConnectDone() == true)
        {
            clientNetwork.Disconnect();
            clientNetwork.Connect(interface.GetServerIP(), interface.GetServerPort());
            clientNetwork.SetName(interface.GetClientName());
            interface.SetIsConnectDone(false);
        }
        if (interface.GetIsInputDone() == true)
        {
            clientNetwork.Run(interface.GetInput());
            interface.SetIsInputDone(false);
        }
        for (size_t iterator = 0; iterator < clientNetwork.GetSystemMessages().size(); iterator++)
        {
            std::string receivedString = clientNetwork.GetSystemMessages()[iterator];
            interface.MofifyTextBoxSystemMessage(receivedString);
        }
        clientNetwork.ClearSystemMessages();

        if (clientNetwork.GetIsPacketRecieved() == true)
        {
            std::string receivedString;
            std::string receivedName;
            std::string senderAddress;
            unsigned short senderPort;
            clientNetwork.GetLastPacket() >> receivedString >> receivedName >> senderAddress >> senderPort;
            interface.ModifyTextBox(receivedString, receivedName);
            clientNetwork.SetIsPacketRecieved(false);
        }
        // Обновляем интерфейс
        interface.Update(window, time);
        // Очищаем окно
        window.clear();
        // Render интерфейса должен быть перед показом его на экране для того, чтобы он был на первом плане
        ImGui::SFML::Render(window);
        // Показываем окно
        window.display();
    }
    // Отключаем ImGui-SFML после закрытия окна
    ImGui::SFML::Shutdown();

}

int main()
{
    system("chcp 65001");

    Client client;
    client.Run();
    
    return 0;
}