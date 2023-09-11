#pragma once

#include <queue>

#include "GameObject.h"
#include "Component.h"
#include "Input.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class ObjectPoolTest : public BJEngine::Component
{
public:
	void Update();

	std::queue<GameObject*> objectQueue;
};

