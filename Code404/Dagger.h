#pragma once

#include "Component.h"

#include "Scene.h"
#include "GameObject.h"
#include "ConvexpolygonCollider.h"
#include "Rigidbody2D.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "Animator.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class Dagger : public Component
{
public:
	void Initialize();

	void Update();

	void OnTriggerEnter(Collider* collider);

private:
	Animator* animator;
	Rigidbody2D* rigidbody;
	bool isHold;

};

