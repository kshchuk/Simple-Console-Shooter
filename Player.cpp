#include "Player.h"


Player::Player(Configs conf)
{
	x = conf.playerX;
	y = conf.playerY;
	rotation = 0;
	health = 100;
}

void Player::RandomPosition(Configs conf)
{
	std::random_device rd;
	std::mt19937 mersenne(rd());

	do {
		x = mersenne() % conf.mapWidth;
		y = mersenne() % conf.mapHeight;
	} while (conf.map[x][y]);

	rotation = (mersenne() % 314) / 100;
}