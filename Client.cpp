#include "Client.hpp"

Client::Client()
{
    // Инициализируем окно интерфейс и сеть
    window = new sf::RenderWindow(sf::VideoMode(1280, 720), L"Мессенджер", sf::Style::Close);
    window->setFramerateLimit(120);
    interface = new Interface();
    interface->Init(*window);
    network = new ClientNetwork();
}

void Client::Run()
{
    // Время
    sf::Clock clock;
    // Основной цикл
    while (window->isOpen())
    {
        sf::Time time = clock.getElapsedTime();
        clock.restart();
        // Обработка событий
        sf::Event event;
        while (window->pollEvent(event)) 
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) 
            {
                window->close();
            }
        }
        // Сеть
        RunNetwork();
        // Передаем системные сообщения
        RunSystemMessages();
        // Передаем пакеты
        RunPackets();
        // Обработка подключения
        RunConnect();
        // Обновляем интерфейс
        interface->Update(*window, time);
        // Очищаем окно
        window->clear();
        // Render интерфейса должен быть перед показом его на экране для того, чтобы он был на первом плане
        ImGui::SFML::Render(*window);
        // Показываем окно
        window->display();
    }
    // Отключаем ImGui-SFML после закрытия окна
    ImGui::SFML::Shutdown();
}

void Client::RunNetwork()
{
    // Если было введено сообщение то отправляем его
    if (interface->GetIsInputDone() == true)
    {
        network->Run(PACKET_TYPE_MESSAGE, interface->GetInput());
        interface->SetIsInputDone(false);
    }
}

void Client::RunSystemMessages()
{
    for (size_t iterator = 0; iterator < network->GetSystemMessages().size(); iterator++)
    {
        std::string receivedString = network->GetSystemMessages()[iterator];
        interface->MofifyTextBoxSystemMessage(receivedString);
    }
    network->ClearSystemMessages();
}

void Client::RunPackets()
{
    for (size_t iterator = 0; iterator < network->GetPackets().size(); iterator++)
    {
        unsigned short type;
        std::string name;
        std::string message;
        // Выводим содержимое пакета на экран
        network->GetPackets()[iterator] >> type >> name >> message;
        interface->ModifyTextBox(message, name);
    }
    network->ClearPackets();
}

void Client::RunConnect()
{
    if (interface->GetIsConnectDone() == true)
    {
        if (network->GetIsConnected())
        {
            network->Disconnect();
        }
        network->Connect(interface->GetServerIP(), interface->GetServerPort());
        // Пауза на 32 миллисекунды
        std::this_thread::sleep_for(std::chrono::milliseconds{ 32 });
        if (network->GetIsConnected() == true)
        {
            network->SetName(interface->GetClientName());
            network->Run(PACKET_TYPE_CLIENT_CONNECTED, "Setting up encryption\n");
        }
        interface->SetIsConnectDone(false);
    }
}