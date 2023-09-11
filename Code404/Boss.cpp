#include "Code404.h"
#include "Boss.h"

#include "GameObject.h"
#include "Transform.h"
#include "BossHand.h"

#include "Input.h"

#include <random>

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "Collider.h"
#include "Rigidbody2D.h"

#include "Camera.h"
#include "CameraShake.h"

#include "ObjectPoolSystem.h"

#include "Player.h"

#include "Enemy.h"

#include "EffectPlayer.h"

#include "Image.h"

#include "SoundManager.h"

using namespace std;
using namespace BJEngine;

void Boss::SetTarget(BJEngine::GameObject* target)
{
	this->target = target;
}

void Boss::SetIsPattern(bool ispattern)
{
	if (!ispattern)
	{
		bossState = BOSS_STATE::IDLE;
		totalpatternTime = 0.f;
	}
	isPattern = ispattern;
}

void Boss::SetIsRightHandSweep(bool isrighthandsweep)
{
	isRightHandSweep = isrighthandsweep;
}

void Boss::SetLeftFinish(bool leftfinish)
{
	isLeftFinish = leftfinish;
}

void Boss::SetRightFinish(bool rightfinish)
{
	isRightFinish = rightfinish;
}

PHASE_STATE Boss::GetPhaseState()
{
	return phaseState;
}

Enemy* Boss::GetEnemyComponent()
{
	return enemyComponent;
}

void Boss::SetIsCameraShake(bool iscamerashake)
{
	isCameraShake = iscamerashake;
}

bool Boss::GetIsCameraShake()
{
	return isCameraShake;
}

void Boss::Awake()
{
	portrait = GetGameObject()->GetScene()->GetGameObject(L"barParent")->GetComponent<Image>();
	enemyComponent = GetGameObject()->GetComponent<Enemy>();
	bossRigid = GetGameObject()->GetComponent<Rigidbody2D>();
	crossLaserParent = GetGameObject()->GetChild(L"laserEmpty");
	patrolLaser = GetGameObject()->GetScene()->GetGameObject(L"patrolLaser");
	timeLaserParent = GetGameObject()->GetScene()->GetGameObject(L"timeLaserParent");
	leftHand = GetGameObject()->GetChild(L"bossLeftHand");
	rightHand = GetGameObject()->GetChild(L"bossRightHand");
	leftHandComponent = leftHand->GetComponent<BossHand>();
	rightHandComponent = rightHand->GetComponent<BossHand>();
	bossEye = GetGameObject()->GetChild(L"bossEye");
	bossHPBar = GetGameObject()->GetScene()->GetGameObject(L"bossHpBar");
}

void Boss::Update()
{
	ChangeHPBarScale();

	if (enemyComponent->GetCurHP() / enemyComponent->GetMaxHP() <= 0.5f)
	{
		ChangePhase();
	}

	CheckDie();
	CheckState();

	if (!isPattern)
	{
		bossState = BOSS_STATE::IDLE;
	}

	if (bossState != BOSS_STATE::DIE)
	{
		if (phaseState == PHASE_STATE::ONE)
		{
			ShotBullet();
			PhaseOne();
		}
		else
		{
			CloseEye();

			if (leftHand->GetActive())
			{
				leftHand->SetActive(false);
			}
			if (rightHand->GetActive())
			{
				rightHand->SetActive(false);
			}
			PhaseTwo();
		}
	}
}

void Boss::OnTriggerEnter(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"Platform")
	{
		if (isTrace)
			return;

		BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetWorldPosition();
		pos.y += 100;
		GameObject* effect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
		effect->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(1.5f, 2.f, 1.f));
		effect->GetTransform()->SetLocalPosition(pos);
		effect->GetComponent<EffectPlayer>()->Reset(L"TakeDown_Effect");
		effect->SetActive(true);

		isGround = true;
		bossRigid->SetVelocity(BJEngine::Math::Vector2D(0, 0));
		CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetMainCamera()->GetGameObject()->GetComponent<CameraShake>()->Shake(0.5, 50, 50);
		// 카메라 쉐이크 효과음 넣기
		SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSCAMERASHAKE);
		SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSHEADATTACK);
	}

	if (other->GetGameObject()->GetLayerName() == L"Player" || other->GetGameObject()->GetLayerName() == L"StealthPlayer")
	{
		if (!isGround && phaseState != PHASE_STATE::ONE)
		{
			if (isTakeDown)
			{
				other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::BOSSHEAD, damage);
			}
		}
	}
}

