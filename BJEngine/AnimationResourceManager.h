#pragma once

#ifdef ANIMATIONRESOURCEMANAGER_EXPORTS
#define ANIMATIONRESOURCEMANAGER_API __declspec(dllexport)
#else
#define ANIMATIONRESOURCEMANAGER_API __declspec(dllimport)
#endif

#include <string>
#include <unordered_map>

namespace BJEngine
{
	class Animation;

	class AnimationResourceManager
	{
	public:
		ANIMATIONRESOURCEMANAGER_API Animation* CreateAnimation(std::wstring animationname, std::wstring spritename, float frametime, bool loop);
		Animation* GetAnimation(std::wstring animationname);

	private:
		std::unordered_map<std::wstring, Animation*> animationList;

		//singleton
	public:
		ANIMATIONRESOURCEMANAGER_API static AnimationResourceManager& GetInstance();

	private:
		AnimationResourceManager();
		~AnimationResourceManager();
		AnimationResourceManager(const AnimationResourceManager& ref);
		AnimationResourceManager& operator=(const AnimationResourceManager& ref) {}
	};
}

