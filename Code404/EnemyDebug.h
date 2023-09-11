#pragma once
#include "Component.h"

#include "GameObject.h"
#include "Scene.h"
#include "TimeController.h"
#include "Text.h"

using namespace BJEngine;

class EnemyDebug :
    public BJEngine:: Component
{
public:
    void Awake();
    void Update();

private:
    Text* text;
};

