#include "Textures.h"
#include "string"
#include <iostream>


Textures::Textures()
{
	// Open and save in the memory gun texture file
	std::ifstream gunf("gun.txt");

	if (gunf)
	{
		gun = new char* [gun_file_height];
		for (size_t i = 0; i < gun_file_height; i++)
		{
			gun[i] = new char[gun_file_width];
			for (int j = 0; j < gun_file_width; j++) {
				gun[i][j] = gunf.get();
			}
		}
		gunf.close();
	}
}
