#pragma once

#ifdef ANIMATOR_EXPORTS
#define ANIMATOR_API __declspec(dllexport)
#else
#define ANIMATOR_API __declspec(dllimport)
#endif

#include <string>

#include "Component.h"

namespace BJEngine
{
	class CoreSystem;
	class Animation;
	class AnimationSystem;
	class SpriteRenderer;

	class Animator : public Component
	{
	public:
		ANIMATOR_API Animator();
		ANIMATOR_API ~Animator();

		ANIMATOR_API void Play(std::wstring animationname);
		ANIMATOR_API bool IsWait();

		ANIMATOR_API std::wstring GetAnimationName();
		ANIMATOR_API int GetFrameNumber();

	private:
		float playTime;
		int frameNumber;

		Animation* animation;
		SpriteRenderer* spriteRenderer;

		std::wstring lastAnimationName;

		void Execute();

		friend AnimationSystem;
	};
}
