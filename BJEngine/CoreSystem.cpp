#include "CoreSystem.h"

#include "EngineSetting.h"
#include "AnimationSystem.h"
#include "SceneManager.h"
#include "PhysicsSystem.h"
#include "GraphicsSystem.h"
#include "UISystem.h"
#include "TimeController.h"
#include "Direct2DGraphics.h"
#include "Direct2DSprite.h"
#include "Direct2DSpriteSheet.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Input.h"

namespace BJEngine
{
	using namespace GraphicsEngine;
	using namespace Math;

	void CoreSystem::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, unsigned int width, unsigned int height, bool screenresizeable)
	{
		//// Load the cursor image
		//HCURSOR hCursor = LoadCursorFromFile(L"cursor.cur");

		//// Set the cursor hotspot position to the center of the image
		//ICONINFO iconInfo;
		//GetIconInfo(hCursor, &iconInfo);
		//SetCursorPos(iconInfo.xHotspot + iconInfo.xHotspot / 2, iconInfo.yHotspot + iconInfo.yHotspot / 2);

		//// Set the modified cursor as the current cursor
		//SetCursor(hCursor);

		// 내가 쓸 윈도우를 등록
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc; // 프로시저
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //LoadCursorFromFile(L"cursor.cur");
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = gamename;
		wcex.hIconSm = NULL;

		RegisterClassExW(&wcex);

		// 그 윈도우를 생성
		hWnd = CreateWindowW(
			gamename,
			gamename,
			screenresizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX), // WS_EX_TOPMOST | WS_POPUP,
			0,
			0,
			width,
			height,
			NULL,
			NULL,
			hInstance,
			NULL);

		SetMenu(hWnd, NULL);

		resolutionX = width;
		resolutionY = height;
		lastWidth = width;
		lastHeight = height;

		isFullScreenMode = false;
		resizable = screenresizeable;

		timeController = new TimeController("INITIALIZATION_START", "PHYSICS_LOOP_START");
		sceneManager = new SceneManager();

		sceneManager->Initialize(timeController);

		GraphicsSystem::GetInstance().Initialize(hWnd);
		Input::GetInstance().Initialize(hWnd);
		PhysicsSystem::GetInstance().Initialize(timeController);
	}

	void CoreSystem::Finalize()
	{
		delete sceneManager;
		delete timeController;
	}

	void CoreSystem::Run(_In_ int nCmdShow)
	{
		// 윈도우 핸들이 가장 중요
		if (!hWnd)
		{
			return;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		// 무한루프 (안에 메시지 루프와 게임 루프 등이 포함됨)
		MSG msg;

		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				DispatchMessage(&msg);
			}
			else
			{
				// 엔진 동작
				run();
			}
		}
	}

	SceneManager* CoreSystem::GetSceneManager()
	{
		return sceneManager;
	}

	Vector2D CoreSystem::GetCurrentWindowSize()
	{
		// 멤버함수로 옮겨야함
		bool fullscreen = false;
		
		if (fullscreen)
		{
			return Vector2D(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		}
		else
		{
			RECT r;
			GetClientRect(hWnd, &r);

			return Vector2D(r.right, r.bottom);
		}
	}

	Vector2D CoreSystem::GetOriginWindowSize()
	{
		return Vector2D(resolutionX, resolutionY);
	}

	void CoreSystem::ChangeToFullScreenMode()
	{
		if (isFullScreenMode == true)
		{
			return;
		}
		isFullScreenMode = true;

		RECT r;
		GetClientRect(hWnd, &r);
		lastWidth = r.right - r.left;
		lastHeight = r.bottom - r.top;

		SetWindowLongPtr(hWnd, GWL_STYLE, WS_EX_TOPMOST | WS_POPUP); //3d argument=style
		SetWindowPos(hWnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	}

	void CoreSystem::ChangeToWindowMode()
	{
		if (isFullScreenMode == false)
		{
			return;
		}
		isFullScreenMode = false;

		SetWindowLongPtr(hWnd, GWL_STYLE, resizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)); //3d argument=style
		SetWindowPos(hWnd, NULL, (GetSystemMetrics(SM_CXSCREEN) - lastWidth + 16) / 2, (GetSystemMetrics(SM_CYSCREEN) - lastHeight + 39) / 2, lastWidth + 16, lastHeight + 39, SWP_SHOWWINDOW);
	}

	void CoreSystem::ChangeWindowSize(unsigned int width, unsigned int height)
	{
		if (isFullScreenMode == true || (lastWidth == width && lastHeight == height))
		{
			return;
		}
		lastWidth = width;
		lastHeight = height;

		SetWindowLongPtr(hWnd, GWL_STYLE, resizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)); //3d argument=style
		SetWindowPos(hWnd, NULL, (GetSystemMetrics(SM_CXSCREEN) - width + 16) / 2, (GetSystemMetrics(SM_CYSCREEN) - height + 39) / 2, width + 16, height + 39, SWP_SHOWWINDOW);
	}

	void CoreSystem::SetResizable(bool value)
	{
		if (resizable == value)
		{
			return;
		}

		resizable = value;

		if (isFullScreenMode == false)
		{
			ChangeToFullScreenMode();
			ChangeToWindowMode();
		}
	}

	unsigned int CoreSystem::GetScreenWidth()
	{
		return lastWidth;
	}

	unsigned int CoreSystem::GetScreenHeight()
	{
		return lastHeight;
	}

	bool CoreSystem::IsFullScreenMode()
	{
		return isFullScreenMode;
	}

	void CoreSystem::run()
	{
		if (runningState == RunningState::INITIALIZATION)
		{
			timeController->Update("INITIALIZATION_START");

			if (sceneManager->nextSceneQueue.empty() == false)
			{
				sceneManager->ChangeScene(sceneManager->nextSceneQueue.front());
				sceneManager->nextSceneQueue.pop();
			}

			sceneManager->InitializeEvent();

			runningState = RunningState::DESTROY;
		}
		else if (runningState == RunningState::DESTROY)
		{
			sceneManager->DestroyEvent();

			runningState = RunningState::PHYSICS;
		}
		else if (runningState == RunningState::PHYSICS)
		{
			timeController->Update("PHYSICS_LOOP_START");

			PhysicsSystem::GetInstance().PrePhysicsEvent();
			PhysicsSystem::GetInstance().CollisionDetection();
			PhysicsSystem::GetInstance().PhysicsEvent();
			PhysicsSystem::GetInstance().CollisionEvent();

			// 씬의 PhysicsEvent는 현재 아무것도 하지 않음
			sceneManager->PhysicsEvent();

			if (firstRun) // || time
			{
				timeController->Update("PHYSICS_LOOP_END");

				runningState = RunningState::INPUTEVENT;
			}
			else
			{
				float timedif = timeController->GetTimeDif("PHYSICS_LOOP_END");
				float physicslooptime = timeController->GetTimeDif("PHYSICS_LOOP_START");
				if (timedif + max(physicslooptime, timeController->GetPhysicsLoopTime()) > (1.0f / (float)timeController->GetFPS()))
				{
					timeController->Update("PHYSICS_LOOP_END");

					runningState = RunningState::INPUTEVENT;
				}
				else
				{
					if (physicslooptime < timeController->GetPhysicsLoopTime())
					{
						runningState = RunningState::PHYSICSWAIT;
					}
				}
			}
		}
		else if (runningState == RunningState::PHYSICSWAIT)
		{
			if (timeController->GetTimeDif("PHYSICS_LOOP_START") >= timeController->GetPhysicsLoopTime())
			{
				runningState = RunningState::PHYSICS;
			}
		}
		else if (runningState == RunningState::INPUTEVENT)
		{
			Input::GetInstance().Update();

			if (EngineSetting::GetInstance().GetDebugToggleKeyAllowed() && Input::GetInstance().PressedKey(VK_CONTROL) && Input::GetInstance().PressedKeyNow(VK_F12))
			{
				EngineSetting::GetInstance().SetShowDebugInfo(!EngineSetting::GetInstance().GetShowDebugInfo());
			}

			runningState = RunningState::GAMELOGIC;
		}
		else if (runningState == RunningState::GAMELOGIC)
		{
			sceneManager->EarlyUpdateEvent();
			sceneManager->UpdateEvent();

			AnimationSystem::GetInstance().ExecuteAllAnimator();

			sceneManager->LateUpdateEvent();

			runningState = RunningState::GUIEVENT;
		}
		else if (runningState == RunningState::GUIEVENT)
		{
			UISystem::GetInstance().GUIEvent();

			runningState = RunningState::RENDER;
		}
		else if (runningState == RunningState::RENDER)
		{
			sceneManager->RenderEvent();
			GraphicsSystem::GetInstance().Render();

			firstRun = false;
			timeController->CountFrameNumber();
			runningState = RunningState::INITIALIZATION;
		}
	}

	LRESULT CoreSystem::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_MOUSEWHEEL:
		{
			Input::GetInstance().updateWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			EndPaint(hWnd, &ps);
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		//case WM_SYSCOMMAND:
		//{
		//	if ((wParam & 0xFFF0) == SC_KEYMENU)
		//	{
		//		return 0;
		//	}
		//	break;
		//}

		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_MENU)
			{
				
				return 0;
			}

			if ((wParam & 0xFFF0) == SC_KEYMENU)
			{
				return 0;
			}

			break;
		}

		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);

			GraphicsSystem::GetInstance().ResizeWindow(width, height);
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		return 0;
	}

	CoreSystem& CoreSystem::GetInstance()
	{
		static CoreSystem instance;
		return instance;
	}

	CoreSystem::CoreSystem() : firstRun(true), runningState(RunningState::INITIALIZATION)
	{

	}

	CoreSystem::~CoreSystem()
	{
		
	}
}