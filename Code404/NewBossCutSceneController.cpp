#include "NewBossCutSceneController.h"

#include "GameObject.h"
#include "TimeController.h"
#include "SpriteRenderer.h"

#include "BJMath.h"
#include "Transform.h"
#include "Vector3D.h"

#include "ObjectPoolSystem.h"
#include "EffectPlayer.h"

#include "CameraShake.h"

#include "CoreSystem.h"
#include "CameraShake.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"

#include "Input.h"

#include "SoundManager.h"

#include <cmath>

using namespace BJEngine;
using namespace BJEngine::Math;

void NewBossCutSceneController::SetEye(SpriteRenderer* eye)
{
	eyeRenderer = eye;
}

void NewBossCutSceneController::SetHead(SpriteRenderer* head)
{
	headRenderer = head;
}

void NewBossCutSceneController::SetLeft(SpriteRenderer* left)
{
	leftRenderer = left;
}

void NewBossCutSceneController::SetRight(SpriteRenderer* right)
{
	rightRenderer = right;
}

void NewBossCutSceneController::SetMask(SpriteRenderer* mask)
{
	blackMask = mask;
}

void NewBossCutSceneController::Awake()
{
	leftHand = leftRenderer->GetGameObject();
	rightHand = rightRenderer->GetGameObject();
}

void NewBossCutSceneController::Update()
{
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
	if (Input::GetInstance().PressedKeyNow(VK_RETURN))
	{
		CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"SampleScene4");
	}
	if (!isWait)
	{
		totalWaitTime += deltaTime;
		if (totalWaitTime >= waitTime)
		{
			totalWaitTime -= waitTime;
			isEye = true;
		}
	}

	if (isEye)
	{
		eyeRenderer->SetOpacity(1);
		isEye = false;
		isEyeWait = true;
	}

	if (isEyeWait)
	{
		totalEyeWaitTime += deltaTime;
		if (totalEyeWaitTime >= eyeWaitTime)
		{
			totalEyeWaitTime -= eyeWaitTime;
			isEyeWait = false;
			isHead = true;
		}
	}

	if (isHead)
	{
		headOpacity += headOpacitySpeed * deltaTime;
		if (headOpacity >= 1.f)
		{
			headOpacity = 1.f;
			isHead = false;
			isHand = true;
		}
		headRenderer->SetOpacity(headOpacity);
	}

	if (isHand)
	{
		handOpacity += handOpacitySpeed * deltaTime;
		if (handOpacity >= 1.f)
		{
			handOpacity = 1.f;
			isHand = false;
			isCrash = true;
		}
		leftRenderer->SetOpacity(handOpacity);
		rightRenderer->SetOpacity(handOpacity);
	}

	if (isCrash)
	{
		BJEngine::Math::Vector3D leftPos = leftHand->GetTransform()->GetLocalPosition();
		BJEngine::Math::Vector3D rightPos = rightHand->GetTransform()->GetLocalPosition();
		if (crashCount != maxCrashCount)
		{
			if (!isEffect)
			{
				if (abs(leftPos.x - (-100)) <= 10.f)
				{
					CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetMainCamera()->GetGameObject()->GetComponent<CameraShake>()->Shake(0.3, 50, 50);
					crashCount++;
					leftPos.x = -100;
					SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSCROSSMETALHARD);
					GameObject* effect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
					BJEngine::Math::Vector3D effectPos = leftHand->GetTransform()->GetWorldPosition();
					effectPos.x += 100;
					effectPos.y += 50;
					effect->GetTransform()->SetLocalPosition(effectPos);
					effect->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(4, 4, 1));
					effect->SetActive(true);
					effect->GetComponent<EffectPlayer>()->Reset(L"Animation_Dagger_Collision_Effect");
					isEffect = true;
				}
				else
				{
					leftPos.x += crashSpeed * deltaTime;
				}
				leftHand->GetTransform()->SetLocalPosition(leftPos);

				if (abs(rightPos.x - (100)) <= 10.f)
				{
					rightPos.x = 100;
				}
				else
				{
					rightPos.x -= crashSpeed * deltaTime;
				}
				rightHand->GetTransform()->SetLocalPosition(rightPos);
			}
			else
			{
				if (abs(leftPos.x - (-200)) <= 10.f)
				{
					leftPos.x = -200;
					isEffect = false;
				}
				else
				{
					leftPos.x -= crashSpeed * deltaTime;
				}
				leftHand->GetTransform()->SetLocalPosition(leftPos);

				if (abs(rightPos.x - (200)) <= 10.f)
				{
					rightPos.x = 200;
				}
				else
				{
					rightPos.x += crashSpeed * deltaTime;
				}
				rightHand->GetTransform()->SetLocalPosition(rightPos);
			}
		}
		else
		{
			if (abs(leftPos.x - (-300)) <= 10.f)
			{
				leftPos.x = -300;
				isCrash = false;
				isBlackMask = true;
				
			}
			else
			{
				leftPos.x -= crashSpeed * deltaTime;
			}
			leftHand->GetTransform()->SetLocalPosition(leftPos);

			if (abs(rightPos.x - (300)) <= 10.f)
			{
				rightPos.x = 300;
			}
			else
			{
				rightPos.x += crashSpeed * deltaTime;
			}
			rightHand->GetTransform()->SetLocalPosition(rightPos);
		}
	}

	if (isBlackMask)
	{
		blackMaskOpacity -= blackMaskOpacitySpeed* deltaTime;
		if (blackMaskOpacity <= 0.f)
		{
			blackMaskOpacity = 0.f;
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"SampleScene4");
		}
		blackMask->SetOpacity(blackMaskOpacity);
	}
}
