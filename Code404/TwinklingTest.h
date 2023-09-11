#pragma once

#include "Component.h"

#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class TwinklingTest : public Component
{
public:
	void Awake();
	void Update();

private:
	float counter;

	float originPosition;
	float originScale;
	Transform* transform;

	float period = 4.0f;
	float minPeriod = 1.2f;
	float maxPeriod = 3.0f;

	const float maxScale = 5.0f;
	const float minScale = 2.0f;
	const float imageSize = 500.0f;
};

