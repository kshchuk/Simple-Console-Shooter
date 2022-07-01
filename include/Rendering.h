#pragma once

#include <vector>
#include <chrono>
#include <cmath>

#include "info/configs.h"
#include "info/textures.h"
#include "info/configs.h"
#include "info/player.h"
#include "network/client_game.h"


namespace rendering
{
	float distance(float x1, float y1, float x2, float y2);

	// get crossing point
	bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y);

	void Collision(Player& player, float fElapsedTime, char key, const std::vector<std::vector<bool>>& map);

	// Getting key commands
	void CalculatePosition(Player& player, Configs& conf, const float fElapsedTime, const std::vector<std::vector<bool>>& map, ClientGame* client,
		std::chrono::system_clock::time_point& last_firing_time);

	void RenderFrame(const Configs& conf, Player& player,
		const std::vector<std::vector<bool>>& map, wchar_t* screen,
		HANDLE hConsole, DWORD dwBytesWritten,
		std::chrono::system_clock::time_point& tp1, std::chrono::system_clock::time_point& tp2,
		float fElapsedTime, Textures* textures, const std::chrono::system_clock::time_point& last_firing_time,
		const std::map<int, Player*>* other_players = nullptr);
};

