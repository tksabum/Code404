#include "RandomNumberGenerator.h"
#include <cmath>


namespace BJEngine
{
	RandomNumberGenerator::RandomNumberGenerator() :
		mersenne(static_cast<unsigned int>(std::time(nullptr)))
	{

	}

	RandomNumberGenerator::~RandomNumberGenerator()
	{

	}

	float RandomNumberGenerator::GetRandomFloat(float low, float high)
	{
		return static_cast<float>(std::uniform_real_distribution<>(low, high)(mersenne));
	}

	float RandomNumberGenerator::GetRandomIntiger(int low, int high)
	{
		return static_cast<int>(std::uniform_int_distribution<>(low, high)(mersenne));
	}

	Math::Vector2D RandomNumberGenerator::GetRandomUnitVector()
	{
		float theta = GetRandomFloat(0.f, 3.141592f * 2.f);

		return Math::Vector2D(cos(theta), -sin(theta));
	}

	Math::Vector2D RandomNumberGenerator::GetRangeUnitVector(const Math::Vector2D& dir, float range)
	{
		Math::Vector2D temp = dir;

		float random = GetRandomFloat(-std::fabs(range) / 2.f, std::fabs(range) / 2.f);
		float tempX = temp.x;
		float tempY = temp.y;

		temp.x = tempX * cos(random) - tempY * sin(random);
		temp.y = tempX * sin(random) + tempY * cos(random);

		return temp;
	}
}
