#pragma once
#include "Component.h"

namespace BJEngine
{
    class Animator;
    class Rigidbody2D;
}

class ScrollingBackGround;
class CutScenePlayer;

class BossCutSceneController :
    public BJEngine::Component
{
private:
    BJEngine::GameObject* player;
    BJEngine::Animator* playerAnim;
    CutScenePlayer* playerComponent;

	BJEngine::GameObject* scrolling;
	ScrollingBackGround* scrollingComponent;

    BJEngine::GameObject* bossHand;
    BJEngine::Rigidbody2D* handRigid;

    BJEngine::GameObject* dangerText;

    bool isScrolling = true;
    float scrollingTime=6.f;
    float totalScrollingTime;

    bool isWait = false;
    float waitTime = 3.f;
    float totalWaitTime;

    bool isCameraShake = false;

public:
    void SetPlayer(BJEngine::GameObject* player);
    void SetScrolling(BJEngine::GameObject* scrolling);
    void SetBossHand(BJEngine::GameObject* hand);
    void SetText(BJEngine::GameObject* text);

public:
    virtual void Awake() override;
    virtual void Update() override;
};

