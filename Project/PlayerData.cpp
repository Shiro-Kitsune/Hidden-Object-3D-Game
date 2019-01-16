#include "PlayerData.h"
#include <sstream>
#include <iomanip> 

PlayerData::PlayerData() : playerName(""), gameTime(0)
{
}

PlayerData::PlayerData(const std::string & name, float time) : playerName(name), gameTime(time)
{
}

void PlayerData::load(XMLElement * element)
{
	XMLElement* nameElement = element->FirstChildElement("PlayerName");
	if (nameElement == nullptr) throw std::exception("PlayerName element is missing in PlayerData");
	playerName = nameElement->GetText();

	XMLElement* timeElement = element->FirstChildElement("GameTime");
	if (timeElement == nullptr) throw std::exception("GameTime element is missing in PlayerData");
	timeElement->QueryFloatText(&gameTime);
}

void PlayerData::save(XMLElement * element)
{
	XMLElement* nameElement = element->GetDocument()->NewElement("PlayerName");
	nameElement->SetText(playerName.c_str());
	element->InsertEndChild(nameElement);

	XMLElement* timeElement = element->GetDocument()->NewElement("GameTime");
	timeElement->SetText(gameTime);
	element->InsertEndChild(timeElement);
}

std::string PlayerData::getFormattedGameTime() const
{
	return formatTime(gameTime);
}

std::string PlayerData::formatTime(float timeInSeconds)
{
	int hours = timeInSeconds / 3600;
	int minutes = ((int)timeInSeconds % 3600) / 60;
	int seconds = ((int)timeInSeconds % 3600) % 60;
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << std::to_string(seconds);
	return ss.str();
}