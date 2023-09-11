#include <vector>
#include "Code404.h"
#include "BossHand.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "GameObject.h"
#include "Transform.h"
#include "BJMath.h"
#include "Collider.h"

#include "Rigidbody2D.h"
#include "Vector3D.h"

#include "Boss.h"

#include "Camera.h"
#include "CameraShake.h"

#include "Player.h"

#include "Enemy.h"

#include "ObjectPoolSystem.h"

#include "EffectPlayer.h"

#include "SoundManager.h"

#include <random>

using namespace std;
using namespace BJEngine;
using namespace BJEngine::Math;

void BossHand::Awake()
{
	bossComponent = GetGameObject()->GetParent()->GetComponent<Boss>();
	enemyComponent = GetGameObject()->GetComponent<Enemy>();
	rigid = GetGameObject()->GetComponent<Rigidbody2D>();
	originPosition = GetGameObject()->GetTransform()->GetLocalPosition();
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	if (handType == HAND_TYPE::LEFT)
	{
		vecPatrol.push_back(BJEngine::Math::Vector3D(-1400, pos.y, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(-1400, 550, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(1400, 550, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(1400, 800, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(pos.x, 800, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(pos.x, pos.y, 0));
	}
	else
	{
		vecPatrol.push_back(BJEngine::Math::Vector3D(1400, pos.y, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(1400, 550, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(-1400, 550, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(-1400, 800, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(pos.x, 800, 0));
		vecPatrol.push_back(BJEngine::Math::Vector3D(pos.x, pos.y, 0));
	}
	maxPatrolIndex = vecPatrol.size();

	bossComponent->GetEnemyComponent()->SetMaxHP(bossComponent->GetEnemyComponent()->GetMaxHP() + enemyComponent->GetMaxHP());
	//bossComponent->GetEnemyComponent()->SetCurHP(bossComponent->GetEnemyComponent()->GetCurHP() + enemyComponent->GetMaxHP());

	//SetTakeDownPoint();
}

int BossHand::GetCallCount()
{
	return callCount;
}

void BossHand::SetHandType(HAND_TYPE handtype)
{
	handType = handtype;
}

void BossHand::SetIsTakeDown(bool istakedown)
{
	isTakeDown = istakedown;
	if (istakedown)
	{
		isHorizontalMove = true;
	}
}

void BossHand::SetIsSweep(bool issweephand)
{
	isSweepHand = issweephand;
}

void BossHand::SweepHand(int positionindex)
{
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	int direction = (int)handType;

	callCount++;

	//if (platformTriggerCount == 3)
	//{
	//	platformTriggerCount = 0;
	//	SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSHANDATTACK);
	//}

	if (platformTriggerCount == 0)
	{
		handShotSpeed = 50;
	}
	else if (platformTriggerCount == 1)
	{
		isRealShot = true;
		handShotSpeed += 2000 * GetGameObject()->GetTimeController()->GetDeltaTime();
	}

	if (isReadyHandShot)
	{
		if (positionindex == 1)
		{
			if (HandSweepMove(pos, BJEngine::Math::Vector3D(-1200 * -direction, 320, 0), handShotSpeed))
			{
				isReadyHandShot = false;
			}
		}
		else if (positionindex == 2)
		{
			if (HandSweepMove(pos, BJEngine::Math::Vector3D(-1200 * -direction, 520, 0), handShotSpeed))
			{
				isReadyHandShot = false;
			}
		}
	}
}

int BossHand::GetTriggerCount()
{
	return platformTriggerCount;
}

void BossHand::TakeDown()
{
#pragma region Old
	//// 내려찍기는 지정한 최대횟수만 진행
	//if (takeDownCount != maxTakeDownCount)
	//{
	//	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();

	//	// 맨 처음 찍기는 대가리의 x좌표로 이동
	//	if (takeDownCount == 0)
	//	{
	//		if (!isVerticalMove)
	//		{
	//			// 보스의 대가리에 도달한 경우
	//			if (abs(pos.x - bossComponent->GetGameObject()->GetTransform()->GetLocalPosition().x) < 10.f)
	//			{
	//				isHorizontalMove = false;
	//				pos.x = bossComponent->GetGameObject()->GetTransform()->GetLocalPosition().x;
	//			}
	//			else
	//			{
	//				isHorizontalMove = true;
	//				pos.x += ((int)(handType)) * takeDownMoveSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	//			}
	//			GetGameObject()->GetTransform()->SetLocalPosition(pos);
	//		}
	//	}
	//	else
	//	{
	//		if (!isVerticalMove)
	//		{
	//			if (abs(pos.x - (prevPosition.x - (((int)(handType)) * 200.f))) < 10.f)
	//			{
	//				pos.x = pos.x;
	//				isHorizontalMove = false;
	//			}
	//			else
	//			{
	//				isHorizontalMove = true;
	//				pos.x -= ((int)(handType)) * takeDownMoveSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	//			}

	//			GetGameObject()->GetTransform()->SetLocalPosition(pos);
	//		}
	//	}

	//	// 횡으로 이동하지 않고 땅에 닿지 않았으면 내려찍기
	//	if (!isHorizontalMove && !isGround)
	//	{
	//		rigid->SetVelocity(Vector2D(0, 1000));
	//		isVerticalMove = true;
	//	}
	//	else
	//	{
	//		if (!isHorizontalMove)
	//		{
	//			// 원래의 높이에 도달했다면
	//			if (abs(pos.y - originPosition.y) < 5.f)
	//			{
	//				prevPosition = GetGameObject()->GetTransform()->GetLocalPosition();

	//				pos.y = originPosition.y;
	//				isVerticalMove = false;
	//				isGround = false;
	//				takeDownCount++;
	//			}
	//			else
	//			{
	//				isVerticalMove = true;
	//				pos.y -= takeDownMoveSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	//			}
	//		}
	//		GetGameObject()->GetTransform()->SetLocalPosition(pos);
	//	}
	//}
	//else
	//{
	//	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	//	// 내려찍기 패턴이 끝남 우선 위치를 원래의 x좌표로 이동
	//	if (abs(pos.x - originPosition.x) < 5.f)
	//	{
	//		isGround = false;
	//		isHorizontalMove = true;
	//		isVerticalMove = false;
	//		isTakeDown = false;
	//		takeDownCount = 0;
	//		pos.x = originPosition.x;
	//		bossComponent->SetIsPattern(false);
	//	}
	//	else
	//	{
	//		pos.x += ((int)(handType)) * takeDownMoveSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	//	}
	//	GetGameObject()->GetTransform()->SetLocalPosition(pos);
	//}
#pragma endregion

#pragma region New
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	if (takeDownCount != maxTakeDownCount)
	{
		if (takeDownCount == 0)
		{
			if (!isVerticalMove)
			{
				if (abs(pos.x - 0) <= 5.f)
				{
					isHorizontalMove = false;
					pos.x = 0;

					if (abs(pos.y - (originPosition.y - verticalOffset)) <= 5.f)
					{
						pos.y = originPosition.y - verticalOffset;
						isVerticalMove = true;
						rigid->SetVelocity(Vector2D(0, 1000));
					}
					else
					{
						pos.y -= takeDownMoveSpeed * deltaTime;
					}
				}
				else
				{
					isHorizontalMove = true;
					pos.x += ((int)handType) * takeDownMoveSpeed * deltaTime;
				}
			}

			if (isGround)
			{
				totalGroundWaitTime += deltaTime;
				if (totalGroundWaitTime >= groundWaitTime)
				{
					if (abs(pos.y - originPosition.y) <= 5.f)
					{
						isVerticalMove = false;
						pos.y = originPosition.y;
						isGround = false;
						takeDownCount++;
						prevPosition = pos;
						totalGroundWaitTime = 0.f;
					}
					else
					{
						isVerticalMove = true;
						pos.y -= takeDownMoveSpeed * deltaTime;
					}
				}
			}
			GetGameObject()->GetTransform()->SetLocalPosition(pos);
		}
		else
		{
			if (!isVerticalMove)
			{
				if (abs(pos.x - (prevPosition.x - ((int)handType) * horizontalOffset)) <= 5.f)
				{
					isHorizontalMove = false;
					pos.x = prevPosition.x - ((int)handType) * horizontalOffset;


					if (abs(pos.y - (originPosition.y - verticalOffset)) <= 5.f)
					{
						pos.y = originPosition.y - verticalOffset;
						isVerticalMove = true;
						rigid->SetVelocity(Vector2D(0, 1000));
					}
					else
					{
						pos.y -= takeDownMoveSpeed * deltaTime;
					}
				}
				else
				{
					isHorizontalMove = true;
					pos.x -= ((int)handType) * takeDownMoveSpeed * deltaTime;
				}
			}

			if (isGround)
			{
				totalGroundWaitTime += deltaTime;
				if (totalGroundWaitTime >= groundWaitTime)
				{
					if (abs(pos.y - originPosition.y) <= 5.f)
					{
						isVerticalMove = false;
						pos.y = originPosition.y;
						isGround = false;
						takeDownCount++;
						prevPosition = pos;
						totalGroundWaitTime = 0.f;
					}
					else
					{
						isVerticalMove = true;
						pos.y -= takeDownMoveSpeed * deltaTime;
					}
				}
			}
			GetGameObject()->GetTransform()->SetLocalPosition(pos);
		}
	}
	else
	{
		if (abs(pos.x - originPosition.x) <= 5.f)
		{
			isGround = false;
			isHorizontalMove = true;
			isVerticalMove = false;
			isTakeDown = false;
			takeDownCount = 0;
			pos.x = originPosition.x;
			bossComponent->SetIsPattern(false);
			if (handType == HAND_TYPE::LEFT)
			{
				bossComponent->SetLeftFinish(true);
			}
			else
			{
				bossComponent->SetRightFinish(true);
			}
		}
		else
		{
			pos.x += ((int)(handType)) * takeDownMoveSpeed * deltaTime;
		}
		GetGameObject()->GetTransform()->SetLocalPosition(pos);
	}
#pragma endregion

#pragma region NewNew
	//if (takeDownIndex != maxTakeDownIndex)
	//{
	//	isTakeDown = true;
	//	if (takeDownIndex == 2 || takeDownIndex == 6 || takeDownIndex == 10)
	//	{
	//		// 실제 내려찍는 지점 빠르게 움직이기
	//		TakeDownMove(GetGameObject()->GetTransform()->GetLocalPosition(), vecTakeDown[takeDownIndex], 500.f);
	//	}
	//	else
	//	{
	//		TakeDownMove(GetGameObject()->GetTransform()->GetLocalPosition(), vecTakeDown[takeDownIndex], 200.f);
	//	}
	//}
	//else
	//{
	//	bossComponent->SetIsPattern(false);
	//	takeDownIndex = 0;
	//	isTakeDown = false;
	//	isGround = false;
	//}
#pragma endregion
}

float BossHand::GetMoveSpeed()
{
	return handShotSpeed;
}

//void BossHand::SweepHand()
//{
//	if (patrolIndex != maxPatrolIndex)
//	{
//		if (patrolIndex == 5)
//		{
//			if (handType == HAND_TYPE::LEFT)
//			{
//				if (!isRightHandSweep && bossComponent->rightHand->GetActive())
//				{
//					isRightHandSweep = true;
//					bossComponent->SetIsRightHandSweep(true);
//				}
//			}
//
//			Move(GetGameObject()->GetTransform()->GetLocalPosition(), vecPatrol[patrolIndex], 500.f);
//		}
//		else
//		{
//			Move(GetGameObject()->GetTransform()->GetLocalPosition(), vecPatrol[patrolIndex], 1500.f);
//		}
//	}
//	else
//	{
//		if (handType == HAND_TYPE::LEFT && !bossComponent->rightHand->GetActive())
//		{
//			bossComponent->SetIsPattern(false);
//			bossComponent->SetIsRightHandSweep(false);
//		}
//
//		if (handType == HAND_TYPE::RIGHT)
//		{
//			bossComponent->SetIsPattern(false);
//			bossComponent->SetIsRightHandSweep(false);
//		}
//
//		isGround = false;
//		isRightHandSweep = false;
//		patrolIndex = 0;
//		isSweepHand = false;
//	}
//}



Enemy* BossHand::GetEnemyComponent()
{
	return enemyComponent;
}

void BossHand::Move(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D endpos, float movespeed)
{
	BJEngine::Math::Vector3D dir = endpos - startpos;
	dir = dir.GetUnitVector();
	if (abs(startpos.x - endpos.x) < 25.f
		&& abs(startpos.y - endpos.y) < 25.f
		&& abs(startpos.z - endpos.z) < 25.f)
	{
		startpos = endpos;
		patrolIndex++;
		if (isSweepHand)
		{
			isVerticalMove = true;
		}
	}
	else
	{
		startpos += dir * movespeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	}
	GetGameObject()->GetTransform()->SetLocalPosition(startpos);
}

void BossHand::TakeDownMove(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D endpos, float movespeed)
{
	BJEngine::Math::Vector3D dir = endpos - startpos;
	dir = dir.GetUnitVector();
	if (abs(startpos.x - endpos.x) < 10.f
		&& abs(startpos.y - endpos.y) < 10.f
		&& abs(startpos.z - endpos.z) < 10.f)
	{
		startpos = endpos;
		takeDownIndex++;
	}
	else
	{
		startpos += dir * movespeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	}
	GetGameObject()->GetTransform()->SetLocalPosition(startpos);
}

void BossHand::HandSweepLoop()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> range(1, 2);

	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	int direction = (int)handType;

	if (!isVerticalMove)
	{
		if (abs(pos.y - 1000) <= 10.f)
		{
			pos.y = 1000;
			isVerticalMove = true;
		}
		else
		{
			pos.y += 300 * GetGameObject()->GetTimeController()->GetDeltaTime();
			GetGameObject()->GetTransform()->SetLocalPosition(pos);
		}
	}


	if (!isReadyHandShot)
	{
		platformTriggerCount = 0;
		handShotSpeed = 50.f;
		isRealShot = false;

		if (handType == HAND_TYPE::LEFT)
		{
			randomPos = range(mt);
			if (isVerticalMove)
			{
				sweepCount++;
				if (sweepCount != maxSweepCount)
				{
					SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSHANDATTACK);
				}
				if (randomPos == 1)
				{
					pos = BJEngine::Math::Vector3D(-1100 * direction, 320, 0);
				}
				else if (randomPos == 2)
				{
					pos = BJEngine::Math::Vector3D(-1100 * direction, 520, 0);
				}
				isReadyHandShot = true;
				GetGameObject()->GetTransform()->SetLocalPosition(pos);
			}
		}
		else
		{
			randomPos = range(mt);
			if (isVerticalMove)
			{
				sweepCount++;
				if (sweepCount != maxSweepCount)
				{
					SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSHANDATTACK);
				}
				if (randomPos == 1)
				{
					pos = BJEngine::Math::Vector3D(-1100 * direction, 320, 0);
				}
				else if (randomPos == 2)
				{
					pos = BJEngine::Math::Vector3D(-1100 * direction, 520, 0);
				}
				isReadyHandShot = true;
				GetGameObject()->GetTransform()->SetLocalPosition(pos);
			}
		}
	}

	if (isReadyHandShot)
	{
		if (sweepCount != maxSweepCount)
		{
			SweepHand(randomPos);
		}
		else
		{
			if (isHorizontalMove)
			{
				pos.x = originPosition.x;
				pos.y = 1000;
				isHorizontalMove = false;
			}

			if (abs(pos.y - originPosition.y) <= 10.f)
			{
				pos.y = originPosition.y;
				isHorizontalMove = true;
				isVerticalMove = false;
				bossComponent->SetIsPattern(false);
				sweepCount = -1;
				platformTriggerCount = 0;
				isSweepHand = false;
				isRealShot = false;
				isReadyHandShot = false;

				if (handType == HAND_TYPE::LEFT)
				{
					bossComponent->SetLeftFinish(true);
				}
				else
				{
					bossComponent->SetRightFinish(true);
				}
			}
			else
			{
				pos.y -= 150 * GetGameObject()->GetTimeController()->GetDeltaTime();
			}

			GetGameObject()->GetTransform()->SetLocalPosition(pos);
		}
	}
}


bool BossHand::HandSweepMove(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D endpos, float movespeed)
{
	BJEngine::Math::Vector3D dir = endpos - startpos;
	dir = dir.GetUnitVector();
	if (abs(startpos.x - endpos.x) < 25.f
		&& abs(startpos.y - endpos.y) < 25.f
		&& abs(startpos.z - endpos.z) < 25.f)
	{
		startpos = endpos;
		GetGameObject()->GetTransform()->SetLocalPosition(startpos);
		return true;
	}
	else
	{
		startpos += dir * movespeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
		GetGameObject()->GetTransform()->SetLocalPosition(startpos);
		return false;
	}
}

void BossHand::CheckDie()
{
	if (enemyComponent->GetCurHP() <= 0.f)
	{
		enemyComponent->SetCurHP(0.f);
		MakeEffect();
	}
}

void BossHand::MakeEffect()
{
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetWorldPosition();
	float xMin = pos.x - 100;
	float xMax = pos.x + 100;
	float yMin = pos.y - 100;
	float yMax = pos.y + 100;

	totalEffectTime += GetGameObject()->GetTimeController()->GetDeltaTime();

	bossComponent->SetIsPattern(false);
	if (handType == HAND_TYPE::LEFT)
	{
		bossComponent->SetLeftFinish(true);
	}
	else
	{
		bossComponent->SetRightFinish(true);
	}
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

			GameObject* effect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
			effect->GetTransform()->SetLocalPosition(randomPos);
			effect->SetActive(true);
			effect->GetComponent<EffectPlayer>()->Reset(L"Enemy_Die_Effect");
			totalEffectTime = 0.f;
			curEffectCount++;
			SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSEXPLOSION);
		}
	}
	else
	{
		GetGameObject()->SetActive(false);
	}
}

void BossHand::SetTakeDownPoint()
{
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	int direction = (int)handType;
	int tempIndex = 1;

	// 1번째
	vecTakeDown.push_back(BJEngine::Math::Vector3D(0, pos.y, 0));					// 0
	vecTakeDown.push_back(BJEngine::Math::Vector3D(0, pos.y - verticalOffset, 0));	// 1
	vecTakeDown.push_back(BJEngine::Math::Vector3D(0, 350, 0));						// 2
	vecTakeDown.push_back(BJEngine::Math::Vector3D(0, pos.y, 0));					// 3
	// 2번째
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, pos.y, 0));	// 4
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, pos.y - verticalOffset, 0));	// 5
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, 550, 0));		// 6
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, pos.y, 0));	// 7
	// 3번째
	tempIndex++;
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, pos.y, 0));	// 8
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, pos.y - verticalOffset, 0));	// 9
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, 550, 0));		// 10
	vecTakeDown.push_back(BJEngine::Math::Vector3D(-direction * horizontalOffset * tempIndex, pos.y, 0));	// 11
	// 4번째
	vecTakeDown.push_back(BJEngine::Math::Vector3D(pos.x, pos.y, 0));	// 12

	maxTakeDownIndex = vecTakeDown.size();
}

