#include "Stage2_4.h"
#include "Level.h"

#include "GameObject.h"
#include "Transform.h"

#include "BJMath.h"
#include "CircleCollider.h"
#include "Vector2D.h"
#include "Vector3D.h"

#include "Text.h"
#include "Image.h"

#include "ObjectPoolSystem.h"
#include "Player.h"
#include "ConvexpolygonCollider.h"
#include "Rigidbody2D.h"
#include "Dagger.h"
#include "JumpTrigger.h"

#include "PlayerHpBar.h"
#include "PlayerMpBar.h"

#include "EffectPlayer.h"
#include "GameObjectFactory.h"
#include "Bullet.h"

/// 씬 전환을 위한 참조
#include "EventTrigger.h"
#include "Image.h"
#include "BlackMask.h"

/// 편한 디버깅을 위한 씬 컨트롤러
// #include "SceneController.h"
#include "DebugInfo.h"
#include "Background.h"

#include "ParryingSystem.h"

// #include "Controller.h"
// #include "GameDataManager.h"

using namespace BJEngine;
using namespace BJEngine::Math;

Stage2_4::Stage2_4() : Scene(L"Stage2_4")
{

}

void Stage2_4::Start()
{
	GameObjectFactory::GetInstance().CreateSettingUI(true);

	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	GameObjectFactory::GetInstance().CreateScreenCover();

	//GameObject* sceneController = CreateGameObject(L"SceneController");
	//sceneController->AddComponent<SceneController>();

	GameObjectFactory::GetInstance().CreatePrefab();

#pragma region 플레이어 오브젝트
	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer(1.0f, true, Code404::Layer::Player,Vector2D(530.f, 715.f), Vector2D(528.f +  1920.f * 1, -357.f + 1080.f * 2), true);
	playerobject->GetTransform()->SetLocalPosition(Vector2D(-300.0f, 2070.f));
#pragma endregion

#pragma region 플랫폼
	Level level(STAGE_TWO_FOUR);
	level.Tiling(level);
	level.CreatePlatformFromLevelData(level);
	level.CreateItem(level);
	{
		GameObject* platform = CreateGameObject(L"platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400, 200));

		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(-64, 2160), Vector2D(-64, 0), Vector2D(-32,0), Vector2D(-32, 2160) });
		platform->SetLayerName(L"InvisiblePlatform");
		rigid->SetPositionXLock(true);
		rigid->SetPositionYLock(true);
		rigid->SetRotationLock(true);
		rigid->SetAffectedByGravity(false);
		rigid->SetRestitutionValue(1.f);
	}

	{
		GameObject* platform = CreateGameObject(L"platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400, 200));

		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(-400 + 65 * 64, 2160), Vector2D(-400 + 65 * 64, 0), Vector2D(-400 + 65 * 64 + 32,0), Vector2D(-400 + 65 * 64 + 32, 2160) });
		platform->SetLayerName(L"InvisiblePlatform");
		rigid->SetPositionXLock(true);
		rigid->SetPositionYLock(true);
		rigid->SetRotationLock(true);
		rigid->SetAffectedByGravity(false);
		rigid->SetRestitutionValue(1.f);
	}
#pragma endregion

#pragma region 다음 씬 로드
	GameObject* preSceneChange = CreateGameObject(L"PreSceneChange");
	preSceneChange->GetTransform()->SetLocalPosition({ -190.f, 0.f });
	Image* blackmaskimage1 = preSceneChange->AddComponent<Image>();
	blackmaskimage1->SetSpriteName(L"SwitchScreen");
	blackmaskimage1->SetImageNumber(0);
	blackmaskimage1->SetLayer(0);
	BlackMask* inBlackMask = preSceneChange->AddComponent<BlackMask>();
	inBlackMask->SetMode(BlackMask::eMode::SCENE_IN);

	GameObject* blackmaskObject = CreateGameObject(L"BlackMaskObject");
	blackmaskObject->GetTransform()->SetLocalPosition({ -2300.f, 0.f });
	blackmaskObject->SetActive(false);
	Image* blackmaskimage = blackmaskObject->AddComponent<Image>();
	blackmaskimage->SetSpriteName(L"SwitchScreen");
	blackmaskimage->SetImageNumber(0);
	blackmaskimage->SetLayer(0);
	BlackMask* outBlackMask = blackmaskObject->AddComponent<BlackMask>();
	outBlackMask->SetMode(BlackMask::eMode::SCENE_OUT);
	outBlackMask->SetScene(L"Stage2_5");

	GameObject* nextScene = CreateGameObject(L"NextScene");
	nextScene->GetTransform()->SetLocalPosition({ -400.f + 3680.f, 200.f + 668.f });
	nextScene->SetLayerName(L"Event");
	ConvexpolygonCollider* changerCollider = nextScene->AddComponent<ConvexpolygonCollider>();
	changerCollider->SetPolygon({ { -64.f, 0.f }, { -64.f, 128.f }, { 64.f, 128.f }, {64.f, 0.f} });
	EventTrigger* trigger = nextScene->AddComponent<EventTrigger>();
	trigger->SetActiveObject(blackmaskObject);
#pragma endregion

#pragma region 배경
	GameObject* background = CreateGameObject(L"BackGround");
	background->GetTransform()->SetLocalScale({ 1.f, 1, 1.f });
	background->GetTransform()->SetLocalPosition(Vector2D(530.f, 1220.f));
	SpriteRenderer* sprite = background->AddComponent<SpriteRenderer>();
	sprite->SetSpriteName(L"InternalBackground1920x2160");
	sprite->SetImageNumber(0);
	sprite->SetLayer(-1);
	// Background* bg = background->AddComponent<Background>();
	// bg->SetCameraDepth(1.0f);
	// bg->SetImageZaxisPosition(0.0f);

	GameObject* background2 = CreateGameObject(L"BackGround2");
	background2->GetTransform()->SetLocalScale({ 1.f, 1, 1.f });
	background2->GetTransform()->SetLocalPosition(Vector2D(529.f + 1920.f, 1220.f));
	SpriteRenderer* sprite2 = background2->AddComponent<SpriteRenderer>();
	sprite2->SetSpriteName(L"InternalBackground1920x2160");
	sprite2->SetImageNumber(0);
	sprite2->SetLayer(-1);
	// Background* bg2 = background2->AddComponent<Background>();
	// bg2->SetCameraDepth(1.0f);
	// bg2->SetImageZaxisPosition(0.0f);
#pragma endregion


#pragma region 적
	GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(1000.0f, 2000.0f));
	GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(2300.0f, 1500.0f));


#pragma endregion

}