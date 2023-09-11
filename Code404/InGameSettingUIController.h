#pragma once
#include "Component.h"

namespace BJEngine
{
    class GameObject;
    class Image;
}


class InGameSettingUIController :
    public BJEngine::Component
{
private:
    BJEngine::GameObject* target;

    BJEngine::GameObject* sound;
    BJEngine::GameObject* screen;

    BJEngine::GameObject* blackMask;
    BJEngine::Image* blackMaskImage;

    bool isMain = false;
    float opacity;
    float opacitySpeed = 0.2f;

public:
    void SetTarget(BJEngine::GameObject* target);
    void SetSound(BJEngine::GameObject* sound);
    void SetScreen(BJEngine::GameObject* screen);
    void SetBlackMask(BJEngine::GameObject* blackmask);

    void GoToMainTitle();

public:
    virtual void Awake() override;
    virtual void Update() override;
};

