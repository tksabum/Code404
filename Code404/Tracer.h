#pragma once

#include "Component.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class Tracer : public Component
{
public:
	void SetTarget(Transform* target, Vector2D tracearea, Vector2D sceneLT = Vector2D(0.0f, 0.0f), Vector2D sceneRB = Vector2D(0.0f, 0.0f), bool usescenetrace = false);

	void Awake();
	void LateUpdate();

	void SetUsingSceneTrace(bool value);
	bool GetUsingSceneTrace();

private:
	Transform* ownTransform;
	Transform* targetTransform;

	Vector2D traceArea;
	Vector2D sceneLeftTop;
	Vector2D sceneRightBottom;

	bool usingSceneTrace;
};

