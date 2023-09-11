#pragma once

#ifdef ANIMATIONSYSTEM_EXPORTS
#define ANIMATIONSYSTEM_API __declspec(dllexport)
#else
#define ANIMATIONSYSTEM_API __declspec(dllimport)
#endif

#include <vector>

namespace BJEngine
{
	class Animator;
	class CoreSystem;

	class AnimationSystem
	{
	public:
		ANIMATIONSYSTEM_API void AddAnimator(Animator* animator);
		ANIMATIONSYSTEM_API void RemoveAnimator(Animator* animator);

	private:
		std::vector<Animator*> animatorList;

		void ExecuteAllAnimator();

		//singleton
	public:
		ANIMATIONSYSTEM_API static AnimationSystem& GetInstance();

	private:
		AnimationSystem();
		~AnimationSystem();
		AnimationSystem(const AnimationSystem& ref);
		AnimationSystem& operator=(const AnimationSystem& ref) {}

		friend CoreSystem;
	};
}