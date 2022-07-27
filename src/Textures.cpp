/*
 *
 * File: textures.cpp
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#include "../include/info/textures.h"

#include <string>
#include <iostream>


// Static members initialization

std::vector<std::vector<char>> Textures::gun_;
std::vector<std::vector<char>> Textures::small_explosion_;
std::vector<std::vector<char>> Textures::middle_explosion_;
std::vector<std::vector<char>> Textures::big_explosion_;

// Reads information from .txt files which are located in 'textures' folder.
Textures::Textures()
{
	std::ifstream gunf("textures/gun.txt");

	if (gunf)
	{
		gun_.resize(kGunHeight);
		for (size_t i = 0; i < kGunHeight; i++)
		{
			gun_[i].resize(kGunWidth);
			for (size_t j = 0; j < kGunWidth; j++) {
				gun_[i][j] = gunf.get();
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
		small_explosion_.resize(kSmallExplosionHeight);
		for (size_t i = 0; i < kSmallExplosionHeight; i++)
		{
			//std::cout << '\n';
			small_explosion_[i].resize(kSmallExplosionWidth);
			for (size_t j = 0; j < kSmallExplosionWidth; j++) {
				small_explosion_[i][j] = s_exp.get();
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
		middle_explosion_.resize(kMiddleExplosionHeight);
		for (size_t i = 0; i < kMiddleExplosionHeight; i++)
		{
			//std::cout << '\n';
			middle_explosion_[i].resize(kMiddleExplosionWidth);
			for (size_t j = 0; j < kMiddleExplosionWidth; j++) {
				middle_explosion_[i][j] = m_exp.get();
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
		big_explosion_.resize(kBigExplosionHeight);
		for (size_t i = 0; i < kBigExplosionHeight; i++)
		{
			//std::cout << '\n';
			big_explosion_[i].resize(kBigExplosionWidth);
			for (size_t j = 0; j < kBigExplosionWidth; j++) {
				big_explosion_[i][j] = b_exp.get();
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
