#include "Stage1_2.h"

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

#include "ParticleSystem.h"
#include "SkillCoolTimeViewer.h";

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

Stage1_2::Stage1_2() : Scene(L"Stage1_2")
{

}

void Stage1_2::Start()
{
	SoundManager::GetInstance().Play(SOUND_BGM_STAGE1);

	GameObjectFactory::GetInstance().CreateSettingUI(true);

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
	outBlackMask->SetScene(L"Stage1_3");

	GameObject* nextScene = CreateGameObject(L"NextScene");
	nextScene->GetTransform()->SetLocalPosition({ -400.f + 3680.f, 200.f + 820.f });
	nextScene->SetLayerName(L"Event");
	ConvexpolygonCollider* changerCollider = nextScene->AddComponent<ConvexpolygonCollider>();
	changerCollider->SetPolygon({ { 0.f, -1080.f }, { 0.f, 64.f }, { 64.f, 64.f }, {64.f, -1080.f} });
	EventTrigger* trigger = nextScene->AddComponent<EventTrigger>();
	trigger->SetActiveObject(blackmaskObject);
#pragma endregion

	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	GameObjectFactory::GetInstance().CreateScreenCover();

	/*GameObject* sceneController = CreateGameObject(L"SceneController");
	sceneController->AddComponent<SceneController>();*/

	GameObjectFactory::GetInstance().CreatePrefab();

#pragma region 플레이어 오브젝트
	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer(1.0f, true, Code404::Layer::Player,Vector2D(560, 715), Vector2D(560 + 1920 * 1, 715 + 1080 * 0), true);
	playerobject->GetTransform()->SetLocalPosition(Vector2D(-250.0f, 970.f));
	
#pragma endregion

#pragma region 플랫폼
	Level level(STAGE_ONE_TWO);
	level.Tiling(level);
	level.CreatePlatformFromLevelData(level);
	level.CreateItem(level);

	{
		BJEngine::GameObject* platform = CreateGameObject(L"platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400, 200));
		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(18.5 * 64, 13.5 * 64), Vector2D(23.5 * 64, 8.5 * 64), Vector2D(24.5 * 64,8.5 * 64), Vector2D(24.5 * 64, 13.5 * 64) });
		platform->SetLayerName(L"Platform");
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
		collider->SetPolygon({ Vector2D(-64, 1080), Vector2D(-64, 0), Vector2D(-32,0), Vector2D(-32, 1080) });
		platform->SetLayerName(L"InvisiblePlatform");
		rigid->SetPositionXLock(true);
		rigid->SetPositionYLock(true);
		rigid->SetRotationLock(true);
		rigid->SetAffectedByGravity(false);
		rigid->SetRestitutionValue(1.f);
	}
#pragma endregion

#pragma region 배경
	{
		GameObject* background = CreateGameObject(GetMainCamera()->GetGameObject(), L"BackGround");
		background->GetTransform()->SetLocalPosition(Vector2D(0.f, 0.f));
		SpriteRenderer* sprite = background->AddComponent<SpriteRenderer>();
		sprite->SetSpriteName(L"ExternalBackgroundDistantView");
		sprite->SetImageNumber(0);
		sprite->SetLayer(-10);
		Background* bg = background->AddComponent<Background>();
		bg->SetCameraDepth(1.0f);
		bg->SetImageZaxisPosition(1.0f);
	}

	{
		GameObject* background = CreateGameObject(GetMainCamera()->GetGameObject(), L"BackGround");
		background->GetTransform()->SetLocalPosition(Vector2D(200.f, 115.f));
		SpriteRenderer* sprite = background->AddComponent<SpriteRenderer>();
		sprite->SetSpriteName(L"ExternalBackgroundMiddle");
		sprite->SetImageNumber(0);
		sprite->SetLayer(-8);
		Background* bg = background->AddComponent<Background>();
		bg->SetCameraDepth(1.0f);
		bg->SetImageZaxisPosition(0.8f);
	}

	{
		GameObject* background = CreateGameObject(GetMainCamera()->GetGameObject(), L"BackGround");
		background->GetTransform()->SetLocalPosition(Vector2D(900.f, 365.f));
		SpriteRenderer* sprite = background->AddComponent<SpriteRenderer>();
		sprite->SetSpriteName(L"ExternalBackgroundObject4");
		sprite->SetImageNumber(0);
		sprite->SetLayer(-6);
		Background* bg = background->AddComponent<Background>();
		bg->SetCameraDepth(1.0f);
		bg->SetImageZaxisPosition(0.5f);
	}
