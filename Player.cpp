#include "Player.h"


Player::Player(Configs conf)
	: FOV(conf.FOV), depth(conf.depth), speed(conf.speed)
{
	x = conf.playerX;
	y = conf.playerY;
	rotation = 0;
}

void Player::RandomPosition(Configs conf)
{
	std::random_device rd;
	std::mt19937 mersenne(rd());

	do {
		x = mersenne() % conf.mapWidth;
		y = mersenne() % conf.mapHeight;
	} while (conf.map[x][y]);

	rotation = 0;
}