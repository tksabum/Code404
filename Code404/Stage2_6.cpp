#include "Stage2_6.h"
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

// #include "Controller.h"
// #include "GameDataManager.h"

#include "ParryingSystem.h"

using namespace BJEngine;
using namespace BJEngine::Math;

Stage2_6::Stage2_6() : Scene(L"Stage2_6")
{

}

void Stage2_6::Start()
{
	GameObjectFactory::GetInstance().CreateSettingUI(true);

	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	GameObjectFactory::GetInstance().CreateScreenCover();

	/*GameObject* sceneController = CreateGameObject(L"SceneController");
	sceneController->AddComponent<SceneController>();*/

	GameObjectFactory::GetInstance().CreatePrefab();

	/*GameDataManager::GetInstance().runningCheat = true;
	GetMainCamera()->GetGameObject()->AddComponent<Controller>();*/

#pragma region 플레이어 오브젝트

	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer(1.0f, true, Code404::Layer::Player,Vector2D(560 - 30, 700), Vector2D(-1400.f + 1920 * 4, 724 + 1080 * 1), true);
	playerobject->GetTransform()->SetLocalPosition(Vector2D(-300.0f, 2055.f));

#pragma endregion

#pragma region 플랫폼
	Level level(STAGE_TWO_SIX_ONE);
	Level level2(STAGE_TWO_SIX_TWO);
	level.Tiling(level);
	level2.TilingSecond(level2);
	level.CreatePlatformFromLevelData(level);
	level2.CreatePlatformFromLevelDataSecond(level2);
	level.CreateItem(level);
	level2.CreateItem(level2);
	/// 투명벽
	{
		GameObject* platform = CreateGameObject(L"platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-320, 2000));

		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(-64, 1080), Vector2D(-64,-1080), Vector2D(-32,-1080), Vector2D(-32, 1080) });
		platform->SetLayerName(L"InvisiblePlatform");
		rigid->SetPositionXLock(true);
		rigid->SetPositionYLock(true);
		rigid->SetRotationLock(true);
		rigid->SetAffectedByGravity(false);
		rigid->SetRestitutionValue(1.f);
	}

	{
		GameObject* platform = CreateGameObject(L"platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400.f + 3880.f + 1920.f * 2, 200));

		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(-64, 1080), Vector2D(-64, -1000), Vector2D(-32,-1000), Vector2D(-32, 1080) });
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
	outBlackMask->SetScene(L"Stage2_7");

	GameObject* nextScene = CreateGameObject(L"NextScene");
	nextScene->GetTransform()->SetLocalPosition({ -400.f + 3680.f + 1920.f * 2, 200.f + 668.f });
	nextScene->SetLayerName(L"Event");
	ConvexpolygonCollider* changerCollider = nextScene->AddComponent<ConvexpolygonCollider>();
	changerCollider->SetPolygon({ { -64.f, 0.f }, { -64.f, 128.f }, { 64.f, 128.f }, {64.f, 0.f} });
	EventTrigger* trigger = nextScene->AddComponent<EventTrigger>();
	trigger->SetActiveObject(blackmaskObject);
#pragma endregion

#pragma region 배경
	GameObject* background = CreateGameObject(L"BackGround");
	background->GetTransform()->SetLocalPosition(Vector2D(529.f, 1220.f));
	SpriteRenderer* sprite = background->AddComponent<SpriteRenderer>();
	sprite->SetSpriteName(L"InternalBackground1920x2160");
	sprite->SetImageNumber(0);
	sprite->SetLayer(-10);

	GameObject* background2 = CreateGameObject(L"BackGround2");
	background2->GetTransform()->SetLocalPosition(Vector2D(529.f + 1920.f * 1 - 2.f, 1220.f));
	SpriteRenderer* sprite2 = background2->AddComponent<SpriteRenderer>();
	sprite2->SetSpriteName(L"InternalBackground1920x2160");
	sprite2->SetImageNumber(0);
	sprite2->SetLayer(-10);

	GameObject* background3 = CreateGameObject(L"BackGround3");
	background3->GetTransform()->SetLocalPosition(Vector2D(529.f + 1920.f * 2 - 4.f, 1220.f));
	SpriteRenderer* sprite3 = background3->AddComponent<SpriteRenderer>();
	sprite3->SetSpriteName(L"InternalBackground1920x2160");
	sprite3->SetImageNumber(0);
	sprite3->SetLayer(-10);

	GameObject* background4 = CreateGameObject(L"BackGround4");
	background4->GetTransform()->SetLocalPosition(Vector2D(529.f + 1920.f * 3 - 6.f, 1220.f));
	SpriteRenderer* sprite4 = background4->AddComponent<SpriteRenderer>();
	sprite4->SetSpriteName(L"InternalBackground1920x2160");
	sprite4->SetImageNumber(0);
	sprite4->SetLayer(-10);
#pragma endregion


#pragma region 적
	GameObjectFactory::GetInstance().CreateEnemyMeleeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(1000.0f,2000.0f));
	GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(3000.0f,0.0f));
	GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(5100.0f,0.0f));
	GameObjectFactory::GetInstance().CreateEnemyMeleeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(5100.0f,2000.0f));
	GameObjectFactory::GetInstance().CreateEnemyMeleeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(7000.0f,2000.0f));
	
#pragma endregion

}