#pragma once

#include "Component.h"

#include "GameObject.h"
#include "Scene.h"
#include "Input.h"
#include "TimeController.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class GamePause : public Component
{
public:
	void OnEnable();
	void OnDisable();

private:


};

