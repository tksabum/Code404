#include "CutScenePlayer.h"
#include "Collider.h"
#include "GameObject.h"
#include "Animator.h"
#include "CoreSystem.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TimeController.h"

#include "SoundManager.h"

using namespace BJEngine;

void CutScenePlayer::SetDangerText(BJEngine::GameObject* text)
{
	dangerText = text;
}

void CutScenePlayer::SetIsWalk(bool iswalk)
{
	isWalk = iswalk;
}

void CutScenePlayer::Update()
{
	if (isWalk)
	{
		totalTime += GetGameObject()->GetTimeController()->GetDeltaTime();
		if (totalTime >= walkTime)
		{
			totalTime -= walkTime;
			SoundManager::GetInstance().Play(SOUND_EFFECT_WALK);
		}
	}
	
}

void CutScenePlayer::OnTriggerEnter(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"CutSceneObject" && other->GetGameObject()->GetName() == L"bossRightHand")
	{
		isSweep = true;
		GetGameObject()->GetComponent<BJEngine::Animator>()->Play(L"Animation_Player_Attacked");
		dangerText->SetActive(false);
	}

	if (other->GetGameObject()->GetLayerName() == L"CutSceneObject" && other->GetGameObject()->GetName() == L"loadSceneTrigger")
	{
		if (isSweep)
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"SampleScene6");
		}
	}
}
