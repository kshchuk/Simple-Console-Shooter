#include "../include/info/player.h"


Player::Player(Configs conf)
	: FOV(conf.FOV), depth(conf.depth), speed(conf.speed)
{
	x = conf.playerX;
	y = conf.playerY;
	rotation = 0;
	health = 100;
	pWidth = conf.pWidth;
	pHight = conf.pHight;
	pDepth = conf.pDepth;
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

void Player::CaclulatePositions()
{
	leftFront_pos.x = pDepth / 2;
	leftFront_pos.y = pWidth / 2;

	rightFront_pos.x = pDepth / 2;
	rightFront_pos.y = -pWidth / 2;

	leftBack_pos.x = -pDepth / 2;
	leftBack_pos.y = pWidth / 2;

	rightBack_pos.x = -pDepth / 2;
	rightBack_pos.y = -pWidth / 2;
	// Turning
	leftFront_pos.x = leftFront_pos.x * cosf(rotation) - leftFront_pos.y * sinf(rotation);
	leftFront_pos.y = leftFront_pos.y * cosf(rotation) + leftFront_pos.x * sinf(rotation);

	rightFront_pos.x = rightFront_pos.x * cosf(rotation) - rightFront_pos.y * sinf(rotation);
	rightFront_pos.y = rightFront_pos.y * cosf(rotation) + rightFront_pos.x * sinf(rotation);

	leftBack_pos.x = leftBack_pos.x * cosf(rotation) - leftBack_pos.y * sinf(rotation);
	leftBack_pos.y = leftBack_pos.y * cosf(rotation) + leftBack_pos.x * sinf(rotation);

	rightBack_pos.x = rightBack_pos.x * cosf(rotation) - rightBack_pos.y * sinf(rotation);
	rightBack_pos.y = rightBack_pos.y * cosf(rotation) + rightBack_pos.x * sinf(rotation);

	leftFront_pos.x += x;
	leftFront_pos.y += y;

	rightFront_pos.x += x;
	rightFront_pos.y += y;

	leftBack_pos.x += x;
	leftBack_pos.y += y;

	rightBack_pos.x += x;
	rightBack_pos.y += y;
}
