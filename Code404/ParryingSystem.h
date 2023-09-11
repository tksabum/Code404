#pragma once

#include "Component.h"

#include "CoreSystem.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class ParryingSystem : public Component
{
public:
	void Awake();

	void Update();

	void Parrying();

private:
	TimeController* timecontroller;

	float elapsedTime;

	const static float ORIGINAL_DELTA;
	const static float SLOW_RATE;
	const static float SLOW_TIME;
};

