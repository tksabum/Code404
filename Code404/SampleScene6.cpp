#include "SampleScene6.h"
#include "GameObjectFactory.h"

/// 편한 디버깅을 위한 씬 컨트롤러
#include "SceneController.h"

#include "GameDataManager.h"

#include "Level.h"
#include "Camera.h"
#include "Controller.h"
#include "SpriteRenderer.h"

#include "BossSceneEvent.h"
#include "NewBossCutSceneController.h"
#include "CameraShake.h"

#include "PlayerStealthCounter.h"
#include "SkillCoolTimeViewer.h"
#include "PlayerHpBar.h"
#include "PlayerMpBar.h"

#include "SoundManager.h"

using namespace BJEngine;
using namespace BJEngine::Math;

SampleScene6::SampleScene6() : Scene(L"SampleScene6")
{

}

void SampleScene6::Start()
{
	SoundManager::GetInstance().Play(SOUND_BGM_BOSSSTAGE);

	GameObjectFactory::GetInstance().CreateScreenCover();
	GameObjectFactory::GetInstance().CreatePrefab();

	// GameObjectFactory::GetInstance().CreateSettingUI();

	GameDataManager::GetInstance().Clear();
	GameDataManager::GetInstance().ClearCheat();

	// 카메라 위치 조정
	GetMainCamera()->GetGameObject()->GetTransform()->SetLocalPosition(Vector2D(530, 700));
	GetMainCamera()->GetGameObject()->AddComponent<CameraShake>();

	// 배경화면 이미지
	GameObject* backGround = CreateGameObject(L"backGround");
	backGround->GetTransform()->SetLocalPosition(Vector2D(530, 700));
	SpriteRenderer* backGroundRenderer = backGround->AddComponent<SpriteRenderer>();
	backGroundRenderer->SetSpriteName(L"Boss_Scene_BackGround");
	backGroundRenderer->SetLayer(-1);
	backGroundRenderer->SetImageNumber(0);

	// 블랙 마스크 이미지
	GameObject* blackMask = CreateGameObject(L"blackMask");
	blackMask->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(5, 5, 1));
	blackMask->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(528, 430, 0));
	SpriteRenderer* blackMaskImage = blackMask->AddComponent<SpriteRenderer>();
	blackMaskImage->SetSpriteName(L"Black");
	blackMaskImage->SetLayer(101);
	blackMaskImage->SetImageNumber(0);

	// 보스 머리 이미지
	GameObject* bossHead = CreateGameObject(L"bossHead");
	bossHead->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.8f, 0.8f, 0.8f));
	bossHead->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(528, 430, 0));
	SpriteRenderer* bossHeadRenderer = bossHead->AddComponent<SpriteRenderer>();
	bossHeadRenderer->SetSpriteName(L"Boss_Head_Blue");
	bossHeadRenderer->SetLayer(102);
	bossHeadRenderer->SetImageNumber(0);
	bossHeadRenderer->SetOpacity(0);

	// 보스의 눈동자 이미지
	GameObject* bossEye = CreateGameObject(bossHead, L"bossEye");
	bossEye->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-1, -19, 0));
	SpriteRenderer* eyeRenderer = bossEye->AddComponent<SpriteRenderer>();
	eyeRenderer->SetSpriteName(L"Boss_Eye_Blue");
	eyeRenderer->SetLayer(103);
	eyeRenderer->SetImageNumber(0);
	eyeRenderer->SetOpacity(0);

	// 보스 왼손 이미지
	GameObject* bossLeftHand = CreateGameObject(bossHead, L"bossLeftHand");
	bossLeftHand->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(0.9f, 0.9f, 0.9f));
	bossLeftHand->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(-300, 150, 0));
	SpriteRenderer* leftHandRenderer = bossLeftHand->AddComponent<SpriteRenderer>();
	leftHandRenderer->SetSpriteName(L"Boss_Hand_Blue");
	leftHandRenderer->SetLayer(104);
	leftHandRenderer->SetImageNumber(0);
	leftHandRenderer->SetOpacity(0);

	// 보스 오른손 이미지
	GameObject* bossRightHand = CreateGameObject(bossHead, L"bossRightHand");
	bossRightHand->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(-0.9f, 0.9f, 0.9f));
	bossRightHand->GetTransform()->SetLocalPosition(BJEngine::Math::Vector3D(300, 150, 0));
	SpriteRenderer* rightHandRenderer = bossRightHand->AddComponent<SpriteRenderer>();
	rightHandRenderer->SetSpriteName(L"Boss_Hand_Blue");
	rightHandRenderer->SetLayer(104);
	rightHandRenderer->SetImageNumber(0);
	rightHandRenderer->SetOpacity(0);

	// 타일 설치
	Level level(STAGE_BOSS);
	level.Tiling(level);
	level.CreatePlatformFromLevelData(level);

	GameObject* controller = CreateGameObject(L"controller");
	NewBossCutSceneController* logic = controller->AddComponent<NewBossCutSceneController>();
	logic->SetEye(eyeRenderer);
	logic->SetHead(bossHeadRenderer);
	logic->SetLeft(leftHandRenderer);
	logic->SetRight(rightHandRenderer);
	logic->SetMask(blackMaskImage);
}
