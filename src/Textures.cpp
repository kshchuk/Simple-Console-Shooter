#include "../include/info/Textures.h"

#include <string>
#include <iostream>


Textures::Textures()
{
	std::ifstream gunf("textures/gun.txt");

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
	else {
		std::cout << "ERROR There is no texture files\n";
		system("PAUSE");
	}


	std::ifstream s_exp("textures/small_explosion.txt");

	if (s_exp)
	{
		small_explosion = new char* [small_explosion_file_height];
		for (size_t i = 0; i < small_explosion_file_height; i++)
		{
			//std::cout << '\n';
			small_explosion[i] = new char[small_explosion_file_width];
			for (int j = 0; j < small_explosion_file_width; j++) {
				small_explosion[i][j] = s_exp.get();
				//std::cout << small_explosion[i][j];
			}
		}
		s_exp.close();
	}
	else
	{
		std::cout << "ERROR There is no texture files";
		system("PAUSE");
	}


	std::ifstream m_exp("textures/middle_explosion.txt");

	if (m_exp)
	{
		middle_explosion = new char* [middle_explosion_file_height];
		for (size_t i = 0; i < middle_explosion_file_height; i++)
		{
			//std::cout << '\n';
			middle_explosion[i] = new char[middle_explosion_file_width];
			for (int j = 0; j < middle_explosion_file_width; j++) {
				middle_explosion[i][j] = m_exp.get();
				//std::cout << middle_explosion[i][j];

			}
		}
		m_exp.close();
	}
	else
	{
		std::cout << "ERROR There is no texture files\n";
		system("PAUSE");
	}

	std::ifstream b_exp("textures/big_explosion.txt");

	if (b_exp)
	{
		big_explosion = new char* [big_explosion_file_height];
		for (size_t i = 0; i < big_explosion_file_height; i++)
		{
			//std::cout << '\n';
			big_explosion[i] = new char[big_explosion_file_width];
			for (int j = 0; j < big_explosion_file_width; j++) {
				big_explosion[i][j] = b_exp.get();
				//std::cout << big_explosion[i][j];
			}
		}
		b_exp.close();
	}
	else
	{
		std::cout << "ERROR There is no texture files\n";
		system("PAUSE");
	}

}
