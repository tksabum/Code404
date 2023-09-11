#include "Stage1_4.h"

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

Stage1_4::Stage1_4() : Scene(L"Stage1_4")
{

}

void Stage1_4::Start()
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
	outBlackMask->SetScene(L"Stage1_5");

	GameObject* nextScene = CreateGameObject(L"NextScene");
	nextScene->GetTransform()->SetLocalPosition({ -400.f + 7550.f, 200.f + 910.f });
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
	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer(1.0f, true, Code404::Layer::Player,Vector2D(560, 715), Vector2D(560 + 1920 * 3, 715 + 1080 * 0), true);
	playerobject->GetTransform()->SetLocalPosition(Vector2D(-250.0f, 970.f));
#pragma endregion

#pragma region 플랫폼
	Level level(STAGE_ONE_FOUR_ONE);
	Level level2(STAGE_ONE_FOUR_TWO);
	level.Tiling(level);
	level2.TilingSecond(level2);
	level.CreatePlatformFromLevelData(level);
	level2.CreatePlatformFromLevelDataSecond(level2);
	level.CreateItem(level);
	level2.CreateItem(level2);

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
		collider->SetPolygon({ Vector2D(7600, 1080), Vector2D(7600, 0), Vector2D(7632,0), Vector2D(7632, 1080) });
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
		sprite->SetSpriteName(L"ExternalBackgroundObject2");
		sprite->SetImageNumber(0);
		sprite->SetLayer(-6);
		Background* bg = background->AddComponent<Background>();
		bg->SetCameraDepth(1.0f);
		bg->SetImageZaxisPosition(0.5f);
	}
#pragma endregion

#pragma region 파티클

	GameObject* RainFloorParticle = CreateGameObject(L"RainFloorParticle");
	RainFloorParticle->GetTransform()->SetLocalPosition({ 8.f, 35.f });
