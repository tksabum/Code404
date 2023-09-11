#pragma once

#include "Code404.h"

#include "Vector2D.h"

namespace BJEngine
{
	class GameObject;
	class Scene;
}

using namespace BJEngine;
using namespace BJEngine::Math;

class Player;
class ParryingSystem;

class GameObjectFactory
{
public:
	static GameObjectFactory& GetInstance();

	void CreatePrefab();
	GameObject* CreatePlayer(float playerscale = 1.0f, bool usetracer = true,int layer = Code404::Layer::Player,Vector2D sceneLT = Vector2D(0.0f, 0.0f), Vector2D sceneRB = Vector2D(0.0f, 0.0f), bool usescenetrace = false);
	void CreateSettingUI(bool isingame = false);
	GameObject* CreatePlatform(Vector2D size);
	GameObject* CreatePlayerAttackTrigger(GameObject* parent, Vector2D position, Vector2D triggersize, PLAYERATTACK_TYPE attacktype, float damage);
	void CreateScreenCover();
	GameObject* CreateParryingSystem();
	GameObject* CreateItem();

#pragma region Àâ¸÷µé
	GameObject* CreateEnemyMeleeA(GameObject* target);
	GameObject* CreateEnemyMeleeB(GameObject* target);
	GameObject* CreateEnemyRangeA(GameObject* target);
	GameObject* CreateEnemyRangeB(GameObject* target);
#pragma endregion
private:
	Scene* GetCurrentScene();

	GameObjectFactory();
	~GameObjectFactory();
};
