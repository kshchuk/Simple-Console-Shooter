/*
 *
 * file player.h
 *
 * author Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */

#pragma once

#include <random>

#include "configs.h"

 // Struct to keep player's information
struct Player
{
	struct Point {
		float x;
		float y;
	};

	Point pos;           // Player Position
	float rotation;      // Player Rotation

	const float kFOV;    // Field of View
	const float kDepth;  // Maximum rendering distance
	const float kSpeed;  // Walking Speed

	// Size of other players' silhouettes

	float profile_width;
	float profile_depth;
	float profile_height;

	// Multiplayer info 

	int index;
	int health;

	// Positions of silhouette (rectangle)

	Point left_front;
	Point right_front;
	Point left_back;
	Point right_back;

	Player(Configs configs);

	void RandomPosition(Configs configs);

	// Calculates siluet's rectangel's corners positions
	void CaclulatePositions();
};