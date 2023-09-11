#pragma once

#ifdef RANDOMNUMBERGENERATOR_EXPORTS
#define RANDOMNUMBERGENERATOR_API __declspec(dllexport)
#else
#define RANDOMNUMBERGENERATOR_API __declspec(dllimport)
#endif

#include "Vector2D.h"
#include <cstdlib>
#include <random>
#include <ctime>

namespace BJEngine
{

	class RandomNumberGenerator
	{
	public:
		RANDOMNUMBERGENERATOR_API RandomNumberGenerator();
		RANDOMNUMBERGENERATOR_API ~RandomNumberGenerator();

		RANDOMNUMBERGENERATOR_API float GetRandomFloat(float low, float high);
		RANDOMNUMBERGENERATOR_API float GetRandomIntiger(int low, int high);
		RANDOMNUMBERGENERATOR_API Math::Vector2D GetRandomUnitVector();
		RANDOMNUMBERGENERATOR_API Math::Vector2D GetRangeUnitVector(const Math::Vector2D& dir, float range);

	private:
		std::mt19937 mersenne;
	};
}

