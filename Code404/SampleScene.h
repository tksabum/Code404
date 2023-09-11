#pragma once

#include <random>

#include "Text.h"
#include "Image.h"
#include "UI.h"
#include "Button.h"
#include "Scene.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Transform.h"
#include "Camera.h"
#include "ConvexpolygonCollider.h"
#include "Rigidbody2D.h"
#include "Vector2D.h"
#include "Vector3D.h"

class SampleScene : public BJEngine::Scene
{
public:
	SampleScene();

	void Start();
};

