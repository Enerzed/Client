// Везде бы использовал std::string, но ImGui использует const char :P

#pragma once
#define INPUT_SIZE 256
#define TEXT_BOX_SIZE 256
#define TEXT_BOX_HEIGHT 600
#define TEXT_BOX_MESSAGE_LIMIT 128
// Debug
// #define textBoxSize 2048

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>


class Interface
{
private:
	std::vector<char*> textBox;
	char inputBuffer[INPUT_SIZE] = { 0 };
	char input[INPUT_SIZE] = { 0 };
	bool isInputDone = false;
public:
	void Init(sf::RenderWindow&);
	void Update(sf::RenderWindow&, sf::Time);
	void ModifyTextBox(std::string, std::string);
	void MofifyTextBoxSystemMessage(std::string);
	std::string GetInput();
	void SetIsInputDone(bool);
	bool GetIsInputDone();
};