#pragma region 빗물 파티클
	{
		// GameObject* RainFloorParticle1_1_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_1");
		// GameObject* RainFloorParticle1_1_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_2");
		// RainFloorParticle1_1_1->GetTransform()->SetLocalPosition(Math::Vector2D(-360.f, 1019.f));
		// RainFloorParticle1_1_2->GetTransform()->SetLocalPosition(Math::Vector2D(190.f, 1019.f));

		GameObject* RainFloorParticle1_1_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_1");
		RainFloorParticle1_1_1->GetTransform()->SetLocalPosition({0.f, 0.f});
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
		// floorParticle1_1->SetParticleLineScope(RainFloorParticle1_1_1->GetTransform()->GetLocalPosition());
		// floorParticle1_1->SetParticleLineScope(RainFloorParticle1_1_2->GetTransform()->GetLocalPosition());
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(-380.f, 1019.f));
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(190.f, 1019.f));
		floorParticle1_1->Initialize(
			30, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
		floorParticle1_1->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_2_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_1");
			// GameObject* RainFloorParticle1_2_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_2");
			// RainFloorParticle1_2_1->GetTransform()->SetLocalPosition(Math::Vector2D(202.f, 897.f));
			// RainFloorParticle1_2_2->GetTransform()->SetLocalPosition(Math::Vector2D(320.f, 897.f));

		GameObject* RainFloorParticle1_2_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_1");
		RainFloorParticle1_2_1->GetTransform()->SetLocalPosition({ 0.f, -5.f });
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
			// floorParticle1_2->SetParticleLineScope(RainFloorParticle1_2_1->GetTransform()->GetLocalPosition());
			// floorParticle1_2->SetParticleLineScope(RainFloorParticle1_2_2->GetTransform()->GetLocalPosition());
			floorParticle1_2->SetParticleLineScope(Math::Vector2D(202.f, 897.f));
			floorParticle1_2->SetParticleLineScope(Math::Vector2D(320.f, 897.f));
			floorParticle1_2->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_2->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_3_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_1");
			// GameObject* RainFloorParticle1_3_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_2");
			// RainFloorParticle1_3_1->GetTransform()->SetLocalPosition(Math::Vector2D(330.f, 1019.f));
			// RainFloorParticle1_3_2->GetTransform()->SetLocalPosition(Math::Vector2D(685.f, 1019.f));
			
			GameObject* RainFloorParticle1_3_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_1");
			RainFloorParticle1_3_1->GetTransform()->SetLocalPosition({ 5.f, 0.f });
			ParticleSystem* floorParticle1_3 = RainFloorParticle1_3_1->AddComponent<ParticleSystem>();
			floorParticle1_3->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_3->SetGravity(200.f);
			floorParticle1_3->SetLayer(1);
			floorParticle1_3->SetDirectionRangeDegree(30.f);
			floorParticle1_3->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_3->SetUsePoint(false);
			// floorParticle1_3->SetGameObjectPointer(RainFloorParticle1_3_1);
			// floorParticle1_3->SetGameObjectPointer(RainFloorParticle1_3_2);
			// floorParticle1_3->SetParticleLineScope(RainFloorParticle1_3_1->GetTransform()->GetLocalPosition());
			// floorParticle1_3->SetParticleLineScope(RainFloorParticle1_3_2->GetTransform()->GetLocalPosition());
			floorParticle1_3->SetParticleLineScope(Math::Vector2D(330.f, 1019.f));
			floorParticle1_3->SetParticleLineScope(Math::Vector2D(700.f, 1019.f));
			floorParticle1_3->Initialize(
				20, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_3->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_4_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_4_1");
			// GameObject* RainFloorParticle1_4_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_4_2");
			// RainFloorParticle1_4_1->GetTransform()->SetLocalPosition(Math::Vector2D(695.f, 773.f));
			// RainFloorParticle1_4_2->GetTransform()->SetLocalPosition(Math::Vector2D(813.f, 773.f));

			GameObject* RainFloorParticle1_4_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_4_1");
			RainFloorParticle1_4_1->GetTransform()->SetLocalPosition({ 20.f, -8.f });
			ParticleSystem* floorParticle1_4 = RainFloorParticle1_4_1->AddComponent<ParticleSystem>();
			floorParticle1_4->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_4->SetGravity(200.f);
			floorParticle1_4->SetLayer(1);
			floorParticle1_4->SetDirectionRangeDegree(30.f);
			floorParticle1_4->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_4->SetUsePoint(false);
			// floorParticle1_4->SetGameObjectPointer(RainFloorParticle1_4_1);
			// floorParticle1_4->SetGameObjectPointer(RainFloorParticle1_4_2);
			// floorParticle1_4->SetParticleLineScope(RainFloorParticle1_4_1->GetTransform()->GetLocalPosition());
			// floorParticle1_4->SetParticleLineScope(RainFloorParticle1_4_2->GetTransform()->GetLocalPosition());
			floorParticle1_4->SetParticleLineScope(Math::Vector2D(695.f, 773.f));
			floorParticle1_4->SetParticleLineScope(Math::Vector2D(813.f, 773.f));
			floorParticle1_4->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_4->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_5_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_5_1");
			// GameObject* RainFloorParticle1_5_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_5_2");
			// RainFloorParticle1_5_1->GetTransform()->SetLocalPosition(Math::Vector2D(820.f, 1019.f));
			// RainFloorParticle1_5_2->GetTransform()->SetLocalPosition(Math::Vector2D(1183.f, 1019.f));

			GameObject* RainFloorParticle1_5_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_5_1");
			RainFloorParticle1_5_1->GetTransform()->SetLocalPosition({ 25.f, 0.f });
			ParticleSystem* floorParticle1_5 = RainFloorParticle1_5_1->AddComponent<ParticleSystem>();
			floorParticle1_5->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_5->SetGravity(200.f);
			floorParticle1_5->SetLayer(1);
			floorParticle1_5->SetDirectionRangeDegree(30.f);
			floorParticle1_5->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_5->SetUsePoint(false);
			// floorParticle1_5->SetGameObjectPointer(RainFloorParticle1_5_1);
			// floorParticle1_5->SetGameObjectPointer(RainFloorParticle1_5_2);
			// floorParticle1_5->SetParticleLineScope(RainFloorParticle1_5_1->GetTransform()->GetLocalPosition());
			// floorParticle1_5->SetParticleLineScope(RainFloorParticle1_5_2->GetTransform()->GetLocalPosition());
			floorParticle1_5->SetParticleLineScope(Math::Vector2D(820.f, 1019.f));
			floorParticle1_5->SetParticleLineScope(Math::Vector2D(1190.f, 1019.f));
			floorParticle1_5->Initialize(
				15, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_5->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_6_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_6_1");
			// GameObject* RainFloorParticle1_6_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_6_2");
			// RainFloorParticle1_6_1->GetTransform()->SetLocalPosition(Math::Vector2D(1190.f, 649.f));
			// RainFloorParticle1_6_2->GetTransform()->SetLocalPosition(Math::Vector2D(1308.f, 649.f));

			GameObject* RainFloorParticle1_6_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_6_1");
			RainFloorParticle1_6_1->GetTransform()->SetLocalPosition({ 35.f, -13.f });
			ParticleSystem* floorParticle1_6 = RainFloorParticle1_6_1->AddComponent<ParticleSystem>();
			floorParticle1_6->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_6->SetGravity(200.f);
			floorParticle1_6->SetLayer(1);
			floorParticle1_6->SetDirectionRangeDegree(30.f);
			floorParticle1_6->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_6->SetUsePoint(false);
			// floorParticle1_6->SetGameObjectPointer(RainFloorParticle1_6_1);
			// floorParticle1_6->SetGameObjectPointer(RainFloorParticle1_6_2);
			// floorParticle1_6->SetParticleLineScope(RainFloorParticle1_6_1->GetTransform()->GetLocalPosition());
			// floorParticle1_6->SetParticleLineScope(RainFloorParticle1_6_2->GetTransform()->GetLocalPosition());
			floorParticle1_6->SetParticleLineScope(Math::Vector2D(1190.f, 649.f));
			floorParticle1_6->SetParticleLineScope(Math::Vector2D(1308.f, 649.f));
			floorParticle1_6->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_6->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_6_1_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_6_1_1");
			// GameObject* RainFloorParticle1_6_1_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_6_1_2");
			// RainFloorParticle1_6_1_1->GetTransform()->SetLocalPosition(Math::Vector2D(1313.f, 1019.f));
			// RainFloorParticle1_6_1_2->GetTransform()->SetLocalPosition(Math::Vector2D(1676.f, 1019.f));

			GameObject* RainFloorParticle1_6_1_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_6_1_1");
			RainFloorParticle1_6_1_1->GetTransform()->SetLocalPosition({ 40.f, 0.f });
			ParticleSystem* floorParticle1_6_1 = RainFloorParticle1_6_1_1->AddComponent<ParticleSystem>();
			floorParticle1_6_1->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_6_1->SetGravity(200.f);
			floorParticle1_6_1->SetLayer(1);
			floorParticle1_6_1->SetDirectionRangeDegree(30.f);
			floorParticle1_6_1->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_6_1->SetUsePoint(false);
			// floorParticle1_6_1->SetGameObjectPointer(RainFloorParticle1_6_1_1);
			// floorParticle1_6_1->SetGameObjectPointer(RainFloorParticle1_6_1_2);
			// floorParticle1_6_1->SetParticleLineScope(RainFloorParticle1_6_1_1->GetTransform()->GetLocalPosition());
			// floorParticle1_6_1->SetParticleLineScope(RainFloorParticle1_6_1_2->GetTransform()->GetLocalPosition());
			floorParticle1_6_1->SetParticleLineScope(Math::Vector2D(1313.f, 1019.f));
			floorParticle1_6_1->SetParticleLineScope(Math::Vector2D(1684.f, 1019.f));
			floorParticle1_6_1->Initialize(
				15, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_6_1->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_7_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_7_1");
			// GameObject* RainFloorParticle1_7_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_7_2");
			// RainFloorParticle1_7_1->GetTransform()->SetLocalPosition(Math::Vector2D(1681.f, 649.f));
			// RainFloorParticle1_7_2->GetTransform()->SetLocalPosition(Math::Vector2D(1799.f, 649.f));


			GameObject* RainFloorParticle1_7_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_7_1");
			RainFloorParticle1_7_1->GetTransform()->SetLocalPosition({ 55.f, -13.f });
			ParticleSystem* floorParticle1_7 = RainFloorParticle1_7_1->AddComponent<ParticleSystem>();
			floorParticle1_7->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_7->SetGravity(200.f);
			floorParticle1_7->SetLayer(1);
			floorParticle1_7->SetDirectionRangeDegree(30.f);
			floorParticle1_7->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_7->SetParticleLineScope(RainFloorParticle1_7_1->GetTransform()->GetLocalPosition());
			// floorParticle1_7->SetParticleLineScope(RainFloorParticle1_7_2->GetTransform()->GetLocalPosition());
			floorParticle1_7->SetParticleLineScope(Math::Vector2D(1681.f, 649.f));
			floorParticle1_7->SetParticleLineScope(Math::Vector2D(1799.f, 649.f));
			floorParticle1_7->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_7->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_8_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_8_1");
			// GameObject* RainFloorParticle1_8_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_8_2");
			// RainFloorParticle1_8_1->GetTransform()->SetLocalPosition(Math::Vector2D(1804.f, 1019.f));
			// RainFloorParticle1_8_2->GetTransform()->SetLocalPosition(Math::Vector2D(2167.f, 1019.f));

			GameObject* RainFloorParticle1_8_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_8_1");
			RainFloorParticle1_8_1->GetTransform()->SetLocalPosition({ 60.f, 0.f });
			ParticleSystem* floorParticle1_8 = RainFloorParticle1_8_1->AddComponent<ParticleSystem>();
			floorParticle1_8->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_8->SetGravity(200.f);
			floorParticle1_8->SetLayer(1);
			floorParticle1_8->SetDirectionRangeDegree(30.f);
			floorParticle1_8->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_8->SetParticleLineScope(RainFloorParticle1_8_1->GetTransform()->GetLocalPosition());
			// floorParticle1_8->SetParticleLineScope(RainFloorParticle1_8_2->GetTransform()->GetLocalPosition());
			floorParticle1_8->SetParticleLineScope(Math::Vector2D(1804.f, 1019.f));
			floorParticle1_8->SetParticleLineScope(Math::Vector2D(2170.f, 1019.f));
			floorParticle1_8->Initialize(
				20, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_8->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_9_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_9_1");
			// GameObject* RainFloorParticle1_9_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_9_2");
			// RainFloorParticle1_9_1->GetTransform()->SetLocalPosition(Math::Vector2D(2176.f, 649.f));
			// RainFloorParticle1_9_2->GetTransform()->SetLocalPosition(Math::Vector2D(2294.f, 649.f));

			GameObject* RainFloorParticle1_9_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_9_1");
			RainFloorParticle1_9_1->GetTransform()->SetLocalPosition({ 74.f, -13.f });
			ParticleSystem* floorParticle1_9 = RainFloorParticle1_9_1->AddComponent<ParticleSystem>();
			floorParticle1_9->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_9->SetGravity(200.f);
			floorParticle1_9->SetLayer(1);
			floorParticle1_9->SetDirectionRangeDegree(30.f);
			floorParticle1_9->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_9->SetParticleLineScope(RainFloorParticle1_9_1->GetTransform()->GetLocalPosition());
			// floorParticle1_9->SetParticleLineScope(RainFloorParticle1_9_2->GetTransform()->GetLocalPosition());
			floorParticle1_9->SetParticleLineScope(Math::Vector2D(2176.f, 649.f));
			floorParticle1_9->SetParticleLineScope(Math::Vector2D(2294.f, 649.f));
			floorParticle1_9->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_9->ParticleSystemSet();
		}

		{
			// GameObject* RainFloorParticle1_10_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_10_1");
			// GameObject* RainFloorParticle1_10_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_10_2");
			// RainFloorParticle1_10_1->GetTransform()->SetLocalPosition(Math::Vector2D(2302.f, 1019.f));
			// RainFloorParticle1_10_2->GetTransform()->SetLocalPosition(Math::Vector2D(2478.f, 1019.f));

			GameObject* RainFloorParticle1_10_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_10_1");
			RainFloorParticle1_10_1->GetTransform()->SetLocalPosition({ 82.f, 0.f });
			ParticleSystem* floorParticle1_10 = RainFloorParticle1_10_1->AddComponent<ParticleSystem>();
			floorParticle1_10->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_10->SetGravity(200.f);
			floorParticle1_10->SetLayer(1);
			floorParticle1_10->SetDirectionRangeDegree(30.f);
			floorParticle1_10->SetDirection(Math::Vector2D(0.f, -1.f));
			// floorParticle1_10->SetParticleLineScope(RainFloorParticle1_10_1->GetTransform()->GetLocalPosition());
			// floorParticle1_10->SetParticleLineScope(RainFloorParticle1_10_2->GetTransform()->GetLocalPosition());
			floorParticle1_10->SetParticleLineScope(Math::Vector2D(2302.f, 1019.f));
			floorParticle1_10->SetParticleLineScope(Math::Vector2D(2478.f, 1019.f));
			floorParticle1_10->Initialize(
				15, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_10->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_11_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_11_1");
			RainFloorParticle1_11_1->GetTransform()->SetLocalPosition({ 95.f, -13.f });
			ParticleSystem* floorParticle1_11 = RainFloorParticle1_11_1->AddComponent<ParticleSystem>();
			floorParticle1_11->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_11->SetGravity(200.f);
			floorParticle1_11->SetLayer(1);
			floorParticle1_11->SetDirectionRangeDegree(30.f);
			floorParticle1_11->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_11->SetParticleLineScope(Math::Vector2D(2734.f, 649.f));
			floorParticle1_11->SetParticleLineScope(Math::Vector2D(2852.f, 649.f));
			floorParticle1_11->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_11->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_12_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_12_1");
			RainFloorParticle1_12_1->GetTransform()->SetLocalPosition({ 95.f, 0.f });
			ParticleSystem* floorParticle1_12 = RainFloorParticle1_12_1->AddComponent<ParticleSystem>();
			floorParticle1_12->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_12->SetGravity(200.f);
			floorParticle1_12->SetLayer(1);
			floorParticle1_12->SetDirectionRangeDegree(30.f);
			floorParticle1_12->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_12->SetParticleLineScope(Math::Vector2D(2862.f, 1019.f));
			floorParticle1_12->SetParticleLineScope(Math::Vector2D(3037.f, 1019.f));
			floorParticle1_12->Initialize(
				13, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_12->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_13_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_13_1");
			RainFloorParticle1_13_1->GetTransform()->SetLocalPosition({ 110.f, -5.f });
			ParticleSystem* floorParticle1_13 = RainFloorParticle1_13_1->AddComponent<ParticleSystem>();
			floorParticle1_13->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_13->SetGravity(200.f);
			floorParticle1_13->SetLayer(1);
			floorParticle1_13->SetDirectionRangeDegree(30.f);
			floorParticle1_13->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_13->SetParticleLineScope(Math::Vector2D(3038.f, 897.f));
			floorParticle1_13->SetParticleLineScope(Math::Vector2D(3156.f, 897.f));
			floorParticle1_13->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_13->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_14_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_14_1");
			RainFloorParticle1_14_1->GetTransform()->SetLocalPosition({ 113.f, -23.f });
			ParticleSystem* floorParticle1_14 = RainFloorParticle1_14_1->AddComponent<ParticleSystem>();
			floorParticle1_14->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_14->SetGravity(200.f);
			floorParticle1_14->SetLayer(1);
			floorParticle1_14->SetDirectionRangeDegree(30.f);
			floorParticle1_14->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_14->SetParticleLineScope(Math::Vector2D(3162.f, 403.f));
			floorParticle1_14->SetParticleLineScope(Math::Vector2D(3280.f, 403.f));
			floorParticle1_14->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_14->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_15_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_15_1");
			RainFloorParticle1_15_1->GetTransform()->SetLocalPosition({ 120.f, -5.f });
			ParticleSystem* floorParticle1_15 = RainFloorParticle1_15_1->AddComponent<ParticleSystem>();
			floorParticle1_15->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_15->SetGravity(200.f);
			floorParticle1_15->SetLayer(1);
			floorParticle1_15->SetDirectionRangeDegree(30.f);
			floorParticle1_15->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_15->SetParticleLineScope(Math::Vector2D(3288.f, 897.f));
			floorParticle1_15->SetParticleLineScope(Math::Vector2D(3406.f, 897.f));
			floorParticle1_15->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_15->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_16_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_16_1");
			RainFloorParticle1_16_1->GetTransform()->SetLocalPosition({ 123.f, 0.f });
			ParticleSystem* floorParticle1_16 = RainFloorParticle1_16_1->AddComponent<ParticleSystem>();
			floorParticle1_16->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_16->SetGravity(200.f);
			floorParticle1_16->SetLayer(1);
			floorParticle1_16->SetDirectionRangeDegree(30.f);
			floorParticle1_16->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_16->SetParticleLineScope(Math::Vector2D(3414.f, 1019.f));
			floorParticle1_16->SetParticleLineScope(Math::Vector2D(3778.f, 1019.f));
			floorParticle1_16->Initialize(
				15, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_16->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_17_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_17_1");
			RainFloorParticle1_17_1->GetTransform()->SetLocalPosition({ 143.f, 0.f });
			ParticleSystem* floorParticle1_17 = RainFloorParticle1_17_1->AddComponent<ParticleSystem>();
			floorParticle1_17->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_17->SetGravity(200.f);
			floorParticle1_17->SetLayer(1);
			floorParticle1_17->SetDirectionRangeDegree(30.f);
			floorParticle1_17->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_17->SetParticleLineScope(Math::Vector2D(4034.f, 1019.f));
			floorParticle1_17->SetParticleLineScope(Math::Vector2D(4335.f, 1019.f));
			floorParticle1_17->Initialize(
				13, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_17->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_18_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_18_1");
			RainFloorParticle1_18_1->GetTransform()->SetLocalPosition({ 157.f, -5.f });
			ParticleSystem* floorParticle1_18 = RainFloorParticle1_18_1->AddComponent<ParticleSystem>();
			floorParticle1_18->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_18->SetGravity(200.f);
			floorParticle1_18->SetLayer(1);
			floorParticle1_18->SetDirectionRangeDegree(30.f);
			floorParticle1_18->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_18->SetParticleLineScope(Math::Vector2D(4333.f, 897.f));
			floorParticle1_18->SetParticleLineScope(Math::Vector2D(4451.f, 897.f));
			floorParticle1_18->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_18->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_19_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_19_1");
			RainFloorParticle1_19_1->GetTransform()->SetLocalPosition({ 160.f, -20.f });
			ParticleSystem* floorParticle1_19 = RainFloorParticle1_19_1->AddComponent<ParticleSystem>();
			floorParticle1_19->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_19->SetGravity(200.f);
			floorParticle1_19->SetLayer(1);
			floorParticle1_19->SetDirectionRangeDegree(30.f);
			floorParticle1_19->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_19->SetParticleLineScope(Math::Vector2D(4457.f, 465.f));
			floorParticle1_19->SetParticleLineScope(Math::Vector2D(4575.f, 465.f));
			floorParticle1_19->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_19->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_20_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_20_1");
			RainFloorParticle1_20_1->GetTransform()->SetLocalPosition({ 170.f, -5.f });
			ParticleSystem* floorParticle1_20 = RainFloorParticle1_20_1->AddComponent<ParticleSystem>();
			floorParticle1_20->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_20->SetGravity(200.f);
			floorParticle1_20->SetLayer(1);
			floorParticle1_20->SetDirectionRangeDegree(30.f);
			floorParticle1_20->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_20->SetParticleLineScope(Math::Vector2D(4583.f, 897.f));
			floorParticle1_20->SetParticleLineScope(Math::Vector2D(4701.f, 897.f));
			floorParticle1_20->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_20->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_21_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_21_1");
			RainFloorParticle1_21_1->GetTransform()->SetLocalPosition({ 170.f, 0.f });
			ParticleSystem* floorParticle1_21 = RainFloorParticle1_21_1->AddComponent<ParticleSystem>();
			floorParticle1_21->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_21->SetGravity(200.f);
			floorParticle1_21->SetLayer(1);
			floorParticle1_21->SetDirectionRangeDegree(30.f);
			floorParticle1_21->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_21->SetParticleLineScope(Math::Vector2D(4709.f, 1019.f));
			floorParticle1_21->SetParticleLineScope(Math::Vector2D(4885.f, 1019.f));
			floorParticle1_21->Initialize(
				13, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_21->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_22_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_22_1");
			RainFloorParticle1_22_1->GetTransform()->SetLocalPosition({ 205.f, 0.f });
			ParticleSystem* floorParticle1_22 = RainFloorParticle1_22_1->AddComponent<ParticleSystem>();
			floorParticle1_22->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_22->SetGravity(200.f);
			floorParticle1_22->SetLayer(1);
			floorParticle1_22->SetDirectionRangeDegree(30.f);
			floorParticle1_22->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_22->SetParticleLineScope(Math::Vector2D(5635.f, 1019.f));
			floorParticle1_22->SetParticleLineScope(Math::Vector2D(5996.f, 1019.f));
			floorParticle1_22->Initialize(
				15, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_22->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_23_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_23_1");
			RainFloorParticle1_23_1->GetTransform()->SetLocalPosition({ 218.f, -13.f });
			ParticleSystem* floorParticle1_23 = RainFloorParticle1_23_1->AddComponent<ParticleSystem>();
			floorParticle1_23->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_23->SetGravity(200.f);
			floorParticle1_23->SetLayer(1);
			floorParticle1_23->SetDirectionRangeDegree(30.f);
			floorParticle1_23->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_23->SetParticleLineScope(Math::Vector2D(6003.f, 588.f));
			floorParticle1_23->SetParticleLineScope(Math::Vector2D(6118.f, 588.f));
			floorParticle1_23->Initialize(
				10, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_23->ParticleSystemSet();
		}

		{
			GameObject* RainFloorParticle1_24_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_24_1");
			RainFloorParticle1_24_1->GetTransform()->SetLocalPosition({ 225.f, 0.f });
			ParticleSystem* floorParticle1_24 = RainFloorParticle1_24_1->AddComponent<ParticleSystem>();
			floorParticle1_24->SystemSetting(
				ParticleSystem::eParticleState::Dynamic,
				ParticleSystem::eScope::Line,
				ParticleSystem::eActivity::Regular,
				ParticleSystem::eEmissionMode::Spray,
				ParticleSystem::eRenderType::Rectangle);
			floorParticle1_24->SetGravity(200.f);
			floorParticle1_24->SetLayer(1);
			floorParticle1_24->SetDirectionRangeDegree(30.f);
			floorParticle1_24->SetDirection(Math::Vector2D(0.f, -1.f));
			floorParticle1_24->SetParticleLineScope(Math::Vector2D(6127.f, 1019.f));
			floorParticle1_24->SetParticleLineScope(Math::Vector2D(6930.f, 1019.f));
			floorParticle1_24->Initialize(
				20, RainFloorParticle->GetTransform()->GetWorldPosition(),
				true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			floorParticle1_24->ParticleSystemSet();
		}
#pragma endregion

#pragma region 물방울
		
		{
			/// 맺힌 물방울 파티클
		// 	GameObject* waterDrop1 = CreateGameObject(RainFloorParticle, L"WaterDrop1");
		// 	waterDrop1->GetTransform()->SetLocalPosition(Math::Vector2D(725.f, 800.f));
		// 	ParticleSystem* waterDrop1_1 = waterDrop1->AddComponent<ParticleSystem>();
		// 	waterDrop1_1->SystemSetting(
		// 		ParticleSystem::eParticleState::Static,
		// 		ParticleSystem::eScope::Point,
		// 		ParticleSystem::eActivity::Regular,
		// 		ParticleSystem::eEmissionMode::Spray,
		// 		ParticleSystem::eRenderType::Ellipse);
		// 	waterDrop1_1->SetLayer(1);
		// 	waterDrop1_1->SetDirection(Math::Vector2D(0.f, 1.f));
		// 	waterDrop1_1->Initialize(
		// 		1, waterDrop1->GetTransform()->GetWorldPosition(),
		// 		true, 1.f, 0.05f, 0.5f, 50, 3, BJEngine::GraphicsEngine::Color::White);
		// 	waterDrop1_1->ParticleSystemSet();
		}

		{
			/// 맺혔던 물방울 떨어지는 파티클
			// ParticleSystem* waterDrop1_2 = RainFloorParticle->AddComponent<ParticleSystem>();
			// waterDrop1_2->SystemSetting(
			// 	ParticleSystem::eParticleState::Dynamic,
			// 	ParticleSystem::eScope::Point,
			// 	ParticleSystem::eActivity::Regular,
			// 	ParticleSystem::eEmissionMode::Spray,
			// 	ParticleSystem::eRenderType::Ellipse);
			// waterDrop1_2->SetGravity(200.f);
			// waterDrop1_2->SetLayer(1);
			// waterDrop1_2->SetDirection(Math::Vector2D(0.f, 1.f));
			// waterDrop1_2->Initialize(
			// 	4, RainFloorParticle->GetTransform()->GetWorldPosition(),
			// 	true, 0.05f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
			// waterDrop1_2->ParticleSystemSet();
		}
		
#pragma endregion

#pragma region 비 파티클
	// GameObject* Rain = CreateGameObject(L"Rain");
	// Rain->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	// ParticleSystem* RainParticle = Rain->AddComponent<ParticleSystem>();
	// RainParticle->SystemSetting(
	// 	ParticleSystem::eParticleState::Dynamic,
	// 	ParticleSystem::eScope::Line,
	// 	ParticleSystem::eActivity::Regular,
	// 	ParticleSystem::eEmissionMode::Spray,
	// 	ParticleSystem::eRenderType::Line);
	// RainParticle->SetGravity(200.f);
	// RainParticle->SetDirectionRangeDegree(30.f);
	// RainParticle->SetDirection(Math::Vector2D(0.f, -1.f));
	// RainParticle->SetParticleLineScope(Math::Vector2D(-360.f, 0.f));
	// RainParticle->SetParticleLineScope(Math::Vector2D(7100.f, 0.f));
	// RainParticle->Initialize(
	// 	700, Rain->GetTransform()->GetWorldPosition(),
	// 	true, 1.f, 0.05f, 3.3f, 50, 15, BJEngine::GraphicsEngine::Color::White);
	// RainParticle->ParticleSystemSet();
	// RainParticle->SetLayer(-1);
	{
		ParticleSystem* RainParticle = RainFloorParticle->AddComponent<ParticleSystem>();
		RainParticle->SystemSetting(
			ParticleSystem::eParticleState::Dynamic,
			ParticleSystem::eScope::Line,
			ParticleSystem::eActivity::Regular,
			ParticleSystem::eEmissionMode::Spray,
			ParticleSystem::eRenderType::Line);
		RainParticle->SetGravity(600.f);
		RainParticle->SetDirectionRangeDegree(30.f);
		RainParticle->SetDirection(Math::Vector2D(0.f, 1.f));
		RainParticle->SetParticleLineScope(Math::Vector2D(-360.f, 0.f));
		RainParticle->SetParticleLineScope(Math::Vector2D(7100.f, 0.f));
		RainParticle->Initialize(
			700, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 0, 0.05f, 3.3f, 100, 15, BJEngine::GraphicsEngine::Color::White);
		RainParticle->ParticleSystemSet();
		RainParticle->SetLayer(-1);
	}

	{
		/// 씬 시작할 때 자연스러운 전환을 위한 비 파티클
		GameObject* preLoadParticle = CreateGameObject(L"PreLoadParticle");
		preLoadParticle->GetTransform()->SetLocalPosition({ 930.f, 480.f });
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


#pragma endregion

#pragma region UI
	// UI - 단검 A
	GameObject* tutorialUI_A = CreateGameObject(L"tutorialUI_A");
	tutorialUI_A->GetTransform()->SetLocalPosition(Vector2D(-200.f, 800.f));
	tutorialUI_A->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_A = tutorialUI_A->AddComponent<SpriteRenderer>();
	tutorialImage_A->SetSpriteName(L"TutorialUI_A");
	tutorialImage_A->SetImageNumber(0);
	tutorialImage_A->SetLayer(1);

	// UI - 단검 S
	GameObject* tutorialUI_S = CreateGameObject(L"tutorialUI_S");
	tutorialUI_S->GetTransform()->SetLocalPosition(Vector2D(275.f, 680.f));
	tutorialUI_S->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_S = tutorialUI_S->AddComponent<SpriteRenderer>();
	tutorialImage_S->SetSpriteName(L"TutorialUI_S");
	tutorialImage_S->SetImageNumber(0);
	tutorialImage_S->SetLayer(1);

	// UI - 단검 D
	GameObject* tutorialUI_D = CreateGameObject(L"tutorialUI_D");
	tutorialUI_D->GetTransform()->SetLocalPosition(Vector2D(785.f, 550.f));
	tutorialUI_D->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_D = tutorialUI_D->AddComponent<SpriteRenderer>();
	tutorialImage_D->SetSpriteName(L"TutorialUI_D");
	tutorialImage_D->SetImageNumber(0);
	tutorialImage_D->SetLayer(1);
#pragma endregion

}