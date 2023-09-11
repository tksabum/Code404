#include "Stage1_1.h"

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
#include "SoundManager.h"
#include "ParticleSystem.h"

/// 씬 전환을 위한 참조
#include "EventTrigger.h"
#include "Image.h"
#include "BlackMask.h"

/// 편한 디버깅을 위한 씬 컨트롤러
// #include "SceneController.h"
#include "DebugInfo.h"

/// 메세지 이벤트를 위해서 참조
#include "MessageEvent.h"
#include "MessageManager.h"
#include "UI.h"
#include "Button.h"

#include "BackGround.h"

#include "Matrix3x3.h"

#include "ParryingSystem.h"

// #include "Controller.h"

using namespace BJEngine;
using namespace BJEngine::Math;

Stage1_1::Stage1_1() : Scene(L"Stage1_1")
{

}

void Stage1_1::Start()
{
	

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
	outBlackMask->SetScene(L"Stage1_2");

	GameObject* nextScene = CreateGameObject(L"NextScene");
	nextScene->GetTransform()->SetLocalPosition({ -400.f + 3680.f, 200.f + 820.f });
	nextScene->SetLayerName(L"Event");
	ConvexpolygonCollider* changerCollider = nextScene->AddComponent<ConvexpolygonCollider>();
	changerCollider->SetPolygon({ { 0.f, -1080.f }, { 0.f, 64.f }, { 64.f, 64.f }, {64.f, -1080.f} });
	EventTrigger* trigger = nextScene->AddComponent<EventTrigger>();
	trigger->SetActiveObject(blackmaskObject);


	GameObject* blackmaskObject2 = CreateGameObject(L"BlackMaskObject");
	blackmaskObject2->GetTransform()->SetLocalPosition({ -2300.f, 0.f });
	blackmaskObject2->SetActive(false);
	Image* blackmaskimage2 = blackmaskObject2->AddComponent<Image>();
	blackmaskimage2->SetSpriteName(L"SwitchScreen");
	blackmaskimage2->SetImageNumber(0);
	blackmaskimage2->SetLayer(0);
	BlackMask* outBlackMask2 = blackmaskObject2->AddComponent<BlackMask>();
	outBlackMask2->SetMode(BlackMask::eMode::SCENE_OUT);
	outBlackMask2->SetScene(L"SampleScene2");

	GameObject* softScene = CreateGameObject(L"SoftBodySceneGo");
	softScene->GetTransform()->SetLocalPosition({ -400.f + 37*64.f, 200.f + 5*64.f });
	softScene->SetLayerName(L"SoftBodyScene");
	ConvexpolygonCollider* changerCollider2 = softScene->AddComponent<ConvexpolygonCollider>();
	changerCollider2->SetPolygon({ { -64.f, -64.f }, { -64.f, 64.f }, { 64.f, 64.f }, {64.f, -64.f} });
	EventTrigger* trigger2 = softScene->AddComponent<EventTrigger>();
	trigger2->SetActiveObject(blackmaskObject2);
	
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
	Level level(STAGE_ONE_ONE);
	level.Tiling(level);
	level.CreatePlatformFromLevelData(level);
	level.CreateItem(level);
	// level.SoftBodyScene(level);
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
		BJEngine::GameObject* platform = CreateGameObject(L"platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400, 200));
		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(24.5*64, 13.5*64), Vector2D(28.5*64, 9.5*64), Vector2D(29.5*64,9.5*64), Vector2D(29.5*64, 13.5*64)});
		platform->SetLayerName(L"Platform");
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
		background->GetTransform()->SetLocalPosition(Vector2D(0.f, 365.f));
		SpriteRenderer* sprite = background->AddComponent<SpriteRenderer>();
		sprite->SetSpriteName(L"ExternalBackgroundObject2");
		sprite->SetImageNumber(0);
		sprite->SetLayer(-6);
		Background* bg = background->AddComponent<Background>();
		bg->SetCameraDepth(1.0f);
		bg->SetImageZaxisPosition(0.5f);
	}
#pragma endregion

#pragma region 적
	 GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(2400.0f, 600.0f));

#pragma endregion
	 
