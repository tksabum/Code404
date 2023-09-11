#pragma once

#include "Component.h"

#include <set>

#include "Scene.h"
#include "GameObject.h"
#include "ConvexpolygonCollider.h"
#include "Rigidbody2D.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "Animator.h"
#include "BJMath.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace std;

using namespace BJEngine;
using namespace BJEngine::Math;

class Skill;

class UniqueDagger : public Component
{
public:
	void Awake();

	void OnEnable();

	void SetUniqueDagger(Vector2D* userdirection, Transform* usertransform, bool* ready, bool* need, Skill* throwskill);
	void SetAnimation(wstring stuckanimation, wstring flyinganimation);

	void Update();
	void LateUpdate();

	void OnTriggerEnter(Collider* collider);
	void OnTriggerStay(Collider* collider);

	void Withdraw();

private:
	enum class State
	{
		READY,
		THROWING,
		HOLDING,
		WITHDRAWING
	};

	void ThrowAttack(Collider* collider);
	void WithdrawAttack(Collider* collider);
	void TriggerEvent(Collider* collider);

	void WithdrawingEvent();

	void PlayCollisionEffect();

	set<Collider*> attackedColliders;

	State state;
	Animator* animator;
	ConvexpolygonCollider* owncollider;
	Rigidbody2D* rigidbody;

	Vector2D* userDirection;
	Transform* userTransform;
	bool* isReady;
	bool* isNeed;
	Skill* throwSkill;

	bool isWithdrawing;

	wstring stuckAnimation;
	wstring flyingAnimation;

	const static float PROJECTILE_SPEED;
	const static float WITHDRAWING_SPEED;
	const static float MAX_DISTANCE_FROM_USER;
	const static float KNIFE_SIZE;
	const static float THROW_DAMAGE;
	const static float WITHDRAW_DAMAGE;
};

