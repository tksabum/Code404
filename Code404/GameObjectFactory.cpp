#include <Windows.h>
#include "GameObjectFactory.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ConvexpolygonCollider.h"
#include "Rigidbody2D.h"
#include "Camera.h"
#include "../Code404/ItemMove.h"
#include "Scene.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "UI.h"
#include "Button.h"
#include "Image.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector2D.h"

#include "SoundManager.h"
#include "BGMSoundController.h"
#include "UISoundController.h"

using namespace BJEngine::Math;

#include "Player.h"
#include "JumpTrigger.h"
#include "Tracer.h"
#include "AttackTrigger.h"

#include "SkillCoolTimeViewer.h"
#include "PlayerHpBar.h"
#include "PlayerMpBar.h"

#pragma region 몬스터 헤더
#include "RangeDetect.h"
#include "GroundDetect.h"
#include "LineCollider.h"
#include "Enemy.h"
#include "AnimTest.h"
#include "AI.h"
#include "Idle_State.h"
#include "Patrol_State.h"
#include "Trace_State.h"
#include "Die_State.h"
#include "NormalAttack_State.h"
#include "Hit_State.h"
#include "AttackCollider.h"
#include "WallEnemyDetect.h"
#pragma endregion
#include "UISoundController.h"
#include "ObjectPoolSystem.h"
#include "EffectPlayer.h"
#include "Bullet.h"
#include "CircleCollider.h"
#include "BossNormalBullet.h"
#include "Text.h"
#include "DebugInfo.h"
#include "PlayerStealthCounter.h"
#include "ParryingSystem.h"

#include "InGameSettingUIController.h"
#include "GamePause.h"
#include "GameDataManager.h"

using namespace BJEngine;
using namespace BJEngine::Math;
using namespace Code404;



GameObjectFactory& GameObjectFactory::GetInstance()
{
	static GameObjectFactory inst;
	return inst;
}

void GameObjectFactory::CreatePrefab()
{
	ObjectPoolSystem::GetInstance().Clear();

	// 이펙트
	ObjectPoolSystem::GetInstance().SetPrefab(
		L"Effect",
		[](GameObject* go)
		{
			go->AddComponent<SpriteRenderer>();
			go->AddComponent<Animator>();
			go->AddComponent<EffectPlayer>();
		},
		[](GameObject* go)
		{
			go->GetComponent<SpriteRenderer>()->SetLayer(Code404::Layer::Effect);
			go->GetTransform()->SetLocalScale(Vector3D(1.0f, 1.0f, 1.0f));
		}
		);

	// 적 총알
	ObjectPoolSystem::GetInstance().SetPrefab(
		L"EnemyA_Bullet",
		[](GameObject* go)
		{
			go->AddComponent<ConvexpolygonCollider>();
			go->AddComponent<Rigidbody2D>();
			go->AddComponent<Bullet>();
			go->AddComponent<SpriteRenderer>();
		},
		[](GameObject* go)
		{
			go->SetLayerName(L"Bullet");

			ConvexpolygonCollider* collider = go->GetComponent<ConvexpolygonCollider>();
			collider->SetPolygon({ Vector2D(-30.0f, 5.0f), Vector2D(-30.0f, -5.0f), Vector2D(30.0f, -5.0f), Vector2D(30.0f, 5.0f) });
			collider->SetTrigger(true);
			Rigidbody2D* rigid = go->GetComponent<Rigidbody2D>();
			rigid->SetAffectedByGravity(false);
			rigid->SetPositionXLock(true);
			rigid->SetPositionYLock(true);
			rigid->SetRotationLock(true);
			rigid->SetVelocity(Vector2D(0, 0));

			SpriteRenderer* bulletRenderer = go->GetComponent<SpriteRenderer>();
			bulletRenderer->SetSpriteName(L"RangeA_Bullet");
			bulletRenderer->SetLayer(Code404::Layer::EnemyBullet);
			bulletRenderer->SetImageNumber(0);

			Bullet* bullet = go->GetComponent<Bullet>();
			bullet->SetDamage(Code404::RANGE_A_DAMAGE);
			bullet->SetSpeed(Code404::RANGE_A_BULLET_SPEED);
		}
		);

	ObjectPoolSystem::GetInstance().SetPrefab(
		L"EnemyB_Bullet",
		[](GameObject* go)
		{
			go->AddComponent<ConvexpolygonCollider>();
			go->AddComponent<Rigidbody2D>();
			go->AddComponent<Bullet>();
			go->AddComponent<SpriteRenderer>();
		},
		[](GameObject* go)
		{
			go->SetLayerName(L"Bullet");

			ConvexpolygonCollider* collider = go->GetComponent<ConvexpolygonCollider>();
			collider->SetPolygon({ Vector2D(-30.0f, 5.0f), Vector2D(-30.0f, -5.0f), Vector2D(30.0f, -5.0f), Vector2D(30.0f, 5.0f) });
			collider->SetTrigger(true);
			Rigidbody2D* rigid = go->GetComponent<Rigidbody2D>();
			rigid->SetAffectedByGravity(false);
			rigid->SetPositionXLock(true);
			rigid->SetPositionYLock(true);
			rigid->SetRotationLock(true);
			rigid->SetVelocity(Vector2D(0, 0));

			SpriteRenderer* bulletRenderer = go->GetComponent<SpriteRenderer>();
			bulletRenderer->SetSpriteName(L"RangeB_Bullet");
			bulletRenderer->SetLayer(Code404::Layer::EnemyBullet);
			bulletRenderer->SetImageNumber(0);

			Bullet* bullet = go->GetComponent<Bullet>();
			bullet->SetDamage(Code404::RANGE_B_DAMAGE);
			bullet->SetSpeed(Code404::RANGE_B_BULLET_SPEED);
		}
		);

	// 보스 총알
	ObjectPoolSystem::GetInstance().SetPrefab(
		L"BossNormalBullet",
		[](GameObject* go)
		{
			go->AddComponent<CircleCollider>();
			go->AddComponent<Rigidbody2D>();
			go->AddComponent<BossNormalBullet>();
			go->AddComponent<SpriteRenderer>();
		},
		[](GameObject* go)
		{
			go->SetLayerName(L"Trigger");
			go->GetComponent<CircleCollider>()->SetCircle(25);
			go->GetComponent<CircleCollider>()->SetTrigger(true);
			Rigidbody2D* rigid = go->GetComponent<Rigidbody2D>();
			rigid->SetAffectedByGravity(false);
			rigid->SetPositionXLock(true);
			rigid->SetPositionYLock(true);
			rigid->SetRotationLock(true);
			rigid->SetVelocity(Vector2D(0, 0));
			SpriteRenderer* bulletRenderer = go->GetComponent<SpriteRenderer>();
			bulletRenderer->SetSpriteName(L"Boss_Normal_Bullet");
			bulletRenderer->SetLayer(93);
			bulletRenderer->SetImageNumber(0);
			go->GetComponent<BossNormalBullet>()->SetBulletDamage(Code404::BOSS_NORMAL_BULLET_DAMAGE);
		}
		);

	ObjectPoolSystem::GetInstance().SetPrefab(
		L"BossTraceBullet",
		[](GameObject* go)
		{
			go->AddComponent<CircleCollider>();
			go->AddComponent<Rigidbody2D>();
			go->AddComponent<BossNormalBullet>();
			go->AddComponent<SpriteRenderer>();
		},
		[](GameObject* go)
		{
			go->SetLayerName(L"Trigger");
			go->GetComponent<CircleCollider>()->SetCircle(250);
			go->GetComponent<CircleCollider>()->SetTrigger(true);
			Rigidbody2D* rigid = go->GetComponent<Rigidbody2D>();
			rigid->SetAffectedByGravity(false);
			rigid->SetPositionXLock(true);
			rigid->SetPositionYLock(true);
			rigid->SetRotationLock(true);
			rigid->SetVelocity(Vector2D(0, 0));
			SpriteRenderer* bulletRenderer = go->GetComponent<SpriteRenderer>();
			bulletRenderer->SetSpriteName(L"Boss_Trace_Bullet");
			bulletRenderer->SetLayer(93);
			bulletRenderer->SetImageNumber(0);
			go->GetTransform()->SetLocalScale(Vector3D(0.25f, 0.25f, 0.25f));
			go->GetComponent<BossNormalBullet>()->SetBulletDamage(Code404::BOSS_TRACE_BULLET_DAMAGE);
		}
		);
}

