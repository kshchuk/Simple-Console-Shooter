#include "../include/info/player.h"


Player::Player(Configs configs)
	: kFOV(configs.FOV), kDepth(configs.depth), kSpeed(configs.speed)
{
	pos.x = configs.playerX;
	pos.y = configs.playerY;
	rotation = configs.playerA;
	health = configs.health;
	profile_width = configs.profile_width;
	profile_height = configs.profile_height;
	profile_depth = configs.profile_depth;
}

void Player::RandomPosition(Configs configs)
{
	std::random_device rd;
	std::mt19937 mersenne(rd());

	do {
		pos.x = mersenne() % configs.map_width;
		pos.y = mersenne() % configs.map_height;
	} while (configs.map[pos.x][pos.y]);

	rotation = (mersenne() % 314) / 100;
}

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
