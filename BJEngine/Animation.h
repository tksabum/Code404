#pragma once

#include <vector>
#include <string>

namespace BJEngine
{
	class Animation
	{
	public:
		// spritesheet���� 0 ~ (countX * countY)�� spriteNumber��
		Animation(std::wstring spritename, float frameperduration, bool loop);

		std::wstring GetSpriteName(int framenumber);
		int GetSpriteNumber(int framenumber);
		float GetTotalDuration(int framenumber);
		int GetFrameCount();
		bool GetLoop();


	private:
		std::vector<std::wstring> spriteName;
		std::vector<int> spriteNumber;
		std::vector<float> totalFramePerDuration;

		int animationFrameCount;
		bool animationLoop;

	};
}