GameObject* GameObjectFactory::CreatePlayer(float playerscale, bool usetracer, int layer, Vector2D sceneLT, Vector2D sceneRB, bool usescenetrace)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	//// 게임오버 판넬
	GameObject* gameoverpanel = currentScene->CreateGameObject(L"gameoverpanel");
	gameoverpanel->SetActive(false);
	gameoverpanel->GetTransform()->SetLocalPosition(Vector2D(460, 40));
	Image* gameoverpanelimage = gameoverpanel->AddComponent<Image>();
	gameoverpanelimage->SetSpriteName(L"UI_PopUp_Window");
	gameoverpanelimage->SetImageNumber(0);
	gameoverpanelimage->SetLayer(Code404::Layer::UI2);
	
	GameObject* failed404text = currentScene->CreateGameObject(gameoverpanel,L"failed404text");
	failed404text->GetTransform()->SetLocalPosition(Vector2D(90, 180));
	Image* failed404textimage = failed404text->AddComponent<Image>();
	failed404textimage->SetSpriteName(L"UI_Failed_Text");
	failed404textimage->SetImageNumber(0);
	failed404textimage->SetLayer(Code404::Layer::UI3);

	GameObject* savebuttonobject = currentScene->CreateGameObject(gameoverpanel, L"savebuttonobject");
	savebuttonobject->GetTransform()->SetLocalPosition(Vector2D(250, 560));
	Image* savebuttonobjectimage = savebuttonobject->AddComponent<Image>();
	savebuttonobjectimage->SetSpriteName(L"UI_Save_Button_Idle");
	savebuttonobjectimage->SetImageNumber(0);
	savebuttonobjectimage->SetLayer(Code404::Layer::UI3);
	savebuttonobject->AddComponent<UI>()->SetSize(Vector2D(480, 120));
	Button* savebutton = savebuttonobject->AddComponent<Button>();
	savebutton->SetImage(L"UI_Save_Button_Idle", L"UI_Save_Button_Enter", L"UI_Save_Button_Click");
	savebutton->SetSound(SOUND_EFFECT_CLICK);

	GameObject* quitbuttonobject = currentScene->CreateGameObject(gameoverpanel, L"quitbuttonobject");
	quitbuttonobject->GetTransform()->SetLocalPosition(Vector2D(250, 680));
	Image* quitbuttonobjectimage = quitbuttonobject->AddComponent<Image>();
	quitbuttonobjectimage->SetSpriteName(L"UI_Quit_Button_Idle");
	quitbuttonobjectimage->SetImageNumber(0);
	quitbuttonobjectimage->SetLayer(Code404::Layer::UI3);
	quitbuttonobject->AddComponent<UI>()->SetSize(Vector2D(480, 120));
	Button* quitbutton = quitbuttonobject->AddComponent<Button>();
	quitbutton->SetImage(L"UI_Quit_Button_Idle", L"UI_Quit_Button_Enter", L"UI_Quit_Button_Click");
	quitbutton->SetSound(SOUND_EFFECT_CLICK);

	// 게임 일시정지를 위한 컴포넌트
	gameoverpanel->AddComponent<GamePause>();


	savebutton->AddButtonEvent([=]()
		{
			GameDataManager::GetInstance().Clear();

			wstring currentSceneName = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetName();

			if (currentSceneName == Code404::SceneName::stage1_1)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_1");
			}
			else if (currentSceneName == Code404::SceneName::stage1_2)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_2");
			}
			else if (currentSceneName == Code404::SceneName::stage1_3)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_3");
			}
			else if (currentSceneName == Code404::SceneName::stage1_4)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_4");
			}
			else if (currentSceneName == Code404::SceneName::stage1_5)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_5");
			}
			else if (currentSceneName == Code404::SceneName::stage2_1)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_1");
			}
			else if (currentSceneName == Code404::SceneName::stage2_2)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_1");
			}
			else if (currentSceneName == Code404::SceneName::stage2_3)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_1");
			}
			else if (currentSceneName == Code404::SceneName::stage2_4)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_1");
			}
			else if (currentSceneName == Code404::SceneName::stage2_5)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_1");
			}
			else if (currentSceneName == Code404::SceneName::stage2_6)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_1");
			}
			else if (currentSceneName == Code404::SceneName::stage2_7)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_7");
			}
			else if (currentSceneName == Code404::SceneName::stage2_8)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_7");
			}
			else if (currentSceneName == Code404::SceneName::stage2_9)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_9");
			}
			else if (currentSceneName == Code404::SceneName::BossStage)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_9");
			}
		});

	quitbutton->AddButtonEvent([=]()
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"MainScene");
		});


	// 플레이어 오브젝트
	GameObject* playerobject = currentScene->CreateGameObject(L"PlayerObject");
	playerobject->GetTransform()->SetLocalScale(Vector3D(playerscale, playerscale, 1.0f));
	ConvexpolygonCollider* playercollider = playerobject->AddComponent<ConvexpolygonCollider>();
	playercollider->SetPolygon({ Vector2D(-60.0f, 100.0f), Vector2D(-60.0f, -90.0f), Vector2D(60.0f, -90.0f), Vector2D(60.0f, 100.0f) });
	Rigidbody2D* playerrigidbody = playerobject->AddComponent<Rigidbody2D>();
	playerrigidbody->SetRotationLock(true);
	playerrigidbody->SetRestitutionValue(0.0f);
	playerrigidbody->SetGravity(Vector2D(0.0f, 3000.0f));
	Player* player = playerobject->AddComponent<Player>();
	player->SetGameOverPanel(gameoverpanel);
	SpriteRenderer* playersprite = playerobject->AddComponent<SpriteRenderer>();
	playersprite->SetSpriteName(L"Player_Stand");
	playersprite->SetImageNumber(0);
	playersprite->SetLayer(layer);
	playerobject->AddComponent<Animator>();

	// 점프 트리거 오브젝트
	GameObject* jumpTriggerBox = currentScene->CreateGameObject(playerobject, L"JumpTriggerBox");
	jumpTriggerBox->SetLayerName(L"Trigger");
	jumpTriggerBox->GetTransform()->SetLocalPosition(Vector2D(0.0f, 110.0f * playerscale));
	jumpTriggerBox->AddComponent<JumpTrigger>();
	ConvexpolygonCollider* jumpcollider = jumpTriggerBox->AddComponent<ConvexpolygonCollider>();
	jumpcollider->SetPolygon({ Vector2D(-55.0f, 5.0f), Vector2D(-55.0f, -5.0f), Vector2D(55.0f, -5.0f), Vector2D(55.0f, 5.0f) });

	// 튜토리얼 키
	GameObject* tutorialkeyobject = currentScene->CreateGameObject(playerobject, L"TutorialKey");
	tutorialkeyobject->SetLayerName(L"Trigger");
	tutorialkeyobject->AddComponent<SpriteRenderer>();
	ConvexpolygonCollider* tutorialkeycollider = tutorialkeyobject->AddComponent<ConvexpolygonCollider>();
	tutorialkeycollider->SetPolygon({ Vector2D(-60.0f, 100.0f), Vector2D(-60.0f, -90.0f), Vector2D(60.0f, -90.0f), Vector2D(60.0f, 100.0f) });


	// UI
	const float skillopacity = 1.0f;

	// 방독면
	GameObject* playerGasMask = currentScene->CreateGameObject(L"PlayerGasMask");
	playerGasMask->GetTransform()->SetLocalPosition(Vector2D(60, 918));
	Image* playerGasMaskImage = playerGasMask->AddComponent<Image>();
	playerGasMaskImage->SetSpriteName(L"GasMask");
	playerGasMaskImage->SetImageNumber(0);
	playerGasMaskImage->SetLayer(Code404::Layer::UIDefault);

	// 플레이어 HP바
	GameObject* playerHPobject = currentScene->CreateGameObject(L"playerHPobject");
	playerHPobject->GetTransform()->SetLocalPosition(Vector2D(196, 930));

	// 바
	GameObject* playerHPbarobject = currentScene->CreateGameObject(playerHPobject, L"HPbar");
	PlayerHpBar* playerHpBar = playerHPbarobject->AddComponent<PlayerHpBar>();
	playerHpBar->SetStatus(player->GetMaxHp(), player->GetCurHp());
	Image* playerHPimage = playerHPbarobject->AddComponent<Image>();
	playerHPimage->SetSpriteName(L"PlayerHP");
	playerHPimage->SetImageNumber(0);
	playerHPimage->SetLayer(Code404::Layer::UILow);

	// 플레이어 HP바 테두리
	GameObject* playerHPBarBackGround = currentScene->CreateGameObject(playerHPobject, L"playerHPBarBackGround");
	Image* playerHPBarBackGroundImage = playerHPBarBackGround->AddComponent<Image>();
	playerHPBarBackGroundImage->SetSpriteName(L"PlayerHP_Outline");
	playerHPBarBackGroundImage->SetImageNumber(0);
	playerHPBarBackGroundImage->SetLayer(Code404::Layer::UIHigh);

	// 플레이어 MP바
	GameObject* playerMPobject = currentScene->CreateGameObject(L"playerMPobject");
	playerMPobject->GetTransform()->SetLocalPosition(Vector2D(196, 965));

	// 바
	GameObject* playerMPbarobject = currentScene->CreateGameObject(playerMPobject, L"BPbar");
	PlayerMpBar* playerMpBar = playerMPbarobject->AddComponent<PlayerMpBar>();
	playerMpBar->SetStatus(player->GetMaxMp(), player->GetCurMp());
	playerMPbarobject->GetTransform()->SetLocalScale(Vector2D(0.8, 1));
	Image* playerMPimage1 = playerMPbarobject->AddComponent<Image>();
	playerMPimage1->SetSpriteName(L"PlayerMP");
	playerMPimage1->SetImageNumber(0);
	playerMPimage1->SetLayer(Code404::Layer::UILow);

	// 플레이어 MP바 테두리
	GameObject* playerMPBarBackGround = currentScene->CreateGameObject(playerMPobject, L"playerMPBarBackGround");
	Image* playerMPBarBackGroundImage = playerMPBarBackGround->AddComponent<Image>();
	playerMPBarBackGroundImage->SetSpriteName(L"PlayerMP_Outline");
	playerMPBarBackGroundImage->SetImageNumber(0);
	playerMPBarBackGroundImage->SetLayer(Code404::Layer::UIHigh);

	// A키
	GameObject* player_knifeA = currentScene->CreateGameObject(L"player_knifeA");
	player_knifeA->GetTransform()->SetLocalPosition(Vector2D(1500, 918));
	Image* player_knifeAImage = player_knifeA->AddComponent<Image>();
	player_knifeAImage->SetSpriteName(L"Player_A");
	player_knifeAImage->SetImageNumber(0);
	player_knifeAImage->SetLayer(Code404::Layer::UILow);

	GameObject* player_knifeA_cover = currentScene->CreateGameObject(L"player_knifeAcover");
	player_knifeA_cover->AddComponent<SkillCoolTimeViewer>()->SetSkill(SkillCoolTimeViewer::SkillName::DaggerA);
	player_knifeA_cover->GetTransform()->SetLocalScale(Vector3D(1.0f, -1.0f, 1.0f));
	player_knifeA_cover->GetTransform()->SetLocalPosition(Vector2D(1517, 1008));
	Image* player_knifeA_coverImage = player_knifeA_cover->AddComponent<Image>();
	player_knifeA_coverImage->SetSpriteName(L"Knife_Cover");
	player_knifeA_coverImage->SetImageNumber(0);
	player_knifeA_coverImage->SetLayer(Code404::Layer::UIDefault);
	player_knifeA_coverImage->SetOpacity(skillopacity);

	GameObject* player_knifeAkey = currentScene->CreateGameObject(L"player_knifeAkey");
	player_knifeAkey->GetTransform()->SetLocalPosition(Vector2D(1500, 918));
	Image* player_knifeAkeyImage = player_knifeAkey->AddComponent<Image>();
	player_knifeAkeyImage->SetSpriteName(L"Player_Akey");
	player_knifeAkeyImage->SetImageNumber(0);
	player_knifeAkeyImage->SetLayer(Code404::Layer::UIHigh);

	// S키
	GameObject* player_knifeS = currentScene->CreateGameObject(L"player_knifeS");
	player_knifeS->GetTransform()->SetLocalPosition(Vector2D(1650, 918));
	Image* player_knifeSImage = player_knifeS->AddComponent<Image>();
	player_knifeSImage->SetSpriteName(L"Player_S");
	player_knifeSImage->SetImageNumber(0);
	player_knifeSImage->SetLayer(Code404::Layer::UILow);

	GameObject* player_knifeS_cover = currentScene->CreateGameObject(L"player_knifeScover");
	player_knifeS_cover->AddComponent<SkillCoolTimeViewer>()->SetSkill(SkillCoolTimeViewer::SkillName::DaggerS);
	player_knifeS_cover->GetTransform()->SetLocalScale(Vector3D(1.0f, -1.0f, 1.0f));
	player_knifeS_cover->GetTransform()->SetLocalPosition(Vector2D(1667, 1008));
	Image* player_knifeS_coverImage = player_knifeS_cover->AddComponent<Image>();
	player_knifeS_coverImage->SetSpriteName(L"Knife_Cover");
	player_knifeS_coverImage->SetImageNumber(0);
	player_knifeS_coverImage->SetLayer(Code404::Layer::UIDefault);
	player_knifeS_coverImage->SetOpacity(skillopacity);

	GameObject* player_knifeSkey = currentScene->CreateGameObject(L"player_knifeSkey");
	player_knifeSkey->GetTransform()->SetLocalPosition(Vector2D(1650, 918));
	Image* player_knifeSkeyImage = player_knifeSkey->AddComponent<Image>();
	player_knifeSkeyImage->SetSpriteName(L"Player_Skey");
	player_knifeSkeyImage->SetImageNumber(0);
	player_knifeSkeyImage->SetLayer(Code404::Layer::UIHigh);

	// D키
	GameObject* player_knifeD = currentScene->CreateGameObject(L"player_knifeD");
	player_knifeD->GetTransform()->SetLocalPosition(Vector2D(1800, 918));
	Image* player_knifeDImage = player_knifeD->AddComponent<Image>();
	player_knifeDImage->SetSpriteName(L"Player_D");
	player_knifeDImage->SetImageNumber(0);
	player_knifeDImage->SetLayer(Code404::Layer::UILow);

	GameObject* player_knifeD_cover = currentScene->CreateGameObject(L"player_knifeDcover");
	player_knifeD_cover->AddComponent<SkillCoolTimeViewer>()->SetSkill(SkillCoolTimeViewer::SkillName::DaggerD);
	player_knifeD_cover->GetTransform()->SetLocalScale(Vector3D(1.0f, -1.0f, 1.0f));
	player_knifeD_cover->GetTransform()->SetLocalPosition(Vector2D(1817, 1008));
	Image* player_knifeD_coverImage = player_knifeD_cover->AddComponent<Image>();
	player_knifeD_coverImage->SetSpriteName(L"Knife_Cover");
	player_knifeD_coverImage->SetImageNumber(0);
	player_knifeD_coverImage->SetLayer(Code404::Layer::UIDefault);
	player_knifeD_coverImage->SetOpacity(skillopacity);

	GameObject* player_knifeDkey = currentScene->CreateGameObject(L"player_knifeDkey");
	player_knifeDkey->GetTransform()->SetLocalPosition(Vector2D(1800, 918));
	Image* player_knifeDkeyImage = player_knifeDkey->AddComponent<Image>();
	player_knifeDkeyImage->SetSpriteName(L"Player_Dkey");
	player_knifeDkeyImage->SetImageNumber(0);
	player_knifeDkeyImage->SetLayer(Code404::Layer::UIHigh);

	GameObject* stealthcounterobject = currentScene->CreateGameObject(L"StealthCounter");
	stealthcounterobject->GetTransform()->SetLocalPosition(Vector2D(195, 1000));
	Image* stealthimage = stealthcounterobject->AddComponent<Image>();
	stealthimage->SetLayer(Code404::Layer::UIDefault);
	PlayerStealthCounter* stealthcounter = stealthcounterobject->AddComponent<PlayerStealthCounter>();
	stealthcounter->SetPlayer(player);
	stealthcounter->SetStealthCounterImage(L"Player_Stealth0", L"Player_Stealth1", L"Player_Stealth2", L"Player_Stealth3", L"Player_Stealth4");


	if (usetracer)
	{
		// 카메라에 추적기 추가
		Tracer* tracer = currentScene->GetMainCamera()->GetGameObject()->AddComponent<Tracer>();
		tracer->SetTarget(playerobject->GetTransform(), Vector2D(600.0f, 200.0f), sceneLT, sceneRB, usescenetrace);
	}
	return playerobject;
}

GameObject* GameObjectFactory::CreatePlatform(Vector2D size)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	GameObject* platform = currentScene->CreateGameObject(L"Platform");
	platform->SetLayerName(L"Platform");
	ConvexpolygonCollider* platformcollider = platform->AddComponent<ConvexpolygonCollider>();
	platformcollider->SetPolygon({ Vector2D(-size.x, size.y) / 2.0f, Vector2D(-size.x, -size.y) / 2.0f, Vector2D(size.x, -size.y) / 2.0f, Vector2D(size.x, size.y) / 2.0f });
	Rigidbody2D* platformrigidbody = platform->AddComponent<Rigidbody2D>();
	platformrigidbody->SetPositionXLock(true);
	platformrigidbody->SetPositionYLock(true);
	platformrigidbody->SetRotationLock(true);
	platformrigidbody->SetRestitutionValue(0.0f);
	platformrigidbody->SetAffectedByGravity(false);

	return platform;
}

