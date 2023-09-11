#include "AttackTrigger.h"

#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"

#include "SoundManager.h"

const float AttackTrigger::ENEMY_TRIGGER_LIFE = 0.1f;

void AttackTrigger::OnEnable()
{
	life = ENEMY_TRIGGER_LIFE;
	hitCount = 0;
}

void AttackTrigger::OnDisable()
{
	if (user == User::PLAYER && hitCount == 0)
	{
		SoundManager::GetInstance().Play(SOUND_EFFECT_HITAIR);
	}
}

void AttackTrigger::OnTriggerEnter(Collider* collider)
{
	switch (user)
	{
	case User::PLAYER:
		if (collider->GetGameObject()->GetLayerName() == L"Enemy" || collider->GetGameObject()->GetLayerName() == L"Boss")
		{
			collider->GetGameObject()->GetComponent<Enemy>()->Attacked(playerAttackType, attackDamage);
			hitCount++;
		}
		else if (collider->GetGameObject()->GetLayerName() == L"Bullet")
		{
			collider->GetGameObject()->GetComponent<Bullet>()->Parrying();
			hitCount++;
		}
		break;

	case User::ENEMY:
		if (collider->GetGameObject()->GetLayerName() == L"Player")
		{
			collider->GetGameObject()->GetComponent<Player>()->Attacked(enemyAttackType, attackDamage);
		}
		break;

	default:
		break;
	}
}

void AttackTrigger::Update()
{
	life -= GetGameObject()->GetTimeController()->GetDeltaTime();

	if (user == User::ENEMY)
	{
		if (life <= 0.0f)
		{
			GetGameObject()->SetActive(false);
		}
	}
}

void AttackTrigger::SetUser(User attacker)
{
	user = attacker;
}

void AttackTrigger::SetPlayerAttack(PLAYERATTACK_TYPE attacktype, float damage)
{
	playerAttackType = attacktype;
	attackDamage = damage;
}

void AttackTrigger::SetEnemyAttack(ENEMYATTACK_TYPE attacktype, float damage)
{
	enemyAttackType = attacktype;
	attackDamage = damage;
}
