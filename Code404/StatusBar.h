#pragma once
#include "Component.h"

class StatusBar
	: public BJEngine::Component
{
private:
	BJEngine::GameObject* target;

public:
	virtual void Awake() override;
	virtual void Update() override;
};