BJEngine::GameObject* GameObjectFactory::CreateEnemyMeleeA(GameObject* target)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	vector<Vector2D> enemyColliderVertex = { Vector2D(-50,110),Vector2D(-50,-125), Vector2D(50,-125), Vector2D(50,110) };
	vector<Vector2D> recognizeVertex = { Vector2D(-300,100),Vector2D(-300,-100), Vector2D(300,-100), Vector2D(300,100) };
	vector<Vector2D> traceVertex = { Vector2D(-200,75),Vector2D(-200,-75), Vector2D(200,-75), Vector2D(200,75) };
	vector<Vector2D> attackVertex = { Vector2D(-100,70),Vector2D(-100,-70), Vector2D(100,-70), Vector2D(100,70) };
	vector<Vector2D> attackColliderVertex = { Vector2D(-120,120),Vector2D(-120,-120), Vector2D(120,-120), Vector2D(120,120) };

	GameObject* enemyParent = currentScene->CreateGameObject(L"enemyParent");

	GameObject* enemyHPBarOutLine = currentScene->CreateGameObject(enemyParent, L"enemyHPBarOutLine");
	enemyHPBarOutLine->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5.f, 2.5f, 1.f));
	enemyHPBarOutLine->SetActive(false);
	Image* HPBarOutLineImage = enemyHPBarOutLine->AddComponent<Image>();
	HPBarOutLineImage->SetSpriteName(L"Mob_HPBar_OutLine");
	HPBarOutLineImage->SetLayer(Code404::Layer::UILow);
	HPBarOutLineImage->SetImageNumber(0);

	GameObject* enemyHPBar = currentScene->CreateGameObject(enemyHPBarOutLine, L"enemyHPBar");
	Image* HPBarImage = enemyHPBar->AddComponent<Image>();
	HPBarImage->SetSpriteName(L"Mob_HPBar");
	HPBarImage->SetLayer(Code404::Layer::UILow);
	HPBarImage->SetImageNumber(0);

	// enemy 셋팅
	GameObject* enemy = currentScene->CreateGameObject(enemyParent, L"enemy");
	enemy->SetLayerName(L"Enemy");
	Enemy* enemyLogic = enemy->AddComponent<Enemy>();
	enemyLogic->SetHPBar(enemyHPBarOutLine);
	enemyLogic->SetMaxHP(MELEE_A_HP);
	enemy->AddComponent<AnimTest>();
	enemy->AddComponent<SpriteRenderer>();
	Animator* enemyAnim = enemy->AddComponent<Animator>();
	enemyAnim->Play(L"MeleeA_Enemy_Stand");
	Rigidbody2D* enemyRigid = enemy->AddComponent<Rigidbody2D>();
	enemyRigid->SetRotationLock(true);
	enemyRigid->SetGravity(Vector2D(0, 500));
	enemyRigid->SetRestitutionValue(0.0f);
	AI* ai = enemy->AddComponent<AI>();
	ai->SetEnemyType(ENEMY_TYPE::MeleeA_Enemy);
	ai->CreateState(AI_STATE::IDLE, new Idle_State(ai, 2.f));
	ai->CreateState(AI_STATE::PATROL, new Patrol_State(ai, target, 6.f, MELEE_A_PATROL_SPEED));
	ai->CreateState(AI_STATE::TRACETARGET, new Trace_State(ai, target, MELEE_A_TRACE_SPEED));
	ai->CreateState(AI_STATE::DIE, new Die_State(ai, 4.f));
	ai->CreateState(AI_STATE::NORMALATTACK, new NormalAttack_State(ai, target, MELEE_A_ATTACK_DELAY));
	ai->CreateState(AI_STATE::HIT, new Hit_State(ai));
	ConvexpolygonCollider* enemyCollider = enemy->AddComponent<ConvexpolygonCollider>();
	enemyCollider->SetPolygon(enemyColliderVertex);

	// enemy공격 Collider설정
	GameObject* enemyAttackColliderObject = currentScene->CreateGameObject(enemy, L"AttackColliderObject");
	enemyAttackColliderObject->SetLayerName(L"Trigger");
	enemyAttackColliderObject->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	AttackTrigger* attackColliderLogic = enemyAttackColliderObject->AddComponent<AttackTrigger>();
	attackColliderLogic->SetUser(AttackTrigger::User::ENEMY);
	attackColliderLogic->SetEnemyAttack(ENEMYATTACK_TYPE::MELEE_A, MELEE_A_DAMAGE);
	((NormalAttack_State*)ai->GetState(AI_STATE::NORMALATTACK))->SetAttackCollider(enemyAttackColliderObject);
	ConvexpolygonCollider* enemyAttackCollider = enemyAttackColliderObject->AddComponent<ConvexpolygonCollider>();
	enemyAttackCollider->SetPolygon(attackColliderVertex);
	enemyAttackColliderObject->SetActive(false);

	// enemy recognizeRange 셋팅
	GameObject* enemyRecognizeRange = currentScene->CreateGameObject(enemy, L"recognize");
	enemyRecognizeRange->SetLayerName(L"Trigger");
	enemyRecognizeRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::RECOGNIZE);
	enemyRecognizeRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* recognizeCollider = enemyRecognizeRange->AddComponent<ConvexpolygonCollider>();
	recognizeCollider->SetPolygon(recognizeVertex);
	// enemy traceRange 셋팅
	GameObject* enemytraceRange = currentScene->CreateGameObject(enemy, L"trace");
	enemytraceRange->SetLayerName(L"Trigger");
	enemytraceRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::TRACETARGET);
	enemytraceRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* traceCollider = enemytraceRange->AddComponent<ConvexpolygonCollider>();
	traceCollider->SetPolygon(traceVertex);
	// enemyAttack 셋팅
	GameObject* enemyAttRange = currentScene->CreateGameObject(enemy, L"attack");
	enemyAttRange->SetLayerName(L"Trigger");
	enemyAttRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::ATTACK);
	enemyAttRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* attCollider = enemyAttRange->AddComponent<ConvexpolygonCollider>();
	attCollider->SetPolygon(attackVertex);

	// enemy groundDetect 셋팅
	GameObject* enemyGroundDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyGroundDetect->SetLayerName(L"Trigger");
	enemyGroundDetect->AddComponent<GroundDetect>();
	BJEngine::Math::Vector3D detectPos = enemyGroundDetect->GetTransform()->GetLocalPosition();
	LineCollider* groundDetectCollider = enemyGroundDetect->AddComponent<LineCollider>();
	groundDetectCollider->SetLine(Vector2D(detectPos.x - 75, detectPos.y), Vector2D(detectPos.x - 75, detectPos.y + 115));

	// 벽과 적을 감지함
	GameObject* enemyWallEnemyDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyWallEnemyDetect->SetLayerName(L"Trigger");
	enemyWallEnemyDetect->AddComponent<WallEnemyDetect>();
	BJEngine::Math::Vector3D wallEnemyDetectPos = enemyWallEnemyDetect->GetTransform()->GetLocalPosition();
	LineCollider* wallEnemyDetectCollider = enemyWallEnemyDetect->AddComponent<LineCollider>();
	wallEnemyDetectCollider->SetLine(Vector2D(wallEnemyDetectPos.x, wallEnemyDetectPos.y), Vector2D(wallEnemyDetectPos.x - 150, wallEnemyDetectPos.y));

	return enemy;
}

BJEngine::GameObject* GameObjectFactory::CreateEnemyMeleeB(GameObject* target)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	vector<Vector2D> enemyColliderVertex = { Vector2D(-50,110),Vector2D(-50,-125), Vector2D(50,-125), Vector2D(50,110) };
	vector<Vector2D> recognizeVertex = { Vector2D(-300,100),Vector2D(-300,-100), Vector2D(300,-100), Vector2D(300,100) };
	vector<Vector2D> traceVertex = { Vector2D(-200,75),Vector2D(-200,-75), Vector2D(200,-75), Vector2D(200,75) };
	vector<Vector2D> attackVertex = { Vector2D(-100,70),Vector2D(-100,-70), Vector2D(100,-70), Vector2D(100,70) };
	vector<Vector2D> attackColliderVertex = { Vector2D(-120,120),Vector2D(-120,-120), Vector2D(120,-120), Vector2D(120,120) };

	GameObject* enemyParent = currentScene->CreateGameObject(L"enemyParent");

	GameObject* enemyHPBarOutLine = currentScene->CreateGameObject(enemyParent, L"enemyHPBarOutLine");
	enemyHPBarOutLine->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5.f, 2.5f, 1.f));
	enemyHPBarOutLine->SetActive(false);
	Image* HPBarOutLineImage = enemyHPBarOutLine->AddComponent<Image>();
	HPBarOutLineImage->SetSpriteName(L"Mob_HPBar_OutLine");
	HPBarOutLineImage->SetLayer(Code404::Layer::UILow);
	HPBarOutLineImage->SetImageNumber(0);

	GameObject* enemyHPBar = currentScene->CreateGameObject(enemyHPBarOutLine, L"enemyHPBar");
	Image* HPBarImage = enemyHPBar->AddComponent<Image>();
	HPBarImage->SetSpriteName(L"Mob_HPBar");
	HPBarImage->SetLayer(Code404::Layer::UILow);
	HPBarImage->SetImageNumber(0);

	// enemy 셋팅
	GameObject* enemy = currentScene->CreateGameObject(enemyParent, L"enemy");
	enemy->SetLayerName(L"Enemy");
	Enemy* enemyLogic = enemy->AddComponent<Enemy>();
	enemyLogic->SetMaxHP(MELEE_B_HP);
	enemyLogic->SetHPBar(enemyHPBarOutLine);
	enemy->AddComponent<AnimTest>();
	enemy->AddComponent<SpriteRenderer>();
	Animator* enemyAnim = enemy->AddComponent<Animator>();
	enemyAnim->Play(L"MeleeB_Enemy_Stand");
	Rigidbody2D* enemyRigid = enemy->AddComponent<Rigidbody2D>();
	enemyRigid->SetRotationLock(true);
	enemyRigid->SetGravity(Vector2D(0, 500));
	enemyRigid->SetRestitutionValue(0.0f);
	AI* ai = enemy->AddComponent<AI>();
	ai->SetEnemyType(ENEMY_TYPE::MeleeB_Enemy);
	ai->CreateState(AI_STATE::IDLE, new Idle_State(ai, 2.f));
	ai->CreateState(AI_STATE::PATROL, new Patrol_State(ai, target, 6.f, MELEE_B_PATROL_SPEED));
	ai->CreateState(AI_STATE::TRACETARGET, new Trace_State(ai, target, MELEE_B_TRACE_SPEED));
	ai->CreateState(AI_STATE::DIE, new Die_State(ai, 4.f));
	ai->CreateState(AI_STATE::NORMALATTACK, new NormalAttack_State(ai, target, MELEE_B_ATTACK_DELAY));
	ai->CreateState(AI_STATE::HIT, new Hit_State(ai));
	ConvexpolygonCollider* enemyCollider = enemy->AddComponent<ConvexpolygonCollider>();
	enemyCollider->SetPolygon(enemyColliderVertex);

	// enemy공격 Collider설정
	GameObject* enemyAttackColliderObject = currentScene->CreateGameObject(enemy, L"AttackColliderObject");
	enemyAttackColliderObject->SetLayerName(L"Trigger");
	enemyAttackColliderObject->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	AttackTrigger* attackColliderLogic = enemyAttackColliderObject->AddComponent<AttackTrigger>();
	attackColliderLogic->SetUser(AttackTrigger::User::ENEMY);
	attackColliderLogic->SetEnemyAttack(ENEMYATTACK_TYPE::MELEE_B, MELEE_B_DAMAGE);
	((NormalAttack_State*)ai->GetState(AI_STATE::NORMALATTACK))->SetAttackCollider(enemyAttackColliderObject);
	ConvexpolygonCollider* enemyAttackCollider = enemyAttackColliderObject->AddComponent<ConvexpolygonCollider>();
	enemyAttackCollider->SetPolygon(attackColliderVertex);
	enemyAttackColliderObject->SetActive(false);

	// enemy recognizeRange 셋팅
	GameObject* enemyRecognizeRange = currentScene->CreateGameObject(enemy, L"recognize");
	enemyRecognizeRange->SetLayerName(L"Trigger");
	enemyRecognizeRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::RECOGNIZE);
	enemyRecognizeRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* recognizeCollider = enemyRecognizeRange->AddComponent<ConvexpolygonCollider>();
	recognizeCollider->SetPolygon(recognizeVertex);
	// enemy traceRange 셋팅
	GameObject* enemytraceRange = currentScene->CreateGameObject(enemy, L"trace");
	enemytraceRange->SetLayerName(L"Trigger");
	enemytraceRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::TRACETARGET);
	enemytraceRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* traceCollider = enemytraceRange->AddComponent<ConvexpolygonCollider>();
	traceCollider->SetPolygon(traceVertex);
	// enemyAttack 셋팅
	GameObject* enemyAttRange = currentScene->CreateGameObject(enemy, L"attack");
	enemyAttRange->SetLayerName(L"Trigger");
	enemyAttRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::ATTACK);
	enemyAttRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* attCollider = enemyAttRange->AddComponent<ConvexpolygonCollider>();
	attCollider->SetPolygon(attackVertex);
	// enemy groundDetect 셋팅
	GameObject* enemyGroundDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyGroundDetect->SetLayerName(L"Trigger");
	enemyGroundDetect->AddComponent<GroundDetect>();
	BJEngine::Math::Vector3D detectPos = enemyGroundDetect->GetTransform()->GetLocalPosition();
	LineCollider* groundDetectCollider = enemyGroundDetect->AddComponent<LineCollider>();
	groundDetectCollider->SetLine(Vector2D(detectPos.x - 75, detectPos.y), Vector2D(detectPos.x - 75, detectPos.y + 115));

	// 벽과 적을 감지함
	GameObject* enemyWallEnemyDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyWallEnemyDetect->SetLayerName(L"Trigger");
	enemyWallEnemyDetect->AddComponent<WallEnemyDetect>();
	BJEngine::Math::Vector3D wallEnemyDetectPos = enemyWallEnemyDetect->GetTransform()->GetLocalPosition();
	LineCollider* wallEnemyDetectCollider = enemyWallEnemyDetect->AddComponent<LineCollider>();
	wallEnemyDetectCollider->SetLine(Vector2D(wallEnemyDetectPos.x, wallEnemyDetectPos.y), Vector2D(wallEnemyDetectPos.x - 150, wallEnemyDetectPos.y));

	return enemy;
}