void BossHand::Update()
{
	CheckDie();

	if (isTakeDown)
	{
		TakeDown();
	}

	if (isSweepHand)
	{
		HandSweepLoop();
	}
}

void BossHand::OnTriggerEnter(BJEngine::Collider* other)
{
#pragma region OLD
	//if (other->GetGameObject()->GetName() == L"bottomWall")
	//{
	//	if (isTakeDown)
	//	{
	//		isHandShake = false;
	//		totalHandShakeTime = 0.f;
	//		rigid->SetVelocity(Vector2D(0, 0));
	//		isGround = true;
	//		CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetMainCamera()->GetGameObject()->GetComponent<CameraShake>()->Shake(0.1f, 10.f, 10.f);
	//	}

	//	if (isSweepHand)
	//	{
	//		if (!isGround)
	//		{
	//			isGround = true;
	//			CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetMainCamera()->GetGameObject()->GetComponent<CameraShake>()->Shake(1.5f, 10.f, 0.f);
	//		}
	//	}
	//}

	//if (other->GetGameObject()->GetName() == L"player")
	//{
	//	other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::BOSSHAND, damage);
	//}
#pragma endregion

#pragma region NEW

	if (other->GetGameObject()->GetLayerName() == L"Platform")
	{
		if (isTakeDown)
		{
			isHandShake = false;
			totalHandShakeTime = 0.f;
			rigid->SetVelocity(Vector2D(0, 0));
			isGround = true;
			CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetMainCamera()->GetGameObject()->GetComponent<CameraShake>()->Shake(0.2f, 20.f, 20.f);
			BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetWorldPosition();
			pos.y += 40;
			GameObject* effect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
			effect->GetTransform()->SetLocalScale(BJEngine::Math::Vector3D(1.f,1.5f,1.f));
			effect->GetTransform()->SetLocalPosition(pos);
			effect->GetComponent<EffectPlayer>()->Reset(L"TakeDown_Effect");
			effect->SetActive(true);

			SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSHANDMASH);
		}

		//if (isSweepHand)
		//{

		//}
	}

	if (other->GetGameObject()->GetLayerName() == L"Player" || other->GetGameObject()->GetLayerName() == L"StealthPlayer")
	{
		if (!isGround)
		{
			if (isTakeDown)
			{
				other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::BOSSHAND, damage);
			}
		}

		if (isSweepHand)
		{
			other->GetGameObject()->GetComponent<Player>()->Attacked(ENEMYATTACK_TYPE::BOSSHAND, damage);
		}
	}

#pragma endregion
}

void BossHand::OnTriggerExit(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"Platform")
	{
		if (isSweepHand && !isRealShot)
		{
			platformTriggerCount++;
		}
	}
}

void BossHand::OnDisable()
{

}