#pragma region 메세지 이벤트
	MessageManager& messageManager = MessageManager::GetInstance();

	// 메세지 여백
	BJEngine::Math::Vector2D margin = {120.f, 100.f};

	// 메세지 박스
	GameObject* messageBox = CreateGameObject(L"MessageBox");
	messageBox->GetTransform()->SetLocalPosition({ 300.f, 200.f });
	messageBox->SetActive(false);
	UI* messageBoxUI = messageBox->AddComponent<UI>();
	messageBoxUI->SetSize(Vector2D(850, 400));
	Image* messageBoxImage = messageBox->AddComponent<Image>();
	messageBoxImage->SetSpriteName(L"UI_MessageBox");
	messageBoxImage->SetImageNumber(0);
	messageBoxImage->SetLayer(0);

	// 이벤트를 부를 트리거
	GameObject* messageEventTriggerObject = CreateGameObject(L"MessageEventTriggerObject");
	messageEventTriggerObject->GetTransform()->SetLocalPosition(Vector2D(250.0f, 1080.f));
	messageEventTriggerObject->SetLayerName(L"Event");
	ConvexpolygonCollider* triggerCollider = messageEventTriggerObject->AddComponent<ConvexpolygonCollider>();
	triggerCollider->SetPolygon({ { 0.f, -1080.f }, { 0.f, 64.f }, { 64.f, 64.f }, {64.f, -1080.f} });
	EventTrigger* messageTrigger = messageEventTriggerObject->AddComponent<EventTrigger>();
	messageTrigger->SetActiveObject(messageBox);


	// 대사1 의 답변1
	GameObject* messageReplyBox1 = CreateGameObject(messageBox, L"MessageReplyObject1_1_1");
	messageReplyBox1->GetTransform()->SetLocalPosition({ 125.f, 250.f });
	messageReplyBox1->GetTransform()->SetLocalScale({0.3f, 0.2f, 1.f});
	messageReplyBox1->AddComponent<UI>()->SetSize({ 850.f * 0.3f, 400.f * 0.2f });
	messageReplyBox1->SetActive(false);
	Image* messageReplyImage1 = messageReplyBox1->AddComponent<Image>();
	messageReplyImage1->SetSpriteName(L"UI_MessageBox");
	messageReplyImage1->SetImageNumber(0);
	messageReplyImage1->SetLayer(1);
	Button* messageReplyButton1_1 = messageReplyBox1->AddComponent<Button>();

	GameObject* messageReplyObject1_1_1 = CreateGameObject(messageBox, L"MessageReplyObject1_1_1");
	messageReplyObject1_1_1->GetTransform()->SetLocalPosition({ 125.f + 80.f, 250.f + 27.f });
	messageReplyObject1_1_1->SetActive(false);
	Text* messageReplyRender1_1_1 = messageReplyObject1_1_1->AddComponent<Text>();
	messageReplyRender1_1_1->SetText(L".......");
	messageReplyRender1_1_1->SetFont(L"MKStencilsansBlack");
	messageReplyRender1_1_1->SetTextAreaSize({ 850.f * 0.3f, 400.f * 0.2f });
	messageReplyRender1_1_1->SetTextSize(20);
	messageReplyRender1_1_1->SetLayer(2);
	
	GameObject* messageReplyBox2 = CreateGameObject(messageBox, L"MessageReplyObject1_1_1");
	messageReplyBox2->GetTransform()->SetLocalPosition({ 470.f, 250.f });
	messageReplyBox2->GetTransform()->SetLocalScale({ 0.3f, 0.2f, 1.f });
	messageReplyBox2->AddComponent<UI>()->SetSize({ 850.f * 0.3f, 400.f * 0.2f });
	messageReplyBox2->SetActive(false);
	Image* messageReplyImage2 = messageReplyBox2->AddComponent<Image>();
	messageReplyImage2->SetSpriteName(L"UI_MessageBox");
	messageReplyImage2->SetImageNumber(0);
	messageReplyImage2->SetLayer(1);
	Button* messageReplyButton1_2 = messageReplyBox2->AddComponent<Button>();

	// 대사1 의 답변2
	GameObject* messageReplyObject1_1_2 = CreateGameObject(messageBox, L"MessageReplyObject1_1_2");
	messageReplyObject1_1_2->GetTransform()->SetLocalPosition({ 470.f + 100.f, 250.f + 27.f });
	messageReplyObject1_1_2->SetActive(false);
	Text* messageReplyRender1_1_2 = messageReplyObject1_1_2->AddComponent<Text>();
	messageReplyRender1_1_2->SetText(L"그래.");
	messageReplyRender1_1_2->SetFont(L"MKStencilsansBlack");
	messageReplyRender1_1_2->SetTextAreaSize({ 850.f * 0.3f, 400.f * 0.2f });
	messageReplyRender1_1_2->SetTextSize(20);
	messageReplyRender1_1_2->SetLayer(2);




	// 대사 1 렌더
	GameObject* messageEventObejct1_1 = CreateGameObject(messageBox, L"MessageEventObejct1_1");
	messageEventObejct1_1->SetActive(true);
	Text* textRenderer1_1 = messageEventObejct1_1->AddComponent<Text>();
	textRenderer1_1->SetLayer(2);
	MessageEvent* messageEvent1_1 = messageEventObejct1_1->AddComponent<MessageEvent>();
	messageEvent1_1->SetRenderer(textRenderer1_1);
	messageEvent1_1->SetMessageEvent(messageManager.GetMessage(L"EventMessage1-1"), L"MKStencilsansBlack", 0.1f, margin.x, margin.y, messageBoxUI->GetSize());
	messageEvent1_1->AddEventButton(messageReplyBox1);
	messageEvent1_1->AddEventButton(messageReplyObject1_1_1);
	messageEvent1_1->AddEventButton(messageReplyBox2);
	messageEvent1_1->AddEventButton(messageReplyObject1_1_2);

	// 대사 2 렌더
	GameObject* messageEventObejct1_2 = CreateGameObject(messageBox, L"MessageEventObejct1_2");
	Text* textRenderer1_2 = messageEventObejct1_2->AddComponent<Text>();
	textRenderer1_2->SetLayer(2);
	MessageEvent* messageEvent1_2 = messageEventObejct1_2->AddComponent<MessageEvent>();
	messageEvent1_2->SetRenderer(textRenderer1_2);
	messageEvent1_2->SetMessageEvent(messageManager.GetMessage(L"EventMessage1-2"), L"MKStencilsansBlack", 0.1f, margin.x, margin.y, messageBoxUI->GetSize());
	messageEventObejct1_2->SetActive(false);

	// 대사 3 렌더
	GameObject* messageEventObejct1_3 = CreateGameObject(messageBox, L"MessageEventObejct1_3");
	Text* textRenderer1_3 = messageEventObejct1_3->AddComponent<Text>();
	textRenderer1_3->SetLayer(2);
	MessageEvent* messageEvent1_3 = messageEventObejct1_3->AddComponent<MessageEvent>();
	messageEvent1_3->SetRenderer(textRenderer1_3);
	messageEvent1_3->SetMessageEvent(messageManager.GetMessage(L"EventMessage1-3"), L"MKStencilsansBlack", 0.1f, margin.x, margin.y, messageBoxUI->GetSize());
	messageEventObejct1_3->SetActive(false);

	GameObject* messageReplyBox4 = CreateGameObject(messageBox, L"MessageReplyObject1_1_1");
	messageReplyBox4->GetTransform()->SetLocalPosition({ 470.f, 250.f });
	messageReplyBox4->GetTransform()->SetLocalScale({ 0.3f, 0.2f, 1.f });
	messageReplyBox4->AddComponent<UI>()->SetSize({ 850.f * 0.3f, 400.f * 0.2f });
	messageReplyBox4->SetActive(false);
	Image* messageReplyImage4 = messageReplyBox4->AddComponent<Image>();
	messageReplyImage4->SetSpriteName(L"UI_MessageBox");
	messageReplyImage4->SetImageNumber(0);
	messageReplyImage4->SetLayer(1);
	Button* messageReplyButton4_1 = messageReplyBox4->AddComponent<Button>();

	// 대사 4의 답변1
	GameObject* messageReplyObject1_4_1 = CreateGameObject(messageBox, L"MessageReplyObject1_4_1");
	messageReplyObject1_4_1->GetTransform()->SetLocalPosition({ 470.f + 40.f, 250.f + 27.f });
	messageReplyObject1_4_1->SetActive(false);
	Text* messageReplyRender1_4_1 = messageReplyObject1_4_1->AddComponent<Text>();
	messageReplyRender1_4_1->SetText(L"(무전기를 끊는다.)");
	messageReplyRender1_4_1->SetFont(L"MKStencilsansBlack");
	messageReplyRender1_4_1->SetTextAreaSize({ 850.f * 0.3f, 400.f * 0.2f });
	messageReplyRender1_4_1->SetTextSize(20);
	messageReplyRender1_4_1->SetLayer(2);

	// 대사 4 렌더
	GameObject* messageEventObejct1_4 = CreateGameObject(messageBox, L"MessageEventObejct1_4");
	Text* textRenderer1_4 = messageEventObejct1_4->AddComponent<Text>();
	textRenderer1_4->SetLayer(2);
	MessageEvent* messageEvent1_4 = messageEventObejct1_4->AddComponent<MessageEvent>();
	messageEvent1_4->SetRenderer(textRenderer1_4);
	messageEvent1_4->SetMessageEvent(messageManager.GetMessage(L"EventMessage1-4"), L"MKStencilsansBlack", 0.1f, margin.x, margin.y, messageBoxUI->GetSize());
	messageEventObejct1_4->SetActive(false);
	messageEvent1_4->AddEventButton(messageReplyBox4);
	messageEvent1_4->AddEventButton(messageReplyObject1_4_1);

	/// 버튼 없이 대사만 이어서 나오는 메세지는
	/// 앞 뒤를 연결해줘서 다음껄 켜주고
	/// 다음 메세지는 이전껄 꺼주는 식으로 귀찮게 만듬
	{
		messageEvent1_2->SetNextMessage(messageEvent1_3);

		messageEvent1_3->SetPrevMessage(messageEvent1_2);
		messageEvent1_3->SetNextMessage(messageEvent1_4);

		messageEvent1_4->SetPrevMessage(messageEvent1_3);
	}

	// 대사1의 답변1
	messageReplyButton1_1->AddButtonEvent([=]()
		{
			// 버튼을 누르면 다음 텍스트 이벤트가 발생함.
			messageEventObejct1_1->SetActive(false);
			messageReplyBox1->SetActive(false);
			messageReplyBox2->SetActive(false);
			messageReplyObject1_1_1->SetActive(false);
			messageReplyObject1_1_2->SetActive(false);
			messageEventObejct1_2->SetActive(true);	
		}
	);

	// 대사1의 답변2
	messageReplyButton1_2->AddButtonEvent([=]()
		{
			// 버튼을 누르면 다음 텍스트 이벤트가 발생함.
			messageEventObejct1_1->SetActive(false);
			messageReplyBox1->SetActive(false);
			messageReplyBox2->SetActive(false);
			messageReplyObject1_1_1->SetActive(false);
			messageReplyObject1_1_2->SetActive(false);
			messageEventObejct1_2->SetActive(true);
		}
	);

	// 대사4의 답변1
	messageReplyButton4_1->AddButtonEvent([=]()
		{
			messageEventObejct1_4->SetActive(false);
			messageBox->SetActive(false);
			messageBox->GetTimeController()->SetTimeScale(1.f);
			Input::GetInstance().SetKeyActive(true);
		}
	);

