/*
 *
 * File: textures.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#pragma once

#include <fstream>
#include <vector>


// Is using for storing textures information
static class Textures
{
private:
	static std::vector<std::vector<char>> gun_, 
		small_explosion_, 
		middle_explosion_, 
		big_explosion_;

public:
	
	// Size of textures in files

	static constexpr size_t kGunWidth = 401, kGunHeight = 120;
	static constexpr const std::vector<std::vector<char>>& gun = gun_;

	static constexpr size_t kSmallExplosionWidth = 81, kSmallExplosionHeight = 43;
	static constexpr const std::vector<std::vector<char>>& small_explosion = small_explosion_;

	static constexpr size_t kMiddleExplosionWidth = 151, kMiddleExplosionHeight = 70;
	static constexpr const std::vector<std::vector<char>>& middle_explosion = middle_explosion_;

	static constexpr size_t kBigExplosionWidth = 201, kBigExplosionHeight = 80;
	static constexpr const std::vector<std::vector<char>>& big_explosion = big_explosion_;

	// Reads information from .txt files.
	Textures();
};
