/*
 *
 * File: rendering.cpp
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#include "../include/rendering.h"


namespace rendering
{
	// Calculates the contact angle and moves player parallel to the obstacle.

	void Collision(const std::vector<std::vector<bool>>& map, Player* player, float elapsed_time, char key)
	{
		if (key == 'W')
		{
			if (sinf(player->rotation) > 0 && cosf(player->rotation) > 0) {
				player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) { 
					player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int>(player->pos.y)])
					player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;

			}
			else if (sinf(player->rotation) > 0 && cosf(player->rotation) < 0) {
				player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) {
					player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
					player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;
			}
			else if (sinf(player->rotation) < 0 && cosf(player->rotation) < 0) {
				player->pos.y += sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) {
					player->pos.y -= sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.x += cosf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
					player->pos.x -= cosf(player->rotation) * player->kSpeed * elapsed_time;
			}
			else if (sinf(player->rotation) < 0 && cosf(player->rotation) > 0) {
				player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) {
					player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
					player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;
			}

		}
		else
		{ // key == 'S'
			if (sinf(player->rotation) > 0 && cosf(player->rotation) > 0) {
				player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) {
					player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.y -= sinf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
					player->pos.y += sinf(player->rotation) * player->kSpeed * elapsed_time;
			}
			else if (sinf(player->rotation) > 0 && cosf(player->rotation) < 0) {
				player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) {
					player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
					player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;
			}
			else if (sinf(player->rotation) < 0 && cosf(player->rotation) < 0) {
				player->pos.y -= sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) {
					player->pos.y += sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.x -= cosf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
					player->pos.x += cosf(player->rotation) * player->kSpeed * elapsed_time;
			}
			else if (sinf(player->rotation) < 0 && cosf(player->rotation) > 0) {
				player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)]) {
					player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
					player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;
				}
				if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
					player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;
			}
		}
	}

	void CalculatePosition(GamingInfo& game_info)
	{
		const float& elapsed_time = get<0>(game_info);
		Player* player = get<1>(game_info);
		auto& last_firing_time = get<2>(game_info);
		network::ClientGame* client = get<3>(game_info);

		// Handle CCW Rotation
		if (GetAsyncKeyState(static_cast<unsigned short> ('A')) & 0x8000)
			player->rotation -= (player->kSpeed * 0.5f) * elapsed_time;

		// Handle CW Rotation
		if (GetAsyncKeyState(static_cast<unsigned short> ('D')) & 0x8000)
			player->rotation += (player->kSpeed * 0.5f) * elapsed_time;

		// Handle Forwards movement & collision
		if (GetAsyncKeyState(static_cast<unsigned short> ('W')) & 0x8000)
		{
			player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
			player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;

			// Collision
			if (Configs::map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
			{
				player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
				player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;

				Collision(Configs::map, player, elapsed_time, 'W');
			}
		}

		// Handle backwards movement & collision
		if (GetAsyncKeyState(static_cast<unsigned short> ('S')) & 0x8000)
		{
			player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
			player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;

			// Collision
			if (Configs::map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
			{
				player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
				player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;

				Collision(Configs::map, player, elapsed_time, 'S');
			}
		}

		// Handle firing
		if (GetAsyncKeyState(static_cast<unsigned short> (VK_RETURN)) & 0x8000)
		{
			std::chrono::duration<float> cur_reloading_time = std::chrono::system_clock::now() - last_firing_time;
			float fcur_reloading_time = cur_reloading_time.count();
			if (fcur_reloading_time > Configs::gun_reloading)
				last_firing_time = std::chrono::system_clock::now();
			if (client) {
				client->SendShootingInfo();
			}
		}
	}

	void RenderFrame(GamingInfo& game_info, ConsoleInfo& console_info)
	{
		const float elapsed_time = get<0>(game_info);
		Player* player = get<1>(game_info);
		auto& last_firing_time = get<2>(game_info);
		network::ClientGame*client = get<3>(game_info);

		HANDLE console = get<0>(console_info);
		DWORD bytes_written = get<1>(console_info);
		wchar_t* screen = get<2>(console_info);

		
		for (int x = 0; x < Configs::screen_width; x++)
		{
			// For each column, calculate the projected ray angle into world space
			float ray_angle = (player->rotation - player->kFOV / 2.0f) + (static_cast<float> (x) / static_cast<float> (Configs::screen_width)) * player->kFOV;

			// Find distance to wall
			float step_size = 0.1f;		      // Increment size for ray casting, decrease to increase resolution						
			float distance_to_obstacle = 0.0f; //														

			bool hit_wall = false;			// Set when ray hits wall block
			bool hit_wall_boundary = false;		// Set when ray hits boundary between two wall blocks
			bool hit_player = false;		// Set when ray hits player silhouette
			bool hit_player_boundary = false;	// Set when ray hits boundary between two sides of player silhouette

			float direction_vector_x = sinf(ray_angle); // Unit vector for ray in player space
			float direction_vector_y = cosf(ray_angle);

			// Incrementally cast ray from player, along ray angle, testing for 
			// intersection with a block or a player
			while (!hit_wall && !hit_player && distance_to_obstacle < player->kDepth)
			{
				distance_to_obstacle += step_size;
				float cur_ray_pos_x = player->pos.x + direction_vector_x * distance_to_obstacle;
				float cur_ray_pos_y = player->pos.y + direction_vector_y * distance_to_obstacle;

				// Test if ray is out of bounds
				if (cur_ray_pos_x < 0 || cur_ray_pos_x >= Configs::map_width || cur_ray_pos_y < 0 || cur_ray_pos_y >= Configs::map_height)
				{
					hit_wall = true;			// Just set distance to maximum kDepth
					distance_to_obstacle = player->kDepth;
				}
				else
					// Ray is inbounds so test to see if the ray cell is a wall block
					if (Configs::map[static_cast<int> (cur_ray_pos_x)][static_cast<int> (cur_ray_pos_y)])
					{
						// Ray has hit wall
						hit_wall = true;

						// To highlight tile boundaries, cast a ray from each corner
						// of the tile, to the player-> The more coincident this ray
						// is to the rendering ray, the closer we are to a tile 
						// boundary, which we'll shade to add detail to the walls
						std::vector<std::pair<float, float>> p;

						// Test each corner of hit tile, storing the distance from
						// the player, and the calculated dot product of the two rays
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// Angle of corner to eye
								float vy = static_cast<int> (cur_ray_pos_y) + ty - player->pos.y;
								float vx = static_cast<int> (cur_ray_pos_x) + tx - player->pos.x;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (direction_vector_x * vx / d) + (direction_vector_y * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// First two/three are closest (we will never see all four)
						float bound = 0.01;
						if (acos(p.at(0).second) < bound) hit_wall_boundary = true;
						if (acos(p.at(1).second) < bound) hit_wall_boundary = true;
						if (acos(p.at(2).second) < bound) hit_wall_boundary = true;
					}
					else // Ray is inbounds so test to see if the ray cell is a player
					{
						if (client)
							for (auto iter : client->other_players_)
							{
								Player* cur_player = iter.second;
								if (distance(cur_player->pos.x, cur_player->pos.y, player->pos.x, player->pos.y) > Configs::depth)	// if player isn't in the reach zone
									continue;
								else
								{
									cur_player->CaclulatePositions();
									// Test to see if the ray cell is in the player 
									// Check all sides of the player
									if (cross(player->pos.x, player->pos.y, cur_ray_pos_x, cur_ray_pos_y,
										cur_player->left_front.x, cur_player->left_front.y,
										cur_player->right_front.x, cur_player->right_front.y, cur_ray_pos_x, cur_ray_pos_y) ||

										cross(player->pos.x, player->pos.y, cur_ray_pos_x, cur_ray_pos_y,
											cur_player->left_front.x, cur_player->left_front.y,
											cur_player->left_back.x, cur_player->left_back.y, cur_ray_pos_x, cur_ray_pos_y) ||

										cross(player->pos.x, player->pos.y, cur_ray_pos_x, cur_ray_pos_y,
											cur_player->left_back.x, cur_player->left_back.y,
											cur_player->right_back.x, cur_player->right_back.y, cur_ray_pos_x, cur_ray_pos_y) ||

										cross(player->pos.x, player->pos.y, cur_ray_pos_x, cur_ray_pos_y,
											cur_player->right_back.x, cur_player->right_back.y,
											cur_player->right_front.x, cur_player->right_front.y, cur_ray_pos_x, cur_ray_pos_y))
									{
										hit_player = true;
										break;
									}
								}
							}
						if (hit_player)
						{
							// To highlight tile boundaries, cast a ray from each corner
							// of the player, to the camera. The more coincident this ray
							// is to the rendering ray, the closer we are to a player 
							// boundary, which we'll shade to add detail to the silhouette
							std::vector<std::pair<float, float>> p;

							// Test each corner of hit player, storing the distance from
							// the camera, and the calculated dot product of the two rays
							for (int tx = 0; tx < 2; tx++)
								for (int ty = 0; ty < 2; ty++)
								{
									// Angle of corner to eye
									float vy = cur_ray_pos_y + ty - player->pos.y;
									float vx = cur_ray_pos_x + tx - player->pos.x;
									float d = sqrt(vx * vx + vy * vy);
									float dot = (direction_vector_x * vx / d) + (direction_vector_y * vy / d);
									p.push_back(std::make_pair(d, dot));
								}

							// Sort Pairs from closest to farthest
							sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

							// First two/three are closest (we will never see all four)
							float bound = 0.01;
							if (acos(p.at(0).second) < bound) hit_player_boundary = true;
							if (acos(p.at(1).second) < bound) hit_player_boundary = true;
							if (acos(p.at(2).second) < bound) hit_player_boundary = true;
						}
					}
			}

			// Calculate distance to ceiling and floor
			int celling_dist = static_cast<float> (Configs::screen_height / 2.0) - Configs::screen_height / (static_cast<float> (distance_to_obstacle));
			int floor_dist = Configs::screen_height - celling_dist;

			short pixel = ' ';

			if (!hit_player)
			{
				// Shaders walls based on distance
				if (distance_to_obstacle <= player->kDepth / 4.0f)			pixel = 0x2588;		// Very close	
				else if (distance_to_obstacle < player->kDepth / 3.0f)		pixel = 0x2593;
				else if (distance_to_obstacle < player->kDepth / 2.0f)		pixel = 0x2592;
				else if (distance_to_obstacle < player->kDepth)				pixel = 0x2591;
				else														pixel = ' ';		// Too far away

				if (hit_wall_boundary)		pixel = ' '; // Black it out
			}
			else
			{
				// Shaders player based on distance
				if (distance_to_obstacle <= player->kDepth / 4.0f)			pixel = 0x2584;	    // Very close	
				else if (distance_to_obstacle < player->kDepth / 3.0f)		pixel = 0x2663;
				else if (distance_to_obstacle < player->kDepth / 2.0f)		pixel = 0x25BC;
				else if (distance_to_obstacle < player->kDepth)				pixel = 0x2642;
				else														pixel = ' ';		// Too far away

				if (hit_player_boundary)		pixel = ' '; // make border
			}

			for (int y = 0; y < Configs::screen_height; y++)
			{
				// Each Row
				if (y <= celling_dist)
					screen[y * Configs::screen_width + x] = ' ';
				else if (y > celling_dist && y <= floor_dist)
					screen[y * Configs::screen_width + x] = pixel;
				else // Floor
				{
					// Shades floor based on distance
					float b = 1.0f - ((static_cast<float>(y) - Configs::screen_height / 2.0f) / (static_cast<float>(Configs::screen_height) / 2.0f));
					if (b < 0.25)		pixel = '#';
					else if (b < 0.5)	pixel = 'x';
					else if (b < 0.75)	pixel = '.';
					else if (b < 0.9)	pixel = '-';
					else				pixel = ' ';
					screen[y * Configs::screen_width + x] = pixel;
				}
			}
		}

		// Display the sight
		int map_center_x = Configs::screen_width / 2,
			   map_center_y = Configs::screen_height / 2;

		int sight_thickness_v = Configs::screen_width / 100,
			sight_thickness_h = Configs::screen_height / 50;

		int left_top_vline_corner_x = map_center_x - sight_thickness_v / 2,
			left_top_vline_corner_y = map_center_y - Configs::screen_height / 40,
			left_bottom_vline_corner_y = map_center_y + Configs::screen_height / 40,

			left_top_hline_corner_x = map_center_x - Configs::screen_width / 80,
			left_top_hline_corner_y = map_center_y - sight_thickness_h / 2,
			right_top_hline_corner_x = map_center_x + Configs::screen_width / 80;

		for (int ny = left_top_vline_corner_y; ny < left_bottom_vline_corner_y; ny++)
			for (int nx = left_top_vline_corner_x; nx < map_center_x + sight_thickness_v / 2; nx++)
			{
				screen[ny * Configs::screen_width + nx] = 0x263B;
			}

		for (int ny = left_top_hline_corner_y; ny < map_center_y + sight_thickness_h / 2; ny++)
			for (int nx = left_top_hline_corner_x; nx < right_top_hline_corner_x; nx++)
			{
				screen[ny * Configs::screen_width + nx] = 0x263B;
			}


		// Display the health line
		int health_line_thickness = Configs::screen_height / 50;
		int health_line_width = Configs::screen_width * player->health / 100;
		for (int ny = Configs::screen_height - health_line_thickness - 1; ny < Configs::screen_height; ny++) {
			for (int nx = 0; nx < health_line_width; nx++)
				screen[ny * Configs::screen_width + nx] = 0x2588;
			for (int nx = health_line_width; nx < Configs::screen_width; nx++)
				screen[ny * Configs::screen_width + nx] = 0x2661;
		}


		// Display explosions
		std::chrono::duration<float> cur_reloading_time = std::chrono::system_clock::now() - last_firing_time;
		float fcur_reloading_time = cur_reloading_time.count();

		if (fcur_reloading_time > 0 && fcur_reloading_time < 0.1) // Small explosion
		{
			int expl_step_x = Configs::screen_width / 70,
				expl_step_y = Configs::screen_height / 40;

			for (int ty = Textures::kSmallExplosionHeight, y = Configs::screen_height / 4 * 3; ty > 0; ty -= expl_step_y, y--)
				for (int tx = 1, x = map_center_x + Configs::screen_width / 8; tx < Textures::kSmallExplosionWidth; tx += expl_step_x, x++) {
					if (Textures::small_explosion[ty - 1][tx - 1] != ' ')
						screen[y * Configs::screen_width + x] = Textures::small_explosion[ty - 1][tx - 1];
					else
						continue;
				}
		}
		else
			if (fcur_reloading_time > 0.1 && fcur_reloading_time < 0.2) // Middle explosion
			{
				int expl_step_x = Configs::screen_width / 70,
					expl_step_y = Configs::screen_height / 40;

				for (int ty = Textures::kMiddleExplosionHeight, y = Configs::screen_height / 4 * 3.2; ty > 0; ty -= expl_step_y, y--)
					for (int tx = 1, x = map_center_x + Configs::screen_width / 9; tx < Textures::kMiddleExplosionWidth; tx += expl_step_x, x++) {
						if (Textures::middle_explosion[ty - 1][tx - 1] != ' ')
							screen[y * Configs::screen_width + x] = Textures::middle_explosion[ty - 1][tx - 1];
						else
							continue;
					}
			}
			else
				if (fcur_reloading_time > 0.2 && fcur_reloading_time < 0.3) // Big explosion
				{
					int expl_step_x = Configs::screen_width / 70,
						expl_step_y = Configs::screen_height / 40;

					for (int ty = Textures::kBigExplosionHeight, y = Configs::screen_height / 4 * 3.3; ty > 0; ty -= expl_step_y, y--)
						for (int tx = 1, x = map_center_x + Configs::screen_width / 10; tx < Textures::kBigExplosionWidth; tx += expl_step_x, x++) {
							if (Textures::big_explosion[ty - 1][tx - 1] != ' ')
								screen[y * Configs::screen_width + x] = Textures::big_explosion[ty - 1][tx - 1];
							else
								continue;
						}
				}

		// Distplay the gun
		int gun_step_x = Configs::screen_width / 70,
			gun_step_y = Configs::screen_height / 40;

		for (int ty = Textures::kGunHeight - 1, y = Configs::screen_height - health_line_thickness * 2; ty > 0; ty -= gun_step_y, y--)
			for (int tx = 1, x = map_center_x + Configs::screen_width / 10; tx < Textures::kGunWidth; tx += gun_step_x, x++) {
				if (Textures::gun[ty][tx - 1] != '&')
					screen[y * Configs::screen_width + x] = Textures::gun[ty][tx - 1];
				else
					continue;
			}


		
		// Display Stats
		swprintf_s(screen, 50, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", player->pos.x, player->pos.y, player->rotation, 1.0f / elapsed_time);

		// Display Map
		for (int nx = 0; nx < Configs::map_width; nx++)
			for (int ny = 0; ny < Configs::map_width; ny++)
			{
				screen[(ny + 1) * Configs::screen_width + nx] = Configs::map[ny][nx] ? '#' : ' ';
			}
		screen[(static_cast<int>(player->pos.x) + 1) * Configs::screen_width + static_cast<int>(player->pos.y)] = 'P';

		// Display Frame
		screen[Configs::screen_width * Configs::screen_height - 1] = '\0';
		WriteConsoleOutputCharacter(console, screen, Configs::screen_width * Configs::screen_height, { 0,0 }, &bytes_written);
	}

	float distance(float x1, float y1, float x2, float y2)
	{
		return sqrtf(powf((x1 - x2), 2) + powf((y1 - y2), 2));
	}

	bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y)
	{
		float Ua, Ub, numerator_a, numerator_b, denominator;

		denominator = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);

		if (denominator == 0) {
			return false;
		}
		else {
			numerator_a = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
			numerator_b = (x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
			Ua = numerator_a / denominator;
			Ub = numerator_b / denominator;

			if (Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1) {
				x = x1 + x2 * Ua;
				y = y1 + y2 * Ua;
				return true;
			}
			else
				return false;
		}
	}
}