#pragma endregion

#pragma region 적

	GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(400.0f, 600.0f));
	GameObjectFactory::GetInstance().CreateEnemyMeleeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(600+1920.0f, 600.0f));
	GameObjectFactory::GetInstance().CreateEnemyRangeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(400+1920.0f, 200.0f));
	GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(800+1920.0f, 200.0f));


#pragma endregion

#pragma region 파티클

	GameObject* RainFloorParticle = CreateGameObject(L"RainFloorParticle");
	RainFloorParticle->GetTransform()->SetLocalPosition({ 0, 35.f });

	{
		// GameObject* RainFloorParticle1_1_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_1");
		// GameObject* RainFloorParticle1_1_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_2");
		// GameObject* RainFloorParticle1_1_3 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_3");
		// GameObject* RainFloorParticle1_1_4 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_4");
		// RainFloorParticle1_1_1->GetTransform()->SetLocalPosition(Math::Vector2D(-360.f, 1020.f));
		// RainFloorParticle1_1_2->GetTransform()->SetLocalPosition(Math::Vector2D(670.f, 1020.f));
		// RainFloorParticle1_1_3->GetTransform()->SetLocalPosition(Math::Vector2D(990.f, 710.f));
		// RainFloorParticle1_1_4->GetTransform()->SetLocalPosition(Math::Vector2D(1310.f, 710.f));

		GameObject* RainFloorParticle1_1_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_1");
		RainFloorParticle1_1_1->GetTransform()->SetLocalPosition({30.f, 0.f});
		ParticleSystem* floorParticle1_1 = RainFloorParticle1_1_1->AddComponent<ParticleSystem>();
		floorParticle1_1->SystemSetting(
			ParticleSystem::eParticleState::Dynamic,
			ParticleSystem::eScope::Line,
			ParticleSystem::eActivity::Regular,
			ParticleSystem::eEmissionMode::Spray,
			ParticleSystem::eRenderType::Rectangle);
		floorParticle1_1->SetGravity(200.f);
		floorParticle1_1->SetLayer(1);
		floorParticle1_1->SetDirectionRangeDegree(30.f);
		floorParticle1_1->SetDirection(Math::Vector2D(0.f, -1.f));
		// floorParticle1_1->SetUsePoint(false);
		// floorParticle1_1->SetGameObjectPointer(RainFloorParticle1_1_1);
		// floorParticle1_1->SetGameObjectPointer(RainFloorParticle1_1_2);
		// floorParticle1_1->SetGameObjectPointer(RainFloorParticle1_1_3);
		// floorParticle1_1->SetGameObjectPointer(RainFloorParticle1_1_4);
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(-380.f, 1020.f));
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(670.f, 1020.f));
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(990.f, 700.f));
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(1320.f, 700.f));
		floorParticle1_1->Initialize(
			70, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
		floorParticle1_1->ParticleSystemSet();
	}

	{
		// GameObject* RainFloorParticle1_2_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_1");
		// GameObject* RainFloorParticle1_2_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_2");
		// RainFloorParticle1_2_1->GetTransform()->SetLocalPosition(Math::Vector2D(1310.f, 1020.f));
		// RainFloorParticle1_2_2->GetTransform()->SetLocalPosition(Math::Vector2D(3220.f, 1020.f));
		
		GameObject* RainFloorParticle1_2_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_1");
		RainFloorParticle1_2_1->GetTransform()->SetLocalPosition({ 64.f, 0.f });
		ParticleSystem* floorParticle1_2 = RainFloorParticle1_2_1->AddComponent<ParticleSystem>();
		floorParticle1_2->SystemSetting(
			ParticleSystem::eParticleState::Dynamic,
			ParticleSystem::eScope::Line,
			ParticleSystem::eActivity::Regular,
			ParticleSystem::eEmissionMode::Spray,
			ParticleSystem::eRenderType::Rectangle);
		floorParticle1_2->SetGravity(200.f);
		floorParticle1_2->SetLayer(1);
		floorParticle1_2->SetDirectionRangeDegree(30.f);
		floorParticle1_2->SetDirection(Math::Vector2D(0.f, -1.f));
		// floorParticle1_2->SetUsePoint(false);
		// floorParticle1_2->SetGameObjectPointer(RainFloorParticle1_2_1);
		// floorParticle1_2->SetGameObjectPointer(RainFloorParticle1_2_2);
		floorParticle1_2->SetParticleLineScope(Math::Vector2D(1310.f, 1020.f));
		floorParticle1_2->SetParticleLineScope(Math::Vector2D(3270.f, 1020.f));
		floorParticle1_2->Initialize(
			70, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
		floorParticle1_2->ParticleSystemSet();
	}

	{
		GameObject* RainFloorParticle1_2_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_1");
		RainFloorParticle1_2_1->GetTransform()->SetLocalPosition({});
		ParticleSystem* RainParticle = RainFloorParticle->AddComponent<ParticleSystem>();
		RainParticle->SystemSetting(
			ParticleSystem::eParticleState::Dynamic,
			ParticleSystem::eScope::Line,
			ParticleSystem::eActivity::Regular,
			ParticleSystem::eEmissionMode::Spray,
			ParticleSystem::eRenderType::Line);
		RainParticle->SetGravity(500.f);
		RainParticle->SetDirectionRangeDegree(30.f);
		RainParticle->SetDirection(Math::Vector2D(0.f, 1.f));
		RainParticle->SetParticleLineScope(Math::Vector2D(-360.f, 0.f));
		RainParticle->SetParticleLineScope(Math::Vector2D(3220.f, 0.f));
		RainParticle->Initialize(
			700, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 0, 0.05f, 3.3f, 100, 15, BJEngine::GraphicsEngine::Color::White);
		RainParticle->ParticleSystemSet();
		RainParticle->SetLayer(-1);
	}

	{
		/// 씬 시작할 때 자연스러운 전환을 위한 비 파티클
		GameObject* preLoadParticle = CreateGameObject(L"PreLoadParticle");
		preLoadParticle->GetTransform()->SetLocalPosition({930.f, 480.f});
		ParticleSystem* preRainParticle = preLoadParticle->AddComponent<ParticleSystem>();
		preRainParticle->SystemSetting(
			ParticleSystem::eParticleState::Dynamic,
			ParticleSystem::eScope::Rectangle,
			ParticleSystem::eActivity::Regular,
			ParticleSystem::eEmissionMode::Spray,
			ParticleSystem::eRenderType::Line);
		preRainParticle->SetGravity(200.f);
		preRainParticle->SetDirectionRangeDegree(30.f);
		preRainParticle->SetDirection(Math::Vector2D(0.f, 1.f));
		preRainParticle->SetRectScope(0.f, 1290.f, 0.f, 508.f);
		preRainParticle->Initialize(
			100, RainFloorParticle->GetTransform()->GetWorldPosition(),
			false, 0.f, 0.03f, 2.f, 100, 15, BJEngine::GraphicsEngine::Color::White);
		preRainParticle->ParticleSystemSet();
		preRainParticle->SetLayer(-1);
	}
#pragma endregion


#pragma region UI
	// UI - 공격 / 패링
	GameObject* tutorialUI_Z = CreateGameObject(L"tutorialUI_Z");
	tutorialUI_Z->GetTransform()->SetLocalPosition(Vector2D(0.f, 800.f));
	tutorialUI_Z->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_Z = tutorialUI_Z->AddComponent<SpriteRenderer>();
	tutorialImage_Z->SetSpriteName(L"TutorialUI_Z");
	tutorialImage_Z->SetImageNumber(0);
	tutorialImage_Z->SetLayer(1);

	// UI - 은신
	GameObject* tutorialUI_F = CreateGameObject(L"tutorialUI_F");
	tutorialUI_F->GetTransform()->SetLocalPosition(Vector2D(2000.f, 800.f));
	tutorialUI_F->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_F = tutorialUI_F->AddComponent<SpriteRenderer>();
	tutorialImage_F->SetSpriteName(L"TutorialUI_F");
	tutorialImage_F->SetImageNumber(0);
	tutorialImage_F->SetLayer(1);
#pragma endregion

}