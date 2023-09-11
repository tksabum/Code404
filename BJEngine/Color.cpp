#include "Color.h"
#include <cmath>

namespace BJEngine
{
	namespace GraphicsEngine
	{
		Color::Color(float r, float g, float b, float a)
		{
			rgb = ((int)round(255.0f * r) * 0x010000) + ((int)round(255.0f * g) * 0x000100) + ((int)round(255.0f * b) * 0x000001);
			alpha = a;
		}

		Color::Color(int rgb, float a) : rgb(rgb), alpha(a)
		{

		}

		Color::Color(Enum knownColor, float a)
		{
			rgb = knownColor;
			alpha = a;
		}

		D2D1::ColorF Color::ConvertD2DColor()
		{
			return D2D1::ColorF(rgb, alpha);
		}
	}
}