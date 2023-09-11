#include "MainSceneAnimation.h"
#include "GameObjectFactory.h"

/// 편한 디버깅을 위한 씬 컨트롤러
// #include "SceneController.h"

#include "GameDataManager.h"

#include "SpriteRenderer.h"
#include "Animator.h"

#include "AnimationToScene.h"

using namespace BJEngine;
using namespace BJEngine::Math;

MainSceneAnimation::MainSceneAnimation() : Scene(L"MainSceneAnimation")
{

}

void MainSceneAnimation::Start()
{
	GameObjectFactory::GetInstance().CreateScreenCover();

	GameObject* mainSceneObject = CreateGameObject(L"mainSceneObject");
	mainSceneObject->AddComponent<SpriteRenderer>()->SetLayer(1);
	Animator* anim = mainSceneObject->AddComponent<Animator>();
	anim->Play(L"MainScreen_1");
	mainSceneObject->AddComponent<AnimationToScene>();

	GameDataManager::GetInstance().Clear();
	GameDataManager::GetInstance().ClearCheat();
	//SoundManager::GetInstance().Play(SOUND_BGM_MAIN404);
}
