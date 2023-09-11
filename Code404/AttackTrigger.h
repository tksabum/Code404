#pragma once

#include "Component.h"

#include "Collider.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Vector3D.h"

#include "Code404.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class AttackTrigger : public Component
{
public:
	enum class User
	{
		PLAYER,
		ENEMY
	};
	
	void OnEnable();
	void OnDisable();

	void OnTriggerEnter(Collider* collider);

	void Update();

	void SetUser(User attacker);
	void SetPlayerAttack(PLAYERATTACK_TYPE attacktype, float damage);
	void SetEnemyAttack(ENEMYATTACK_TYPE attacktype, float damage);

private:
	User user;
	PLAYERATTACK_TYPE playerAttackType;
	ENEMYATTACK_TYPE enemyAttackType;

	int hitCount;

	float attackDamage;

	float life;

	const static float ENEMY_TRIGGER_LIFE;
};

