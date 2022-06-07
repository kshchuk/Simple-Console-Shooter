#include "Configs.h"
#include <string>

Configs::Configs()
{
	std::ifstream jsonFile("config.json");
	if (jsonFile)
	{
		nlohmann::json j;
		jsonFile >> j;

		screenWidth = j["screenWidth"];
		screenHeight = j["screenHeight"];
		mapWidth = j["mapWidth"];
		mapHeight = j["mapHeight"];
		playerX = j["PlayerX"];
		playerY = j["PlayerY"];
		playerA = j["PlayerA"];
		FOV = j["FOV"];
		depth = j["depth"];
		speed = j["speed"];
		map = j["map"];

		jsonFile.close();
	}
	else  // There's no config file
	{
		jsonFile.close();

		nlohmann::json j;

		// defaults
		screenWidth = 384;		j["screenWidth"] = screenWidth;
		screenHeight = 123;		j["screenHeight"] = screenHeight;
		mapWidth = 16;			j["mapWidth"] = mapWidth;
		mapHeight = 16;			j["mapHeight"] = mapHeight;

		playerX = 14.7f;		j["PlayerX"] = playerX;
		playerY = 5.09f;		j["PlayerY"] = playerY;
		playerA = 0.0f;			j["PlayerA"] = playerA;
		FOV = 3.14159f / 4.0f;	j["FOV"] = FOV;
		depth = 16.0f;			j["depth"] = depth;
		speed = 4.0f;			j["speed"] = speed;

		map = {
			{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
			{1,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};

		j["map"] = map;

		std::ofstream jsonFile("config.json");
		jsonFile << j;
		jsonFile.close();
	}
};