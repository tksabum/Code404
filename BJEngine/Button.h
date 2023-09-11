#pragma once

#ifdef BUTTON_EXPORTS
#define BUTTON_API __declspec(dllexport)
#else
#define BUTTON_API __declspec(dllimport)
#endif

#include <string>
#include <functional>
#include <vector>

#include "Component.h"

namespace BJEngine
{
	class Image;

	class Button : public Component
	{
	public:
		BUTTON_API Button();
		BUTTON_API ~Button();

		// 함수등록
		BUTTON_API void AddButtonEvent(std::function<void()> eventfunction);

		void Awake();

		void OnMouseLeftDown();
		void OnMouseLeftUp();
		void OnMouseEnter();
		void OnMouseExit();

		BUTTON_API void SetSound(int effectsoundID);
		BUTTON_API void SetImage(std::wstring idleimage, std::wstring enterimage, std::wstring clickimage);

	private:
		Image* image;

		bool isMouseLeftDown;
		int effectSoundID;
		std::wstring idleImage;
		std::wstring enterImage;
		std::wstring clickImage;

		std::vector<std::function<void()>> buttonEventList;

	};
}