void Boss::PhaseOne()
{
	totalpatternTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();

	if (totalpatternTime >= patternTime && !isPattern && isLeftFinish && isRightFinish)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> range(1, 2);
		int random = range(mt);
		//random = 2;
		if (random == 1)
		{
			bossState = BOSS_STATE::HANDTAKEDOWN;
		}
		else if (random == 2)
		{
			bossState = BOSS_STATE::HANDSWEEP;
		}
	}
}

void Boss::PhaseTwo()
{
	TraceTarget();

	if ((calCoolTime > takeDownCoolTime))
	{
		bossState = BOSS_STATE::HEAD_TAKEDOWN;
	}
}

void Boss::CheckDie()
{
	if (!isDie)
	{
		if (enemyComponent->GetCurHP() <= 0.f)
		{
			if (patrolLaser->GetActive())
			{
				patrolLaser->SetActive(false);
			}
			if (timeLaserParent->GetActive())
			{
				timeLaserParent->SetActive(false);
			}
			//SoundManager::GetInstance().Play();
			bossState = BOSS_STATE::DIE;
		}
	}
}

void Boss::ChangePhase()
{
	if (phaseState == PHASE_STATE::ONE)
	{
		if (!leftHand->GetActive() && !rightHand->GetActive())
		{
			totalChangeTime += GetGameObject()->GetTimeController()->GetDeltaTime();
			if (totalChangeTime >= phaseChangeTime)
			{
				GetGameObject()->GetComponent<SpriteRenderer>()->SetSpriteName(L"Boss_Head_Red");
				bossEye->GetComponent<SpriteRenderer>()->SetSpriteName(L"Boss_Eye_Red");
				crossLaserParent->SetActive(true);
				patrolLaser->SetActive(true);
				timeLaserParent->SetActive(true);
				portrait->SetSpriteName(L"Boss_Protrait_Red");
				phaseState = PHASE_STATE::TWO;
			}
		}
	}
}

void Boss::ChangeHPBarScale()
{
	float ratio = (enemyComponent->GetCurHP() + leftHandComponent->GetEnemyComponent()->GetCurHP() + rightHandComponent->GetEnemyComponent()->GetCurHP()) / enemyComponent->GetMaxHP();

	BJEngine::Math::Vector3D scale = bossHPBar->GetTransform()->GetLocalScale();
	scale.x = ratio;
	if (scale.x <= 0.f)
	{
		scale.x = 0.f;
	}
	bossHPBar->GetTransform()->SetLocalScale(scale);
}

void Boss::ShotBullet()
{
	if (enemyComponent->GetCurHP() > 50.f)
	{
		totalShootTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
		if (totalShootTime >= shootTime)
		{
			for (int i = 0; i < 8; i++)
			{
				BJEngine::Math::Vector2D dir = BJEngine::Math::Vector2D(cosf(BJEngine::Math::DegreeToRadian(45) * i), sinf(BJEngine::Math::DegreeToRadian(45) * i));
				dir = dir.GetUnitVector();
				GameObject* obj = ObjectPoolSystem::GetInstance().PopFromPool(L"BossNormalBullet");
				obj->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
				obj->SetActive(true);
				obj->GetComponent<Rigidbody2D>()->SetVelocity(dir * 200);
			}
			BJEngine::Math::Vector2D traceDir = target->GetTransform()->GetLocalPosition() - GetGameObject()->GetTransform()->GetLocalPosition();
			traceDir = traceDir.GetUnitVector();
			GameObject* obj = ObjectPoolSystem::GetInstance().PopFromPool(L"BossTraceBullet");
			obj->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
			obj->SetActive(true);
			obj->GetComponent<Rigidbody2D>()->SetVelocity(traceDir * 200);
			shootTime = 8;
			totalShootTime = 0.f;
		}
	}
}

