#pragma once
#include <vector>
#include <fstream>
#include "json.hpp"

const class Configs
{
public:
	int screenWidth;			// Console Screen Size X (columns)
	int screenHeight;			// Console Screen Size Y (rows)
	int mapWidth;				// World Dimensions (Width)
	int mapHeight;				// World Dimensions (Height)

	float playerX;				// Start Position X
	float playerY;				// Start Position Y
	float playerA;				// Start Rotation
	float FOV;					// Field of View
	float depth;				// Maximum rendering distance
	float speed;				// Walking Speed
	float maxShootingRange;		
	int shootingDamage;			

	float pWidth;				// Profile width
	float pDepth;				// Profile depth
	float pHight;				// Profile height

	int health = 100;			// default health 

	//char* serverIP = nullptr;			// IP to connect multiplay
	//char* serverPORT = nullptr;		// PORT to send information

	// Map of the world
	std::vector<std::vector<bool>> map;

	Configs();

	Configs(char*);

private:
	bool readFromFile();
	void saveToFile();
};

