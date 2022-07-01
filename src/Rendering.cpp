#include "../include/rendering.h"

namespace rendering
{

	void Collision(float elapsed_time, char key, const std::vector<std::vector<bool>>& map, Player* player)
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

	void CalculatePosition(const std::vector<std::vector<bool>>& map,
							const Configs* configs,
							float elapsed_time,
							std::chrono::system_clock::time_point& last_firing_time,
							network::ClientGame* client,
							Player* player)
	{
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
			if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
			{
				player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
				player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;

				Collision(elapsed_time, 'W', map, player);
			}
		}

		// Handle backwards movement & collision
		if (GetAsyncKeyState(static_cast<unsigned short> ('S')) & 0x8000)
		{
			player->pos.x -= sinf(player->rotation) * player->kSpeed * elapsed_time;
			player->pos.y -= cosf(player->rotation) * player->kSpeed * elapsed_time;

			// Collision
			if (map[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)])
			{
				player->pos.x += sinf(player->rotation) * player->kSpeed * elapsed_time;
				player->pos.y += cosf(player->rotation) * player->kSpeed * elapsed_time;

				Collision(elapsed_time, 'S', map, player);
			}
		}

		// Handle firing
		if (GetAsyncKeyState(static_cast<unsigned short> (VK_RETURN)) & 0x8000)
		{
			std::chrono::duration<float> cur_reloading_time = std::chrono::system_clock::now() - last_firing_time;
			float fcur_reloading_time = cur_reloading_time.count();
			if (fcur_reloading_time > configs->gun_reloading)
				last_firing_time = std::chrono::system_clock::now();
			if (client) {
				client->SendShootingInfo();
			}
		}
	}

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
		const std::map<int, Player*>* other_players)
	{

		for (int x = 0; x < configs->screen_width; x++)
		{
			// For each column, calculate the projected ray angle into world space
			float ray_angle = (player->rotation - player->kFOV / 2.0f) + ((static_cast<float> (x) / static_cast<float> (configs->screen_width)) * player->kFOV);

			// Find distance to wall
			float step_size = 0.1f;		      // Increment size for ray casting, decrease to increase resolution						
			float distance_to_obstacle = 0.0f; //														

			bool hit_wall = false;			// Set when ray hits wall block
			bool hit_wall_boundary = false;		// Set when ray hits boundary between two wall blocks
			bool hit_player = false;		// Set when ray hits player silhouette
			bool hit_player_boundary = false;	// Set when ray hits boundary between two sides of player silhouette

			float unit_vector_for_a_ray_X = sinf(ray_angle); // Unit vector for ray in player space
			float unit_vector_for_a_ray_Y = cosf(ray_angle);

			// Incrementally cast ray from player, along ray angle, testing for 
			// intersection with a block or a player
			while (!hit_wall && !hit_player && distance_to_obstacle < player->kDepth)
			{
				distance_to_obstacle += step_size;
				float ray_pos_X = player->pos.x + unit_vector_for_a_ray_X * distance_to_obstacle;
				float ray_pos_Y = player->pos.y + unit_vector_for_a_ray_Y * distance_to_obstacle;

				// Test if ray is out of bounds
				if (ray_pos_X < 0 || ray_pos_X >= configs->map_width || ray_pos_Y < 0 || ray_pos_Y >= configs->map_height)
				{
					hit_wall = true;			// Just set distance to maximum depth
					distance_to_obstacle = player->kDepth;
				}
				else
					// Ray is inbounds so test to see if the ray cell is a wall block
					if (map[static_cast<int> (ray_pos_X)][static_cast<int> (ray_pos_Y)])
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
								float vy = static_cast<int> (ray_pos_Y + ty - player->pos.y);
								float vx = static_cast<int> (ray_pos_X + tx - player->pos.x);
								float d = sqrt(vx * vx + vy * vy);
								float dot = (unit_vector_for_a_ray_X * vx / d) + (unit_vector_for_a_ray_Y * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// First two/three are closest (we will never see all four)
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) hit_wall_boundary = true;
						if (acos(p.at(1).second) < fBound) hit_wall_boundary = true;
						if (acos(p.at(2).second) < fBound) hit_wall_boundary = true;
					}
					else // Ray is inbounds so test to see if the ray cell is a player
					{
						if (other_players)
							for (auto iter : *other_players)
							{
								Player* cur_player = iter.second;
								if (rendering::distance(cur_player->pos.x, cur_player->pos.y, player->pos.x, player->pos.y) > configs->depth)	// if player isn't in the reach zone
									continue;
								else
								{
									cur_player->CaclulatePositions();

									// Test to see if the ray cell is in the player 
									// Check all sides of the player

									if (rendering::cross(player->pos.x, player->pos.y, ray_pos_X, ray_pos_Y,
										cur_player->left_front.x, cur_player->left_front.y,
										cur_player->right_front.x, cur_player->right_front.y, ray_pos_X, ray_pos_Y) ||

										rendering::cross(player->pos.x, player->pos.y, ray_pos_X, ray_pos_Y,
											cur_player->left_front.x, cur_player->left_front.y,
											cur_player->left_back.x, cur_player->left_back.y, ray_pos_X, ray_pos_Y) ||

										rendering::cross(player->pos.x, player->pos.y, ray_pos_X, ray_pos_Y,
											cur_player->left_back.x, cur_player->left_back.y,
											cur_player->right_back.x, cur_player->right_back.y, ray_pos_X, ray_pos_Y) ||

										rendering::cross(player->pos.x, player->pos.y, ray_pos_X, ray_pos_Y,
											cur_player->right_back.x, cur_player->right_back.y,
											cur_player->right_front.x, cur_player->right_front.y, ray_pos_X, ray_pos_Y))
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
									float vy = ray_pos_Y + ty - player->pos.y;
									float vx = ray_pos_X + tx - player->pos.x;
									float d = sqrt(vx * vx + vy * vy);
									float dot = (unit_vector_for_a_ray_X * vx / d) + (unit_vector_for_a_ray_Y * vy / d);
									p.push_back(std::make_pair(d, dot));
								}

							// Sort Pairs from closest to farthest
							sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

							// First two/three are closest (we will never see all four)
							float fBound = 0.01;
							if (acos(p.at(0).second) < fBound) hit_player_boundary = true;
							if (acos(p.at(1).second) < fBound) hit_player_boundary = true;
							if (acos(p.at(2).second) < fBound) hit_player_boundary = true;
						}
					}
			}

			// Calculate distance to ceiling and floor
			int nCeiling = static_cast<float> (configs->screen_height / 2.0) - configs->screen_height / distance_to_obstacle;
			int nFloor = configs->screen_height - nCeiling;

			short nShade = ' ';

			if (!hit_player)
			{
				// Shader walls based on distance
				if (distance_to_obstacle <= player->kDepth / 4.0f)			nShade = 0x2588;	// Very close	
				else if (distance_to_obstacle < player->kDepth / 3.0f)		nShade = 0x2593;
				else if (distance_to_obstacle < player->kDepth / 2.0f)		nShade = 0x2592;
				else if (distance_to_obstacle < player->kDepth)			nShade = 0x2591;
				else													nShade = ' ';		// Too far away

				if (hit_wall_boundary)		nShade = ' '; // Black it out
			}
			else
			{
				// Shader player based on distance
				if (distance_to_obstacle <= player->kDepth / 4.0f)			nShade = 0x2584;	// Very close	
				else if (distance_to_obstacle < player->kDepth / 3.0f)		nShade = 0x2663;
				else if (distance_to_obstacle < player->kDepth / 2.0f)		nShade = 0x25BC;
				else if (distance_to_obstacle < player->kDepth)			nShade = 0x2642;
				else													nShade = ' ';		// Too far away

				if (hit_player_boundary)		nShade = ' '; // make border
			}

			for (int y = 0; y < configs->screen_height; y++)
			{
				// Each Row
				if (y <= nCeiling)
					screen[y * configs->screen_width + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y * configs->screen_width + x] = nShade;
				else // Floor
				{
					// Shade floor based on distance
					float b = 1.0f - (((float)y - configs->screen_height / 2.0f) / ((float)configs->screen_height / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y * configs->screen_width + x] = nShade;
				}
			}
		}

		// Display the sight
		size_t map_center_x = configs->screen_width / 2,
			map_center_y = configs->screen_height / 2;

		size_t sight_thickness_v = configs->screen_width / 100,
			sight_thickness_h = configs->screen_height / 50;

		size_t left_top_vline_corner_x = map_center_x - sight_thickness_v / 2,
			left_top_vline_corner_y = map_center_y - configs->screen_height / 40,
			left_bottom_vline_corner_y = map_center_y + configs->screen_height / 40,

			left_top_hline_corner_x = map_center_x - configs->screen_width / 80,
			left_top_hline_corner_y = map_center_y - sight_thickness_h / 2,
			right_top_hline_corner_x = map_center_x + configs->screen_width / 80;

		for (int ny = left_top_vline_corner_y; ny < left_bottom_vline_corner_y; ny++)
			for (int nx = left_top_vline_corner_x; nx < map_center_x + sight_thickness_v / 2; nx++)
			{
				screen[ny * configs->screen_width + nx] = 0x263B;
			}

		for (int ny = left_top_hline_corner_y; ny < map_center_y + sight_thickness_h / 2; ny++)
			for (int nx = left_top_hline_corner_x; nx < right_top_hline_corner_x; nx++)
			{
				screen[ny * configs->screen_width + nx] = 0x263B;
			}


		// Display the health line
		size_t health_line_thickness = configs->screen_height / 50;
		size_t health_line_width = configs->screen_width * player->health / 100;
		for (int ny = configs->screen_height - health_line_thickness - 1; ny < configs->screen_height; ny++) {
			for (int nx = 0; nx < health_line_width; nx++)
				screen[ny * configs->screen_width + nx] = 0x2588;
			for (int nx = health_line_width; nx < configs->screen_width; nx++)
				screen[ny * configs->screen_width + nx] = 0x2661;
		}


		// Display explosions
		std::chrono::duration<float> cur_reloading_time = std::chrono::system_clock::now() - last_firing_time;
		float fcur_reloading_time = cur_reloading_time.count();

		if (fcur_reloading_time > 0 && fcur_reloading_time < 0.1) // Small explosion
		{
			int expl_step_x = configs->screen_width / 70,
				expl_step_y = configs->screen_height / 40;

			for (int ty = textures->small_explosion_file_height, y = configs->screen_height / 4 * 3; ty > 0; ty -= expl_step_y, y--)
				for (int tx = 1, x = map_center_x + configs->screen_width / 8; tx < textures->small_explosion_file_width; tx += expl_step_x, x++) {
					if (textures->small_explosion[ty - 1][tx - 1] != ' ')
						screen[y * configs->screen_width + x] = textures->small_explosion[ty - 1][tx - 1];
					else
						continue;
				}
		}
		else
			if (fcur_reloading_time > 0.1 && fcur_reloading_time < 0.2) // Middle explosion
			{
				int expl_step_x = configs->screen_width / 70,
					expl_step_y = configs->screen_height / 40;

				for (int ty = textures->middle_explosion_file_height, y = configs->screen_height / 4 * 3.2; ty > 0; ty -= expl_step_y, y--)
					for (int tx = 1, x = map_center_x + configs->screen_width / 9; tx < textures->middle_explosion_file_width; tx += expl_step_x, x++) {
						if (textures->middle_explosion[ty - 1][tx - 1] != ' ')
							screen[y * configs->screen_width + x] = textures->middle_explosion[ty - 1][tx - 1];
						else
							continue;
					}
			}
			else
				if (fcur_reloading_time > 0.2 && fcur_reloading_time < 0.3) // Big explosion
				{
					int expl_step_x = configs->screen_width / 70,
						expl_step_y = configs->screen_height / 40;

					for (int ty = textures->big_explosion_file_height, y = configs->screen_height / 4 * 3.3; ty > 0; ty -= expl_step_y, y--)
						for (int tx = 1, x = map_center_x + configs->screen_width / 10; tx < textures->big_explosion_file_width; tx += expl_step_x, x++) {
							if (textures->big_explosion[ty - 1][tx - 1] != ' ')
								screen[y * configs->screen_width + x] = textures->big_explosion[ty - 1][tx - 1];
							else
								continue;
						}
				}

		// Distplay the gun
		int gun_step_x = configs->screen_width / 90,
			gun_step_y = configs->screen_height / 40;

		for (int ty = textures->gun_file_height - 1, y = configs->screen_height - health_line_thickness * 2; ty > 0; ty -= gun_step_y, y--)
			for (int tx = 1, x = map_center_x + configs->screen_width / 10; tx < textures->gun_file_width; tx += gun_step_x, x++) {
				if (textures->gun[ty][tx - 1] != '&')
					screen[y * configs->screen_width + x] = textures->gun[ty][tx - 1];
				else
					continue;
			}



		// Display Stats
		swprintf_s(screen, 50, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", player->pos.x, player->pos.y, player->rotation, 1.0f / elapsed_time);

		// Display Map
		for (int nx = 0; nx < configs->map_width; nx++)
			for (int ny = 0; ny < configs->map_width; ny++)
			{
				screen[(ny + 1) * configs->screen_width + nx] = map[ny][nx] ? '#' : ' ';
			}
		screen[((int)player->pos.x + 1) * configs->screen_width + (int)player->pos.y] = 'P';

		// Display Frame
		screen[configs->screen_width * configs->screen_height - 1] = '\0';
		WriteConsoleOutputCharacter(console, screen, configs->screen_width * configs->screen_height, { 0,0 }, &bytes_written);
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
