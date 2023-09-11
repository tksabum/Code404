#include "Bullet.h"

#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

#include  "TimeController.h"

#include "ObjectPoolSystem.h"

#include "Player.h"
#include "Enemy.h"
#include "EffectPlayer.h"
#include "ParryingSystem.h"

void Bullet::SetDamage(float damage)
{
	this->damage = damage;
}

void Bullet::SetSpeed(float speed)
{
	moveSpeed = speed;
}


void Bullet::SetDirection(Vector2D direction)
{
	this->direction = direction;
}

void Bullet::Parrying()
{
	if (isParrying == false)
	{
		isParrying = true;
		direction = -direction;
		totalAbleTime = 0.0f;
		Vector3D orgscale = GetGameObject()->GetTransform()->GetLocalScale();
		GetGameObject()->GetTransform()->SetLocalScale(Vector3D(-orgscale.x, orgscale.y, orgscale.z));

		// ÆÐ¸µ ÀÌÆåÆ®
		GameObject* parryingeffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
		parryingeffect->SetActive(true);

		EffectPlayer* effectanimator = parryingeffect->GetComponent<EffectPlayer>();
		effectanimator->Reset(L"Animation_Parrying_Effect");
		parryingeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(0.0f, 0.0f));
		parryingeffect->GetTransform()->SetLocalScale(Vector3D(2.0f, 2.0f, 1.0f));

		parryingSystem->Parrying();
	}
}

void Bullet::Awake()
{
	GameObject* parryingsystemobject = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetGameObject(L"ParringSystem");
	if (parryingsystemobject != nullptr)
	{
		parryingSystem = parryingsystemobject->GetComponent<ParryingSystem>();
	}
}

void Bullet::OnEnable()
{
	isParrying = false;
}

void Bullet::Update()
{
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();

	totalAbleTime += deltaTime;
	if (totalAbleTime >= ableTime)
	{
		totalAbleTime = 0.f;
		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}

	Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	pos += direction.GetUnitVector()* moveSpeed * GetGameObject()->GetTimeController()->GetDeltaTime();
	GetGameObject()->GetTransform()->SetLocalPosition(pos);
}

void Bullet::OnTriggerEnter(Collider* other)
{
	if (isParrying == false && other->GetGameObject()->GetLayerName() == L"Player")
	{
		other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::RANGE_A, damage);
		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}

	if (isParrying == true && other->GetGameObject()->GetLayerName() == L"Enemy")
	{
		Enemy* enemy = other->GetGameObject()->GetComponent<Enemy>();
		enemy->Attacked(PLAYERATTACK_TYPE::PARRYING_BULLET, damage);
		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}

	if (other->GetGameObject()->GetLayerName() == L"Platform")
	{
		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}
}

void Bullet::OnDisable()
{
	totalAbleTime = 0.f;
	isParrying = false;
}
