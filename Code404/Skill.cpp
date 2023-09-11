#include "Skill.h"

using namespace BJEngine;

Skill::Skill(Vector2D* userdirection, Transform* usertransform, bool* uniquecost1, bool* uniquecost2, float* usermp, float defaultcost, float levelcostcoefficient, float levelcostmultiplier, float repeatcostcoefficient, float repeatcostmultiplier, int level, int levelMax, float cooltime, float repeatusetime, bool nagativecostallowed) :
	userDirection(userdirection),
	userTransform(usertransform),
	uniqueCost1(uniquecost1),
	uniqueCost2(uniquecost2),
	userMp(usermp),
	defaultCost(defaultcost),
	levelCostCoefficient(levelcostcoefficient),
	levelCostMultiplier(levelcostmultiplier),
	repeatCostCoefficient(repeatcostcoefficient),
	repeatCostMultiplier(repeatcostmultiplier),
	skillLevel(level),
	skillLevelMax(levelMax),
	repeatUse(0),
	coolTime(cooltime),
	repeatUseTime(repeatusetime),
	elapsedTime(max(cooltime, repeatusetime)),
	nagativeCostAllowed(nagativecostallowed)
{

}

Skill::~Skill()
{

}

void Skill::TimeUpdate()
{
	elapsedTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	if (elapsedTime >= repeatUseTime)
	{
		repeatUse = 0;
	}
}

bool Skill::Usable()
{
	// 고유비용
	bool uniquecost = (uniqueCost1 == nullptr || *uniqueCost1) && (uniqueCost2 == nullptr || *uniqueCost2);


	// 코스트와 쿨타임 체크
	return ((*userMp) >= GetCost()) && (GetRemainingCooltime() <= 0.0f) && uniquecost;
}

float Skill::GetCost()
{
	if (nagativeCostAllowed == true)
	{
		return defaultCost + levelCostCoefficient * pow(skillLevel, levelCostMultiplier) + repeatCostCoefficient * pow(repeatUse, repeatCostMultiplier);
	}

	return max(defaultCost + levelCostCoefficient * pow(skillLevel, levelCostMultiplier) + repeatCostCoefficient * pow(repeatUse, repeatCostMultiplier), 0.0f);
}

float Skill::GetRemainingCooltime()
{
	return max(0.0f, coolTime - elapsedTime);
}

float Skill::GetCooltime()
{
	return coolTime;
}

void Skill::ResetElapsedTime()
{
	elapsedTime = 0.0f;
}

void Skill::ResetCooltime()
{
	elapsedTime = coolTime;
}

void Skill::UseCost()
{
	if (Usable())
	{
		*userMp -= GetCost();
	}
}
