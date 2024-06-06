// Везде бы использовал std::string, но ImGui использует const char :P

#pragma once
#define IP_SIZE 16
#define PORT_SIZE 5
#define CLIENT_NAME_SIZE 32
#define INPUT_SIZE 256
#define TEXT_BOX_SIZE 256
#define TEXT_BOX_HEIGHT 580
#define TEXT_BOX_MESSAGE_LIMIT 128
// Debug
// #define textBoxSize 2048

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>
#include <cstring>


class Interface
{
private:
	std::vector<char*> textBox;						// Поле чата
	char inputBuffer[INPUT_SIZE] = { 0 };			// Буффер ввода
	char input[INPUT_SIZE] = { 0 };					// То что было введено окончательно
	char serverIP[IP_SIZE] = { 0 };					// Введенное IP сервера
	char serverPort[PORT_SIZE] = { 0 };				// Введенный порт для подключение
	char clientName[CLIENT_NAME_SIZE] = { 0 };		// Имя клиента, которое было выбрано при подключении
	bool isInputDone = false;						// Флаг введен ли текст (нажат Enter)
	bool isConnectDone = false;						// Была ли нажата кнопка подключения
public:
	void Init(sf::RenderWindow&);					// Инициализация интерфейса и поддержки кириллицы
	void Update(sf::RenderWindow&, sf::Time);		// Функция обновления для управления
	void ModifyTextBox(std::string, std::string);	// Получаем сообщения и загружаем их textBox
	void MofifyTextBoxSystemMessage(std::string);	// Уведомления системы
	// Setters
	void SetIsInputDone(bool);						// Установка флага введен ли текст
	void SetIsConnectDone(bool);					// Установка флага нажата ли была кнопка подключения
	// Getters
	std::string GetInput();							// Получение введенного текста
	bool GetIsInputDone();							// Получение флага введен ли был текст (нажат Enter)
	bool GetIsConnectDone();						// Получение флага нажали ли была кнопка подключения
	char* GetServerIP();							// Получение IP сервера для подключения
	unsigned short GetServerPort();					// Получение порта сервера для подключения
	std::string GetClientName();					// Получение введенного имени
};