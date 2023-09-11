#include "UITest.h"

void UITest::Awake()
{
	isDrag = false;
}

void UITest::OnMouseLeftDown()
{
	isDrag = true;
	dragPosition = Input::GetInstance().GetMousePosition();
}

void UITest::OnMouseLeftUp()
{
	isDrag = false;
}

void UITest::Update()
{
	if (isDrag)
	{
		Vector2D currentpos = Input::GetInstance().GetMousePosition();
		Vector2D movePos = currentpos - dragPosition;
		dragPosition = currentpos;

		GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movePos);
	}
}