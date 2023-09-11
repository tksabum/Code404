#include "Code404.h"
#include "laser.h"

#include "Collider.h"
#include "GameObject.h"

#include "Player.h"

void laser::SetLaserDamage(float damage)
{
	this->damage = damage;
}

void laser::OnTriggerEnter(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"Player" || other->GetGameObject()->GetLayerName() == L"StealthPlayer")
	{
		other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::BOSSCROSSLASER, damage);
	}
}

void laser::OnTriggerStay(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"Player" || other->GetGameObject()->GetLayerName() == L"StealthPlayer")
	{
		other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::BOSSCROSSLASER, damage);
	}
}

