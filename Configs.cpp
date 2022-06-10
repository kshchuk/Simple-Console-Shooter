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

		maxShootingRange = 50.0f;
		gunReloading_seconds = 1.0f;
		shootingDamage = 33;

		pWidth = 0.25f;
		pDepth = 0.17f;
		pHight = 1.0f;

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
	int* imap = (int*)(config_data + sizeof(Configs));

	map.clear();
	map.resize(mapHeight);
	for (auto& line : map)
		line.resize(mapWidth);
	printf("Received map:\n");

	for (size_t i = 0; i < mapHeight; i++) {
		printf("\n");
		for (size_t j = 0; j < mapWidth; j++) {
			map[i][j] = (bool)imap[i * mapWidth + j];
			printf("%i", imap[i * mapWidth + j]);
		}
	}
	printf("\n");
	printf("\n");


	saveToFile();
}

bool Configs::readFromFile()
{
	std::ifstream jsonFile("config.json");
	if (jsonFile)
	{
		nlohmann::json j;
		jsonFile >> j;

		screenWidth  =  j["screenWidth"];
		screenHeight = j["screenHeight"];
		mapWidth  = j["mapWidth"];
		mapHeight = j["mapHeight"];
		playerX = j["PlayerX"];
		playerY = j["PlayerY"];
		playerA = j["PlayerA"];
		FOV =   j["FOV"];
		depth = j["depth"];
		speed = j["speed"];

		maxShootingRange = j["maxShootingRange"];
		gunReloading_seconds = j["gunReloading_seconds"];
		shootingDamage   = j["shootingDamage"];

		pWidth = j["pWidth"];
		pDepth = j["pDepth"];
		pHight = j["pHight"];
		
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

	j["maxShootingRange"] = maxShootingRange;
	j["gunReloading_seconds"] = gunReloading_seconds;
	j["shootingDamage"]   = shootingDamage;

	j["pWidth"] = pWidth;
	j["pDepth"] = pDepth;
	j["pHight"] = pHight;

	j["map"]    = map;

	std::ofstream jsonFile("config.json");
	jsonFile << j;
	jsonFile.close();
};