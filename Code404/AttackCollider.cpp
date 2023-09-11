#include "AttackCollider.h"

#include "GameObject.h"
#include "Collider.h"

#include "BJMath.h"

#include "Player.h"

using namespace BJEngine;
using namespace BJEngine::Math;


void AttackCollider::SetDamage(float damage)
{
	this->damage = damage;
}

void AttackCollider::Update()
{
	if (GetGameObject()->GetActive())
	{
		float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
		totalTime += deltaTime;

		if (totalTime >= 0.1f)
		{
			GetGameObject()->SetActive(false);
			totalTime = 0.f;
		}
	}
}

void AttackCollider::OnTriggerEnter(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"Player")
	{
		int a = 1;
		//other->GetGameObject()->GetComponent<Player>()->Attacked();
	}
}
