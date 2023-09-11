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

	// ��ų ��� �� �帥 �ð��� ������Ʈ�Ѵ�.
	void TimeUpdate();

	// ��ų�� ����� �� �ִ��� �Ǵ�
	bool Usable();

	float GetCost();

	// ���� ��Ÿ��
	float GetRemainingCooltime();

	// ��Ÿ��
	float GetCooltime();

	// ��ų��� �� �帥 �ð��� �����Ѵ�. (��ų�� ���� ����� �������Ͽ� ��Ÿ���� �ٽ� ��ٸ����� �Ѵ�.)
	void ResetElapsedTime();

	// ��Ÿ�� �ʱ�ȭ (��ų ��� �� �帥 �ð��� ��Ÿ�Ӱ� �����ϰ� �ʱ�ȭ�Ѵ�.)
	void ResetCooltime();

	// ��ų ���
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

	// ��ų����
	int skillLevel;
	int skillLevelMax;
	
	// �ݺ����
	int repeatUse;

	// �ݺ���뽺�� �ʱ�ȭ �ֱ�
	float repeatUseTime;

	// ��Ÿ��
	float coolTime;

	// ���������� ��ų�� ����� �� �帥�ð�
	float elapsedTime;

	// ������ ������� ����
	bool nagativeCostAllowed;

	friend DebugInfo;
	friend SkillCoolTimeViewer;
};

