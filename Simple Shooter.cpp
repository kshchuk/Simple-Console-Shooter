

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>

#include "json.hpp"
#include <fstream>
#include <random>


#include <stdio.h>
#include <Windows.h>


const class Configs 
{

public:
	int screenWidth;			// Console Screen Size X (columns)
	int screenHeight;			// Console Screen Size Y (rows)
	int mapWidth;				// World Dimensions (Width)
	int mapHeight;				// World Dimensions (Height)

	float playerX;				// Start Position X
	float playerY;				// Start Position Y
	float playerA;				// Start Rotation
	float FOV;					// Field of View
	float depth;				// Maximum rendering distance
	float speed;				// Walking Speed

	// Map of world
	std::vector<std::vector<bool>> map;

	Configs()
	{
		std::ifstream jsonFile("config.json");
		if (jsonFile) 
		{
			nlohmann::json j;
			jsonFile >> j;

			screenWidth  = j["screenWidth"];
			screenHeight = j["screenHeight"];
			mapWidth     = j["mapWidth"];
			mapHeight	 = j["mapHeight"];
			playerX		 = j["PlayerX"];
			playerY		 = j["PlayerY"];
			playerA		 = j["PlayerA"];
			FOV			 = j["FOV"];
			depth		 = j["depth"];
			speed		 = j["speed"];
			map			 = j["map"];

			jsonFile.close();
		}
		else
		{
			jsonFile.close();

			nlohmann::json j;

			// defaults
			screenWidth = 384;		j["screenWidth"] = screenWidth;
			screenHeight = 123;		j["screenHeight"] = screenHeight;
			mapWidth = 16;			j["mapWidth"] = mapWidth;
			mapHeight = 16;			j["mapHeight"] = mapHeight;

			playerX = 14.7f;		j["PlayerX"] = playerX;
			playerY = 5.09f;		j["PlayerY"] = playerY;
			playerA = 0.0f;			j["PlayerA"] = playerA;
			FOV = 3.14159f / 4.0f;	j["FOV"] = FOV;
			depth = 16.0f;			j["depth"] = depth;
			speed = 4.0f;			j["speed"] = speed;

			map = {
				{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1},
				{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
				{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
				{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
				{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
				{1,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
				{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
			};


			j["map"] = map;

			std::ofstream jsonFile("config.json");
			jsonFile << j;
			jsonFile.close();
		}
	}
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


	Player(Configs conf)
	: FOV(conf.FOV), depth(conf.depth), speed(conf.speed)
	{
		x = conf.playerX;
		y = conf.playerY;
	}

	void RandomPosition(Configs conf)
	{
		std::random_device rd;
		std::mt19937 mersenne(rd());

		do {
			x = mersenne() % conf.mapWidth;
			y = mersenne() % conf.mapHeight;
		}
		while (conf.map[x][y]);

		rotation = 0;
	}
};

void RenderFrame(Configs conf, Player* players, size_t players_num, )
{
		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();


		// Handle CCW Rotation
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			player.rotation -= (player.speed * 0.75f) * fElapsedTime;

		// Handle CW Rotation
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			player.rotation += (player.speed * 0.75f) * fElapsedTime;

		// Handle Forwards movement & collision
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			player.x += sinf(player.rotation) * player.speed * fElapsedTime;;
			player.y += cosf(player.rotation) * player.speed * fElapsedTime;;
			if (map[(int)player.x][(int)player.y])
			{
				player.x -= sinf(player.rotation) * player.speed * fElapsedTime;;
				player.y -= cosf(player.rotation) * player.speed * fElapsedTime;;
			}
		}

		// Handle backwards movement & collision
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			player.x -= sinf(player.rotation) * player.speed * fElapsedTime;;
			player.y -= cosf(player.rotation) * player.speed * fElapsedTime;;
			if (map[(int)player.x][(int)player.y])
			{
				player.x += sinf(player.rotation) * player.speed * fElapsedTime;;
				player.y += cosf(player.rotation) * player.speed * fElapsedTime;;
			}
		}

		for (int x = 0; x < conf.screenWidth; x++)
		{
			// For each column, calculate the projected ray angle into world space
			float fRayAngle = (player.rotation - player.FOV / 2.0f) + ((float)x / (float)conf.screenWidth) * player.FOV;

			// Find distance to wall
			float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
			float fDistanceToWall = 0.0f; //                                      resolution

			bool bHitWall = false;		// Set when ray hits wall block
			bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

			float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
			float fEyeY = cosf(fRayAngle);

			// Incrementally cast ray from player, along ray angle, testing for 
			// intersection with a block
			while (!bHitWall && fDistanceToWall < player.depth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(player.x + fEyeX * fDistanceToWall);
				int nTestY = (int)(player.y + fEyeY * fDistanceToWall);

				// Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= conf.mapWidth || nTestY < 0 || nTestY >= conf.mapHeight)
				{
					bHitWall = true;			// Just set distance to maximum depth
					fDistanceToWall = player.depth;
				}
				else
				{
					// Ray is inbounds so test to see if the ray cell is a wall block
					if (map[nTestX][nTestY])
					{
						// Ray has hit wall
						bHitWall = true;

						// To highlight tile boundaries, cast a ray from each corner
						// of the tile, to the player. The more coincident this ray
						// is to the rendering ray, the closer we are to a tile 
						// boundary, which we'll shade to add detail to the walls
						std::vector<std::pair<float, float>> p;

						// Test each corner of hit tile, storing the distance from
						// the player, and the calculated dot product of the two rays
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// Angle of corner to eye
								float vy = (float)nTestY + ty - player.y;
								float vx = (float)nTestX + tx - player.x;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// First two/three are closest (we will never see all four)
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
						if (acos(p.at(2).second) < fBound) bBoundary = true;
					}
				}
			}

			// Calculate distance to ceiling and floor
			int nCeiling = (float)(conf.screenHeight / 2.0) - conf.screenHeight / ((float)fDistanceToWall);
			int nFloor = conf.screenHeight - nCeiling;

			// Shader walls based on distance
			short nShade = ' ';
			if (fDistanceToWall <= player.depth / 4.0f)			nShade = 0x2588;	// Very close	
			else if (fDistanceToWall < player.depth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToWall < player.depth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToWall < player.depth)				nShade = 0x2591;
			else											nShade = ' ';		// Too far away

			if (bBoundary)		nShade = ' '; // Black it out

			for (int y = 0; y < conf.screenHeight; y++)
			{
				// Each Row
				if (y <= nCeiling)
					screen[y * conf.screenWidth + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y * conf.screenWidth + x] = nShade;
				else // Floor
				{
					// Shade floor based on distance
					float b = 1.0f - (((float)y - conf.screenHeight / 2.0f) / ((float)conf.screenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y * conf.screenWidth + x] = nShade;
				}
			}
		}

		// Display Stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", player.x, player.y, player.rotation, 1.0f / fElapsedTime);

		// Display Map
		for (int nx = 0; nx < conf.mapWidth; nx++)
			for (int ny = 0; ny < conf.mapWidth; ny++)
			{
				screen[(ny + 1) * conf.screenWidth + nx] = map[ny][nx] ? '#' : ' ';
			}
		screen[((int)player.x + 1) * conf.screenWidth + (int)player.y] = 'P';

		// Display Frame
		screen[conf.screenWidth * conf.screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, conf.screenWidth * conf.screenHeight, { 0,0 }, &dwBytesWritten);
}


int main()
{
	Configs conf;
	Player* player = new Player(conf); 
	player->RandomPosition(conf);

	std::vector<std::vector<bool>> map = conf.map;

	// Create Screen Buffer
	wchar_t* screen = new wchar_t[conf.screenWidth * conf.screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	while (true)
	{
		RenderFrame(conf, player, 1, )
	}

	return 0;
}