BJEngine::GameObject* GameObjectFactory::CreateEnemyRangeA(GameObject* target)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	vector<Vector2D> enemyColliderVertex = { Vector2D(-50,110),Vector2D(-50,-125), Vector2D(50,-125), Vector2D(50,110) };
	vector<Vector2D> recognizeVertex = { Vector2D(-500,100),Vector2D(-500,-100), Vector2D(500,-100), Vector2D(500,100) };
	vector<Vector2D> traceVertex = { Vector2D(-400,75),Vector2D(-400,-75), Vector2D(300,-75), Vector2D(300,75) };
	vector<Vector2D> attackVertex = traceVertex;
	vector<Vector2D> attackColliderVertex = {};

	GameObject* enemyParent = currentScene->CreateGameObject(L"enemyParent");

	GameObject* enemyHPBarOutLine = currentScene->CreateGameObject(enemyParent, L"enemyHPBarOutLine");
	enemyHPBarOutLine->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5.f, 2.5f, 1.f));
	enemyHPBarOutLine->SetActive(false);
	Image* HPBarOutLineImage = enemyHPBarOutLine->AddComponent<Image>();
	HPBarOutLineImage->SetSpriteName(L"Mob_HPBar_OutLine");
	HPBarOutLineImage->SetLayer(Code404::Layer::UILow);
	HPBarOutLineImage->SetImageNumber(0);

	GameObject* enemyHPBar = currentScene->CreateGameObject(enemyHPBarOutLine, L"enemyHPBar");
	Image* HPBarImage = enemyHPBar->AddComponent<Image>();
	HPBarImage->SetSpriteName(L"Mob_HPBar");
	HPBarImage->SetLayer(Code404::Layer::UILow);
	HPBarImage->SetImageNumber(0);

	// enemy 셋팅
	GameObject* enemy = currentScene->CreateGameObject(L"enemy");
	enemy->SetLayerName(L"Enemy");
	Enemy* enemyLogic = enemy->AddComponent<Enemy>();
	enemyLogic->SetMaxHP(RANGE_A_HP);
	enemyLogic->SetHPBar(enemyHPBarOutLine);
	enemy->AddComponent<AnimTest>();
	enemy->AddComponent<SpriteRenderer>();
	Animator* enemyAnim = enemy->AddComponent<Animator>();
	enemyAnim->Play(L"RangeA_Enemy_Stand");
	Rigidbody2D* enemyRigid = enemy->AddComponent<Rigidbody2D>();
	enemyRigid->SetRotationLock(true);
	enemyRigid->SetGravity(Vector2D(0, 500));
	enemyRigid->SetRestitutionValue(0.0f);
	AI* ai = enemy->AddComponent<AI>();
	ai->SetEnemyType(ENEMY_TYPE::RangeA_Enemy);
	ai->CreateState(AI_STATE::IDLE, new Idle_State(ai, 2.f));
	ai->CreateState(AI_STATE::PATROL, new Patrol_State(ai, target, 6.f, RANGE_A_PATROL_SPEED));
	ai->CreateState(AI_STATE::TRACETARGET, new Trace_State(ai, target, RANGE_A_TRACE_SPEED));
	ai->CreateState(AI_STATE::DIE, new Die_State(ai, 4.f));
	ai->CreateState(AI_STATE::NORMALATTACK, new NormalAttack_State(ai, target, RANGE_A_ATTACK_DELAY));
	ai->CreateState(AI_STATE::HIT, new Hit_State(ai));
	ConvexpolygonCollider* enemyCollider = enemy->AddComponent<ConvexpolygonCollider>();
	enemyCollider->SetPolygon(enemyColliderVertex);

	// enemy recognizeRange 셋팅
	GameObject* enemyRecognizeRange = currentScene->CreateGameObject(enemy, L"recognize");
	enemyRecognizeRange->SetLayerName(L"Trigger");
	enemyRecognizeRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::RECOGNIZE);
	enemyRecognizeRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* recognizeCollider = enemyRecognizeRange->AddComponent<ConvexpolygonCollider>();
	recognizeCollider->SetPolygon(recognizeVertex);
	// enemy traceRange 셋팅
	GameObject* enemytraceRange = currentScene->CreateGameObject(enemy, L"trace");
	enemytraceRange->SetLayerName(L"Trigger");
	enemytraceRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::TRACETARGET);
	enemytraceRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* traceCollider = enemytraceRange->AddComponent<ConvexpolygonCollider>();
	traceCollider->SetPolygon(traceVertex);
	// enemyAttack 셋팅
	GameObject* enemyAttRange = currentScene->CreateGameObject(enemy, L"attack");
	enemyAttRange->SetLayerName(L"Trigger");
	enemyAttRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::ATTACK);
	enemyAttRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* attCollider = enemyAttRange->AddComponent<ConvexpolygonCollider>();
	attCollider->SetPolygon(attackVertex);
	// enemy groundDetect 셋팅
	GameObject* enemyGroundDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyGroundDetect->SetLayerName(L"Trigger");
	enemyGroundDetect->AddComponent<GroundDetect>();
	BJEngine::Math::Vector3D detectPos = enemyGroundDetect->GetTransform()->GetLocalPosition();
	LineCollider* groundDetectCollider = enemyGroundDetect->AddComponent<LineCollider>();
	groundDetectCollider->SetLine(Vector2D(detectPos.x - 75, detectPos.y), Vector2D(detectPos.x - 75, detectPos.y + 115));

	// 벽과 적을 감지함
	GameObject* enemyWallEnemyDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyWallEnemyDetect->SetLayerName(L"Trigger");
	enemyWallEnemyDetect->AddComponent<WallEnemyDetect>();
	BJEngine::Math::Vector3D wallEnemyDetectPos = enemyWallEnemyDetect->GetTransform()->GetLocalPosition();
	LineCollider* wallEnemyDetectCollider = enemyWallEnemyDetect->AddComponent<LineCollider>();
	wallEnemyDetectCollider->SetLine(Vector2D(wallEnemyDetectPos.x, wallEnemyDetectPos.y), Vector2D(wallEnemyDetectPos.x - 150, wallEnemyDetectPos.y));

	return enemy;
}

BJEngine::GameObject* GameObjectFactory::CreateEnemyRangeB(GameObject* target)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	vector<Vector2D> enemyColliderVertex = { Vector2D(-50,110),Vector2D(-50,-125), Vector2D(50,-125), Vector2D(50,110) };
	vector<Vector2D> recognizeVertex = { Vector2D(-500,100),Vector2D(-500,-100), Vector2D(500,-100), Vector2D(500,100) };
	vector<Vector2D> traceVertex = { Vector2D(-400,75),Vector2D(-400,-75), Vector2D(300,-75), Vector2D(300,75) };
	vector<Vector2D> attackVertex = traceVertex;
	vector<Vector2D> attackColliderVertex = {};

	GameObject* enemyParent = currentScene->CreateGameObject(L"enemyParent");

	GameObject* enemyHPBarOutLine = currentScene->CreateGameObject(enemyParent, L"enemyHPBarOutLine");
	enemyHPBarOutLine->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5.f, 2.5f, 1.f));
	enemyHPBarOutLine->SetActive(false);
	Image* HPBarOutLineImage = enemyHPBarOutLine->AddComponent<Image>();
	HPBarOutLineImage->SetSpriteName(L"Mob_HPBar_OutLine");
	HPBarOutLineImage->SetLayer(Code404::Layer::UILow);
	HPBarOutLineImage->SetImageNumber(0);

	GameObject* enemyHPBar = currentScene->CreateGameObject(enemyHPBarOutLine, L"enemyHPBar");
	Image* HPBarImage = enemyHPBar->AddComponent<Image>();
	HPBarImage->SetSpriteName(L"Mob_HPBar");
	HPBarImage->SetLayer(Code404::Layer::UILow);
	HPBarImage->SetImageNumber(0);

	// enemy 셋팅
	GameObject* enemy = currentScene->CreateGameObject(L"enemy");
	enemy->SetLayerName(L"Enemy");
	Enemy* enemyLogic = enemy->AddComponent<Enemy>();
	enemyLogic->SetMaxHP(RANGE_B_HP);
	enemyLogic->SetHPBar(enemyHPBarOutLine);
	enemy->AddComponent<AnimTest>();
	enemy->AddComponent<SpriteRenderer>();
	Animator* enemyAnim = enemy->AddComponent<Animator>();
	enemyAnim->Play(L"RangeB_Enemy_Stand");
	Rigidbody2D* enemyRigid = enemy->AddComponent<Rigidbody2D>();
	enemyRigid->SetRotationLock(true);
	enemyRigid->SetGravity(Vector2D(0, 500));
	enemyRigid->SetRestitutionValue(0.0f);
	AI* ai = enemy->AddComponent<AI>();
	ai->SetEnemyType(ENEMY_TYPE::RangeB_Enemy);
	ai->CreateState(AI_STATE::IDLE, new Idle_State(ai, 2.f));
	ai->CreateState(AI_STATE::PATROL, new Patrol_State(ai, target, 6.f, RANGE_B_PATROL_SPEED));
	ai->CreateState(AI_STATE::TRACETARGET, new Trace_State(ai, target, RANGE_B_TRACE_SPEED));
	ai->CreateState(AI_STATE::DIE, new Die_State(ai, 4.f));
	ai->CreateState(AI_STATE::NORMALATTACK, new NormalAttack_State(ai, target, RANGE_B_ATTACK_DELAY));
	ai->CreateState(AI_STATE::HIT, new Hit_State(ai));
	ConvexpolygonCollider* enemyCollider = enemy->AddComponent<ConvexpolygonCollider>();
	enemyCollider->SetPolygon(enemyColliderVertex);

	// enemy recognizeRange 셋팅
	GameObject* enemyRecognizeRange = currentScene->CreateGameObject(enemy, L"recognize");
	enemyRecognizeRange->SetLayerName(L"Trigger");
	enemyRecognizeRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::RECOGNIZE);
	enemyRecognizeRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* recognizeCollider = enemyRecognizeRange->AddComponent<ConvexpolygonCollider>();
	recognizeCollider->SetPolygon(recognizeVertex);
	// enemy traceRange 셋팅
	GameObject* enemytraceRange = currentScene->CreateGameObject(enemy, L"trace");
	enemytraceRange->SetLayerName(L"Trigger");
	enemytraceRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::TRACETARGET);
	enemytraceRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* traceCollider = enemytraceRange->AddComponent<ConvexpolygonCollider>();
	traceCollider->SetPolygon(traceVertex);
	// enemyAttack 셋팅
	GameObject* enemyAttRange = currentScene->CreateGameObject(enemy, L"attack");
	enemyAttRange->SetLayerName(L"Trigger");
	enemyAttRange->AddComponent<RangeDetect>()->SetRangeType(RANGE_TYPE::ATTACK);
	enemyAttRange->GetComponent<RangeDetect>()->SetAI(ai);
	ConvexpolygonCollider* attCollider = enemyAttRange->AddComponent<ConvexpolygonCollider>();
	attCollider->SetPolygon(attackVertex);
	// enemy groundDetect 셋팅
	GameObject* enemyGroundDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyGroundDetect->SetLayerName(L"Trigger");
	enemyGroundDetect->AddComponent<GroundDetect>();
	BJEngine::Math::Vector3D detectPos = enemyGroundDetect->GetTransform()->GetLocalPosition();
	LineCollider* groundDetectCollider = enemyGroundDetect->AddComponent<LineCollider>();
	groundDetectCollider->SetLine(Vector2D(detectPos.x - 75, detectPos.y), Vector2D(detectPos.x - 75, detectPos.y + 115));

	// 벽과 적을 감지함
	GameObject* enemyWallEnemyDetect = currentScene->CreateGameObject(enemy, L"detect");
	enemyWallEnemyDetect->SetLayerName(L"Trigger");
	enemyWallEnemyDetect->AddComponent<WallEnemyDetect>();
	BJEngine::Math::Vector3D wallEnemyDetectPos = enemyWallEnemyDetect->GetTransform()->GetLocalPosition();
	LineCollider* wallEnemyDetectCollider = enemyWallEnemyDetect->AddComponent<LineCollider>();
	wallEnemyDetectCollider->SetLine(Vector2D(wallEnemyDetectPos.x, wallEnemyDetectPos.y), Vector2D(wallEnemyDetectPos.x - 150, wallEnemyDetectPos.y));

	return enemy;
}

