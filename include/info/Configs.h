#pragma once

#include <fstream>
#include <vector>

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
	float gunReloading_seconds;
	int shootingDamage;

	float pWidth;				// Profile width
	float pDepth;				// Profile depth
	float pHight;				// Profile height

	int health = 100;			// default health 

	// Map of the world
	std::vector<std::vector<bool>> map;

	Configs();
	Configs(char*);
	void GetMap(char*);

private:
	bool readFromFile();
	void saveToFile();
};