#pragma endregion
	 

#pragma region 파티클
	GameObject* RainFloorParticle = CreateGameObject(L"RainFloorParticle");
	RainFloorParticle->GetTransform()->SetLocalPosition({ 0, 35.f });
	{
		// GameObject* RainFloorParticle1_1_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_2");
		// RainFloorParticle1_1_1->GetTransform()->SetLocalPosition(Math::Vector2D(-390.f, 1019.f));
		// RainFloorParticle1_1_2->GetTransform()->SetLocalPosition(Math::Vector2D(390.f, 1019.f));
		// ParticleSystem* floorParticle1_1 = RainFloorParticle1_1_1->AddComponent<ParticleSystem>();

		GameObject* RainFloorParticle1_1_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_1_1");
		RainFloorParticle1_1_1->GetTransform()->SetLocalPosition({ 0.f, 0.f });
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
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(-380.f, 1019.f));
		floorParticle1_1->SetParticleLineScope(Math::Vector2D(390.f, 1019.f));
		floorParticle1_1->Initialize(
			40, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 3.f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
		floorParticle1_1->ParticleSystemSet();
	}

	{
		// GameObject* RainFloorParticle1_2_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_1");
		// GameObject* RainFloorParticle1_2_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_2");
		// RainFloorParticle1_2_1->GetTransform()->SetLocalPosition(Math::Vector2D(390.f, 895.f));
		// RainFloorParticle1_2_2->GetTransform()->SetLocalPosition(Math::Vector2D(500.f, 895.f));

		GameObject* RainFloorParticle1_2_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_2_1");
		RainFloorParticle1_2_1->GetTransform()->SetLocalPosition({15.f, 0.f});
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
		floorParticle1_2->SetParticleLineScope(Math::Vector2D(390.f, 895.f));
		floorParticle1_2->SetParticleLineScope(Math::Vector2D(505.f, 895.f));
		floorParticle1_2->Initialize(
			10, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 3.f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
		floorParticle1_2->ParticleSystemSet();
	}

	{
		// GameObject* RainFloorParticle1_3_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_1");
		// GameObject* RainFloorParticle1_3_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_2");
		// GameObject* RainFloorParticle1_3_3 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_3");
		// GameObject* RainFloorParticle1_3_4 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_4");
		// RainFloorParticle1_3_1->GetTransform()->SetLocalPosition(Math::Vector2D(500.f, 1020.f));
		// RainFloorParticle1_3_2->GetTransform()->SetLocalPosition(Math::Vector2D(1053, 1020.f));
		// RainFloorParticle1_3_3->GetTransform()->SetLocalPosition(Math::Vector2D(1300.f, 770.f));
		// RainFloorParticle1_3_4->GetTransform()->SetLocalPosition(Math::Vector2D(1560.f, 770.f));

		GameObject* RainFloorParticle1_3_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_3_1");
		RainFloorParticle1_3_1->GetTransform()->SetLocalPosition({50.f, 0.f});
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
		// floorParticle1_3->SetGameObjectPointer(RainFloorParticle1_3_3);
		// floorParticle1_3->SetGameObjectPointer(RainFloorParticle1_3_4);
		floorParticle1_3->SetParticleLineScope(Math::Vector2D(490.f, 1020.f));
		floorParticle1_3->SetParticleLineScope(Math::Vector2D(1053, 1020.f));
		floorParticle1_3->SetParticleLineScope(Math::Vector2D(1300.f, 765.f));
		floorParticle1_3->SetParticleLineScope(Math::Vector2D(1560.f, 765.f));
		floorParticle1_3->Initialize(
			60, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 3.f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
		floorParticle1_3->ParticleSystemSet();
	}

	
	{
		// GameObject* RainFloorParticle1_4_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_4_1");
		// GameObject* RainFloorParticle1_4_2 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_4_2");
		// RainFloorParticle1_4_1->GetTransform()->SetLocalPosition(Math::Vector2D(1550.f, 1020.f));
		// RainFloorParticle1_4_2->GetTransform()->SetLocalPosition(Math::Vector2D(3220.f, 1020.f));

		GameObject* RainFloorParticle1_4_1 = CreateGameObject(RainFloorParticle, L"RainFloorParticle1_4_1");
		RainFloorParticle1_4_1->GetTransform()->SetLocalPosition({73.f, 0.f});
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
		floorParticle1_4->SetParticleLineScope(Math::Vector2D(1550.f, 1020.f));
		floorParticle1_4->SetParticleLineScope(Math::Vector2D(3220.f, 1020.f));
		floorParticle1_4->Initialize(
			60, RainFloorParticle->GetTransform()->GetWorldPosition(),
			true, 3.f, 0.05f, 0.5f, 50, 2, BJEngine::GraphicsEngine::Color::White);
		floorParticle1_4->ParticleSystemSet();
	}
	
	GameObject* Rain = CreateGameObject(L"Rain");
	Rain->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	ParticleSystem* RainParticle = Rain->AddComponent<ParticleSystem>();
	RainParticle->SystemSetting(
		ParticleSystem::eParticleState::Dynamic,
		ParticleSystem::eScope::Line,
		ParticleSystem::eActivity::Regular,
		ParticleSystem::eEmissionMode::Spray,
		ParticleSystem::eRenderType::Line);
	RainParticle->SetGravity(500.f);
	RainParticle->SetDirectionRangeDegree(30.f);
	RainParticle->SetDirection(Math::Vector2D(0.f, -1.f));
	RainParticle->SetParticleLineScope(Math::Vector2D(-360.f, 0.f));
	RainParticle->SetParticleLineScope(Math::Vector2D(3220.f, 0.f));
	RainParticle->Initialize(
		500, Rain->GetTransform()->GetWorldPosition(),
		true, 1.f, 0.05f, 2.f, 50, 15, BJEngine::GraphicsEngine::Color::White);
	RainParticle->ParticleSystemSet();
	RainParticle->SetLayer(-1);
	
#pragma endregion

#pragma region UI
	// UI - 왼쪽 이동
	GameObject* tutorialUI_Left = CreateGameObject(L"tutorialUI_Left");
	tutorialUI_Left->GetTransform()->SetLocalPosition(Vector2D(-300.0f, 800.f));
	tutorialUI_Left->GetTransform()->SetLocalScale({2.f,2.f});
	SpriteRenderer* tutorialImage_Left = tutorialUI_Left->AddComponent<SpriteRenderer>();
	tutorialImage_Left->SetSpriteName(L"TutorialUI_Left");
	tutorialImage_Left->SetImageNumber(0);
	tutorialImage_Left->SetLayer(1);

	// UI - 오른쪽 이동
	GameObject* tutorialUI_Right = CreateGameObject(L"tutorialUI_Right");
	tutorialUI_Right->GetTransform()->SetLocalPosition(Vector2D(-200.0f, 800.f));
	tutorialUI_Right->GetTransform()->SetLocalScale({2.f,2.f});
	SpriteRenderer* tutorialImage_Right = tutorialUI_Right->AddComponent<SpriteRenderer>();
	tutorialImage_Right->SetSpriteName(L"TutorialUI_Right");
	tutorialImage_Right->SetImageNumber(0);
	tutorialImage_Right->SetLayer(1);

	// UI - 점프
	GameObject* tutorialUI_C = CreateGameObject(L"tutorialUI_C");
	tutorialUI_C->GetTransform()->SetLocalPosition(Vector2D(260.f, 800.f));
	tutorialUI_C->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_C = tutorialUI_C->AddComponent<SpriteRenderer>();
	tutorialImage_C->SetSpriteName(L"TutorialUI_C");
	tutorialImage_C->SetImageNumber(0);
	tutorialImage_C->SetLayer(1);

	// UI - 대쉬
	GameObject* tutorialUI_X = CreateGameObject(L"tutorialUI_X");
	tutorialUI_X->GetTransform()->SetLocalPosition(Vector2D(1050.f, 700.f));
	tutorialUI_X->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_X = tutorialUI_X->AddComponent<SpriteRenderer>();
	tutorialImage_X->SetSpriteName(L"TutorialUI_X");
	tutorialImage_X->SetImageNumber(0);
	tutorialImage_X->SetLayer(1);

	// UI - 공격
	GameObject* tutorialUI_Z = CreateGameObject(L"tutorialUI_Z");
	tutorialUI_Z->GetTransform()->SetLocalPosition(Vector2D(2500.f, 800.f));
	tutorialUI_Z->GetTransform()->SetLocalScale({ 2.f,2.f });
	SpriteRenderer* tutorialImage_Z = tutorialUI_Z->AddComponent<SpriteRenderer>();
	tutorialImage_Z->SetSpriteName(L"TutorialUI_Z");
	tutorialImage_Z->SetImageNumber(0);
	tutorialImage_Z->SetLayer(1);
#pragma endregion

	SoundManager::GetInstance().StopAllBGM();
	SoundManager::GetInstance().Play(SOUND_BGM_STAGE1);

}

