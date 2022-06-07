#pragma once
#include "Configs.h"
#include "Player.h"
#include <vector>
#include <chrono>
#include <Windows.h>


class Rendering
{

private:

	static void Collision(Player& player, float fElapsedTime, char key, const std::vector<std::vector<bool>>& map);


public:

	// Movement
	static void CalculatePosition(Player& player, const float fElapsedTime, const std::vector<std::vector<bool>>& map);

	static void RenderFrame(const Configs& conf, Player& player, size_t players_num,
		const std::vector<std::vector<bool>>& map, wchar_t* screen,
		HANDLE hConsole, DWORD dwBytesWritten,
		std::chrono::system_clock::time_point& tp1, std::chrono::system_clock::time_point& tp2, 
		float fElapsedTime);
};

