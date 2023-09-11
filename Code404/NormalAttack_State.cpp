#include "NormalAttack_State.h"

#include "AI.h"

#include "BJMath.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"

#include "TimeController.h"

#include "Animator.h"

#include "ObjectPoolSystem.h"
#include "Bullet.h"
#include "EffectPlayer.h"
#include "SoundManager.h"

using namespace BJEngine;
using namespace BJEngine::Math;

NormalAttack_State::NormalAttack_State(AI* ai, BJEngine::GameObject* targetobject, float attackdelay)
	: State(ai)
	, attackDelay(attackdelay)
	, target(targetobject)
{

}

NormalAttack_State::~NormalAttack_State()
{

}

bool NormalAttack_State::GetIsAttack()
{
	return isAttack;
}

void NormalAttack_State::SetIsAttack(bool isattack)
{
	isAttack = isattack;
}

void NormalAttack_State::SetAttackCollider(BJEngine::GameObject* attackcollider)
{
	attackCollider = attackcollider;
}

void NormalAttack_State::Update()
{
	State::Update();

	// 타겟의 위치에 따른 scale.x값 바꾸기
	Vector3D targetPos = target->GetTransform()->GetLocalPosition();
	GameObject* owner = GetAI()->GetGameObject();
	Vector3D ownerPos = owner->GetTransform()->GetLocalPosition();

	// 왼쪽을 공격 해야함
	if (targetPos.x < ownerPos.x)
	{
		// 왼쪽을 공격해야 하는데 scale.x 값이 음수면 양수로
		if (owner->GetTransform()->GetLocalScale().x < 0)
		{
			owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
		}
		attackDirection = Attack_Direction::LEFT;
	}
	else
	{
		// 오른쪽을 공격해야하는데 scale.x 값이 양수면 음수로
		if (owner->GetTransform()->GetLocalScale().x > 0)
		{
			owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
		}
		attackDirection = Attack_Direction::RIGHT;
	}

	CheckAttackDirection();
	AttackColliderEnAble();

	if (totalAttackDelay >= attackDelay)
	{
		// 공격 시간이니 공격
		Attack();

		isShot = false;
		totalAttackDelay = 0.f;
		totalAttackFrameTime = 0.f;
	}
	else
	{
		PlayStandAnimation();
	}
}

void NormalAttack_State::Enter()
{
	isAttack = true;
}

void NormalAttack_State::Exit()
{
	isAttack = false;
}

void NormalAttack_State::AlwaysUpdate()
{
	totalAttackDelay += GetAI()->GetGameObject()->GetTimeController()->GetDeltaTime();
	totalAttackFrameTime += GetAI()->GetGameObject()->GetTimeController()->GetDeltaTime();
}

bool NormalAttack_State::IsParryingTime()
{
	return GetAnimator()->GetFrameNumber() < 2 && (GetAnimator()->GetAnimationName() == L"MeleeA_Enemy_Attack" || GetAnimator()->GetAnimationName() == L"MeleeB_Enemy_Attack");
}

void NormalAttack_State::Attack()
{
	ENEMY_TYPE type = GetAI()->GetEnemyType();
	
	switch (type)
	{
		case ENEMY_TYPE::MeleeA_Enemy:
		{
			GetAnimator()->Play(L"MeleeA_Enemy_Attack");
		}
		break;
		case ENEMY_TYPE::MeleeB_Enemy:
		{
			GetAnimator()->Play(L"MeleeB_Enemy_Attack");
		}
		break;
		case ENEMY_TYPE::RangeA_Enemy:
		{
			GetAnimator()->Play(L"RangeA_Enemy_Attack");
			SoundManager::GetInstance().Play(SOUND_EFFECT_GUNLASER);
		}
		break;
		case ENEMY_TYPE::RangeB_Enemy:
		{
			GetAnimator()->Play(L"RangeB_Enemy_Attack");
			SoundManager::GetInstance().Play(SOUND_EFFECT_GUNLASER);
		}
		break;
	}
}

