#pragma once
#include "Component.h"

using namespace BJEngine;

namespace BJEngine
{
	class GameObject;
}


class TimeLaser 
	: public Component
{
private:
	GameObject* timeLaser;

	GameObject* leftHoleEffect;
	GameObject* rightHoleEffect;

	bool isAble = false;
	bool isEffect = false;
	bool isDisable = false;

	float ablePeriod = 7.f;
	float totalAblePeriod = 0.f;

	float disablePeriod = 1.f;
	float totalDisablePeriod;

	float effectActiveTime;
	float totalEffectActiveTime;

public:
	virtual void Awake() override;
	virtual void Update() override;

};

