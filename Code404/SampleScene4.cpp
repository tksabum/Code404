#include "SampleScene4.h"

#include "SceneController.h"

#include "CircleRenderer.h"
#include "CircleCollider.h"
#include "Rigidbody2D.h"
#include "PolygonRenderer.h"
#include "ConvexpolygonCollider.h"
#include "LineCollider.h"
#include "LineRenderer.h"

#include "Controller.h"

#include "Idle_State.h"
#include "Patrol_State.h"
#include "Trace_State.h"
#include "Die_State.h"
#include "NormalAttack_State.h"
#include "Hit_State.h"

#include "GroundDetect.h"
#include "RangeDetect.h"

#include "AI.h"

#include "Boss.h"
#include "BossHand.h"

#include "CameraShake.h"
#include "Vector3D.h"

#include "Player.h"

#include "ObjectPoolSystem.h"
#include "BossNormalBullet.h"

#include "Patrol.h"

#include "Twinkling.h";

#include "GraphicsSystem.h"

#include "AttackCollider.h"

#include "laser.h"

#include "BossEye.h"
#include "Enemy.h"

#include "AnimTest.h"

#include "EffectPlayer.h"

#include "GameObjectFactory.h"

#include "Text.h"
#include "EnemyDebug.h"

#include "Bullet.h"

#include "Dagger.h"

#include "Level.h"

#include "LaserScaleUp.h"

#include "BossHandRedOpacity.h"

#include "TimeLaser.h"

#include "BossSceneEvent.h"
#include "ParryingSystem.h"

#include "DebugHand.h"

#include "ItemMove.h"

using namespace std;
using namespace BJEngine;
using namespace BJEngine::Math;

SampleScene4::SampleScene4() : Scene(L"SampleScene4")
{

}

