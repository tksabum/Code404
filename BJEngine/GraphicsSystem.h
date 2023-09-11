#pragma once

#ifdef GRAPHICSSYSTEM_EXPORTS
#define GRAPHICSSYSTEM_API __declspec(dllexport)
#else
#define GRAPHICSSYSTEM_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <vector>

#include "Vector2D.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Graphics;
		class Direct2DSpriteSheet;
	}

	class CoreSystem;
	class GraphicsResourceManager;
	class Renderer;
	class UIRenderer;
	class Camera;
	class PhysicsSystem;


	class GraphicsSystem
	{
	public:
		GRAPHICSSYSTEM_API void Initialize(HWND hwnd);
		GRAPHICSSYSTEM_API Math::Vector2D GetWindowSize();

		// �׷��� �ý����� âũ�⸦ �����Ѵ�. (���� âũ�⸦ �ٲٴ� �Լ��� �ƴ�)
		GRAPHICSSYSTEM_API void ResizeWindow(unsigned int width, unsigned int height);

	private:
		void Render();
		void AddRenderer(Renderer* spriterenderer);
		void RemoveRenderer(Renderer* spriterenderer);
		
		void AddUIRenderer(UIRenderer* uirenderer);
		void RemoveUIRenderer(UIRenderer* uirenderer);

		GraphicsEngine::Graphics* graphics;

		bool isRendererSorted;
		bool isUIRendererSorted;

		std::vector<Renderer*> rendererList;
		std::vector<UIRenderer*> uiRendererList;

		Math::Vector2D windowSize;

		friend GraphicsResourceManager;
		friend CoreSystem;
		friend Renderer;
		friend UIRenderer;
		friend PhysicsSystem;
		friend GraphicsEngine::Direct2DSpriteSheet;


		//singleton
	public:
		GRAPHICSSYSTEM_API static GraphicsSystem& GetInstance();

	private:
		GraphicsSystem();
		~GraphicsSystem();
		GraphicsSystem(const GraphicsSystem& ref);
		GraphicsSystem& operator=(const GraphicsSystem& ref) {}
	};
}