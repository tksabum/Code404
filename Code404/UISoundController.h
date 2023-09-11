#pragma once

#include "Component.h"
#include "Input.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class UISoundController : public Component
{
public:
	UISoundController();
	void Awake();

	void OnMouseLeftDown();
	void OnMouseLeftUp();

	void Update();

	void SetRange(float rangeX);

private:
	bool isDrag;
	Math::Vector2D dragPosition;

	float originPosX;
	float originPosY;
	float moveRangeX;
		
};