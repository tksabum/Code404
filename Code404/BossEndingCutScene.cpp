#include "Code404.h"
#include "BossEndingCutScene.h"

#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"

#include "Vector3D.h"

#include "GameObjectFactory.h"

#include "Camera.h"
#include "Controller.h"

#include "EndCutSceneController.h"

#include "SoundManager.h"

using namespace BJEngine;

BossEndingCutScene::BossEndingCutScene() : Scene(L"BossEndingCutScene")
{

}

void BossEndingCutScene::Start()
{
	SoundManager::GetInstance().Play(SOUND_BGM_CLEAR);

	GameObjectFactory::GetInstance().CreateScreenCover();

	GameObject* cutSceneEndLine = CreateGameObject(L"cutSceneEndLine");
	SpriteRenderer* cutSceneEndLineImage = cutSceneEndLine->AddComponent<SpriteRenderer>();
	cutSceneEndLineImage->SetSpriteName(L"CutSceneEndLine");
	cutSceneEndLineImage->SetLayer(2);
	cutSceneEndLineImage->SetImageNumber(0);

	GameObject* blackMask1 = CreateGameObject(L"blackMask1");
	blackMask1->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5, 5, 1));
	SpriteRenderer* blackMask1Image = blackMask1->AddComponent<SpriteRenderer>();
	blackMask1Image->SetSpriteName(L"Black");
	blackMask1Image->SetLayer(0);
	blackMask1Image->SetImageNumber(0);

	GameObject* blackMask2 = CreateGameObject(L"blackMask2");
	blackMask2->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5, 5, 1));
	SpriteRenderer* blackMask2Image = blackMask2->AddComponent<SpriteRenderer>();
	blackMask2Image->SetSpriteName(L"Black");
	blackMask2Image->SetLayer(2);
	blackMask2Image->SetImageNumber(0);
	blackMask2Image->SetOpacity(0);

	GameObject* blackMask3 = CreateGameObject(L"blackMask3");
	blackMask3->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5, 5, 1));
	SpriteRenderer* blackMask3Image = blackMask3->AddComponent<SpriteRenderer>();
	blackMask3Image->SetSpriteName(L"Black");
	blackMask3Image->SetLayer(4);
	blackMask3Image->SetImageNumber(0);
	blackMask3Image->SetOpacity(0);

	GameObject* cutScene1 = CreateGameObject(L"cutScene1");
	SpriteRenderer* cutScene1Image = cutScene1->AddComponent<SpriteRenderer>();
	cutScene1Image->SetSpriteName(L"CutScene1");
	cutScene1Image->SetLayer(1);
	cutScene1Image->SetImageNumber(0);
	cutScene1Image->SetOpacity(0);

	GameObject* cutScene2 = CreateGameObject(L"cutScene2");
	SpriteRenderer* cutScene2Image = cutScene2->AddComponent<SpriteRenderer>();
	cutScene2Image->SetSpriteName(L"CutScene2");
	cutScene2Image->SetLayer(1);
	cutScene2Image->SetImageNumber(0);
	cutScene2Image->SetOpacity(0);

	GameObject* cutScene3 = CreateGameObject(L"cutScene3");
	SpriteRenderer* cutScene3Image = cutScene3->AddComponent<SpriteRenderer>();
	cutScene3Image->SetSpriteName(L"CutScene3");
	cutScene3Image->SetLayer(1);
	cutScene3Image->SetImageNumber(0);
	cutScene3Image->SetOpacity(0);

	GameObject* cutScene4 = CreateGameObject(L"cutScene4");
	SpriteRenderer* cutScene4Image = cutScene4->AddComponent<SpriteRenderer>();
	cutScene4Image->SetSpriteName(L"CutScene4");
	cutScene4Image->SetLayer(3);
	cutScene4Image->SetImageNumber(0);
	cutScene4Image->SetOpacity(0);

	GameObject* anyKey = CreateGameObject(L"anyKey");
	SpriteRenderer* anyKeyImage = anyKey->AddComponent<SpriteRenderer>();
	anyKeyImage->SetSpriteName(L"Press_Any_Key");
	anyKeyImage->SetLayer(4);
	anyKeyImage->SetImageNumber(0);
	anyKeyImage->SetOpacity(0);

	GameObject* cutSceneController = CreateGameObject(L"cutSceneController");
	EndCutSceneController* cutSceneControllerLogic = cutSceneController->AddComponent<EndCutSceneController>();
	cutSceneControllerLogic->SetCutScene1(cutScene1);
	cutSceneControllerLogic->SetCutScene2(cutScene2);
	cutSceneControllerLogic->SetCutScene3(cutScene3);
	cutSceneControllerLogic->SetBlackMask(blackMask2);
	cutSceneControllerLogic->SetCutScene4(cutScene4);
	cutSceneControllerLogic->SetChangekMask(blackMask3);
	cutSceneControllerLogic->SetAnyKek(anyKey);
}
