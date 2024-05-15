#include "Interface.hpp"

void Interface::Init(sf::RenderWindow& window)
{
	// Инициализируем интерфейс и поддержку кириллицы
	ImGui::SFML::Init(window, false);
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("arial.ttf", 16.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
}
// Окно интерфейса
void Interface::Update(sf::RenderWindow& window, sf::Time time)
{
	// Интерфейс, его размер и позиция на экране
	ImGui::SFML::Update(window, time);
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(1280.f, 720.f));
	// Определяем окно ImGui и необходимые флаги
	ImGui::Begin(u8"Автор - Ошурков Никита Алексеевич, АлтГТУ им. И.И. Ползунова", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	
	// Вывод строк
	ImGui::BeginChild("##child", ImVec2(0, TEXT_BOX_HEIGHT), true);
	for (size_t iterator = 0; iterator < textBox.size(); iterator++)
	{
		ImGui::TextWrapped(textBox[iterator]);
	}
	ImGui::EndChild();

	// Ввод строки
	ImGui::Text(u8"Ввод текста");
	if (ImGui::InputTextMultiline("##soure", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 3),
		ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine))
	{
		// Debug
		// std::cout << "Pressed enter in input" << std::endl;
		strcpy(input, inputBuffer);
		isInputDone = true;
		strncpy(inputBuffer, "", INPUT_SIZE);
	}

	// Classic - красивый черный цвет ^_^
	ImGui::StyleColorsClassic();
	ImGui::End();
}

void Interface::ModifyTextBox(std::string receivedString, std::string receivedName)
{
	textBox.push_back(new char[TEXT_BOX_SIZE]());
	std::strcat(textBox[textBox.size() - 1], receivedName.c_str());
	std::strcat(textBox[textBox.size() - 1], " : ");
	std::strcat(textBox[textBox.size() - 1], receivedString.c_str());
}

void Interface::MofifyTextBoxSystemMessage(std::string receivedString)
{
	textBox.push_back(new char[TEXT_BOX_SIZE]());
	std::strcat(textBox[textBox.size() - 1], "\n");
	std::strcat(textBox[textBox.size() - 1], "\t");
	std::strcat(textBox[textBox.size() - 1], receivedString.c_str());
	std::strcat(textBox[textBox.size() - 1], "\n");
}


std::string Interface::GetInput()
{
	return input;
}

void Interface::SetIsInputDone(bool newIsInputDone)
{
	isInputDone = newIsInputDone;
}

bool Interface::GetIsInputDone()
{
	return isInputDone;
}