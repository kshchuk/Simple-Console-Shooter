#pragma once
#include <random>
#include "Configs.h"


class Player
{
public:
	int index;	
	float x;				    // Player Position X
	float y;				    // Player Position Y
	float rotation;				// Player Rotation

	int health;					// min 1, max 100

	Player(Configs conf);

	void RandomPosition(Configs conf);
};