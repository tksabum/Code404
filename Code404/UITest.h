#pragma once

#include "Component.h"
#include "Input.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class UITest : public Component
{
public:
	void Awake();

	void OnMouseLeftDown();
	void OnMouseLeftUp();

	void Update();

private:
	bool isDrag;
	Vector2D dragPosition;

};

