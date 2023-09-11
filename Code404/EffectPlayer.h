#pragma once

#include "Component.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "GameObject.h"

using namespace BJEngine;
using namespace std;

class EffectPlayer : public Component
{
public:
	void Reset(wstring effectname);

	void Update();

private:
	SpriteRenderer* sprite;
	Animator* animator;
	bool isPlaying;

};

