#pragma once

#include "Image.h"
#include "Scene.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Transform.h"
#include "Camera.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Color.h"

#include "ParticleSystem.h"

class SampleScene2 : public BJEngine::Scene
{
public:
	SampleScene2();

	void Start();
};