void SampleScene4::Start()
{
	GameObject* test = CreateGameObject(L"test");
	test->AddComponent<CircleRenderer>();
	test->GetComponent<CircleRenderer>()->SetCircle(100);
	test->AddComponent<ItemMove>();

	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	GameObjectFactory::GetInstance().CreateSettingUI(true);
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	//CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->SetTimeScale(0);

	GameObjectFactory::GetInstance().CreateScreenCover();


	GameObjectFactory::GetInstance().CreatePrefab();

	GetMainCamera()->GetGameObject()->GetTransform()->SetLocalPosition(Vector2D(530,700));


	GameObject* backGround = CreateGameObject(L"backGround");
	backGround->GetTransform()->SetLocalPosition(Vector2D(530, 700));
	SpriteRenderer* backGroundRenderer = backGround->AddComponent<SpriteRenderer>();
	backGroundRenderer->SetSpriteName(L"Boss_Scene_BackGround");
	backGroundRenderer->SetLayer(-1);
	backGroundRenderer->SetImageNumber(0);

#pragma region 연출 이벤트
	//GameObject* blackMask = CreateGameObject(L"blackMask");
	//blackMask->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5, 5, 1));
	//Image* blackMaskImage = blackMask->AddComponent<Image>();
	//blackMaskImage->SetSpriteName(L"Black");
	//blackMaskImage->SetLayer(Code404::Layer::BOSS_BLACKMASK);
	//blackMaskImage->SetImageNumber(0);

	//GameObject* bossHeadUI = CreateGameObject(L"bossHeadUI");
	//bossHeadUI->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.8f, 0.8f, 0.8f));
	//bossHeadUI->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(786, 62, 1));
	//Image* bossHeadUIImage = bossHeadUI->AddComponent<Image>();
	//bossHeadUIImage->SetSpriteName(L"Boss_Head_Blue");
	//bossHeadUIImage->SetLayer(Code404::Layer::BOSS_BLACKMASK);
	//bossHeadUIImage->SetImageNumber(0);

	//GameObject* bossEyeUI = CreateGameObject(bossHeadUI, L"bossEyeUI");
	//bossEyeUI->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(134, 152, 1));
	//Image* bossEyeUIImage = bossEyeUI->AddComponent<Image>();
	//bossEyeUIImage->SetSpriteName(L"Boss_Eye_Blue");
	//bossEyeUIImage->SetLayer(Code404::Layer::BOSS_BLACKMASK);
	//bossEyeUIImage->SetImageNumber(0);

	//GameObject* bossleftHandUI = CreateGameObject(bossHeadUI, L"bossleftHandUI");
	//bossleftHandUI->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.9f, 0.9f, 0.9f));
	//bossleftHandUI->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-284, 240, 1));
	//Image* bossleftHandUIImage = bossleftHandUI->AddComponent<Image>();
	//bossleftHandUIImage->SetSpriteName(L"Boss_Hand_Blue");
	//bossleftHandUIImage->SetLayer(Code404::Layer::BOSS_BLACKMASK);
	//bossleftHandUIImage->SetImageNumber(0);

	//GameObject* bossRightHandUI = CreateGameObject(bossHeadUI, L"bossRightHandUI");
	//bossRightHandUI->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(-0.9f, 0.9f, 0.9f));
	//bossRightHandUI->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(628, 240, 1));
	//Image* bossRightHandUIImage = bossRightHandUI->AddComponent<Image>();
	//bossRightHandUIImage->SetSpriteName(L"Boss_Hand_Blue");
	//bossRightHandUIImage->SetLayer(Code404::Layer::BOSS_BLACKMASK);
	//bossRightHandUIImage->SetImageNumber(0);


#pragma endregion

#pragma region 보스 테스트

	GameObject* barParent = CreateGameObject(L"barParent");
	barParent->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(80,10,1));
	Image* bossImage = barParent->AddComponent<Image>();
	bossImage->SetSpriteName(L"Boss_Protrait_Blue");
	bossImage->SetLayer(Code404::Layer::UILow);
	bossImage->SetImageNumber(0);

	GameObject* bossHpBar = CreateGameObject(barParent,L"bossHpBar");
	bossHpBar->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(150, 5, 1));
	Image* bossHpBarImage = bossHpBar->AddComponent<Image>();
	bossHpBarImage->SetSpriteName(L"Boss_HP_Bar");
	bossHpBarImage->SetLayer(Code404::Layer::UILow);
	bossHpBarImage->SetImageNumber(0);

	GameObject* bossHpBarOutLine = CreateGameObject(barParent, L"bossHpBarOutLine");
	bossHpBarOutLine->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(150, 5, 1));
	Image* bossHpBarOutLineImage = bossHpBarOutLine->AddComponent<Image>();
	bossHpBarOutLineImage->SetSpriteName(L"Boss_HP_Bar_OutLine");
	bossHpBarOutLineImage->SetLayer(Code404::Layer::UIHigh);
	bossHpBarOutLineImage->SetImageNumber(0);

	GetMainCamera()->GetGameObject()->AddComponent<CameraShake>();

	// player 셋팅
	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer(0.7f, false, 93);

#pragma region 벽 셋팅
	vector<Vector2D> topWallVertex = { Vector2D(-1100,-10),Vector2D(-1100,10), Vector2D(1100,10), Vector2D(1100,-10) };
	vector<Vector2D> leftWallVertex = { Vector2D(-10,-350),Vector2D(-10,350), Vector2D(10,350), Vector2D(10,-350) };
	vector<Vector2D> bossHeadVertex = { Vector2D(-150,245),Vector2D(-150,-225), Vector2D(150,-225), Vector2D(150,245) };
	vector<Vector2D> bossHandVertex = { Vector2D(-150,150),Vector2D(-150,-80), Vector2D(150,-80), Vector2D(150,150) };
	vector<BJEngine::Math::Vector3D> laserPatrolPoint = { BJEngine::Math::Vector3D(530,340,0), BJEngine::Math::Vector3D(530,700,0) };

	vector<Vector2D> bossCrossLaserVertex = { Vector2D(-25,2200),Vector2D(-25,-2200), Vector2D(25,-2200), Vector2D(25,2200) };
	vector<Vector2D> bossPatrolLaserVertex = { Vector2D(-2200,16),Vector2D(-2200,-16), Vector2D(2200,-16), Vector2D(2200,16) };
	vector<Vector2D> bossTimeLaserVertex = { Vector2D(-2200,16),Vector2D(-2200,-16), Vector2D(2200,-16), Vector2D(2200,16) };

