#pragma once
#include <fstream>

const class Textures
{
public:

	char** gun;
	size_t gun_file_width = 401,
		   gun_file_height = 120;

	// TODO:
	char** wall;
	size_t wall_file_width,
		wall_file_height;

	Textures();
};