void Boss::HandTakeDown()
{
	if (!isPattern)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> range(1, 2);
		int random = range(mt);

		if (random == 1)
		{
			if (leftHand->GetActive())
			{
				isLeftFinish = false;
				leftHandComponent->SetIsTakeDown(true);
				leftHandComponent->TakeDown();
			}
			else
			{
				isRightFinish = false;
				rightHandComponent->SetIsTakeDown(true);
				rightHandComponent->TakeDown();
			}
		}
		else
		{
			if (rightHand->GetActive())
			{
				isRightFinish = false;
				rightHandComponent->SetIsTakeDown(true);
				rightHandComponent->TakeDown();
			}
			else
			{
				isLeftFinish = false;
				leftHandComponent->SetIsTakeDown(true);
				leftHandComponent->TakeDown();
			}
		}
		isPattern = true;
	}
}

void Boss::RandomRocket()
{
	if (!isPattern)
	{
		//bossState = BOSS_STATE::HANDSWEEP;
		isPattern = true;
		isLeftFinish = false;
		isRightFinish = false;
		rightHandComponent->SetIsSweep(true);
		leftHandComponent->SetIsSweep(true);
		//leftHandComponent->SweepHand(0);
	}
	//if (leftHand->GetActive())
	//{
	//	if (!isPattern)
	//	{
	//		//bossState = BOSS_STATE::HANDSWEEP;
	//		isPattern = true;
	//		rightHandComponent->SetIsSweep(true);
	//		leftHandComponent->SetIsSweep(true);
	//		//leftHandComponent->SweepHand(0);
	//	}
	//	//if (isPattern && isRightHandSweep && rightHand->GetActive())
	//	//{
	//	//	isRightHandSweep = false;
	//	//	rightHandComponent->SetIsSweep(true);
	//	//	//rightHandComponent->SweepHand(0);
	//	//}
	//}
	//else
	//{
	//	if (!isPattern)
	//	{
	//		bossState = BOSS_STATE::HANDSWEEP;
	//		isPattern = true;
	//		rightHandComponent->SetIsSweep(true);
	//		//rightHandComponent->SweepHand(0);
	//	}
	//}

}

void Boss::HandSweep()
{
	//if (leftHand->GetActive())
	//{
	//	if (!isPattern)
	//	{
	//		bossState = BOSS_STATE::HANDSWEEP;
	//		isPattern = true;
	//		leftHandComponent->SetIsSweep(true);
	//		leftHandComponent->SweepHand(0);
	//	}
	//	if (isPattern && isRightHandSweep && rightHand->GetActive())
	//	{
	//		isRightHandSweep = false;
	//		rightHandComponent->SetIsSweep(true);
	//		rightHandComponent->SweepHand(0);
	//	}
	//}
	//else
	//{
	//	if (!isPattern)
	//	{
	//		bossState = BOSS_STATE::HANDSWEEP;
	//		isPattern = true;
	//		rightHandComponent->SetIsSweep(true);
	//		rightHandComponent->SweepHand(0);
	//	}
	//}
}

void Boss::HeadTakeDown()
{
	isTrace = false;
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();

	isTakeDown = true;

	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();

	calSleepTime += deltaTime;

	if (!isCalPrevPos && !isGround)
	{
		crossLaserParent->SetActive(false);
		prevPos = pos;
		isCalPrevPos = true;
	}

	// 내려찍기 전 멈춰있는 시간 계산 후 내려찍기
	if (calSleepTime > sleepTime && !isGround)
	{
		bossRigid->SetVelocity(BJEngine::Math::Vector2D(0, 1000));
	}

	// 내려찍은 후
	if (isGround)
	{
		totalGroundWaitTime += deltaTime;
		if (totalGroundWaitTime >= groundWaitTime)
		{
			if (abs(pos.y - prevPos.y) <= 10.f)
			{
				// 내려찍기 끝남 최기화 필요한 것들 초기화
				crossLaserParent->SetActive(true);
				calCoolTime = 0.f;
				calSleepTime = 0.f;
				pos.y = prevPos.y;
				isGround = false;
				isTakeDown = false;
				isCalPrevPos = false;
				SetIsPattern(false);
				totalGroundWaitTime = 0.f;
			}
			else
			{
				pos.y += -300 * BJEngine::CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
			}
			GetGameObject()->GetTransform()->SetLocalPosition(pos);
		}
	}
}