#pragma endregion

	// 보스 몬스터 머리 셋팅
	GameObject* bossHead = CreateGameObject(L"bossHead");
	bossHead->SetLayerName(L"Boss");
	bossHead->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.8f, 0.8f, 0.8f));
	bossHead->AddComponent<Enemy>()->SetMaxHP(Code404::BOSS_HEAD_MAX_HP);
	SpriteRenderer* bossHeadRenderer = bossHead->AddComponent<SpriteRenderer>();
	bossHeadRenderer->SetSpriteName(L"Boss_Head_Blue");
	bossHeadRenderer->SetLayer(90);
	bossHeadRenderer->SetImageNumber(0);
	Boss* bossLogic = bossHead->AddComponent<Boss>();
	bossLogic->SetTarget(playerobject);
	bossHead->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(528, 430, 0));
	bossHead->AddComponent<ConvexpolygonCollider>()->SetPolygon(bossHeadVertex);
	bossHead->GetComponent<ConvexpolygonCollider>()->SetTrigger(true);
	Rigidbody2D* bossRigid = bossHead->AddComponent<Rigidbody2D>();
	bossRigid->SetGravity(Vector2D(0, 1000));
	bossRigid->SetRotationLock(true);
	bossRigid->SetRestitutionValue(0.f);
	bossRigid->SetAffectedByGravity(false);

	// 포인트들
	GameObject* point = CreateGameObject(bossHead, L"point");
	point->AddComponent<CircleRenderer>()->SetCircle(10);
	point->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(0, 1000, 0));
	GameObject* point0 = CreateGameObject(bossHead,L"point0");
	point0->AddComponent<CircleRenderer>()->SetCircle(10);
	point0->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-1200, 350, 0));
	GameObject* point1 = CreateGameObject(bossHead,L"point1");
	point1->AddComponent<CircleRenderer>()->SetCircle(10);
	point1->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(1200, 350, 0));
	GameObject* point2 = CreateGameObject(bossHead,L"point2");
	point2->AddComponent<CircleRenderer>()->SetCircle(10);
	point2->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-1200, 550, 0));
	GameObject* point3 = CreateGameObject(bossHead,L"point3");
	point3->AddComponent<CircleRenderer>()->SetCircle(10);
	point3->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(1200, 550, 0));

	// 보스의 눈동자 셋팅
	GameObject* bossEye = CreateGameObject(bossHead, L"bossEye");
	SpriteRenderer* eyeRenderer = bossEye->AddComponent<SpriteRenderer>();
	eyeRenderer->SetSpriteName(L"Boss_Eye_Blue");
	eyeRenderer->SetLayer(91);
	eyeRenderer->SetImageNumber(0);
	BossEye* bossEyeComponent = bossEye->AddComponent<BossEye>();
	bossEyeComponent->SetTarget(playerobject);

#pragma region 패트롤 레이저

	// 보스 페이지2에서 사용하는 패트롤하는 레이저
	GameObject* patrolLaser = CreateGameObject(L"patrolLaser");
	patrolLaser->SetLayerName(L"Trigger");
	SpriteRenderer* patrolLaserRenderer = patrolLaser->AddComponent<SpriteRenderer>();
	patrolLaserRenderer->SetLayer(93);
	patrolLaser->AddComponent<Animator>()->Play(L"Boss_Yellow_Laser");
	patrolLaser->SetActive(false);
	patrolLaser->AddComponent<ConvexpolygonCollider>()->SetPolygon(bossPatrolLaserVertex);
	patrolLaser->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(530, 380, 0));
	patrolLaser->GetTransform()->SetLocalRotation(90);
	Patrol* patrolLaserPatrolLaser = patrolLaser->AddComponent<Patrol>();
	laser* patrolLaserLogic = patrolLaser->AddComponent<laser>();
	patrolLaserLogic->SetLaserDamage(Code404::BOSS_PATROL_LASER_DAMAGE);

#pragma endregion

