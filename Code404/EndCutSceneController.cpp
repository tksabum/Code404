#include "EndCutSceneController.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"
#include "SpriteRenderer.h"
#include "BJMath.h"
#include "TimeController.h"

#include "Input.h"

#include "CoreSystem.h"
#include "SceneManager.h"

#include "SoundManager.h"

#include <cmath>

using namespace BJEngine;
using namespace BJEngine::Math;

void EndCutSceneController::SetCutScene1(BJEngine::GameObject* cutscene1)
{
	cutScene1 = cutscene1;
}

void EndCutSceneController::SetCutScene2(BJEngine::GameObject* cutscene2)
{
	cutScene2 = cutscene2;
}

void EndCutSceneController::SetCutScene3(BJEngine::GameObject* cutscene3)
{
	cutScene3 = cutscene3;
}

void EndCutSceneController::SetCutScene4(BJEngine::GameObject* cutscene4)
{
	cutScene4 = cutscene4;
}

void EndCutSceneController::SetBlackMask(BJEngine::GameObject* blackmask)
{
	blackMask = blackmask;
}

void EndCutSceneController::SetChangekMask(BJEngine::GameObject* changemask)
{
	changeMask = changemask;
}

void EndCutSceneController::SetAnyKek(BJEngine::GameObject* anykey)
{
	anyKey = anykey;
}

void EndCutSceneController::Awake()
{
	cutScene1Renderer = cutScene1->GetComponent<SpriteRenderer>();
	cutScene2Renderer = cutScene2->GetComponent<SpriteRenderer>();
	cutScene3Renderer = cutScene3->GetComponent<SpriteRenderer>();
	blackMaskRenderer = blackMask->GetComponent<SpriteRenderer>();
	cutScene4Renderer = cutScene4->GetComponent<SpriteRenderer>();
	changeMaskRenderer = changeMask->GetComponent<SpriteRenderer>();
	anyKeyRenderer = anyKey->GetComponent<SpriteRenderer>();
}

void EndCutSceneController::Update()
{
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();

	if (isCutScene1)
	{
		cutScene1Opacity += cutScene1OpacitySpeed * deltaTime;
		if (cutScene1Opacity >= 1.f)
		{
			isCutScene1 = false;
			isCutScene2 = true;
			cutScene1Opacity = 1.f;
		}
		cutScene1Renderer->SetOpacity(cutScene1Opacity);
	}
	
	if (isCutScene2)
	{
		cutScene2Opacity += cutScene2OpacitySpeed * deltaTime;
		if (cutScene2Opacity >= 1.f)
		{
			isCutScene2 = false;
			isCutScene3 = true;
			cutScene2Opacity = 1.f;
		}
		cutScene2Renderer->SetOpacity(cutScene2Opacity);
	}

	if (isCutScene3)
	{
		cutScene3Opacity += cutScene3OpacitySpeed * deltaTime;
		if (cutScene3Opacity >= 1.f)
		{
			isCutScene3 = false;
			isblackMask = true;
			cutScene3Opacity = 1.f;
		}
		cutScene3Renderer->SetOpacity(cutScene3Opacity);
	}

	if (isblackMask)
	{
		blackMaskOpacity += blackMaskOpacitySpeed * deltaTime;
		if (blackMaskOpacity >= 1.f)
		{
			isblackMask = false;
			isCutScene4 = true;
			blackMaskOpacity = 1.f;
		}
		blackMaskRenderer->SetOpacity(blackMaskOpacity);
	}

	if (isCutScene4)
	{
		cutScene4Opacity += cutScene4OpacitySpeed * deltaTime;
		if (cutScene4Opacity >= 1.f)
		{
			isCutScene4 = false;
			isAnyWait = true;
			cutScene4Opacity = 1.f;
		}
		cutScene4Renderer->SetOpacity(cutScene4Opacity);
	}

	if (isAnyWait)
	{
		totalAnyWaitTime += deltaTime;
		if (totalAnyWaitTime >= anyWaitTime)
		{
			totalAnyWaitTime -= anyWaitTime;
			isAnyWait = false;
			ischangeMask = true;
		}
	}

	if (ischangeMask)
	{
		changeMaskOpacity += changeMaskOpacitySpeed * deltaTime;
		if (changeMaskOpacity >= 1.f)
		{
			ischangeMask = true;
			isAnykey = true;
			isAnyKeyDraw = true;
		}
		changeMaskRenderer->SetOpacity(changeMaskOpacity);
	}

	if (isAnyKeyDraw)
	{
		if (!isPingPong)
		{
			anyKeyOpacity += anyKeyOpacitySpeed * deltaTime;
			if (anyKeyOpacity >= 1.f)
			{
				isPingPong = true;
			}
		}
		else
		{
			anyKeyOpacity -= anyKeyOpacitySpeed * deltaTime;
			if (anyKeyOpacity <= 0.4f)
			{
				isPingPong = false;
			}
		}
		anyKeyRenderer->SetOpacity(anyKeyOpacity);
	}
			
	if (isAnykey)
	{
		if (Input::GetInstance().PressedAnyKeyNow())
		{
			SoundManager::GetInstance().Play(SOUND_EFFECT_PARRYING);
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"MainSceneAnimation");
		}
	}
}
