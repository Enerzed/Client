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
        for (size_t iterator = 0; iterator < clientNetwork.GetSystemMessages().size(); iterator++)
        {
            std::string receivedString = clientNetwork.GetSystemMessages()[iterator];
            interface.MofifyTextBoxSystemMessage(receivedString);
        }
        clientNetwork.ClearSystemMessages();

        for (size_t iterator = 0; iterator < clientNetwork.GetPackets().size(); iterator++)
        {
            size_t type;
            std::string name;
            std::string message;
            std::string address;
            unsigned short port;

            clientNetwork.GetPackets()[iterator] >> type >> name >> message >> address >> port;
            interface.ModifyTextBox(message, name);
        }
        clientNetwork.ClearPackets();

        if (interface.GetIsConnectDone() == true)
        {
            if (clientNetwork.GetIsConnected())
            {
                clientNetwork.Disconnect();
            }
            clientNetwork.Connect(interface.GetServerIP(), interface.GetServerPort());
            clientNetwork.SetName(interface.GetClientName());
            clientNetwork.Run(PACKET_TYPE_NAME, clientNetwork.GetName());
            interface.SetIsConnectDone(false);
        }

        if (interface.GetIsInputDone() == true)
        {
            clientNetwork.Run(PACKET_TYPE_MESSAGE, interface.GetInput());
            interface.SetIsInputDone(false);
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