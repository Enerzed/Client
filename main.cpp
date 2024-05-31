#pragma execution_character_set("utf-8")
#include "Client.hpp"


int main()
{
    system("chcp 65001");
    std::cout << std::endl;

    Client client;
    client.Run();

    return 0;
}