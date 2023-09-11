#include "Code404.h"
#include "BossNormalBullet.h"

#include "Collider.h"
#include "GameObject.h"

#include "ObjectPoolSystem.h"
#include "Player.h"

void BossNormalBullet::SetBulletDamage(float damage)
{
	this->damage = damage;
}

void BossNormalBullet::Update()
{
	totalReleaseTime += GetGameObject()->GetTimeController()->GetDeltaTime();
	if (totalReleaseTime >= releaseTime)
	{
		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}
}

void BossNormalBullet::OnTriggerEnter(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName()==L"Platform")
	{
		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}

	if (other->GetGameObject()->GetLayerName() == L"Player" || other->GetGameObject()->GetLayerName() == L"StealthPlayer")
	{
		other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::BOSSBULLET, damage);
		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}
}

void BossNormalBullet::OnDisable()
{
	totalReleaseTime = 0.f;
}