GameObject* GameObjectFactory::CreatePlayerAttackTrigger(GameObject* parent, Vector2D position, Vector2D triggersize, PLAYERATTACK_TYPE attacktype, float damage)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	GameObject* object;
	if (parent == nullptr)
	{
		object = currentScene->CreateGameObject(L"AttackTrigger");
	}
	else
	{
		object = currentScene->CreateGameObject(parent, L"AttackTrigger");
	}

	object->GetTransform()->SetLocalPosition(position);
	object->SetLayerName(L"Trigger");
	ConvexpolygonCollider* collider = object->AddComponent<ConvexpolygonCollider>();
	float left = -abs(triggersize.x / 2.0f);
	float right = abs(triggersize.x / 2.0f);
	float top = abs(triggersize.y / 2.0f);
	float bottom = -abs(triggersize.y / 2.0f);

	collider->SetPolygon({ Vector2D(left, top), Vector2D(left, bottom), Vector2D(right, bottom), Vector2D(right, top) });

	AttackTrigger* attacktrigger = object->AddComponent<AttackTrigger>();
	attacktrigger->SetUser(AttackTrigger::User::PLAYER);
	attacktrigger->SetPlayerAttack(attacktype, damage);

	return object;
}

void GameObjectFactory::CreateScreenCover()
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	GameObject* coverTop = currentScene->CreateGameObject(L"Top");
	coverTop->GetTransform()->SetLocalPosition(Vector2D(-6000, -15000));
	coverTop->GetTransform()->SetLocalScale(Vector3D(30, 30, 1));
	Image* imageTop = coverTop->AddComponent<Image>();
	imageTop->SetSpriteName(L"Black");
	imageTop->SetImageNumber(0);
	imageTop->SetLayer(Code404::Layer::SuperTop);

	GameObject* coverBottom = currentScene->CreateGameObject(L"Bottom");
	coverBottom->GetTransform()->SetLocalPosition(Vector2D(-6000, 1080));
	coverBottom->GetTransform()->SetLocalScale(Vector3D(30, 30, 1));
	Image* imageBottom = coverBottom->AddComponent<Image>();
	imageBottom->SetSpriteName(L"Black");
	imageBottom->SetImageNumber(0);
	imageBottom->SetLayer(Code404::Layer::SuperTop);

	GameObject* coverLeft = currentScene->CreateGameObject(L"Left");
	coverLeft->GetTransform()->SetLocalPosition(Vector2D(-15000, -6000));
	coverLeft->GetTransform()->SetLocalScale(Vector3D(30, 30, 1));
	Image* imageLeft = coverLeft->AddComponent<Image>();
	imageLeft->SetSpriteName(L"Black");
	imageLeft->SetImageNumber(0);
	imageLeft->SetLayer(Code404::Layer::SuperTop);

	GameObject* coverRight = currentScene->CreateGameObject(L"Right");
	coverRight->GetTransform()->SetLocalPosition(Vector2D(1920, -6000));
	coverRight->GetTransform()->SetLocalScale(Vector3D(30, 30, 1));
	Image* imageRight = coverRight->AddComponent<Image>();
	imageRight->SetSpriteName(L"Black");
	imageRight->SetImageNumber(0);
	imageRight->SetLayer(Code404::Layer::SuperTop);
}

GameObject* GameObjectFactory::CreateParryingSystem()
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	GameObject* object = currentScene->CreateGameObject(L"ParringSystem");
	object->AddComponent<ParryingSystem>();

	return object;
}

GameObject* GameObjectFactory::CreateItem()
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	GameObject* object = currentScene->CreateGameObject(L"Item");
	object->SetLayerName(L"Item");
	ConvexpolygonCollider* collider = object->AddComponent<ConvexpolygonCollider>();
	collider->SetRegularPolygon(4, 50.0f);
	SpriteRenderer* sprite = object->AddComponent<SpriteRenderer>();
	sprite->SetSpriteName(L"Item");
	sprite->SetImageNumber(0);
	sprite->SetLayer(Code404::Layer::Item);

	return object;
}

void GameObjectFactory::CreateSettingUI(bool isingame)
{
	// 버튼소리 SOUND_EFFECT_CLICK

	// 배경
	GameObject* background = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->CreateGameObject(L"BackGround");
	background->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	SpriteRenderer* backgroundimage = background->AddComponent<SpriteRenderer>();
	backgroundimage->SetSpriteName(L"MainScene_BackGround");
	backgroundimage->SetImageNumber(0);
	backgroundimage->SetLayer(Code404::Layer::UI0);

	// 시작버튼
	GameObject* startbuttonobject = GetCurrentScene()->CreateGameObject(L"StartButton");
	//startbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1442, 258 + 192 * 0));
	startbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1442, 180 + 192 * 0));
	startbuttonobject->AddComponent<UI>()->SetSize(Vector2D(350, 128));
	Image* startbuttonimage = startbuttonobject->AddComponent<Image>();
	startbuttonimage->SetSpriteName(L"UI_Start_Button_Idle");
	startbuttonimage->SetImageNumber(0);
	startbuttonimage->SetLayer(Code404::Layer::UI1);
	Button* startbutton = startbuttonobject->AddComponent<Button>();
	startbutton->SetImage(L"UI_Start_Button_Idle", L"UI_Start_Button_Enter", L"UI_Start_Button_Click");
	startbutton->SetSound(SOUND_EFFECT_CLICK);

	// 세팅버튼
	GameObject* settingbuttonobject = GetCurrentScene()->CreateGameObject(L"SettingButton");
	//settingbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1442, 258 + 192 * 1));
	settingbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1442, 180 + 192 * 1));
	settingbuttonobject->AddComponent<UI>()->SetSize(Vector2D(350, 128));
	Image* settingbuttonimage = settingbuttonobject->AddComponent<Image>();
	settingbuttonimage->SetSpriteName(L"UI_Setting_Button_Idle");
	settingbuttonimage->SetImageNumber(0);
	settingbuttonimage->SetLayer(Code404::Layer::UI1);
	settingbuttonobject->AddComponent<UI>();
	Button* settingbutton = settingbuttonobject->AddComponent<Button>();
	settingbutton->SetImage(L"UI_Setting_Button_Idle", L"UI_Setting_Button_Enter", L"UI_Setting_Button_Click");
	settingbutton->SetSound(SOUND_EFFECT_CLICK);

	// 제작진버튼
	GameObject* crewbuttonobject = GetCurrentScene()->CreateGameObject(L"CrewButton");
	//crewbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1442, 258 + 192 * 2));
	crewbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1442, 180 + 192 * 2));
	crewbuttonobject->AddComponent<UI>()->SetSize(Vector2D(350, 128));
	Image* crewbuttonimage = crewbuttonobject->AddComponent<Image>();
	crewbuttonimage->SetSpriteName(L"UI_Team_Button_Idle");
	crewbuttonimage->SetImageNumber(0);
	crewbuttonimage->SetLayer(Code404::Layer::UI1);
	Button* crewbutton = crewbuttonobject->AddComponent<Button>();
	crewbutton->SetImage(L"UI_Team_Button_Idle", L"UI_Team_Button_Enter", L"UI_Team_Button_Click");
	crewbutton->SetSound(SOUND_EFFECT_CLICK);

	// 나가기버튼
	GameObject* exitbuttonobject = GetCurrentScene()->CreateGameObject(L"ExitButton");
	//exitbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1442, 258 + 192 * 3));
	exitbuttonobject->GetTransform()->SetLocalPosition(Vector2D(1592, 180 + 192 * 3));
	exitbuttonobject->AddComponent<UI>()->SetSize(Vector2D(200, 128));
	Image* exitbuttonimage = exitbuttonobject->AddComponent<Image>();
	exitbuttonimage->SetSpriteName(L"UI_Exit_Button_Idle");
	exitbuttonimage->SetImageNumber(0);
	exitbuttonimage->SetLayer(Code404::Layer::UI1);
	Button* exitbutton = exitbuttonobject->AddComponent<Button>();
	exitbutton->SetImage(L"UI_Exit_Button_Idle", L"UI_Exit_Button_Enter", L"UI_Exit_Button_Click");
	exitbutton->SetSound(SOUND_EFFECT_CLICK);

#pragma region 설정창
	// 설정창
	GameObject* settingwindow = GetCurrentScene()->CreateGameObject(L"SettingWindow");
	settingwindow->SetActive(false);
	settingwindow->GetTransform()->SetLocalPosition(Vector2D(460, 90));
	Image* settingwindowimage = settingwindow->AddComponent<Image>();
	settingwindowimage->SetSpriteName(L"UI_PopUp_Window");
	settingwindowimage->SetImageNumber(0);
	settingwindowimage->SetLayer(Code404::Layer::UI2);

	// 설정창-닫기버튼
	GameObject* settingclosebuttonobject = GetCurrentScene()->CreateGameObject(settingwindow, L"SettingCloseButton");
	settingclosebuttonobject->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	settingclosebuttonobject->GetTransform()->SetLocalScale(Vector3D(0.7f, 0.7f, 1.0f));
	settingclosebuttonobject->AddComponent<UI>()->SetSize(Vector2D(128 * 0.7f, 128 * 0.7f));
	Image* settingclosebuttonimage = settingclosebuttonobject->AddComponent<Image>();
	settingclosebuttonimage->SetSpriteName(L"UI_Close_Button_Idle");
	settingclosebuttonimage->SetImageNumber(0);
	settingclosebuttonimage->SetLayer(Code404::Layer::UI3);
	Button* settingclosebutton = settingclosebuttonobject->AddComponent<Button>();
	settingclosebutton->SetImage(L"UI_Close_Button_Idle", L"UI_Close_Button_Enter", L"UI_Close_Button_Click");
	settingclosebutton->SetSound(SOUND_EFFECT_CLICK);

	// 설정창-소리설정버튼
	GameObject* soundsettingbuttonobject = GetCurrentScene()->CreateGameObject(settingwindow, L"SoundSettingButton");
	soundsettingbuttonobject->GetTransform()->SetLocalPosition(Vector2D(200, 176 + 204 * 0)); // 버튼 간의 간격 64 + 버튼 y 크기 140
	soundsettingbuttonobject->AddComponent<UI>()->SetSize(Vector2D(600, 140));
	Image* soundsettingbuttonimage = soundsettingbuttonobject->AddComponent<Image>();
	soundsettingbuttonimage->SetSpriteName(L"UI_Sound_Button_Idle");
	soundsettingbuttonimage->SetImageNumber(0);
	soundsettingbuttonimage->SetLayer(Code404::Layer::UI3);
	Button* soundsettingbutton = soundsettingbuttonobject->AddComponent<Button>();
	soundsettingbutton->SetImage(L"UI_Sound_Button_Idle", L"UI_Sound_Button_Enter", L"UI_Sound_Button_Click");
	soundsettingbutton->SetSound(SOUND_EFFECT_CLICK);

	// 설정창-화면설정버튼
	GameObject* screensettingbuttonobject = GetCurrentScene()->CreateGameObject(settingwindow, L"ScreenSettingButton");
	screensettingbuttonobject->GetTransform()->SetLocalPosition(Vector2D(200, 176 + 204 * 1));
	screensettingbuttonobject->AddComponent<UI>()->SetSize(Vector2D(600, 140));
	Image* screensettingbuttonimage = screensettingbuttonobject->AddComponent<Image>();
	screensettingbuttonimage->SetSpriteName(L"UI_Display_Button_Idle");
	screensettingbuttonimage->SetImageNumber(0);
	screensettingbuttonimage->SetLayer(Code404::Layer::UI3);
	Button* screensettingbutton = screensettingbuttonobject->AddComponent<Button>();
	screensettingbutton->SetImage(L"UI_Display_Button_Idle", L"UI_Display_Button_Enter", L"UI_Display_Button_Click");
	screensettingbutton->SetSound(SOUND_EFFECT_CLICK);

	/*
	// 설정창-키설정버튼
	GameObject* keysettingbuttonobject =GetCurrentScene()->CreateGameObject(settingwindow, L"KeySettingButton");
	keysettingbuttonobject->GetTransform()->SetLocalPosition(Vector2D(200, 480));
	keysettingbuttonobject->AddComponent<UI>()->SetSize(Vector2D(600, 140));
	Image* keysettingbuttonimage = keysettingbuttonobject->AddComponent<Image>();
	keysettingbuttonimage->SetSpriteName(L"KeySettingButton");
	keysettingbuttonimage->SetImageNumber(0);
	keysettingbuttonimage->SetLayer(3);
	Button* keysettingbutton = keysettingbuttonobject->AddComponent<Button>();
	*/

	// 설정창-돌아가기
	GameObject* returnbuttonobject = GetCurrentScene()->CreateGameObject(settingwindow, L"returnbuttonobject");
	returnbuttonobject->GetTransform()->SetLocalPosition(Vector2D(200, 176 + 204 * 2));
	returnbuttonobject->AddComponent<UI>()->SetSize(Vector2D(600, 140));
	Image* returnbuttonimage = returnbuttonobject->AddComponent<Image>();
	returnbuttonimage->SetSpriteName(L"UI_Back_Button_Idle");
	returnbuttonimage->SetImageNumber(0);
	returnbuttonimage->SetLayer(Code404::Layer::UI3);
	Button* returnbutton = returnbuttonobject->AddComponent<Button>();
	returnbutton->SetImage(L"UI_Back_Button_Idle", L"UI_Back_Button_Enter", L"UI_Back_Button_Click");
	returnbutton->SetSound(SOUND_EFFECT_CLICK);