#pragma region 크로스 레이저

	// 보스가 가지고 있을 레이저의 최상위 부모 하지만 보스 대가리의 자식으로 들어감
	GameObject* laserEmpty = CreateGameObject(bossHead, L"laserEmpty");
	laserEmpty->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(0, -25, 0));
	laserEmpty->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0, 0, 1));
	laserEmpty->SetActive(false);
	laserEmpty->AddComponent<LaserScaleUp>();
	// laserEmpty의 자식으로 들어갈 실제 laser 중 하나 셋팅
	GameObject* laserOne = CreateGameObject(laserEmpty, L"laserOne");
	laserOne->SetLayerName(L"Trigger");
	laserOne->AddComponent<ConvexpolygonCollider>()->SetPolygon(bossCrossLaserVertex);
	SpriteRenderer* laserOneRenderer = laserOne->AddComponent<SpriteRenderer>();
	laserOneRenderer->SetLayer(93);
	Animator* laserOneAnim = laserOne->AddComponent<Animator>();
	laserOneAnim->Play(L"Boss_Cross_Laser");
	laser* laserOneLogic = laserOne->AddComponent<laser>();
	laserOneLogic->SetLaserDamage(Code404::BOSS_CROSS_LASER_DAMAGE);
	// laserEmpty의 자식으로 들어갈 실제 laser 다른 거 셋팅
	GameObject* laserTwo = CreateGameObject(laserEmpty, L"laserTwo");
	laserTwo->SetLayerName(L"Trigger");
	laserTwo->AddComponent<ConvexpolygonCollider>()->SetPolygon(bossCrossLaserVertex);
	SpriteRenderer* laserTwoRenderer = laserTwo->AddComponent<SpriteRenderer>();
	laserTwoRenderer->SetLayer(93);
	Animator* laserTwoAnim = laserTwo->AddComponent<Animator>();
	laserTwoAnim->Play(L"Boss_Cross_Laser");
	laserTwo->GetTransform()->SetLocalRotation(90);
	laser* laserTwoLogic = laserTwo->AddComponent<laser>();
	laserTwoLogic->SetLaserDamage(Code404::BOSS_CROSS_LASER_DAMAGE);
	// laserEmpty에 들어 갈 동그란 이미지
	GameObject* laserCenter = CreateGameObject(laserEmpty, L"laserTwo");
	SpriteRenderer* laserCenterRenderer = laserCenter->AddComponent<SpriteRenderer>();
	laserCenterRenderer->SetLayer(93);
	Animator* laserCenterAnim = laserCenter->AddComponent<Animator>();
	laserCenterAnim->Play(L"Boss_Cross_Laser_Center");

#pragma endregion

#pragma region 타임 레이저

	// 좌 구멍
	GameObject* leftHole = CreateGameObject(L"leftHole");
	leftHole->AddComponent<CircleRenderer>()->SetCircle(10.f);
	leftHole->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-350, 1020, 0));
	SpriteRenderer* leftHoleRenderer = leftHole->AddComponent<SpriteRenderer>();
	leftHoleRenderer->SetSpriteName(L"Time_Laser_CCTV");
	leftHoleRenderer->SetLayer(93);
	leftHoleRenderer->SetImageNumber(0);
	GameObject* leftHoleEffect = CreateGameObject(leftHole,L"leftHoleEffect");
	leftHoleEffect->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.5f,0.5f,1.f));
	leftHoleEffect->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(32,0,0));
	leftHoleEffect->SetActive(false);
	SpriteRenderer* leftHoleEffectRenderer = leftHoleEffect->AddComponent<SpriteRenderer>();
	leftHoleEffectRenderer->SetLayer(93);
	Animator* leftHoleAnim = leftHoleEffect->AddComponent<Animator>();
	leftHoleAnim->Play(L"Time_Laser_CCTV_Effect");

	// 우 구멍
	GameObject* rightHole = CreateGameObject(L"rightHole");
	rightHole->AddComponent<CircleRenderer>()->SetCircle(10.f);
	rightHole->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(1405, 1020, 0));
	rightHole->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(-1,1,1));
	SpriteRenderer* rightHoleRenderer = rightHole->AddComponent<SpriteRenderer>();
	rightHoleRenderer->SetSpriteName(L"Time_Laser_CCTV");
	rightHoleRenderer->SetLayer(93);
	rightHoleRenderer->SetImageNumber(0);
	GameObject* rightHoleEffect = CreateGameObject(rightHole, L"rightHoleEffect");
	rightHoleEffect->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.5f, 0.5f, 1.f));
	rightHoleEffect->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-32, 0, 0));
	rightHoleEffect->SetActive(false);
	SpriteRenderer* rightHoleEffectRenderer = rightHoleEffect->AddComponent<SpriteRenderer>();
	rightHoleEffectRenderer->SetLayer(93);
	Animator* rightHoleAnim = rightHoleEffect->AddComponent<Animator>();
	rightHoleAnim->Play(L"Time_Laser_CCTV_Effect");

	// 레이저
	GameObject* timeLaserParent = CreateGameObject(L"timeLaserParent");
	timeLaserParent->SetActive(false);
	timeLaserParent->AddComponent<TimeLaser>();

	GameObject* timeLaser = CreateGameObject(timeLaserParent,L"timeLaser");
	timeLaser->SetLayerName(L"Trigger");
	timeLaser->AddComponent<ConvexpolygonCollider>()->SetPolygon(bossTimeLaserVertex);
	SpriteRenderer* timeLaserREenderer = timeLaser->AddComponent<SpriteRenderer>();
	timeLaserREenderer->SetLayer(92);
	timeLaser->AddComponent<Animator>()->Play(L"Boss_Yellow_Laser");
	timeLaser->SetActive(false);
	timeLaser->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(530, 1020, 0));
	laser* timeLaserLogic = timeLaser->AddComponent<laser>();
	timeLaserLogic->SetLaserDamage(Code404::BOSS_TIME_LASER_DAMAGE);

