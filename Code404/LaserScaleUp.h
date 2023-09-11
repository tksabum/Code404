#pragma once
#include "Component.h"

class LaserScaleUp :
    public BJEngine::Component
{
private:
    float scaleUpSpeed = 0.4f;
    bool isScaleUp = true;

public:
    virtual void Update() override;

    virtual void OnDisable() override;
    virtual void OnEnable() override;
};

