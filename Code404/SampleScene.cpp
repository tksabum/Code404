 #include "SampleScene.h"

#include "SceneController.h"
#include "ObjectPoolSystem.h"
#include "ObjectPoolTest.h"
#include "AnimationTest.h"
#include "Controller.h"
#include "Tracer.h"
#include "Player.h"
#include "UITest.h"
#include "JumpTrigger.h"
#include "Dagger.h"
#include "Text.h"
#include "DebugInfo.h"

#include "EffectPlayer.h"
#include "GameObjectFactory.h"
#include "ParryingSystem.h"

using namespace BJEngine;
using namespace BJEngine::Math;
using namespace BJEngine::GraphicsEngine;

SampleScene::SampleScene() : Scene(L"SampleScene")
{

}

void SampleScene::Start()
{
	GameObject* parringsystemobject = GameObjectFactory::GetInstance().CreateParryingSystem();
	ParryingSystem* parringSystem = parringsystemobject->GetComponent<ParryingSystem>();

	GameObject* sceneController = CreateGameObject(L"SceneController");
	sceneController->AddComponent<SceneController>();

	GameObject* background = CreateGameObject(L"background");
	SpriteRenderer* backSprite = background->AddComponent<SpriteRenderer>();
	backSprite->SetSpriteName(L"back");
	backSprite->SetLayer(0);
	backSprite->SetImageNumber(0);

	// 프리펩 생성
	GameObjectFactory::GetInstance().CreatePrefab();

	GameObject* playerobject = GameObjectFactory::GetInstance().CreatePlayer();

	GameObject* platform001 = GameObjectFactory::GetInstance().CreatePlatform(Vector2D(1200.0f, 40.0f));
	platform001->GetTransform()->SetLocalPosition(Vector2D(-400.0f, 200.0f));

	GameObject* platform002 = GameObjectFactory::GetInstance().CreatePlatform(Vector2D(1200.0f, 40.0f));
	platform002->GetTransform()->SetLocalPosition(Vector2D(800.0f, 0.0f));

	GameObject* platform003 = GameObjectFactory::GetInstance().CreatePlatform(Vector2D(200.0f, 40.0f));
	platform003->GetTransform()->SetLocalPosition(Vector2D(180.0f, 80.0f));
	platform003->GetTransform()->SetLocalRotation(Vector3D(0.0f, 0.0f, -90.0f));

	GameObject* platform004 = GameObjectFactory::GetInstance().CreatePlatform(Vector2D(1200.0f, 40.0f));
	platform004->GetTransform()->SetLocalPosition(Vector2D(-1600.0f, 0.0f));

	GameObject* platform005 = GameObjectFactory::GetInstance().CreatePlatform(Vector2D(200.0f, 40.0f));
	platform005->GetTransform()->SetLocalPosition(Vector2D(-980.0f, 80.0f));
	platform005->GetTransform()->SetLocalRotation(Vector3D(0.0f, 0.0f, -90.0f));

	GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(-200.0f, 0.0f));
	GameObjectFactory::GetInstance().CreateEnemyMeleeA(playerobject)->GetTransform()->SetLocalPosition(Vector2D(-200.0f, 0.0f));
	//GameObjectFactory::GetInstance().CreateEnemyMeleeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(200.0f, 0.0f));
	//GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(400.0f, 0.0f));
	//GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(600.0f, 0.0f));
	//GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(800.0f, 0.0f));
	//GameObjectFactory::GetInstance().CreateEnemyRangeB(playerobject)->GetTransform()->SetLocalPosition(Vector2D(1000.0f, 0.0f));

	// 아이템
	GameObjectFactory::GetInstance().CreateItem()->GetTransform()->SetLocalPosition(Vector2D(1200, 0));


	GetMainCamera()->GetGameObject()->AddComponent<Controller>();
}