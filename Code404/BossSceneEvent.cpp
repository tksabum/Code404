#include "BossSceneEvent.h"

#include "Image.h"

#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"
#include "Vector3D.h"

#include "Input.h"
#include "Enemy.h"

#include "CoreSystem.h"
#include "SceneManager.h"

using namespace BJEngine::Math;

void BossSceneEvent::SetBlackMask(Image* renderer)
{
	blackMask = renderer;
}

void BossSceneEvent::SetEyeRenderer(Image* renderer)
{
	eyeRenderer = renderer;
}

void BossSceneEvent::SetHeadRenderer(Image* renderer)
{
	headRenderer = renderer;
}

void BossSceneEvent::SetLeftHandRenderer(Image* renderer)
{
	leftHandRenderer = renderer;
}

void BossSceneEvent::SetRightHandRenderer(Image* renderer)
{
	rightHandRenderer = renderer;
}

void BossSceneEvent::SetBossObject(BJEngine::GameObject* boss)
{
	bossObject = boss;
}

void BossSceneEvent::SetCutSceneRenderer(Image* renderer)
{
	cutSceneRenderer = renderer;
}

void BossSceneEvent::Awake()
{
	//eyeRenderer->SetOpacity(0);
	//headRenderer->SetOpacity(0);
	//leftHandRenderer->SetOpacity(0);
	//rightHandRenderer->SetOpacity(0);
	cutSceneRenderer->SetOpacity(0);
	bossEnemyComponent = bossObject->GetComponent<Enemy>();
}

void BossSceneEvent::Update()
{
	float deltaTime = GetGameObject()->GetTimeController()->GetEventDeltaTime();

	if (bossEnemyComponent->GetCurHP() <= 0.f)
	{
		isBossDie = true;
	}

	if (isBossDie)
	{
		maskOpacity += maskSpeed * deltaTime;
		cutSceneRenderer->SetOpacity(maskOpacity);
	}

	if (cutSceneRenderer->GetOpacity() >= 1.f)
	{
		CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"BossEndingCutScene");
	}

	//if (Input::GetInstance().PressedKeyNow(VK_RETURN))
	//{
	//	blackMask->SetOpacity(0);
	//	blackMask->GetGameObject()->SetActive(false);
	//	eyeRenderer->GetGameObject()->SetActive(false);
	//	headRenderer->GetGameObject()->SetActive(false);
	//	leftHandRenderer->GetGameObject()->SetActive(false);
	//	rightHandRenderer->GetGameObject()->SetActive(false);
	//	GetGameObject()->GetTimeController()->SetTimeScale(1);
	//	//GetGameObject()->SetActive(false);
	//}

	
	//if (!isEyeOpen)
	//{
	//	totalEyeTime += deltaTime;
	//	if (totalEyeTime >= openEyeTime)
	//	{
	//		isEyeOpen = true;
	//		totalEyeTime -= openEyeTime;
	//		eyeRenderer->SetOpacity(1);
	//	}
	//}
	
	//if (isEyeOpen)
	//{
	//	if (!isWait)
	//	{
	//		totalWaitTime += deltaTime;
	//		if (totalWaitTime >= waitTime)
	//		{
	//			totalWaitTime -= waitTime;
	//			isWait = true;
	//		}
	//	}
	//}
	
	//if (isEyeOpen && isWait)
	//{
	//	if (!isOpacity)
	//	{
	//		opacity += opacitySpeed * deltaTime;
	//		if (opacity >= 1.f)
	//		{
	//			isOpacity = true;
	//			headRenderer->SetOpacity(1.f);
	//			opacity = 0.f;
	//		}
	//		else
	//		{
	//			headRenderer->SetOpacity(opacity);
	//		}
	//	}
	//}

	//if (isEyeOpen && isWait && isOpacity)
	//{
	//	if (!isHadnOpacity)
	//	{
	//		opacity += opacitySpeed * deltaTime;
	//		if (opacity >= 1.f)
	//		{
	//			isHadnOpacity = true;
	//			leftHandRenderer->SetOpacity(1.f);
	//			rightHandRenderer->SetOpacity(1.f);
	//			isOpacity = 1.f;
	//		}
	//		else
	//		{
	//			leftHandRenderer->SetOpacity(opacity);
	//			rightHandRenderer->SetOpacity(opacity);
	//		}
	//	}
	//}
	
	//if (isEyeOpen && isWait && isOpacity && isHadnOpacity)
	//{
	//	if (isOpacity)
	//	{
	//		opacity -= opacitySpeed * deltaTime;
	//		if (opacity <= 0.f)
	//		{
	//			isOpacity = true;
	//			blackMask->SetOpacity(0);
	//			isOpacity = 0.f;
	//			blackMask->GetGameObject()->SetActive(false);
	//			eyeRenderer->GetGameObject()->SetActive(false);
	//			headRenderer->GetGameObject()->SetActive(false);
	//			leftHandRenderer->GetGameObject()->SetActive(false);
	//			rightHandRenderer->GetGameObject()->SetActive(false);
	//			GetGameObject()->GetTimeController()->SetTimeScale(1);
	//			//GetGameObject()->SetActive(false);
	//		}
	//		else
	//		{
	//			blackMask->SetOpacity(opacity);
	//		}
	//	}
	//}
}
