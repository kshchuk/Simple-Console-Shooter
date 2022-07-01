#pragma once

#include <fstream>

const class Textures
{
public:
	char** gun;
	size_t gun_file_width = 401,
		gun_file_height = 120;

	char** small_explosion;
	size_t small_explosion_file_width = 81,
		small_explosion_file_height = 43;

	char** middle_explosion;
	size_t middle_explosion_file_width = 151,
		middle_explosion_file_height = 70;

	char** big_explosion;
	size_t big_explosion_file_width = 201,
		big_explosion_file_height = 80;

	// TODO:
	char** wall;
	size_t wall_file_width,
		wall_file_height;

	Textures();
};