#pragma endregion

#pragma region 보스 왼손

	// 보스 왼손 셋팅
	GameObject* bossLeftHand = CreateGameObject(bossHead, L"bossLeftHand");
	bossLeftHand->SetLayerName(L"Boss");
	bossLeftHand->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.9f, 0.9f, 0.9f));
	Enemy* leftHandEnemyLogic = bossLeftHand->AddComponent<Enemy>();
	leftHandEnemyLogic->SetMaxHP(Code404::BOSS_HAND_MAX_HP);
	bossLeftHand->AddComponent<BossHand>();
	bossLeftHand->GetComponent<BossHand>()->SetHandType(HAND_TYPE::LEFT);
	bossLeftHand->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-300, 150, 0));
	SpriteRenderer* leftHandRenderer = bossLeftHand->AddComponent<SpriteRenderer>();
	leftHandRenderer->SetSpriteName(L"Boss_Hand_Blue");
	leftHandRenderer->SetLayer(92);
	leftHandRenderer->SetImageNumber(0);
	ConvexpolygonCollider* leftHandCollider = bossLeftHand->AddComponent<ConvexpolygonCollider>();
	leftHandCollider->SetPolygon(bossHandVertex);
	leftHandCollider->SetTrigger(true);
	Rigidbody2D* leftHandRigid = bossLeftHand->AddComponent<Rigidbody2D>();
	leftHandRigid->SetGravity(Vector2D(0, 1000));
	leftHandRigid->SetRotationLock(true);
	leftHandRigid->SetRestitutionValue(0.f);
	leftHandRigid->SetAffectedByGravity(false);
	// 보스 왼손 빨간색 셋팅
	GameObject* leftHandRed = CreateGameObject(bossLeftHand, L"leftHandRed");
	BossHandRedOpacity* leftRedOpacity = leftHandRed->AddComponent<BossHandRedOpacity>();
	leftRedOpacity->SetHP(leftHandEnemyLogic->GetMaxHPAdd(), leftHandEnemyLogic->GetCurHPAdd());
	SpriteRenderer* leftHandRedRenderer = leftHandRed->AddComponent<SpriteRenderer>();
	leftHandRedRenderer->SetSpriteName(L"Boss_Hand_Red");
	leftHandRedRenderer->SetLayer(92);
	leftHandRedRenderer->SetImageNumber(0);
	leftHandRedRenderer->SetOpacity(0.f);

#pragma endregion

