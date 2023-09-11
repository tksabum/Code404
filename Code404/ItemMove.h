#pragma once
#include "Component.h"


class ItemMove :
    public BJEngine::Component
{
private:
    float moveSpeed = 5;
    float verticalOffset = 25;
    float angle;
    float originPosY;

public:
    virtual void Awake() override;
    virtual void Update() override;

};

