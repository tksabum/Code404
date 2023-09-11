#pragma once
#include "Component.h"

namespace BJEngine
{
    class Animator;
}

class AnimationToScene :
    public BJEngine::Component
{
private:
    BJEngine::Animator* anim;

public:
    virtual void Awake() override;
    virtual void Update() override;
};

