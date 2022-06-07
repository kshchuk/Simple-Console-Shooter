#pragma once
#include <random>
#include "Configs.h"


class Player
{
public:
	float x;				    // Player Position X
	float y;				    // Player Position Y
	float rotation;				// Player Rotation

	const float FOV;				// Field of View
	const float depth;				// Maximum rendering distance
	const float speed;				// Walking Speed

	Player(Configs conf);

	void RandomPosition(Configs conf);
};