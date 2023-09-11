#pragma once

#ifdef UI_EXPORTS
#define UI_API __declspec(dllexport)
#else
#define UI_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class UI : public Component
	{
	public:
		UI_API UI();
		UI_API ~UI();
		
		UI_API void SetSize(Math::Vector2D size);
		UI_API Math::Vector2D GetSize();

	private:
		Math::Vector2D uiSize;

	};
}