#pragma endregion

#pragma region 제작진창
	// 제작진창
	GameObject* crewwindow = GetCurrentScene()->CreateGameObject(L"CrewWindow");
	crewwindow->SetActive(false);
	crewwindow->GetTransform()->SetLocalPosition(Vector2D(460, 90));
	Image* crewwindowimage = crewwindow->AddComponent<Image>();
	crewwindowimage->SetSpriteName(L"UI_Team");
	crewwindowimage->SetImageNumber(0);
	crewwindowimage->SetLayer(Code404::Layer::UI2);

	// 제작진창-닫기버튼
	GameObject* crewclosebuttonobject = GetCurrentScene()->CreateGameObject(crewwindow, L"CrewCloseButton");
	crewclosebuttonobject->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	crewclosebuttonobject->GetTransform()->SetLocalScale(Vector3D(0.7f, 0.7f, 1.0f));
	crewclosebuttonobject->AddComponent<UI>()->SetSize(Vector2D(128 * 0.7f, 128 * 0.7f));
	Image* crewclosebuttonimage = crewclosebuttonobject->AddComponent<Image>();
	crewclosebuttonimage->SetSpriteName(L"UI_Close_Button_Idle");
	crewclosebuttonimage->SetImageNumber(0);
	crewclosebuttonimage->SetLayer(Code404::Layer::UI3);
	Button* crewclosebutton = crewclosebuttonobject->AddComponent<Button>();
	crewclosebutton->SetImage(L"UI_Close_Button_Idle", L"UI_Close_Button_Enter", L"UI_Close_Button_Click");
	crewclosebutton->SetSound(SOUND_EFFECT_CLICK);
#pragma endregion

#pragma region 소리 설정창
	// 소리 설정창
	GameObject* soundsettingwindow = GetCurrentScene()->CreateGameObject(L"SoundSettingWindow");
	soundsettingwindow->SetActive(false);
	soundsettingwindow->GetTransform()->SetLocalPosition(Vector2D(460, 90));
	Image* soundsettingwindowimage = soundsettingwindow->AddComponent<Image>();
	soundsettingwindowimage->SetSpriteName(L"UI_PopUp_Window");
	soundsettingwindowimage->SetImageNumber(0);
	soundsettingwindowimage->SetLayer(Code404::Layer::UI2);

	// 소리 설정창-닫기버튼
	GameObject* soundsettingclosebuttonobject = GetCurrentScene()->CreateGameObject(soundsettingwindow, L"SoundSettingCloseButton");
	soundsettingclosebuttonobject->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	soundsettingclosebuttonobject->GetTransform()->SetLocalScale(Vector3D(0.7f, 0.7f, 1.0f));
	soundsettingclosebuttonobject->AddComponent<UI>()->SetSize(Vector2D(128 * 0.7f, 128 * 0.7f));
	Image* soundsettingclosebuttonimage = soundsettingclosebuttonobject->AddComponent<Image>();
	soundsettingclosebuttonimage->SetSpriteName(L"UI_Close_Button_Idle");
	soundsettingclosebuttonimage->SetImageNumber(0);
	soundsettingclosebuttonimage->SetLayer(Code404::Layer::UI3);
	Button* soundsettingclosebutton = soundsettingclosebuttonobject->AddComponent<Button>();
	soundsettingclosebutton->SetImage(L"UI_Close_Button_Idle", L"UI_Close_Button_Enter", L"UI_Close_Button_Click");
	soundsettingclosebutton->SetSound(SOUND_EFFECT_CLICK);

	// 소리 설정창 - BGM 숫자 나오는 곳
	GameObject* bgmtextbox = GetCurrentScene()->CreateGameObject(soundsettingwindow, L"bgmtextbox");
	bgmtextbox->GetTransform()->SetLocalPosition(Vector2D(300, 100));
	bgmtextbox->AddComponent<UI>()->SetSize(Vector2D(400, 100));
	Image* bgmtextboximage = bgmtextbox->AddComponent<Image>();
	bgmtextboximage->SetSpriteName(L"UI_BGM");
	bgmtextboximage->SetImageNumber(0);
	bgmtextboximage->SetLayer(Code404::Layer::UI3);

	// 소리 설정창 - BGM Bar 배경
	GameObject* bgmbarbackground = GetCurrentScene()->CreateGameObject(soundsettingwindow, L"bgmbarbackground");
	bgmbarbackground->GetTransform()->SetLocalPosition(Vector2D(250, 230));
	bgmbarbackground->AddComponent<UI>()->SetSize(Vector2D(440, 120));
	Image* settingbarbackgroundimage = bgmbarbackground->AddComponent<Image>();
	settingbarbackgroundimage->SetSpriteName(L"UI_Soundbar_Background");
	settingbarbackgroundimage->SetImageNumber(0);
	settingbarbackgroundimage->SetLayer(Code404::Layer::UI3);

	// 소리 설정창 - BGM Bar
	GameObject* bgmsettingbar = GetCurrentScene()->CreateGameObject(bgmbarbackground, L"bgmsettingbar");
	bgmsettingbar->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	UI* bgmbar = bgmsettingbar->AddComponent<UI>();
	bgmbar->SetSize(Vector2D(400, 15));
	Image* bgmsettingbarimage = bgmsettingbar->AddComponent<Image>();
	bgmsettingbarimage->SetSpriteName(L"UI_Soundbar");
	bgmsettingbarimage->SetImageNumber(0);
	bgmsettingbarimage->SetLayer(Code404::Layer::UI4);

	GameObject* bgmsettingcontrollerobject = GetCurrentScene()->CreateGameObject(bgmsettingbar, L"bgmsettingcontrollerobject");
	bgmsettingcontrollerobject->GetTransform()->SetLocalPosition(Vector2D(60, 50));
	bgmsettingcontrollerobject->AddComponent<UI>()->SetSize(Vector2D(35, 45));
	BGMSoundController* bgmcontroller = bgmsettingcontrollerobject->AddComponent<BGMSoundController>();
	bgmcontroller->SetRange(350);
	Image* bgmsettingcontrollerimage = bgmsettingcontrollerobject->AddComponent<Image>();
	bgmsettingcontrollerimage->SetSpriteName(L"UI_SoundController_Button_Idle");
	bgmsettingcontrollerimage->SetImageNumber(0);
	bgmsettingcontrollerimage->SetLayer(Code404::Layer::UI5);
	Button* bgmsettingcontroller = bgmsettingcontrollerobject->AddComponent<Button>();
	bgmsettingcontroller->SetImage(L"UI_SoundController_Button_Idle", L"UI_SoundController_Button_Enter", L"UI_SoundController_Button_Click");
	bgmsettingcontroller->SetSound(SOUND_EFFECT_CLICK);

	//// 소리 설정창 - BGM 최소로 줄이기 버튼
	//GameObject* bgmminimumbuttonobject = GetCurrentScene()->CreateGameObject(bgmbarbackground, L"bgmminimumbuttonobject");
	//bgmminimumbuttonobject->GetTransform()->SetLocalPosition(Vector2D(-130, 10));
	//bgmminimumbuttonobject->AddComponent<UI>()->SetSize(Vector2D(100, 100));
	//Image* bgmminimumbuttonimage = bgmminimumbuttonobject->AddComponent<Image>();
	//bgmminimumbuttonimage->SetSpriteName(L"BgmControlMIN");
	//bgmminimumbuttonimage->SetImageNumber(0);
	//bgmminimumbuttonimage->SetLayer(Code404::Layer::UI3);
	//Button* bgmminimumbutton = bgmminimumbuttonobject->AddComponent<Button>();

	//// 소리 설정창 - BGM 최대로 올리기 버튼
	//GameObject* bgmmaximumbuttonobject = GetCurrentScene()->CreateGameObject(bgmbarbackground, L"bgmmaximumbuttonobject");
	//bgmmaximumbuttonobject->GetTransform()->SetLocalPosition(Vector2D(470, 10));
	//bgmmaximumbuttonobject->AddComponent<UI>()->SetSize(Vector2D(100, 100));
	//Image* bgmmaximumbuttonimage = bgmmaximumbuttonobject->AddComponent<Image>();
	//bgmmaximumbuttonimage->SetSpriteName(L"BgmControlMAX");
	//bgmmaximumbuttonimage->SetImageNumber(0);
	//bgmmaximumbuttonimage->SetLayer(Code404::Layer::UI3);
	//Button* bgmmaximumbutton = bgmmaximumbuttonobject->AddComponent<Button>();


	// 소리 설정창 - 효과음 숫자 나오는 곳
	GameObject* soundeffecttextbox = GetCurrentScene()->CreateGameObject(soundsettingwindow, L"soundeffecttextbox");
	soundeffecttextbox->GetTransform()->SetLocalPosition(Vector2D(300, 400));
	soundeffecttextbox->AddComponent<UI>()->SetSize(Vector2D(400, 100));
	Image* soundeffecttextboximage = soundeffecttextbox->AddComponent<Image>();
	soundeffecttextboximage->SetSpriteName(L"UI_EffectSound");
	soundeffecttextboximage->SetImageNumber(0);
	soundeffecttextboximage->SetLayer(Code404::Layer::UI3);

	// 소리 설정창 - 효과음 Bar 배경
	GameObject* effectbarbackground = GetCurrentScene()->CreateGameObject(soundsettingwindow, L"effectbarbackground");
	effectbarbackground->GetTransform()->SetLocalPosition(Vector2D(250, 530));
	effectbarbackground->AddComponent<UI>()->SetSize(Vector2D(440, 120));
	Image* effectbarbackgroundimage = effectbarbackground->AddComponent<Image>();
	effectbarbackgroundimage->SetSpriteName(L"UI_Soundbar_Background");
	effectbarbackgroundimage->SetImageNumber(0);
	effectbarbackgroundimage->SetLayer(Code404::Layer::UI3);

	// 소리 설정창 - 효과음 Bar
	GameObject* effectsettingbar = GetCurrentScene()->CreateGameObject(effectbarbackground, L"effectsettingbar");
	effectsettingbar->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	UI* effectbar = effectsettingbar->AddComponent<UI>();
	effectbar->SetSize(Vector2D(400, 15));
	Image* effectsettingbarimage = effectsettingbar->AddComponent<Image>();
	effectsettingbarimage->SetSpriteName(L"UI_Soundbar");
	effectsettingbarimage->SetImageNumber(0);
	effectsettingbarimage->SetLayer(Code404::Layer::UI4);
	Button* effectsettingbarbutton = effectsettingbar->AddComponent<Button>();

	// 소리 설정창 - 효과음 Bar 컨트롤러
	GameObject* effectsettingcontrollerobject = GetCurrentScene()->CreateGameObject(effectsettingbar, L"effectsettingcontrollerobject");
	effectsettingcontrollerobject->GetTransform()->SetLocalPosition(Vector2D(60, 50));
	effectsettingcontrollerobject->AddComponent<UI>()->SetSize(Vector2D(35, 45));
	UISoundController* effectcontroller = effectsettingcontrollerobject->AddComponent<UISoundController>();
	effectcontroller->SetRange(350);
	Image* effectsettingcontrollerimage = effectsettingcontrollerobject->AddComponent<Image>();
	effectsettingcontrollerimage->SetSpriteName(L"UI_SoundController_Button_Idle");
	effectsettingcontrollerimage->SetImageNumber(0);
	effectsettingcontrollerimage->SetLayer(Code404::Layer::UI5);
	Button* effectsettingcontroller = effectsettingcontrollerobject->AddComponent<Button>();
	effectsettingcontroller->SetImage(L"UI_SoundController_Button_Idle", L"UI_SoundController_Button_Enter", L"UI_SoundController_Button_Click");
	effectsettingcontroller->SetSound(SOUND_EFFECT_CLICK);

	//// 소리 설정창 - 효과음 줄이기 버튼
	//GameObject* soundeffectminimumbuttonobject = GetCurrentScene()->CreateGameObject(effectbarbackground, L"soundeffectminimumbuttonobject");
	//soundeffectminimumbuttonobject->GetTransform()->SetLocalPosition(Vector2D(-130, 10));
	//soundeffectminimumbuttonobject->AddComponent<UI>()->SetSize(Vector2D(100, 100));
	//Image* soundeffectminimumbuttonimage = soundeffectminimumbuttonobject->AddComponent<Image>();
	//soundeffectminimumbuttonimage->SetSpriteName(L"BgmControlMIN");
	//soundeffectminimumbuttonimage->SetImageNumber(0);
	//soundeffectminimumbuttonimage->SetLayer(Code404::Layer::UI3);
	//Button* soundeffectminusbutton = soundeffectminimumbuttonobject->AddComponent<Button>();

	//// 소리 설정창 - 효과음 올리기 버튼
	//GameObject* soundeffectmaximumbuttonobject = GetCurrentScene()->CreateGameObject(effectbarbackground, L"soundeffectmaximumbuttonobject");
	//soundeffectmaximumbuttonobject->GetTransform()->SetLocalPosition(Vector2D(470, 10));
	//soundeffectmaximumbuttonobject->AddComponent<UI>()->SetSize(Vector2D(100, 100));
	//Image* soundeffectmaximumbuttonimage = soundeffectmaximumbuttonobject->AddComponent<Image>();
	//soundeffectmaximumbuttonimage->SetSpriteName(L"BgmControlMAX");
	//soundeffectmaximumbuttonimage->SetImageNumber(0);
	//soundeffectmaximumbuttonimage->SetLayer(Code404::Layer::UI3);
	//Button* soundeffectplusbutton = soundeffectmaximumbuttonobject->AddComponent<Button>();


	// 소리 설정창- 설정창 돌아가기 버튼
	GameObject* returnsettingwindowbuttonobject = GetCurrentScene()->CreateGameObject(soundsettingwindow, L"returnsettingwindowbuttonobject");
	returnsettingwindowbuttonobject->GetTransform()->SetLocalPosition(Vector2D(200, 700));
	returnsettingwindowbuttonobject->AddComponent<UI>()->SetSize(Vector2D(600, 140));
	Image* returnsettingwindowbuttonimage = returnsettingwindowbuttonobject->AddComponent<Image>();
	returnsettingwindowbuttonimage->SetSpriteName(L"UI_Back_Button_Idle");
	returnsettingwindowbuttonimage->SetImageNumber(0);
	returnsettingwindowbuttonimage->SetLayer(Code404::Layer::UI3);
	Button* returnsettingwindowbutton = returnsettingwindowbuttonobject->AddComponent<Button>();
	returnsettingwindowbutton->SetImage(L"UI_Back_Button_Idle", L"UI_Back_Button_Enter", L"UI_Back_Button_Click");
	returnsettingwindowbutton->SetSound(SOUND_EFFECT_CLICK);
