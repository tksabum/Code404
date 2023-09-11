#include "Animation.h"

#include "GraphicsResourceManager.h"

using namespace std;

namespace BJEngine
{
	using namespace GraphicsEngine;

	Animation::Animation(wstring spritename, float frameperduration, bool loop)
	{
		Sprite* sprite = GraphicsResourceManager::GetInstance().GetSprite(spritename);
		int size = sprite->GetCountX() * sprite->GetCountY();
		animationFrameCount = size;

		spriteName = vector<wstring>(size, spritename);

		spriteNumber = vector<int>(size);
		for (int i = 0; i < size; i++)
		{
			spriteNumber[i] = i;
		}

		totalFramePerDuration = vector<float>(size);
		float total = frameperduration;
		for (int i = 0; i < size; i++)
		{
			totalFramePerDuration[i] = total;
			total += frameperduration;
		}

		animationLoop = loop;
	}

	wstring Animation::GetSpriteName(int framenumber)
	{
		return spriteName[framenumber];
	}

	int Animation::GetSpriteNumber(int framenumber)
	{
		return spriteNumber[framenumber];
	}

	float Animation::GetTotalDuration(int framenumber)
	{
		return totalFramePerDuration[framenumber];
	}

	int Animation::GetFrameCount()
	{
		return animationFrameCount;
	}

	bool Animation::GetLoop()
	{
		return animationLoop;
	}
}
