#pragma once
#include <random>
#include "Configs.h"


class Player
{
public:
	int index;					// Player Index (for multiplayer)
	float x;				    // Player Position X
	float y;				    // Player Position Y
	float rotation;				// Player Rotation

	int health;						// min 1, max 100

	const float FOV;				// Field of View
	const float depth;				// Maximum rendering distance
	const float speed;				// Walking Speed

	float pWidth;				// Profile width
	float pDepth;				// Profile depth
	float pHight;				// Profile height

	Player(Configs conf);

	void RandomPosition(Configs conf);
};