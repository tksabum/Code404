#pragma once

#include "Component.h"

#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "TimeController.h"
#include "Text.h"

using namespace BJEngine;

class DebugInfo : public Component
{
public:
	void Awake();
	void Update();

private:
	Text* text;
};