#pragma endregion

#pragma region 화면 설정창
	// 화면 설정창
	GameObject* screensettingwindow = GetCurrentScene()->CreateGameObject(L"screensettingwindow");
	screensettingwindow->SetActive(false);
	screensettingwindow->GetTransform()->SetLocalPosition(Vector2D(460, 90));
	Image* screensettingwindowimage = screensettingwindow->AddComponent<Image>();
	screensettingwindowimage->SetSpriteName(L"UI_PopUp_Window");
	screensettingwindowimage->SetImageNumber(0);
	screensettingwindowimage->SetLayer(Code404::Layer::UI2);

	// 화면 설정창 - 닫기버튼
	GameObject* screensettingclosebuttonobject = GetCurrentScene()->CreateGameObject(screensettingwindow, L"screensettingclosebuttonobject");
	screensettingclosebuttonobject->GetTransform()->SetLocalPosition(Vector2D(0, 0));
	screensettingclosebuttonobject->GetTransform()->SetLocalScale(Vector3D(0.7f, 0.7f, 1.0f));
	screensettingclosebuttonobject->AddComponent<UI>()->SetSize(Vector2D(128 * 0.7f, 128 * 0.7f));
	Image* screensettingclosebuttonimage = screensettingclosebuttonobject->AddComponent<Image>();
	screensettingclosebuttonimage->SetSpriteName(L"UI_Close_Button_Idle");
	screensettingclosebuttonimage->SetImageNumber(0);
	screensettingclosebuttonimage->SetLayer(Code404::Layer::UI3);
	Button* screensettingclosebutton = screensettingclosebuttonobject->AddComponent<Button>();
	screensettingclosebutton->SetImage(L"UI_Close_Button_Idle", L"UI_Close_Button_Enter", L"UI_Close_Button_Click");
	screensettingclosebutton->SetSound(SOUND_EFFECT_CLICK);

	// 화면 설정창 - 1920 * 1080 text box
	GameObject* resolution1920textbox = GetCurrentScene()->CreateGameObject(screensettingwindow, L"resolution1920textbox");
	resolution1920textbox->GetTransform()->SetLocalPosition(Vector2D(300, 70));
	resolution1920textbox->AddComponent<UI>()->SetSize(Vector2D(500, 100));
	Image* resolution1920textboximage = resolution1920textbox->AddComponent<Image>();
	resolution1920textboximage->SetSpriteName(L"UI_1920x1080_Button_Idle");
	resolution1920textboximage->SetImageNumber(0);
	resolution1920textboximage->SetLayer(Code404::Layer::UI3);
	Button* selecttogle1920button = resolution1920textbox->AddComponent<Button>();
	selecttogle1920button->SetImage(L"UI_1920x1080_Button_Idle", L"UI_1920x1080_Button_Enter", L"UI_1920x1080_Button_Click");
	selecttogle1920button->SetSound(SOUND_EFFECT_CLICK);

	// 화면 설정창 - 1920 * 1080 Toggle
	GameObject* toggle1920object = GetCurrentScene()->CreateGameObject(resolution1920textbox, L"selecttogle1920object");
	toggle1920object->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	toggle1920object->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* toggle1920objectimage = toggle1920object->AddComponent<Image>();
	toggle1920objectimage->SetSpriteName(L"UI_Toggle_Border");
	toggle1920objectimage->SetImageNumber(0);
	toggle1920objectimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창 - 1920 * 1080 Selector
	GameObject* selector1920object = GetCurrentScene()->CreateGameObject(resolution1920textbox, L"selecttogle1920object");
	selector1920object->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	selector1920object->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* selector1920objectimage = selector1920object->AddComponent<Image>();
	selector1920objectimage->SetSpriteName(L"UI_Toggle_Selector");
	selector1920objectimage->SetImageNumber(0);
	selector1920objectimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창 - 1080 * 720 text box
	GameObject* resolution1080textbox = GetCurrentScene()->CreateGameObject(screensettingwindow, L"resolution1080textbox");
	resolution1080textbox->GetTransform()->SetLocalPosition(Vector2D(300, 210));
	resolution1080textbox->AddComponent<UI>()->SetSize(Vector2D(500, 100));
	Image* resolution1080textboximage = resolution1080textbox->AddComponent<Image>();
	resolution1080textboximage->SetSpriteName(L"UI_1080x720_Button_Idle");
	resolution1080textboximage->SetImageNumber(0);
	resolution1080textboximage->SetLayer(Code404::Layer::UI3);
	Button* selecttogle1080button = resolution1080textbox->AddComponent<Button>();
	selecttogle1080button->SetImage(L"UI_1080x720_Button_Idle", L"UI_1080x720_Button_Enter", L"UI_1080x720_Button_Click");
	selecttogle1080button->SetSound(SOUND_EFFECT_CLICK);

	// 화면 설정창 - 1080 * 720 Toggle
	GameObject* toggle1080object = GetCurrentScene()->CreateGameObject(resolution1080textbox, L"selecttogle1080object");
	toggle1080object->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	toggle1080object->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* toggle1080objectimage = toggle1080object->AddComponent<Image>();
	toggle1080objectimage->SetSpriteName(L"UI_Toggle_Border");
	toggle1080objectimage->SetImageNumber(0);
	toggle1080objectimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창 - 1080 * 720 Selector
	GameObject* selector1080object = GetCurrentScene()->CreateGameObject(resolution1080textbox, L"selecttogle1080object");
	selector1080object->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	selector1080object->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* selector1080objectimage = selector1080object->AddComponent<Image>();
	selector1080objectimage->SetSpriteName(L"UI_Toggle_Selector");
	selector1080objectimage->SetImageNumber(0);
	selector1080objectimage->SetLayer(Code404::Layer::UI3);


	//// 화면 설정창 - 구분선
	//GameObject* dividinglineobject = GetCurrentScene()->CreateGameObject(screensettingwindow, L"dividinglineobject");
	//dividinglineobject->GetTransform()->SetLocalPosition(Vector2D(200, 350));
	//dividinglineobject->AddComponent<UI>()->SetSize(Vector2D(500, 150));
	//Image* dividinglineobjectimage = dividinglineobject->AddComponent<Image>();
	//dividinglineobjectimage->SetSpriteName(L"DividingLine");
	//dividinglineobjectimage->SetImageNumber(0);
	//dividinglineobjectimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창 - FullScreen textbox
	GameObject* fullscreentextbox = GetCurrentScene()->CreateGameObject(screensettingwindow, L"fullscreentextbox");
	fullscreentextbox->GetTransform()->SetLocalPosition(Vector2D(300, 400));
	fullscreentextbox->AddComponent<UI>()->SetSize(Vector2D(500, 100));
	Image* fullscreentextboximage = fullscreentextbox->AddComponent<Image>();
	fullscreentextboximage->SetSpriteName(L"UI_Fullscreen_Button_Idle");
	fullscreentextboximage->SetImageNumber(0);
	fullscreentextboximage->SetLayer(Code404::Layer::UI3);
	Button* fullscreenselecttoglebutton = fullscreentextbox->AddComponent<Button>();
	fullscreenselecttoglebutton->SetImage(L"UI_Fullscreen_Button_Idle", L"UI_Fullscreen_Button_Enter", L"UI_Fullscreen_Button_Click");
	fullscreenselecttoglebutton->SetSound(SOUND_EFFECT_CLICK);

	// 화면 설정창 - FullScreen textbox Toggle
	GameObject* fullscreentoggleobject = GetCurrentScene()->CreateGameObject(fullscreentextbox, L"fullscreenselecttogleobject");
	fullscreentoggleobject->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	fullscreentoggleobject->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* fullscreentoggleimage = fullscreentoggleobject->AddComponent<Image>();
	fullscreentoggleimage->SetSpriteName(L"UI_Toggle_Border");
	fullscreentoggleimage->SetImageNumber(0);
	fullscreentoggleimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창 - FullScreen textbox Selector
	GameObject* fullscreenselectorobject = GetCurrentScene()->CreateGameObject(fullscreentextbox, L"fullscreenselecttogleobject");
	fullscreenselectorobject->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	fullscreenselectorobject->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* fullscreenselectorimage = fullscreenselectorobject->AddComponent<Image>();
	fullscreenselectorimage->SetSpriteName(L"UI_Toggle_Selector");
	fullscreenselectorimage->SetImageNumber(0);
	fullscreenselectorimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창 - WindowMode textbox
	GameObject* windowmodetextbox = GetCurrentScene()->CreateGameObject(screensettingwindow, L"windowmodetextbox");
	windowmodetextbox->GetTransform()->SetLocalPosition(Vector2D(300, 540));
	windowmodetextbox->AddComponent<UI>()->SetSize(Vector2D(500, 100));
	Image* windowmodetextboximage = windowmodetextbox->AddComponent<Image>();
	windowmodetextboximage->SetSpriteName(L"UI_Window_Button_Idle");
	windowmodetextboximage->SetImageNumber(0);
	windowmodetextboximage->SetLayer(Code404::Layer::UI3);
	Button* windowmodeselecttoglebutton = windowmodetextbox->AddComponent<Button>();
	windowmodeselecttoglebutton->SetImage(L"UI_Window_Button_Idle", L"UI_Window_Button_Enter", L"UI_Window_Button_Click");
	windowmodeselecttoglebutton->SetSound(SOUND_EFFECT_CLICK);

	// 화면 설정창 - WindowMode textbox Toggle
	GameObject* windowmodetoggleobject = GetCurrentScene()->CreateGameObject(windowmodetextbox, L"windowmodeselecttogleobject");
	windowmodetoggleobject->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	windowmodetoggleobject->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* windowmodetoggleimage = windowmodetoggleobject->AddComponent<Image>();
	windowmodetoggleimage->SetSpriteName(L"UI_Toggle_Border");
	windowmodetoggleimage->SetImageNumber(0);
	windowmodetoggleimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창 - WindowMode textbox Selector
	GameObject* windowmodeselectorobject = GetCurrentScene()->CreateGameObject(windowmodetextbox, L"windowmodeselecttogleobject");
	windowmodeselectorobject->GetTransform()->SetLocalPosition(Vector2D(-100, 15));
	windowmodeselectorobject->AddComponent<UI>()->SetSize(Vector2D(64, 64));
	Image* windowmodeselectorimage = windowmodeselectorobject->AddComponent<Image>();
	windowmodeselectorimage->SetSpriteName(L"UI_Toggle_Selector");
	windowmodeselectorimage->SetImageNumber(0);
	windowmodeselectorimage->SetLayer(Code404::Layer::UI3);

	// 화면 설정창- 설정창 돌아가기 버튼
	GameObject* screenreturnsettingwindowbuttonobject = GetCurrentScene()->CreateGameObject(screensettingwindow, L"screenreturnsettingwindowbuttonobject");
	screenreturnsettingwindowbuttonobject->GetTransform()->SetLocalPosition(Vector2D(200, 700));
	screenreturnsettingwindowbuttonobject->AddComponent<UI>()->SetSize(Vector2D(600, 140));
	Image* screenreturnsettingwindowbuttonimage = screenreturnsettingwindowbuttonobject->AddComponent<Image>();
	screenreturnsettingwindowbuttonimage->SetSpriteName(L"UI_Back_Button_Idle");
	screenreturnsettingwindowbuttonimage->SetImageNumber(0);
	screenreturnsettingwindowbuttonimage->SetLayer(Code404::Layer::UI3);
	Button* screenreturnsettingwindowbutton = screenreturnsettingwindowbuttonobject->AddComponent<Button>();
	screenreturnsettingwindowbutton->SetImage(L"UI_Back_Button_Idle", L"UI_Back_Button_Enter", L"UI_Back_Button_Click");
	screenreturnsettingwindowbutton->SetSound(SOUND_EFFECT_CLICK);
