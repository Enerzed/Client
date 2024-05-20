// Client.cpp: определяет точку входа для приложения.

#pragma execution_character_set("utf-8")

#include "Client.h"

void Client::Run()
{
    ClientNetwork network;

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
        for (size_t iterator = 0; iterator < network.GetSystemMessages().size(); iterator++)
        {
            std::string receivedString = network.GetSystemMessages()[iterator];
            interface.MofifyTextBoxSystemMessage(receivedString);
        }
        network.ClearSystemMessages();

        for (size_t iterator = 0; iterator < network.GetPackets().size(); iterator++)
        {
            size_t type;
            std::string name;
            std::string message;
            std::string address;
            unsigned short port;

            network.GetPackets()[iterator] >> type >> name >> message >> address >> port;
            interface.ModifyTextBox(message, name);
        }
        network.ClearPackets();

        if (interface.GetIsConnectDone() == true)
        {
            if (network.GetIsConnected())
            {
                network.Disconnect();
            }
            network.Connect(interface.GetServerIP(), interface.GetServerPort());
            network.Run(PACKET_TYPE_NAME, interface.GetClientName());
            interface.SetIsConnectDone(false);
        }

        if (interface.GetIsInputDone() == true)
        {
            network.Run(PACKET_TYPE_MESSAGE, interface.GetInput());
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