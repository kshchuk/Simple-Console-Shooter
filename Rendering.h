#pragma once
#include "Configs.h"
#include "Player.h"
#include <vector>
#include <chrono>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include <cmath>

float distance(float x1, float y1, float x2, float y2);

// get crossing point
bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y);


class Rendering
{

private:

	static void Collision(Player& player, float fElapsedTime, char key, const std::vector<std::vector<bool>>& map);


public:

	// Movement
	static void CalculatePosition(Player& player, const float fElapsedTime, const std::vector<std::vector<bool>>& map);

	static void RenderFrame(const Configs& conf, Player& player,
		const std::vector<std::vector<bool>>& map, wchar_t* screen,
		HANDLE hConsole, DWORD dwBytesWritten,
		std::chrono::system_clock::time_point& tp1, std::chrono::system_clock::time_point& tp2, 
		float fElapsedTime, const std::map<int, Player*>& other_players);
};

