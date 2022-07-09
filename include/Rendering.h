/*
 *
 * File: rendering.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#pragma once

#include <vector>
#include <chrono>
#include <cmath>
#include <tuple>

#include "info/configs.h"
#include "info/textures.h"
#include "info/configs.h"
#include "info/player.h"
#include "network/client_game.h"


namespace rendering
{
	using ConsoleInfo = std::tuple<HANDLE,
								DWORD,
								wchar_t*,
								std::chrono::system_clock::time_point&, 
								std::chrono::system_clock::time_point&>;

	using GamingInfo =	std::tuple<float,
								Configs&,
								Player*,
								std::chrono::system_clock::time_point&,
								network::ClientGame*>;

	// Gets distance between two points
	float distance(float x1, float y1, float x2, float y2);

	// Gets crossing point
	bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y);

	// Collision event
	void Collision(const std::vector<std::vector<bool>>& map, Player* player, float elapsed_time, char key);

	// Gets key commands
	void CalculatePosition(GamingInfo& game_info);

	void RenderFrame(GamingInfo& game_info, const Textures& textures, ConsoleInfo& console_info);
};

