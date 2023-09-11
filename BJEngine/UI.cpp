#include "UI.h"

#include "UISystem.h"

namespace BJEngine
{
	using namespace Math;

	UI::UI()
	{
		UISystem::GetInstance().AddUI(this);
	}

	UI::~UI()
	{
		UISystem::GetInstance().RemoveUI(this);
	}

	void UI::SetSize(Vector2D size)
	{
		uiSize = size;
	}
	
	Vector2D UI::GetSize()
	{
		return uiSize;
	}
}