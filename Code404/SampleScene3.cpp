#include "SampleScene3.h"
#include "SoundManager.h"
#include "SceneController.h"
#include "ConvexpolygonCollider.h"
#include "Rigidbody2D.h"
#include "GameObjectFactory.h"
#include "Level.h"
#include "SoundManager.h"
#include "Image.h"
#include <iostream>
#include "ObjectPoolSystem.h"
#include "Dagger.h"
#include "EffectPlayer.h"
#include "Text.h"
#include "DebugInfo.h"
#include "ParryingSystem.h"
#include "UISoundController.h"

using namespace BJEngine;
using namespace BJEngine::Math;

SampleScene3::SampleScene3() : Scene(L"SampleScene3")
{

}



void SampleScene3::Start()
{
	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	GameObject* sceneController = CreateGameObject(L"SceneController");
	sceneController->AddComponent<SceneController>();

	GameObjectFactory::GetInstance().CreatePrefab();

	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer();

	GameObject* uitestobject = CreateGameObject(L"UITestObject");
	Image* image = uitestobject->AddComponent<Image>();
	image->SetSpriteName(L"UITest");
	image->SetImageNumber(0);
	uitestobject->GetTransform()->SetLocalPosition(Vector2D(400.0f, 0.0f));

	//float volume = SoundManager::GetInstance().GetVolume();
	//float volume2 = (UISoundController::volumeBar/380);
	Level level(STAGE_ONE_TWO);
	level.Tiling(level);
	level.CreatePlatformFromLevelData(level);
	//SoundManager& soundManager = SoundManager::GetInstance();
	SoundManager::GetInstance().StopAll();
	SoundManager::GetInstance().Play(SOUND_BGM_STAGE1);
}

