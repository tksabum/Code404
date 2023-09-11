#include "Animator.h"

#include "AnimationResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"
#include "Animation.h"
#include "AnimationSystem.h"

using namespace std;

namespace BJEngine
{
	Animator::Animator() : spriteRenderer(nullptr), animation(nullptr), playTime(0.0f), frameNumber(0)
	{
		AnimationSystem::GetInstance().AddAnimator(this);
	}

	Animator::~Animator()
	{
		AnimationSystem::GetInstance().RemoveAnimator(this);
	}

	void Animator::Play(wstring animationname)
	{
		lastAnimationName = animationname;
		animation = AnimationResourceManager::GetInstance().GetAnimation(animationname);

		playTime = 0.0f;
		frameNumber = 0;
	}

	bool Animator::IsWait()
	{
		return !animation->GetLoop() && (animation->GetTotalDuration(animation->GetFrameCount() - 1) < playTime);
	}

	int Animator::GetFrameNumber()
	{
		return frameNumber;
	}

	wstring Animator::GetAnimationName()
	{
		return lastAnimationName;
	}

	void Animator::Execute()
	{
		if (animation == nullptr)
		{
			return;
		}

		if (spriteRenderer == nullptr)
		{
			spriteRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
		}

		if (animation->GetTotalDuration(frameNumber) < playTime)
		{
			frameNumber++;

			if (animation->GetTotalDuration(animation->GetFrameCount() - 1) < playTime)
			{
				if (animation->GetLoop())
				{
					playTime = 0.0f;
					frameNumber = 0;
				}
				else
				{
					frameNumber--;
				}
			}
		}

		spriteRenderer->SetImageNumber(animation->GetSpriteNumber(frameNumber));
		spriteRenderer->SetSpriteName(animation->GetSpriteName(frameNumber));

		playTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	}
}