#pragma once
#include "Component.h"

namespace BJEngine
{
    class GameObject;
}


class CutScenePlayer :
    public BJEngine::Component
{
private:
    BJEngine::GameObject* dangerText;
    bool isSweep = false;

    bool isWalk = true;
    float totalTime;
    float walkTime = 0.3f;

public:
    void SetDangerText(BJEngine::GameObject* text);
    void SetIsWalk(bool iswalk);

public:
    virtual void Update() override;
    virtual void OnTriggerEnter(BJEngine::Collider* other);
};

