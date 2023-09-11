#pragma once

#ifdef UISYSTEM_EXPORTS
#define UISYSTEM_API __declspec(dllexport)
#else
#define UISYSTEM_API __declspec(dllimport)
#endif

#include <vector>

namespace BJEngine
{
	class CoreSystem;
	class UI;

	class UISystem
	{
	public:
		UISYSTEM_API void AddUI(UI* ui);
		UISYSTEM_API void RemoveUI(UI* ui);

	private:
		std::vector<UI*> uiList;

		std::vector<bool> lastMouseOver;
		std::vector<bool> currentMouseOver;

		void GUIEvent();

		//singleton
	public:
		UISYSTEM_API static UISystem& GetInstance();

	private:
		UISystem();
		~UISystem();
		UISystem(const UISystem& ref);
		UISystem& operator=(const UISystem& ref) {}

		friend CoreSystem;
	};
}