#pragma region 보스 오른손

	// 보스 오른손 셋팅
	GameObject* bossRightHand = CreateGameObject(bossHead, L"bossRightHand");
	bossRightHand->SetLayerName(L"Boss");
	bossRightHand->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(-0.9f, 0.9f, 0.9f));
	SpriteRenderer* rightHandRenderer = bossRightHand->AddComponent<SpriteRenderer>();
	rightHandRenderer->SetSpriteName(L"Boss_Hand_Blue");
	rightHandRenderer->SetLayer(92);
	rightHandRenderer->SetImageNumber(0);
	Enemy* rightHandEnemyLogic = bossRightHand->AddComponent<Enemy>();
	rightHandEnemyLogic->SetMaxHP(Code404::BOSS_HAND_MAX_HP);
	bossRightHand->AddComponent<BossHand>();
	bossRightHand->GetComponent<BossHand>()->SetHandType(HAND_TYPE::RIGHT);
	bossRightHand->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(300, 150, 0));
	bossRightHand->AddComponent<ConvexpolygonCollider>()->SetPolygon(bossHandVertex);
	bossRightHand->GetComponent<ConvexpolygonCollider>()->SetTrigger(true);
	Rigidbody2D* rightHandRigid = bossRightHand->AddComponent<Rigidbody2D>();
	rightHandRigid->SetGravity(Vector2D(0, 1000));
	rightHandRigid->SetRotationLock(true);
	rightHandRigid->SetRestitutionValue(0.f);
	rightHandRigid->SetAffectedByGravity(false);
	// 보스 오른손 빨간색 셋팅
	GameObject* rightHandRed = CreateGameObject(bossRightHand, L"rightHandRed");
	BossHandRedOpacity* rightRedOpacity = rightHandRed->AddComponent<BossHandRedOpacity>();
	rightRedOpacity->SetHP(rightHandEnemyLogic->GetMaxHPAdd(), rightHandEnemyLogic->GetCurHPAdd());
	SpriteRenderer* rightHandRedRenderer = rightHandRed->AddComponent<SpriteRenderer>();
	rightHandRedRenderer->SetSpriteName(L"Boss_Hand_Red");
	rightHandRedRenderer->SetLayer(92);
	rightHandRedRenderer->SetImageNumber(0);
	rightHandRedRenderer->SetOpacity(0.f);

#pragma endregion
#pragma endregion

	Level level(STAGE_BOSS);
	level.Tiling(level);
	level.CreatePlatformFromLevelData(level);
	for (int j = 0; j < 17; j++)
	{
		GameObject* platform = CreateGameObject(L"Platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-464.0f, 200.0f + j * 64));
		SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();
		platformImage->SetSpriteName(L"TileSheet1");
		platformImage->SetImageNumber(8);
		platformImage->SetLayer(100);
		platform->SetLayerName(L"Platform");
		
	}

	for (int j = 0; j < 17; j++)
	{
		GameObject* platform = CreateGameObject(L"Platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400+30*64.0f, 200.0f + j * 64));
		SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();
		platformImage->SetSpriteName(L"TileSheet1");
		platformImage->SetImageNumber(10);
		platformImage->SetLayer(100);
		platform->SetLayerName(L"Platform");

	}
	for (int i = -1; i < 31; i++)
	{
		GameObject* platform = CreateGameObject(L"Platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400 +  i* 64.0f, 200.0f-64.f));
		SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();
		platformImage->SetSpriteName(L"TileSheet1");
		platformImage->SetImageNumber(5);
		platformImage->SetLayer(100);
		platform->SetLayerName(L"Platform");
	}

	for (int i = -1; i < 31; i++)
	{
		GameObject* platform = CreateGameObject(L"Platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(-400 + i * 64.0f, 200.0f + 64.f*17));
		SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();
		platformImage->SetSpriteName(L"TileSheet0");
		platformImage->SetImageNumber(25);
		platformImage->SetLayer(100);
		platform->SetLayerName(L"Platform");
	}

	GameObject* blackMask2 = CreateGameObject(L"blackMask2");
	blackMask2->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5, 5, 1));
	Image* blackMask2Image = blackMask2->AddComponent<Image>();
	blackMask2Image->SetSpriteName(L"Black");
	blackMask2Image->SetLayer(Code404::Layer::BOSS_BLACKMASK);
	blackMask2Image->SetImageNumber(0);

	GameObject* bossSceneEventObject = CreateGameObject(L"bossSceneEventObject");
	BossSceneEvent* eventLogic = bossSceneEventObject->AddComponent<BossSceneEvent>();
	eventLogic->SetBossObject(bossHead);
	eventLogic->SetCutSceneRenderer(blackMask2Image);
}