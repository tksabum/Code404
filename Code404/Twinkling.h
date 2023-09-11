#pragma once
#include "Component.h"


class Twinkling :
    public BJEngine::Component
{
private:
    float originScaleY;
    float originPosY;
    float curScaleY;
    float targetScaleY;
    float deltaScaleY;

public:
    virtual void Awake() override;
    virtual void Update() override;

};

