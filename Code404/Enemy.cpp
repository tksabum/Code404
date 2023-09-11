
#include "Enemy.h"

#include "Input.h"

#include "GameObject.h"
#include "Animator.h"
#include "AI.h"

#include "Boss.h"
#include "SoundManager.h"
#include "Transform.h"

using namespace BJEngine;

void Enemy::SetHPBar(BJEngine::GameObject* hpbar)
{
	hpBarParent = hpbar;
}

void Enemy::SetMaxHP(float maxhp)
{
	maxHP = maxhp;
}

void Enemy::SetCurHP(float curhp)
{
	curHP = curhp;
}

void Enemy::Attacked(PLAYERATTACK_TYPE type, float damage)
{
	if (!hpBar)
	{
		// 보스 피격
		SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSHIT);
	}
	else
	{
		SoundManager::GetInstance().Play(SOUND_EFFECT_DAMAGED);
	}
	if (hpBarParent != nullptr)
	{
		if (!hpBarParent->GetActive())
		{
			hpBarParent->SetActive(true);
		}
	}

	if (isBossHead)
	{
		if (bossHeadComponent->GetPhaseState() == PHASE_STATE::ONE)
		{
			return;
		}
	}
	//if (!isAttacked)
	//{
		switch (type)
		{
			case PLAYERATTACK_TYPE::COMBO1:
			{

			}
			break;
			case PLAYERATTACK_TYPE::COMBO2:
			{

			}
			break;
			case PLAYERATTACK_TYPE::COMBO3:
			{

			}
			break;
			case PLAYERATTACK_TYPE::DAGGER:
			{

			}
			break;
		}

		isAttacked = true;
		curHP -= damage;

		if (hpBarParent != nullptr)
		{
			if (curHP <= 0.f)
			{
				hpBarParent->SetActive(false);
				SoundManager::GetInstance().Play(SOUND_EFFECT_ENEMYDIE);
			}
		}
	//}
}

float Enemy::GetCurHP()
{
	return curHP;
}

float Enemy::GetMaxHP()
{
	return maxHP;
}

bool Enemy::IsAttacked()
{
	return isAttacked;
}

void Enemy::SetIsAttacked(bool isattacked)
{
	isAttacked = isattacked;
}

float* Enemy::GetCurHPAdd()
{
	return &curHP;
}

float* Enemy::GetMaxHPAdd()
{
	return &maxHP;
}

void Enemy::Awake()
{
	curHP = maxHP;
	anim = GetGameObject()->GetComponent<Animator>();
	ai = GetGameObject()->GetComponent<AI>();
	bossHeadComponent = GetGameObject()->GetComponent<Boss>();
	if (bossHeadComponent)
	{
		isBossHead = true;
	}
	if (hpBarParent != nullptr)
	{
		hpBar = hpBarParent->GetChild(0);
	}
}

void Enemy::Update()
{
	if (hpBarParent != nullptr)
	{
		BJEngine::Math::Vector3D hpBarPos = GetGameObject()->GetTransform()->Get2DScreenPosition();
		BJEngine::Math::Vector3D hpBarScale = hpBar->GetTransform()->GetLocalScale();
		hpBarPos.y -= 115;
		hpBarPos.x -= 35;
		hpBarParent->GetTransform()->SetLocalPosition(hpBarPos);

		float ratio = curHP / maxHP;
		if (ratio <= 0.f)
		{
			ratio = 0.f;
		}
		hpBar->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(ratio, hpBarScale.y, hpBarScale.z));
	}
}

void Enemy::Test()
{
	curHP = 0;
}
