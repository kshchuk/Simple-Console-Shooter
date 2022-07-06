#pragma once

#include <random>

#include "configs.h"


struct Player
{
	struct Point {
		float x;
		float y;
	};


	Point pos;						// Player Position
	float rotation;				// Player Rotation

	const float kFOV;				// Field of View
	const float kDepth;				// Maximum rendering distance
	const float kSpeed;				// Walking Speed

	float profile_width;				// Profile width
	float profile_depth;				// Profile depth
	float profile_height;				// Profile height

	// multiplayer
	int index;
	int health;						// min 1, max 100

	// position of siluet (rectangle)
	Point left_front;
	Point right_front;
	Point left_back;
	Point right_back;

	Player(Configs configs);

	void RandomPosition(Configs configs);

	// Calculate siluet's rectangel corners positions
	void CaclulatePositions();
};