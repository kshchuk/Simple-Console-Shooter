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
	// Gets distance between two points
	float distance(float x1, float y1, float x2, float y2);

	// Gets crossing point
	bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y);

	// Collision event
	void Collision(float elapsed_time, char key, const std::vector<std::vector<bool>>& map, Player* player);

	// Getting key commands
	void CalculatePosition(const std::vector<std::vector<bool>>& map, 
						   const Configs* configs, 
						   float elapsed_time,
						   std::chrono::system_clock::time_point& last_firing_time, 
						   network::ClientGame* client, 
						   Player* player);

	void RenderFrame(const Configs* configs,	
					 const Textures* textures,
					 const std::vector<std::vector<bool>>& map, 
					 float elapsed_time, 
					 std::chrono::system_clock::time_point last_firing_time,
					 HANDLE console, 
					 DWORD bytes_written,
					 std::chrono::system_clock::time_point& tp1, 
					 std::chrono::system_clock::time_point& tp2,
					 wchar_t* screen,
					 Player* player, 
					 const std::map<int, Player*>* other_players = nullptr);
};

