#include "MainScene.h"

#include "GameObjectFactory.h"

/// ���� ������� ���� �� ��Ʈ�ѷ�
// #include "SceneController.h"

#include "GameDataManager.h"

#include "SpriteRenderer.h"
#include "Animator.h"


using namespace BJEngine;
using namespace BJEngine::Math;

MainScene::MainScene() : Scene(L"MainScene")
{

}

void MainScene::Start()
{
	
	GameObjectFactory::GetInstance().CreateSettingUI();

	GameObjectFactory::GetInstance().CreateScreenCover();

	// GameObject* sceneController = CreateGameObject(L"SceneController");
	// sceneController->AddComponent<SceneController>();
	SoundManager::GetInstance().Play(SOUND_BGM_MAIN404);
	GameDataManager::GetInstance().Clear();
	//GameDataManager::GetInstance().ClearCheat();
	//SoundManager::GetInstance().Play(SOUND_BGM_MAIN404);
	//SoundManager::GetInstance().Play(SOUND_BGM_MAIN404, SoundManager::GetInstance().GetBGMVolume());
}
