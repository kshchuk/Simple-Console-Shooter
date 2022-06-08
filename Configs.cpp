#include "Configs.h"
#include <string>

Configs::Configs()
{
	if (readFromFile())
		return;
	else  // There's no config file
	{
		// defaults
		screenWidth = 384;		
		screenHeight = 123;		
		mapWidth = 20;			
		mapHeight = 20;			

		playerX = 14.7f;		
		playerY = 5.09f;		
		playerA = 0.0f;			
		FOV = 3.14159f / 4.0f;	
		depth = 16.0f;			
		speed = 4.0f;			

		map = {
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,1},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};

		saveToFile();
	}
}
Configs::Configs(char* config_data)
{
	// Getting information
	memcpy(this, config_data, sizeof(Configs) - sizeof(map)); // map is corrupted vector

	// Getting map
	bool* bmap = (bool*)(config_data + sizeof(Configs) + sizeof(map));
	map.clear();
	map.resize(mapHeight);
	for (auto& line : map)
		line.resize(mapWidth);
	for (size_t i = 0; i < mapHeight; i++)
		for (size_t j = 0; j < mapWidth; j++)
			map[i][j] = bmap[i * mapWidth + j];

	saveToFile();
}

bool Configs::readFromFile()
{
	std::ifstream jsonFile("config.json");
	if (jsonFile)
	{
		nlohmann::json j;
		jsonFile >> j;

		screenWidth =  j["screenWidth"];
		screenHeight = j["screenHeight"];
		mapWidth =  j["mapWidth"];
		mapHeight = j["mapHeight"];
		playerX = j["PlayerX"];
		playerY = j["PlayerY"];
		playerA = j["PlayerA"];
		FOV =   j["FOV"];
		depth = j["depth"];
		speed = j["speed"];
		map =   j["map"];

		jsonFile.close();
		return true;
	}
	else {
		jsonFile.close();
		return false;
	}
}

void Configs::saveToFile()
{
	nlohmann::json j;

	j["screenWidth"]  = screenWidth;
	j["screenHeight"] = screenHeight;
	j["mapWidth"]     = mapWidth;
	j["mapHeight"]    = mapHeight;

	j["PlayerX"] = playerX;
	j["PlayerY"] = playerY;
	j["PlayerA"] = playerA;
	j["FOV"]     = FOV;
	j["depth"]   = depth;
	j["speed"]   = speed;
	j["map"]     = map;

	std::ofstream jsonFile("config.json");
	jsonFile << j;
	jsonFile.close();
};