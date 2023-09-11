#pragma once
#include "Component.h"



namespace BJEngine
{
    class GameObject;
}


class ScrollingBackGround :
    public BJEngine::Component
{
    BJEngine::GameObject* backGround1;
    BJEngine::GameObject* backGround2;

    float moveSpeed = 500.f;

    bool isScrolling = true;
    float scrollingTime = 6.f;
    float totalScrollingTime;

public:
    void SetBackGround1(BJEngine::GameObject* background1);
    void SetBackGround2(BJEngine::GameObject* background2);
    void SetScrolling(bool isscrolling);
    bool GetScrolling();
public:
    virtual void Update() override;

};