#pragma  endregion

	if (isingame == true)
	{
		// 인게임에서는 창을 약간 위로 이동
		settingwindow->GetTransform()->SetLocalPosition(Vector2D(460, 40));
		soundsettingwindow->GetTransform()->SetLocalPosition(Vector2D(460, 40));
		screensettingwindow->GetTransform()->SetLocalPosition(Vector2D(460, 40));

		// 인게임에서 꺼줘야 하는 것들
		background->SetActive(false);
		startbuttonobject->SetActive(false);
		settingbuttonobject->SetActive(false);
		crewbuttonobject->SetActive(false);
		exitbuttonobject->SetActive(false);

		// 게임 일시정지를 위한 컴포넌트 추가
		settingwindow->AddComponent<GamePause>();
		soundsettingwindow->AddComponent<GamePause>();
		screensettingwindow->AddComponent<GamePause>();

		// BackButton을 MenuButton으로 변경
		returnbuttonimage->SetSpriteName(L"UI_Menu_Button_Idle");
		returnbutton->SetImage(L"UI_Menu_Button_Idle", L"UI_Menu_Button_Enter", L"UI_Menu_Button_Click");

		GameObject* blackMask = GetCurrentScene()->CreateGameObject(L"blackMask");
		blackMask->GetTransform()->SetLocalScale(Vector3D(5, 5, 1));
		blackMask->SetActive(false);
		Image* blackMaskImage = blackMask->AddComponent<Image>();
		blackMaskImage->SetOpacity(0);
		blackMaskImage->SetImageNumber(0);
		blackMaskImage->SetLayer(Code404::Layer::UIHigh);
		blackMaskImage->SetSpriteName(L"Black");

		// 설정창 On / Off 오브젝트
		GameObject* inGameSettingUIControllerObject = GetCurrentScene()->CreateGameObject(L"inGameSettingUIControllerObject");
		InGameSettingUIController* logic = inGameSettingUIControllerObject->AddComponent<InGameSettingUIController>();
		logic->SetTarget(settingwindow);
		logic->SetSound(soundsettingwindow);
		logic->SetScreen(screensettingwindow);
		logic->SetBlackMask(blackMask);
	}

	// 윈도우 상태에 따라 selector상태 변경
	if (CoreSystem::GetInstance().IsFullScreenMode() == true)
	{
		selector1920object->SetActive(false);
		selector1080object->SetActive(false);
		fullscreenselectorobject->SetActive(true);
		windowmodeselectorobject->SetActive(false);
	}
	else
	{
		if (CoreSystem::GetInstance().GetScreenWidth() == 1920)
		{
			selector1920object->SetActive(true);
			selector1080object->SetActive(false);
			fullscreenselectorobject->SetActive(false);
			windowmodeselectorobject->SetActive(true);
		}
		else
		{
			selector1920object->SetActive(false);
			selector1080object->SetActive(true);
			fullscreenselectorobject->SetActive(false);
			windowmodeselectorobject->SetActive(true);
		}
	}

	startbutton->AddButtonEvent([=]()
		{
			if (crewwindow->GetActive() == false && settingwindow->GetActive() == false
				&& soundsettingwindow->GetActive() == false && screensettingwindow->GetActive() == false)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_1");
			}
		});

	settingbutton->AddButtonEvent([=]()
		{
			if (crewwindow->GetActive() == false && soundsettingwindow->GetActive() == false && screensettingwindow->GetActive() == false)
			{
				settingwindow->SetActive(true);
			}
		});

	settingclosebutton->AddButtonEvent([=]()
		{
			settingwindow->SetActive(false);
		});

	crewbutton->AddButtonEvent([=]()
		{
			if (settingwindow->GetActive() == false && soundsettingwindow->GetActive() == false && screensettingwindow->GetActive() == false)
			{
				crewwindow->SetActive(true);
			}
		});

	crewclosebutton->AddButtonEvent([=]()
		{
			crewwindow->SetActive(false);
		});

	exitbutton->AddButtonEvent([=]()
		{
			if (crewwindow->GetActive() == false && settingwindow->GetActive() == false
				&& soundsettingwindow->GetActive() == false && screensettingwindow->GetActive() == false)
			{
				PostQuitMessage(0);
			}
		});

	// 추가예정인 버튼 이벤트

	soundsettingbutton->AddButtonEvent([=]()
		{
			// 소리 설정창 버튼
			// not implement
			settingwindow->SetActive(false);
			soundsettingwindow->SetActive(true);
		});

	screensettingbutton->AddButtonEvent([=]()
		{
			// 화면 설정창 버튼
			// not implement
			settingwindow->SetActive(false);
			screensettingwindow->SetActive(true);
		});

	returnbutton->AddButtonEvent([=]()
		{
			// 설정창 나가기 버튼 이벤트
			if (isingame)
			{
				CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"MainScene");
			}
			else
			{
				settingwindow->SetActive(false);
			}
		});

	soundsettingclosebutton->AddButtonEvent([=]()
		{
			// 소리 설정창 나가기 버튼
			soundsettingwindow->SetActive(false);
			settingwindow->SetActive(true);
		}
	);

	//bgmsettingbarbutton->AddButtonEvent([=]()
	//	{
	//		// 마우스를 클릭하면
	//		// 해당 위치로 컨트롤러를 옮겨줌
	//		// 옮길 수 있는 범위는 로컬 x 0 ~ x 380 사이
	//		Vector2D mousePos = Input::GetInstance().GetMousePosition();
	//		Vector2D pos = bgmsettingbarbutton->GetGameObject()->GetTransform()->GetUIPosition();
	//		GameObject* child = bgmsettingbarbutton->GetGameObject()->GetChild(L"bgmsettingcontrollerobject");
	//		child->GetTransform()->SetLocalPosition({ mousePos.x - pos.x, child->GetTransform()->GetLocalPosition().y });
	//	}
	//);

	bgmsettingcontroller->AddButtonEvent([=]()
		{
			// BGM 설정 컨트롤러
			// 마우스 클릭, 스크린 좌표를 받아서 bgmsettingcontrollerobject의 로컬 좌표를 변경
			// BGM 크기도 변경해줘야함
			// 반드시 로컬 좌표는 x 0 ~ x 380 사이.

			// 버튼은 마우스 클릭을 땠을 때 실행이 됨
			// 마우스 클릭을 땠을 때의 시점의 좌표를 받고

			/// 사운드 컨트롤러로 대체됨.
		}
	);

	//bgmminimumbutton->AddButtonEvent([=]()
	//	{
	//		// BGM 최소로 줄이기 버튼
	//		// BGM 소리를 0으로 만들고
	//		// 컨트롤러 오브젝트의 로컬좌표 x 0으로 옮겨야함
	//		bgmsettingcontrollerobject->GetTransform()->SetLocalPosition({ 0, -28 });
	//	}
	//);

	//bgmmaximumbutton->AddButtonEvent([=]()
	//	{
	//		// BGM 최대로 올리기 버튼
	//		// BGM 소리를 100으로 만들고
	//		// 컨트롤러 오브젝트의 로컬좌표 x 400으로 옮겨야함
	//		bgmsettingcontrollerobject->GetTransform()->SetLocalPosition({ 380, -28 });
	//	}
	//);

	effectsettingbarbutton->AddButtonEvent([=]()
		{
			Vector2D mousePos = Input::GetInstance().GetMousePosition();
			Vector2D pos = effectsettingbarbutton->GetGameObject()->GetTransform()->GetUIPosition();
			GameObject* child = effectsettingbarbutton->GetGameObject()->GetChild(L"effectsettingcontrollerobject");
			child->GetTransform()->SetLocalPosition({ mousePos.x - pos.x, child->GetTransform()->GetLocalPosition().y });
		}
	);

	effectsettingcontroller->AddButtonEvent([=]()
		{
			// 효과음 설정 컨트롤러
			// 마우스 클릭, 스크린 좌표를 받아서 effectsettingcontrollerobject의 로컬 좌표를 변경
			// 효과음 크기도 변경해줘야함
			// 반드시 로컬 좌표는 x 0 ~ x 380 사이.

			/// 사운드 컨트롤러로 대체됨.
		}
	);

	//soundeffectminusbutton->AddButtonEvent([=]()
	//	{
	//		// sound effect 최소로 줄이기 버튼
	//		// sound effect 소리를 0으로 만들고
	//		// 컨트롤러 오브젝트의 로컬좌표 x 0으로 옮겨야함
	//		effectsettingcontrollerobject->GetTransform()->SetLocalPosition({ 0, -28 });
	//	});

	//soundeffectplusbutton->AddButtonEvent([=]()
	//	{
	//		// sound effect 최대로 올리기 버튼
	//		// sound effect 소리를 100으로 만들고
	//		// 컨트롤러 오브젝트의 로컬좌표 x 400으로 옮겨야함
	//		effectsettingcontrollerobject->GetTransform()->SetLocalPosition({ 380, -28 });
	//	});

	returnsettingwindowbutton->AddButtonEvent([=]()
		{
			// 소리 설정창 -> 설정창 버튼
			soundsettingwindow->SetActive(false);
			settingwindow->SetActive(true);
		});

	screensettingclosebutton->AddButtonEvent([=]()
		{
			// 화면 설정창 닫기 버튼
			screensettingwindow->SetActive(false);
			settingwindow->SetActive(true);
		});

	selecttogle1920button->AddButtonEvent([=]()
		{
			// 1920*1080 선택 버튼
			if (CoreSystem::GetInstance().IsFullScreenMode() == true)
			{
				CoreSystem::GetInstance().ChangeToWindowMode();
			}
			CoreSystem::GetInstance().ChangeWindowSize(1920, 1080);

			// 윈도우 상태에 따라 selector상태 변경
			if (CoreSystem::GetInstance().IsFullScreenMode() == true)
			{
				selector1920object->SetActive(false);
				selector1080object->SetActive(false);
				fullscreenselectorobject->SetActive(true);
				windowmodeselectorobject->SetActive(false);
			}
			else
			{
				if (CoreSystem::GetInstance().GetScreenWidth() == 1920)
				{
					selector1920object->SetActive(true);
					selector1080object->SetActive(false);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
				else
				{
					selector1920object->SetActive(false);
					selector1080object->SetActive(true);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
			}
		});

	selecttogle1080button->AddButtonEvent([=]()
		{
			// 1080*700 선택 버튼
			if (CoreSystem::GetInstance().IsFullScreenMode() == true)
			{
				CoreSystem::GetInstance().ChangeToWindowMode();
			}
			CoreSystem::GetInstance().ChangeWindowSize(1080, 720);

			// 윈도우 상태에 따라 selector상태 변경
			if (CoreSystem::GetInstance().IsFullScreenMode() == true)
			{
				selector1920object->SetActive(false);
				selector1080object->SetActive(false);
				fullscreenselectorobject->SetActive(true);
				windowmodeselectorobject->SetActive(false);
			}
			else
			{
				if (CoreSystem::GetInstance().GetScreenWidth() == 1920)
				{
					selector1920object->SetActive(true);
					selector1080object->SetActive(false);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
				else
				{
					selector1920object->SetActive(false);
					selector1080object->SetActive(true);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
			}
		});

	fullscreenselecttoglebutton->AddButtonEvent([=]()
		{
			// 전제화면 선택 버튼
			CoreSystem::GetInstance().ChangeToFullScreenMode();

			// 윈도우 상태에 따라 selector상태 변경
			if (CoreSystem::GetInstance().IsFullScreenMode() == true)
			{
				selector1920object->SetActive(false);
				selector1080object->SetActive(false);
				fullscreenselectorobject->SetActive(true);
				windowmodeselectorobject->SetActive(false);
			}
			else
			{
				if (CoreSystem::GetInstance().GetScreenWidth() == 1920)
				{
					selector1920object->SetActive(true);
					selector1080object->SetActive(false);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
				else
				{
					selector1920object->SetActive(false);
					selector1080object->SetActive(true);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
			}
		});

	windowmodeselecttoglebutton->AddButtonEvent([=]()
		{
			// 창모드 선택 버튼
			CoreSystem::GetInstance().ChangeToWindowMode();

			// 윈도우 상태에 따라 selector상태 변경
			if (CoreSystem::GetInstance().IsFullScreenMode() == true)
			{
				selector1920object->SetActive(false);
				selector1080object->SetActive(false);
				fullscreenselectorobject->SetActive(true);
				windowmodeselectorobject->SetActive(false);
			}
			else
			{
				if (CoreSystem::GetInstance().GetScreenWidth() == 1920)
				{
					selector1920object->SetActive(true);
					selector1080object->SetActive(false);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
				else
				{
					selector1920object->SetActive(false);
					selector1080object->SetActive(true);
					fullscreenselectorobject->SetActive(false);
					windowmodeselectorobject->SetActive(true);
				}
			}
		});

	screenreturnsettingwindowbutton->AddButtonEvent([=]()
		{
			// 화면 설정창에서 설정창으로 나가는 버튼
			screensettingwindow->SetActive(false);
			settingwindow->SetActive(true);
		});
}

BJEngine::Scene* GameObjectFactory::GetCurrentScene()
{
	return CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();
}

GameObjectFactory::GameObjectFactory()
{

}

GameObjectFactory::~GameObjectFactory()
{

}
