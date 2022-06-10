#pragma once
#include <random>
#include "Configs.h"

struct Point {
	float x;
	float y;
};

class Player
{
public:
	float x;				    // Player Position X
	float y;				    // Player Position Y
	float rotation;				// Player Rotation

	const float FOV;				// Field of View
	const float depth;				// Maximum rendering distance
	const float speed;				// Walking Speed

	float pWidth;				// Profile width
	float pDepth;				// Profile depth
	float pHight;				// Profile height

	// multiplayer

	int index;
	int health;						// min 1, max 100
	// position of siluet (rectangle)
	Point leftFront_pos;
	Point rightFront_pos;
	Point leftBack_pos;
	Point rightBack_pos;


	Player(Configs conf);

	void RandomPosition(Configs conf);

	// Calculate siluet's rectangel corners positions
	void CaclulatePositions();
};