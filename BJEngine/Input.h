#pragma once

#ifdef INPUT_EXPORTS
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API __declspec(dllimport)
#endif

#include <Windows.h>

#include "Vector2D.h"

namespace BJEngine
{
	class CoreSystem;

	class Input
	{
	public:
		INPUT_API void Initialize(HWND hwnd);

		INPUT_API void Update();
		
		INPUT_API int GetWheelDelta();

		// 키의 눌렸는지를 리턴한다.
		INPUT_API bool PressedKey(int keycode);
		// 키가 지금 눌렸는지를 리턴한다.
		INPUT_API bool PressedKeyNow(int keycode);
		// 키가 떨어졌는지 리턴한다.
		INPUT_API bool ReleasedKey(int keycode);
		// 키가 지금 떨어졌는지 리턴한다.
		INPUT_API bool ReleasedKeyNow(int keycode);

		// 눌린키가 하나이상이면 true
		INPUT_API bool PressedAnyKey();
		// 지금 눌린키가 하나 이상이면 true
		INPUT_API bool PressedAnyKeyNow();

		// 마우스 왼쪽 버튼을 마지막으로 누른 위치
		INPUT_API Math::Vector2D GetMouseLeftPressedPosition();
		// 마우스 왼쪽 버튼을 마지막으로 뗀 위치
		INPUT_API Math::Vector2D GetMouseLeftReleasedPosition();
		// 마우스 오른쪽 버튼을 마지막으로 누른 위치
		INPUT_API Math::Vector2D GetMouseRightPressedPosition();
		// 마우스 오른쪽 버튼을 마지막으로 뗀 위치
		INPUT_API Math::Vector2D GetMouseRightReleasedPosition();
		// 마우스 위치
		INPUT_API Math::Vector2D GetMousePosition();

		INPUT_API void SetKeyActive(bool value);
		INPUT_API bool GetKeyActive();

		INPUT_API void SetMouseActive(bool value);
		INPUT_API bool GetMouseActive();

	private:
		void updateWheelDelta(SHORT wheeldelta);

		bool currentAnyKeyDown;
		bool lastAnyKeyDown;
		bool currentKeyState[0xFF];
		bool lastKeyState[0xFF];
		Math::Vector2D currentMousePosition;
		Math::Vector2D mouseLeftPressedPosition;
		Math::Vector2D mouseLeftReleasedPosition;
		Math::Vector2D mouseRightPressedPosition;
		Math::Vector2D mouseRightReleasedPosition;

		int wheelDelta;
		int currentWheel;

		bool isKeyActive;
		bool isMouseActive;

		HWND hWnd;


		//singleton
	private:
		INPUT_API Input();
		INPUT_API ~Input();
		INPUT_API Input(const Input& ref);
		INPUT_API Input& operator=(const Input& ref) {}

	public:
		INPUT_API static Input& GetInstance();

		friend CoreSystem;
	};
}