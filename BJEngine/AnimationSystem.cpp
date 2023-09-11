#include "AnimationSystem.h"

#include "Animator.h"

using namespace std;

namespace BJEngine
{
	void AnimationSystem::AddAnimator(Animator* animator)
	{
		animatorList.push_back(animator);
	}

	void AnimationSystem::RemoveAnimator(Animator* animator)
	{
		animatorList.erase(remove(animatorList.begin(), animatorList.end(), animator), animatorList.end());
	}

	void AnimationSystem::ExecuteAllAnimator()
	{
		for (int i = 0; i < animatorList.size(); i++)
		{
			animatorList[i]->Execute();
		}
	}

	AnimationSystem& AnimationSystem::GetInstance()
	{
		static AnimationSystem instance;
		return instance;
	}

	AnimationSystem::AnimationSystem()
	{
		
	}

	AnimationSystem::~AnimationSystem()
	{

	}
}