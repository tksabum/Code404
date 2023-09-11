#include "Stage2_9.h"
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
#include "Background.h"

/// 편한 디버깅을 위한 씬 컨트롤러
// #include "SceneController.h"
#include "DebugInfo.h"

#include "ParryingSystem.h"

#include "SoundManager.h"

using namespace BJEngine;
using namespace BJEngine::Math;

Stage2_9::Stage2_9() : Scene(L"Stage2_9")
{

}

void Stage2_9::Start()
{
	SoundManager::GetInstance().Play(SOUND_BGM_STAGE2);

	GameObjectFactory::GetInstance().CreateSettingUI(true);

	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	GameObjectFactory::GetInstance().CreateScreenCover();

	/*GameObject* sceneController = CreateGameObject(L"SceneController");
	sceneController->AddComponent<SceneController>();*/

	GameObjectFactory::GetInstance().CreatePrefab();

#pragma region 플레이어 오브젝트
	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer(1.0f, true, Code404::Layer::Player,Vector2D(530, 715), Vector2D(530 + 1920 * 0, 715 + 1080 * 0), true);
	playerobject->GetTransform()->SetLocalPosition(Vector2D(-300.0f, 970.f));
#pragma endregion

#pragma region 플랫폼
	Level level(STAGE_TWO_NINE);
	//Level level2(STAGE_ONE_FOUR_TWO);
	level.Tiling(level);
	//level2.TilingSecond(level2);
	level.CreatePlatformFromLevelData(level);
	level.CreateItem(level);
	//level2.CreatePlatformFromLevelDataSecond(level2);

	/// 투명벽
	{
		GameObject* platform = CreateGameObject(L"platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400, 200));

		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(-64, 1080), Vector2D(-64, 0), Vector2D(-32,0), Vector2D(-32, 1080) });
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
		collider->SetPolygon({ Vector2D(-64 + 1920, 1080), Vector2D(-64 + 1920, 0), Vector2D(-32 + 1920,0), Vector2D(-32 + 1920, 1080) });
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
	outBlackMask->SetScene(L"SampleScene5");

	GameObject* nextScene = CreateGameObject(L"NextScene");
	nextScene->GetTransform()->SetLocalPosition({ -400.f + 1792.f, 200.f + 668.f });
	nextScene->SetLayerName(L"Event");
	ConvexpolygonCollider* changerCollider = nextScene->AddComponent<ConvexpolygonCollider>();
	changerCollider->SetPolygon({ { -96.f, 0.f }, { -96.f, 256.f }, { 32.f, 256.f }, {32.f, 0.f} });
	EventTrigger* trigger = nextScene->AddComponent<EventTrigger>();
	trigger->SetActiveObject(blackmaskObject);
#pragma endregion


#pragma region 배경
	{
		GameObject* background = CreateGameObject(L"BackGround");
		background->GetTransform()->SetLocalPosition(Vector2D(462.f + 60.f, 715.f));
		SpriteRenderer* sprite = background->AddComponent<SpriteRenderer>();
		sprite->SetSpriteName(L"InternalBackgroundOrigin");
		sprite->SetImageNumber(0);
		sprite->SetLayer(-10);
		// Background* bg = background->AddComponent<Background>();
		// bg->SetCameraDepth(1.0f);
		// bg->SetImageZaxisPosition(0.0f);
	}
#pragma endregion


#pragma region 적
	/*GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(400.0f, 600.0f));
	GameObjectFactory::GetInstance().CreateEnemyMeleeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(600.0f, 600.0f));

	GameObjectFactory::GetInstance().CreateEnemyRangeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(900.0f, 200.0f));
	GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(1100.0f, 200.0f));
	GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(1300.0f, 200.0f));

	for (int i = 0; i < 15; i++)
	{
		GameObjectFactory::GetInstance().CreateEnemyMeleeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(1800.0f, 600.0f));
	}*/
#pragma endregion

}