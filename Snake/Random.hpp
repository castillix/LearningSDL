#pragma once

#include <random>

class Random {
public:
	static int get(int min, int max) {
		static std::mt19937 generator(std::random_device{}());
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}
};