#pragma once

#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

class PlayerData
{
public:
	PlayerData();
	PlayerData(const std::string& playerName, float gameTime);
	void load(XMLElement* element);
	void save(XMLElement* element);
	const std::string& getPlayerName() const { return playerName; }
	float getGameTime() const { return gameTime; }
	std::string getFormattedGameTime() const;
	static std::string formatTime(float timeInSeconds);
private:
	std::string playerName;
	float gameTime;
};