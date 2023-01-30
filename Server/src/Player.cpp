/*
 *
 * File: player.cpp
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#include "../include/info/player.h"


Player::Player()
	: kFOV(Configs::FOV), kDepth(Configs::depth), kSpeed(Configs::speed)
{
	pos.x = Configs::playerX;
	pos.y = Configs::playerY;
	rotation = Configs::playerA;
	health = Configs::health;

	profile_width = Configs::profile_width;
	profile_height = Configs::profile_height;
	profile_depth = Configs::profile_depth;
}

void Player::RandomPosition()
{
	std::random_device rd;
	std::mt19937 mersenne(rd());

	do {
		pos.x = mersenne() % Configs::map_width;
		pos.y = mersenne() % Configs::map_height;
	} while (Configs::map[pos.x][pos.y]);

	rotation = (mersenne() % 314) / 100;
}

// Calculates siluet's rectangel's corners positions with approprivate 
// transfer formulas

void Player::CaclulatePositions()
{
	left_front.x = profile_depth / 2;
	left_front.y = profile_width / 2;

	right_front.x = profile_depth / 2;
	right_front.y = -profile_width / 2;

	left_back.x = -profile_depth / 2;
	left_back.y = profile_width / 2;

	right_back.x = -profile_depth / 2;
	right_back.y = -profile_width / 2;

	// Turning

	left_front.x = left_front.x * cosf(rotation) - left_front.y * sinf(rotation);
	left_front.y = left_front.y * cosf(rotation) + left_front.x * sinf(rotation);

	right_front.x = right_front.x * cosf(rotation) - right_front.y * sinf(rotation);
	right_front.y = right_front.y * cosf(rotation) + right_front.x * sinf(rotation);

	left_back.x = left_back.x * cosf(rotation) - left_back.y * sinf(rotation);
	left_back.y = left_back.y * cosf(rotation) + left_back.x * sinf(rotation);

	right_back.x = right_back.x * cosf(rotation) - right_back.y * sinf(rotation);
	right_back.y = right_back.y * cosf(rotation) + right_back.x * sinf(rotation);

	left_front.x += pos.x;
	left_front.y += pos.y;

	right_front.x += pos.x;
	right_front.y += pos.y;

	left_back.x += pos.x;
	left_back.y += pos.y;

	right_back.x += pos.x;
	right_back.y += pos.y;
}
