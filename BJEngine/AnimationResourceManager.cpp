#include "AnimationResourceManager.h"

#include "GraphicsResourceManager.h"
#include "Sprite.h"
#include "Animation.h"

namespace BJEngine
{
	using namespace GraphicsEngine;

	Animation* AnimationResourceManager::CreateAnimation(std::wstring animationname, std::wstring spritename, float frameperduration, bool loop)
	{
		if (animationList.count(animationname) == 0)
		{
			animationList[animationname] = new Animation(spritename, frameperduration, loop);
		}

		return animationList[animationname];
	}

	Animation* AnimationResourceManager::GetAnimation(std::wstring animationname)
	{
		return animationList[animationname];
	}

	AnimationResourceManager& AnimationResourceManager::GetInstance()
	{
		static AnimationResourceManager instance;
		return instance;
	}

	AnimationResourceManager::AnimationResourceManager()
	{

	}

	AnimationResourceManager::~AnimationResourceManager()
	{

	}
}