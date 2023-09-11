#include "SampleScene2.h"

#include "SceneController.h"

#include "Rigidbody2D.h"
#include "ConvexpolygonCollider.h"
#include "SoftBody.h"
#include "CircleRenderer.h"

#include "DebugInfo.h"
#include "Text.h"
#include "Controller.h"
#include "ParryingSystem.h"
#include "EngineSetting.h"

#include "BlackMask.h"

using namespace BJEngine;
using namespace BJEngine::Math;

SampleScene2::SampleScene2() : Scene(L"SampleScene2")
{

}

void SampleScene2::Start()
{
	GameObject* sceneController = CreateGameObject(L"SceneController");
	sceneController->AddComponent<SceneController>();

	EngineSetting::GetInstance().SetShowDebugInfo(true);

	GameObject* preSceneChange = CreateGameObject(L"PreSceneChange");
	preSceneChange->GetTransform()->SetLocalPosition({ -190.f, 0.f });
	Image* blackmaskimage1 = preSceneChange->AddComponent<Image>();
	blackmaskimage1->SetSpriteName(L"SwitchScreen");
	blackmaskimage1->SetImageNumber(0);
	blackmaskimage1->SetLayer(0);
	BlackMask* inBlackMask = preSceneChange->AddComponent<BlackMask>();
	inBlackMask->SetMode(BlackMask::eMode::SCENE_IN);

	GameObject* debuginfoobject = CreateGameObject(L"DebugInfo");
	Text* text = debuginfoobject->AddComponent<Text>();
	text->SetTextAreaSize(Vector2D(500.0f, 500.0f));
	debuginfoobject->AddComponent<DebugInfo>();

	{
		GameObject* platform = CreateGameObject(L"platform");
		platform->SetLayerName(L"Platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(0, 300));
		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(-500, 50), Vector2D(-500, -50), Vector2D(500, -50), Vector2D(500, 50) });
		rigid->SetPositionXLock(true);
		rigid->SetPositionYLock(true);
		rigid->SetRotationLock(true);
		rigid->SetAffectedByGravity(false); 
		rigid->SetRestitutionValue(1.f);
	}

	{
		GameObject* platform = CreateGameObject(L"platform");
		platform->SetLayerName(L"Platform");
		platform->GetTransform()->SetLocalPosition(Vector2D(0, -300));
		Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
		ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
		collider->SetPolygon({ Vector2D(-500, 50), Vector2D(-500, -50), Vector2D(500, -50), Vector2D(500, 50) });
		rigid->SetPositionXLock(true);
		rigid->SetPositionYLock(true);
		rigid->SetRotationLock(true);
		rigid->SetAffectedByGravity(false);
		rigid->SetRestitutionValue(1.f);
	}

	GameObject* softbody = CreateGameObject(L"softbody");
	softbody->SetLayerName(L"Player");
	CircleRenderer* circlerenderer = softbody->AddComponent<CircleRenderer>();
	circlerenderer->SetCircle(200.0f);
	softbody->GetTransform()->SetLocalRotation(45);
	SoftBody* soft = softbody->AddComponent<SoftBody>();
	
	/// 꼭짓점 갯수 / 꼭짓점마다 연결될 스프링 갯수 / 반지름 / 스프링 계수 / 감쇠
	soft->SetSoftBall(30, 10, 100.f, 300.f, 10.f);

	/// 스프링 길이 / 열 / 행 / 스프링 계수 / 감쇠
	// soft->SetSoftBlock(30.f, 10, 7, 300.f, 10.f);
	
}