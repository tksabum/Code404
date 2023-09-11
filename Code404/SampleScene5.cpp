#include "SampleScene5.h"

/// 편한 디버깅을 위한 씬 컨트롤러
#include "SceneController.h"
#include "DebugInfo.h"

#include "GameObjectFactory.h"
#include "Rigidbody2D.h"
#include "Vector3D.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Controller.h"
#include "ScrollingBackGround.h"
#include "BossCutSceneController.h"
#include "ConvexpolygonCollider.h"
#include "TextRenderer.h"
#include "CameraShake.h"
#include "CutScenePlayer.h"
#include "Image.h"
#include "BlackMask.h"
#include "ParryingSystem.h"

#include "SoundManager.h"

using namespace BJEngine;
using namespace BJEngine::Math;

SampleScene5::SampleScene5() : Scene(L"SampleScene5")
{

}

void SampleScene5::Start()
{
	SoundManager::GetInstance().Play(SOUND_BGM_BOSSSTAGE);

	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	GetMainCamera()->GetGameObject()->GetTransform()->SetLocalPosition(Vector2D(-500, 250));
	GetMainCamera()->GetGameObject()->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(2,2,1));

	GetMainCamera()->GetGameObject()->AddComponent<CameraShake>();

	GameObjectFactory::GetInstance().CreatePrefab();
	GameObjectFactory::GetInstance().CreateScreenCover();

	GameObject* dangerText = CreateGameObject(L"dangerText");
	dangerText->SetActive(false);
	TextRenderer* textLogic = dangerText->AddComponent<TextRenderer>();
	textLogic->SetTextAreaSize(Vector2D(500, 100));
	textLogic->SetLayer(2);
	textLogic->SetText(L"! ! !");
	textLogic->SetTextColor(BJEngine::GraphicsEngine::Color::Red);
	textLogic->SetTextSize(45);

	GameObject* tempPlayer = CreateGameObject(L"tempPlayer");
	tempPlayer->SetLayerName(L"CutSceneObject");
	tempPlayer->AddComponent<CutScenePlayer>()->SetDangerText(dangerText);
	Rigidbody2D* playerRigid = tempPlayer->AddComponent<Rigidbody2D>();
	playerRigid->SetAffectedByGravity(false);
	playerRigid->SetMass(0.f);
	tempPlayer->AddComponent<ConvexpolygonCollider>()->SetPolygon({ Vector2D(-60.0f, 100.0f), Vector2D(-60.0f, -90.0f), Vector2D(60.0f, -90.0f), Vector2D(60.0f, 100.0f) });
	tempPlayer->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-850, 250, 1));
	SpriteRenderer* playerRenderer = tempPlayer->AddComponent<SpriteRenderer>();
	playerRenderer->SetLayer(1);
	playerRenderer->SetImageNumber(0);
	Animator* playerAnim = tempPlayer->AddComponent<Animator>();
	playerAnim->Play(L"Animation_Player_Run");

	GameObject* backGround1 = CreateGameObject(L"backGround1");
	backGround1->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-100, 0, 1));
	SpriteRenderer* backGroundRender1 = backGround1->AddComponent<SpriteRenderer>();
	backGroundRender1->SetSpriteName(L"back");
	backGroundRender1->SetLayer(0);
	backGroundRender1->SetImageNumber(0);

	GameObject* backGround2 = CreateGameObject(L"backGround2");
	backGround2->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(2017, 0, 1));
	SpriteRenderer* backGroundRender2 = backGround2->AddComponent<SpriteRenderer>();
	backGroundRender2->SetSpriteName(L"back");
	backGroundRender2->SetLayer(0);
	backGroundRender2->SetImageNumber(0);

	GameObject* backGroundParent = CreateGameObject(L"backGroundParent");
	ScrollingBackGround* scrollingLogic = backGroundParent->AddComponent<ScrollingBackGround>();
	scrollingLogic->SetBackGround1(backGround1);
	scrollingLogic->SetBackGround2(backGround2);

	GameObject* bossRightHand = CreateGameObject(L"bossRightHand");
	bossRightHand->SetLayerName(L"CutSceneObject");
	Rigidbody2D* handRigid = bossRightHand->AddComponent<Rigidbody2D>();
	handRigid->SetAffectedByGravity(false);
	handRigid->SetRotationLock(true);
	handRigid->SetRestitutionValue(0.f);
	SpriteRenderer* handRenderer = bossRightHand->AddComponent<SpriteRenderer>();
	handRenderer->SetSpriteName(L"Boss_Hand_Blue");
	handRenderer->SetLayer(1);
	handRenderer->SetImageNumber(0);
	bossRightHand->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(-1,1,1));
	bossRightHand->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(3000, 210, 1));
	bossRightHand->AddComponent<ConvexpolygonCollider>()->SetPolygon({ Vector2D(-150,150),Vector2D(-150,-80), Vector2D(150,-80), Vector2D(150,150) });

	GameObject* loadSceneTrigger = CreateGameObject(L"loadSceneTrigger");
	loadSceneTrigger->SetLayerName(L"CutSceneObject");
	loadSceneTrigger->AddComponent<ConvexpolygonCollider>()->SetPolygon({ Vector2D(-60.0f, 100.0f), Vector2D(-60.0f, -90.0f), Vector2D(60.0f, -90.0f), Vector2D(60.0f, 100.0f) });
	loadSceneTrigger->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-1500,250,0));

	GameObject* bossCutsceneController = CreateGameObject(L"bossCutsceneController");
	BossCutSceneController* controllerLogic = bossCutsceneController->AddComponent<BossCutSceneController>();
	controllerLogic->SetPlayer(tempPlayer);
	controllerLogic->SetScrolling(backGroundParent);
	controllerLogic->SetBossHand(bossRightHand);
	controllerLogic->SetText(dangerText);

	GameObject* preSceneChange = CreateGameObject(L"PreSceneChange");
	preSceneChange->GetTransform()->SetLocalPosition({ -190.f, 0.f });
	Image* blackmaskimage1 = preSceneChange->AddComponent<Image>();
	blackmaskimage1->SetSpriteName(L"SwitchScreen");
	blackmaskimage1->SetImageNumber(0);
	blackmaskimage1->SetLayer(0);
	BlackMask* inBlackMask = preSceneChange->AddComponent<BlackMask>();
	inBlackMask->SetMode(BlackMask::eMode::SCENE_IN);

	// GameObject* preSceneChange = CreateGameObject(L"PreSceneChange");
	// preSceneChange->GetTransform()->SetLocalPosition({ 21.f, 0.f });
	// Image* blackmaskimage1 = preSceneChange->AddComponent<Image>();
	// blackmaskimage1->SetSpriteName(L"BlackMask2");
	// blackmaskimage1->SetImageNumber(0);
	// blackmaskimage1->SetLayer(0);
	// BlackMask* inBlackMask = preSceneChange->AddComponent<BlackMask>();
	// inBlackMask->SetMode(BlackMask::eMode::SCENE_IN);
}