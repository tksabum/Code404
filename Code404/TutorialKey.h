#pragma once

#include "Component.h"

#include "SpriteRenderer.h"
#include "GameObject.h"

using namespace std;
using namespace BJEngine;

class TutorialKey : public Component
{
public:
	void Awake();

	void OnTriggerEnter(Collider* collider);

private:
	SpriteRenderer* sprite;

	void ChangeSprite(wstring str);
	void ShowSprite();
	void HideSprite();
};