void NormalAttack_State::AttackColliderEnAble()
{
	ENEMY_TYPE type = GetAI()->GetEnemyType();

	switch (type)
	{
		case ENEMY_TYPE::MeleeA_Enemy:
		{
			if (GetAnimator()->GetFrameNumber() == 2 && GetAnimator()->GetAnimationName() == L"MeleeA_Enemy_Attack")
			{
				if (attackDirection == Attack_Direction::RIGHT)
				{
					Vector3D pos = attackCollider->GetTransform()->GetLocalPosition();
					
					attackCollider->GetTransform()->SetLocalPosition(Vector3D(pos));
				}
				attackCollider->SetActive(true);
			}
		}
		break;
		case ENEMY_TYPE::MeleeB_Enemy:
		{
			if (GetAnimator()->GetFrameNumber() == 2 && GetAnimator()->GetAnimationName() == L"MeleeB_Enemy_Attack")
			{
				attackCollider->SetActive(true);
			}
		}
		break;
		case ENEMY_TYPE::RangeA_Enemy:
		{
			if (!isShot)
			{
				if (GetAnimator()->GetAnimationName() == L"RangeA_Enemy_Attack" && GetAnimator()->GetFrameNumber() == 3)
				{
					Vector3D ownerPos = GetAI()->GetGameObject()->GetTransform()->GetLocalPosition();
					GameObject* bulletObject = ObjectPoolSystem::GetInstance().PopFromPool(L"EnemyA_Bullet");
					GameObject* bulletEffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
					if (attackDirection == Attack_Direction::LEFT)
					{
						bulletEffect->GetTransform()->SetLocalScale(Vector3D(-2.f, 2.f, 2.f));
						bulletEffect->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x - 120, ownerPos.y, ownerPos.z));
						bulletObject->GetTransform()->SetLocalScale(Vector3D(-2.f, 2.f, 2.f));
						bulletObject->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x - 100, ownerPos.y, ownerPos.z));
					}
					else
					{
						bulletEffect->GetTransform()->SetLocalScale(Vector3D(2.f, 2.f, 2.f));
						bulletEffect->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x + 120, ownerPos.y, ownerPos.z));
						bulletObject->GetTransform()->SetLocalScale(Vector3D(2.f, 2.f, 2.f));
						bulletObject->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x + 100, ownerPos.y, ownerPos.z));
					}
					Vector3D dir = target->GetTransform()->GetLocalPosition() - ownerPos;
					dir.y = 0;
					bulletObject->GetComponent<Bullet>()->SetDirection(dir);
					bulletObject->SetActive(true);
					bulletEffect->GetComponent<EffectPlayer>()->Reset(L"Bullet_Effect");
					bulletEffect->SetActive(true);
					isShot = true;
				}
			}
		}
		break;
		case ENEMY_TYPE::RangeB_Enemy:
		{
			if (!isShot)
			{
				if (GetAnimator()->GetAnimationName() == L"RangeB_Enemy_Attack" && GetAnimator()->GetFrameNumber() == 3)
				{
					Vector3D ownerPos = GetAI()->GetGameObject()->GetTransform()->GetLocalPosition();
					GameObject* bulletObject = ObjectPoolSystem::GetInstance().PopFromPool(L"EnemyB_Bullet");
					GameObject* bulletEffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
					if (attackDirection == Attack_Direction::LEFT)
					{
						bulletEffect->GetTransform()->SetLocalScale(Vector3D(-2.f, 2.f, 2.f));
						bulletEffect->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x - 120, ownerPos.y, ownerPos.z));
						bulletObject->GetTransform()->SetLocalScale(Vector3D(-2.f, 2.f, 2.f));
						bulletObject->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x - 100, ownerPos.y, ownerPos.z));
					}
					else
					{
						bulletEffect->GetTransform()->SetLocalScale(Vector3D(2.f, 2.f, 2.f));
						bulletEffect->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x + 120, ownerPos.y, ownerPos.z));
						bulletObject->GetTransform()->SetLocalScale(Vector3D(2.f, 2.f, 2.f));
						bulletObject->GetTransform()->SetLocalPosition(Vector3D(ownerPos.x + 100, ownerPos.y, ownerPos.z));
					}
					Vector3D dir = target->GetTransform()->GetLocalPosition() - ownerPos;
					dir.y = 0;
					bulletObject->GetComponent<Bullet>()->SetDirection(dir);
					bulletObject->SetActive(true);
					bulletEffect->GetComponent<EffectPlayer>()->Reset(L"Bullet_Effect");
					bulletEffect->SetActive(true);
					isShot = true;
				}
			}
		}
		break;
	}
}

void NormalAttack_State::CheckAttackDirection()
{
	Vector3D targetPos = target->GetTransform()->GetLocalPosition();
	Vector3D ownerPos = GetAI()->GetGameObject()->GetTransform()->GetLocalPosition();
	if (targetPos.x < ownerPos.x)
	{
		attackDirection = Attack_Direction::LEFT;
	}
	else
	{
		attackDirection = Attack_Direction::RIGHT;
	}
}

void NormalAttack_State::PlayStandAnimation()
{
	ENEMY_TYPE type = GetAI()->GetEnemyType();

	switch (type)
	{
		case ENEMY_TYPE::MeleeA_Enemy:
		{
			if (totalAttackFrameTime > meleeAttackFrameTime)
			{
				if (GetAnimator()->GetAnimationName() != L"MeleeA_Enemy_Stand")
				{
					GetAnimator()->Play(L"MeleeA_Enemy_Stand");
				}
			}
		}
		break;
		case ENEMY_TYPE::MeleeB_Enemy:
		{
			if (totalAttackFrameTime > meleeAttackFrameTime)
			{
				if (GetAnimator()->GetAnimationName() != L"MeleeB_Enemy_Stand")
				{
					GetAnimator()->Play(L"MeleeB_Enemy_Stand");
				}
			}
		}
		break;
		case ENEMY_TYPE::RangeA_Enemy:
		{
			if (totalAttackFrameTime > rangeAttackFrameTime)
			{
				if (GetAnimator()->GetAnimationName() != L"RangeA_Enemy_Stand")
				{
					GetAnimator()->Play(L"RangeA_Enemy_Stand");
				}
			}
		}
		break;
		case ENEMY_TYPE::RangeB_Enemy:
		{
			if (totalAttackFrameTime > rangeAttackFrameTime)
			{
				if (GetAnimator()->GetAnimationName() != L"RangeB_Enemy_Stand")
				{
					GetAnimator()->Play(L"RangeB_Enemy_Stand");
				}
			}
		}
		break;
	}
}
