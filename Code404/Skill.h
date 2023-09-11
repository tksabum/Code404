#pragma once

#include <vector>

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Transform.h"
#include "TimeController.h"
#include "Vector2D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class DebugInfo;
class SkillCoolTimeViewer;

class Skill
{
public:
	Skill(Vector2D* userdirection, Transform* usertransform, bool* uniquecost1, bool* uniquecost2, float* usermp, float defaultcost, float levelcostcoefficient, float levelcostmultiplier, float repeatcostcoefficient, float repeatcostmultiplier, int level, int levelMax, float cooltime, float repeatusetime, bool nagativecostallowed);
	virtual ~Skill();

	// 스킬 사용 후 흐른 시간을 업데이트한다.
	void TimeUpdate();

	// 스킬을 사용할 수 있는지 판단
	bool Usable();

	float GetCost();

	// 남은 쿨타임
	float GetRemainingCooltime();

	// 쿨타임
	float GetCooltime();

	// 스킬사용 후 흐른 시간을 리셋한다. (스킬을 지금 사용한 것으로하여 쿨타임을 다시 기다리도록 한다.)
	void ResetElapsedTime();

	// 쿨타임 초기화 (스킬 사용 후 흐른 시간을 쿨타임과 동일하게 초기화한다.)
	void ResetCooltime();

	// 스킬 사용
	virtual void Use() abstract;


protected:
	void UseCost();

	Vector2D* userDirection;
	Transform* userTransform;
	bool* uniqueCost1;
	bool* uniqueCost2;
	float* userMp;

	float defaultCost;
	float levelCostCoefficient;
	float levelCostMultiplier;
	float repeatCostCoefficient;
	float repeatCostMultiplier;

	// 스킬레벨
	int skillLevel;
	int skillLevelMax;
	
	// 반복사용
	int repeatUse;

	// 반복사용스택 초기화 주기
	float repeatUseTime;

	// 쿨타임
	float coolTime;

	// 마지막으로 스킬을 사용한 후 흐른시간
	float elapsedTime;

	// 음수의 마나비용 허용됨
	bool nagativeCostAllowed;

	friend DebugInfo;
	friend SkillCoolTimeViewer;
};