void Boss::TraceTarget()
{
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();

	crossLaserParent->GetTransform()->SetLocalRotation(crossLaserParent->GetTransform()->GetLocalRotation().z + (rotateSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime()));

	if (!isTakeDown)
	{
		isTrace = true;
		BJEngine::Math::Vector3D dir = target->GetTransform()->GetLocalPosition() - GetGameObject()->GetTransform()->GetLocalPosition();
		dir = dir.GetUnitVector();
		dir.y = 0;
		pos += dir * traceSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
		GetGameObject()->GetTransform()->SetLocalPosition(pos);
		calCoolTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	}
}

void Boss::CloseEye()
{
	if (crossLaserParent->GetActive())
	{
		bossEye->SetActive(false);
	}
	else
	{
		bossEye->SetActive(true);
	}
}

void Boss::CheckState()
{
	switch (bossState)
	{
		case BOSS_STATE::IDLE:
		{

		}
		break;
		case BOSS_STATE::HANDTAKEDOWN:
		{
			HandTakeDown();
		}
		break;
		case BOSS_STATE::HANDSWEEP:
		{
			RandomRocket();
			// HandSweep();
		}
		break;
		case BOSS_STATE::HEAD_TAKEDOWN:
		{
			HeadTakeDown();
		}
		break;
		case BOSS_STATE::DIE:
		{
			crossLaserParent->SetActive(false);
			patrolLaser->SetActive(false);
			timeLaserParent->SetActive(false);
			if (!isDieEffect)
			{
				isDieEffect = true;
				BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetWorldPosition();
				pos.y += 20;
				GameObject* effect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
				effect->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(1.f, 1.f, 1.f));
				effect->GetTransform()->SetLocalPosition(pos);
				effect->GetComponent<EffectPlayer>()->Reset(L"Boss_Die_Effect");
				effect->SetActive(true);
			}
			if (!isCameraShake)
			{
				SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSCAMERASHAKE);
				isCameraShake = true;
				CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetMainCamera()->GetGameObject()->GetComponent<CameraShake>()
					->Shake(10.f, 25, 25);
			}
			MakeDieEffect();
		}
		break;
	}
}

void Boss::MakeDieEffect()
{
	BJEngine::Math::Vector3D pos = BJEngine::Math::Vector3D(530, 380, 0);
	float xMin = pos.x - 500;
	float xMax = pos.x + 500;
	float yMin = pos.y - 500;
	float yMax = pos.y + 500;

	totalEffectTime += GetGameObject()->GetTimeController()->GetDeltaTime();

	if (curEffectCount == 0)
	{
		totalEffectTime = nextEffectTime;
	}

	if (curEffectCount < maxEffectCount)
	{
		if (totalEffectTime >= nextEffectTime)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_real_distribution<float> xRange(xMin, xMax);
			float xRandom = xRange(mt);

			uniform_real_distribution<float> yRange(yMin, yMax);
			float yRandom = yRange(mt);

			BJEngine::Math::Vector3D randomPos;
			randomPos.x = xRandom;
			randomPos.y = yRandom;

			SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSEXPLOSION);

			GameObject* effect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
			effect->GetTransform()->SetLocalPosition(randomPos);
			effect->SetActive(true);
			effect->GetComponent<EffectPlayer>()->Reset(L"Enemy_Die_Effect");
			totalEffectTime = 0.f;
			curEffectCount++;
		}
	}
	else
	{
		GetGameObject()->SetActive(false);
	}
}
