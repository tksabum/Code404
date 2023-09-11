#pragma once
#include "Component.h"

using namespace BJEngine;

namespace BJEngine
{
    class Text;
}

class BossHand;

class DebugHand :
    public Component
{
    Text* text;
    BossHand* hand;
    int temp;

public:
    void SetTemp(int temp);

public:
    virtual void Awake() override;
    virtual void Update() override;
};

