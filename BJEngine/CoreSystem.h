#pragma once

#ifdef CORESYSTEM_EXPORTS
#define CORESYSTEM_API __declspec(dllexport)
#else
#define CORESYSTEM_API __declspec(dllimport)
#endif

#include <Windows.h>

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class SceneManager;
	class TimeController;

	namespace GraphicsEngine
	{
		class Sprite;
		class SpriteSheet;
		class Graphics;
	}

	class CoreSystem
	{
	public:
		// Initialize Coresystem and Subsystem
		CORESYSTEM_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, unsigned int width, unsigned int height, bool screenresizeable = true);

		CORESYSTEM_API void Finalize();

		CORESYSTEM_API void Run(_In_ int nCmdShow);

		CORESYSTEM_API SceneManager* GetSceneManager();

		CORESYSTEM_API Math::Vector2D GetCurrentWindowSize();
		CORESYSTEM_API Math::Vector2D GetOriginWindowSize();

		CORESYSTEM_API void ChangeToFullScreenMode();
		CORESYSTEM_API void ChangeToWindowMode();
		CORESYSTEM_API void ChangeWindowSize(unsigned int width, unsigned int height);
		
		CORESYSTEM_API void SetResizable(bool value);

		CORESYSTEM_API unsigned int GetScreenWidth();
		CORESYSTEM_API unsigned int GetScreenHeight();
		CORESYSTEM_API bool IsFullScreenMode();

	private:
		enum class RunningState
		{
			INITIALIZATION,
			DESTROY,
			PHYSICS,
			PHYSICSWAIT,
			INPUTEVENT,
			GAMELOGIC,
			GUIEVENT,
			RENDER
		};

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void run();

		HWND hWnd;

		bool isFullScreenMode;
		bool resizable;

		bool firstRun;
		RunningState runningState;
		SceneManager* sceneManager;
		TimeController* timeController;

		// ÇØ»óµµ
		unsigned int resolutionX;
		unsigned int resolutionY;
		unsigned int lastWidth;
		unsigned int lastHeight;

		HCURSOR hCursor;

		//singleton
	public:
		CORESYSTEM_API static CoreSystem& GetInstance();

	private:
		CoreSystem();
		~CoreSystem();
		CoreSystem(const CoreSystem& ref);
		CoreSystem& operator=(const CoreSystem& ref) {}
	};
}