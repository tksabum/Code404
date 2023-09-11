#include "BossCutSceneController.h"

#include "GameObject.h"
#include "ScrollingBackGround.h"
#include "TimeController.h"
#include "Animator.h"
#include "Rigidbody2D.h"
#include "Vector2D.h"
#include "BJMath.h"
#include "Transform.h"
#include "Scene.h"
#include "CameraShake.h"
#include "Camera.h"
#include "Input.h"
#include "CoreSystem.h"
#include "SceneManager.h"
#include "CutScenePlayer.h"
#include "SoundManager.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void BossCutSceneController::SetPlayer(BJEngine::GameObject* player)
{
	this->player = player;
}

void BossCutSceneController::SetScrolling(BJEngine::GameObject* scrolling)
{
	this->scrolling = scrolling;
}

void BossCutSceneController::SetBossHand(BJEngine::GameObject* hand)
{
	bossHand = hand;
}

void BossCutSceneController::SetText(BJEngine::GameObject* text)
{
	dangerText = text;
}

void BossCutSceneController::Awake()
{
	playerAnim = player->GetComponent<Animator>();
	scrollingComponent = scrolling->GetComponent<ScrollingBackGround>();
	handRigid = bossHand->GetComponent<Rigidbody2D>();
	playerComponent = player->GetComponent<CutScenePlayer>();
}

void BossCutSceneController::Update()
{
	if (Input::GetInstance().PressedKeyNow(VK_RETURN))
	{
		// ´ÙÀ½ ¾À ·Îµå
		CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"SampleScene6");
	}

	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
	if (scrollingComponent->GetScrolling())
	{
		totalScrollingTime += deltaTime;
		if (totalScrollingTime >= scrollingTime)
		{
			isScrolling = false;
			scrollingComponent->SetScrolling(false);
		}
	}
	else
	{
		playerComponent->SetIsWalk(false);
		playerAnim->Play(L"Animation_Player_Stand");
	}

	if (!isScrolling)
	{
		if (!isWait)
		{
			totalWaitTime += deltaTime;
			if (totalWaitTime >= waitTime)
			{
				isWait = true;
				totalWaitTime -= waitTime;
				BJEngine::Math::Vector3D textPos = player->GetTransform()->GetLocalPosition();
				textPos.y += -90.f;
				textPos.x += 200.f;
				dangerText->GetTransform()->SetLocalPosition(textPos);
				dangerText->SetActive(true);
			}
		}
	}

	if (isWait)
	{
		handRigid->SetVelocity(Vector2D(-1000,0));
		if (!isCameraShake)
		{
			isCameraShake = true;
			GetGameObject()->GetScene()->GetMainCamera()->GetGameObject()->GetComponent<CameraShake>()->Shake(5.f,5,5);
			SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSCAMERASHAKE);
		}
	}
}
