#include "Input.h"

#include "GraphicsSystem.h"

namespace BJEngine
{
	using namespace Math;

	Input::Input() : wheelDelta(0), currentWheel(0), isKeyActive(true), isMouseActive(true), currentAnyKeyDown(false), lastAnyKeyDown(false)
	{
		Update();
	}

	Input::~Input()
	{

	}

	Input& Input::GetInstance()
	{
		static Input instance;
		return instance;
	}

	void Input::Initialize(HWND hwnd)
	{
		hWnd = hwnd;
	}

	void Input::Update()
	{
		lastAnyKeyDown = currentAnyKeyDown;
		currentAnyKeyDown = false;

		for (int keycode = 0x01; keycode < 0xFF; keycode++)
		{
			lastKeyState[keycode] = currentKeyState[keycode];

			currentKeyState[keycode] = GetAsyncKeyState(keycode) & 0x8000;

			currentAnyKeyDown = currentAnyKeyDown || currentKeyState[keycode];

			// â�� ��Ȱ��ȭ�� ��� Ű �Է��� �����Ѵ�.
			if (hWnd != GetFocus())
			{
				currentKeyState[keycode] = false;
			}

			// isActive�� false�� ��� Ű, ���콺 �Է��� �����Ѵ�.
			if (keycode <= 0x06 && isMouseActive == false)
			{
				currentKeyState[keycode] = false;
			}

			if (0x06 < keycode && isKeyActive == false)
			{
				currentKeyState[keycode] = false;
			}
		}

		POINT mousepoint;
		GetCursorPos(&mousepoint);
		ScreenToClient(hWnd, &mousepoint);
		currentMousePosition = Vector2D(mousepoint.x, mousepoint.y);

		if (!lastKeyState[VK_LBUTTON] && currentKeyState[VK_LBUTTON])
		{
			mouseLeftPressedPosition = currentMousePosition;
		}

		if (lastKeyState[VK_LBUTTON] && !currentKeyState[VK_LBUTTON])
		{
			mouseLeftReleasedPosition = currentMousePosition;
		}

		if (!lastKeyState[VK_RBUTTON] && currentKeyState[VK_RBUTTON])
		{
			mouseRightPressedPosition = currentMousePosition;
		}

		if (lastKeyState[VK_RBUTTON] && !currentKeyState[VK_RBUTTON])
		{
			mouseRightReleasedPosition = currentMousePosition;
		}

		wheelDelta = currentWheel;
		currentWheel = 0;
	}

	int Input::GetWheelDelta()
	{
		return wheelDelta;
	}

	bool Input::PressedKey(int keycode)
	{
		return currentKeyState[keycode];
	}

	bool Input::PressedKeyNow(int keycode)
	{
		return currentKeyState[keycode] && !lastKeyState[keycode];
	}

	bool Input::ReleasedKey(int keycode)
	{
		return !currentKeyState[keycode];
	}

	bool Input::ReleasedKeyNow(int keycode)
	{
		return !currentKeyState[keycode] && lastKeyState[keycode];
	}

	bool Input::PressedAnyKey()
	{
		return currentAnyKeyDown;
	}

	bool Input::PressedAnyKeyNow()
	{
		return currentAnyKeyDown && !lastAnyKeyDown;
	}

	Math::Vector2D Input::GetMouseLeftPressedPosition()
	{
		return mouseLeftPressedPosition;
	}
	
	Math::Vector2D Input::GetMouseLeftReleasedPosition()
	{
		return mouseLeftReleasedPosition;
	}
	
	Math::Vector2D Input::GetMouseRightPressedPosition()
	{
		return mouseRightPressedPosition;
	}
	
	Math::Vector2D Input::GetMouseRightReleasedPosition()
	{
		return mouseRightReleasedPosition;
	}
	
	Math::Vector2D Input::GetMousePosition()
	{
		return currentMousePosition;
	}

	void Input::SetKeyActive(bool value)
	{
		isKeyActive = value;
	}

	bool Input::GetKeyActive()
	{
		return isKeyActive;
	}

	void Input::SetMouseActive(bool value)
	{
		isMouseActive = value;
	}

	bool Input::GetMouseActive()
	{
		return isMouseActive;
	}

	void Input::updateWheelDelta(SHORT wheeldelta)
	{
		// â�� ��Ȱ��ȭ�� ��� Ű �Է��� �����Ѵ�.
		if (hWnd != GetFocus())
		{
			return;
		}

		currentWheel += wheeldelta;
	}
}