#include "ScrollingBackGround.h"

#include "TimeController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"
#include "BJMath.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void ScrollingBackGround::SetBackGround1(GameObject* background1)
{
	backGround1 = background1;
}

void ScrollingBackGround::SetBackGround2(GameObject* background2)
{
	backGround2 = background2;
}

void ScrollingBackGround::SetScrolling(bool isscrolling)
{
	isScrolling = isscrolling;
}

bool ScrollingBackGround::GetScrolling()
{
	return isScrolling;
}

void ScrollingBackGround::Update()
{
	if (isScrolling)
	{
		float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
		Vector3D back1Pos = backGround1->GetTransform()->GetLocalPosition();
		Vector3D back2Pos = backGround2->GetTransform()->GetLocalPosition();

		back1Pos.x += -1 * moveSpeed * deltaTime;
		back2Pos.x += -1 * moveSpeed * deltaTime;

		if (back1Pos.x <= -2100.f)
		{
			back1Pos.x = back2Pos.x + 2117;
		}
		if (back2Pos.x <= -2100.f)
		{
			back2Pos.x = back1Pos.x + 2117;
		}

		backGround1->GetTransform()->SetLocalPosition(back1Pos);
		backGround2->GetTransform()->SetLocalPosition(back2Pos);
	}
}
