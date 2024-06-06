// Устанавливаем кодировку utf-8
#pragma execution_character_set("utf-8")
#include "Client.hpp"


int main()
{
    // Кодировка
    system("chcp 65001");
    std::cout << std::endl;
    // Клиент
    Client client;
    client.Run();
    return 0;
}