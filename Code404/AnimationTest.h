#pragma once

#include "Input.h"
#include "Animator.h"
#include "Component.h"
#include "GameObject.h"

using namespace BJEngine;

class AnimationTest : public Component
{
public:
	void Awake();
	void Update();

private:
	Animator* animator;
};

