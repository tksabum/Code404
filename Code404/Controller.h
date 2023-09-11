#pragma once

#include "Component.h"
#include "Input.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class Controller : public Component
{
public:
	void Awake();

	void Update();

private:
	float moveSpeed;
	float scaleSpeed;

};

