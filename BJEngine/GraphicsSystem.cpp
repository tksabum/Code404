#include "GraphicsSystem.h"

#include <algorithm>

#include "EngineSetting.h"

#include "Direct2DGraphics.h"

#include "SpriteRenderer.h"
#include "UIRenderer.h"

#include "GraphicsResourceManager.h"

#include "GameObject.h"
#include "Transform.h"

#include "PhysicsSystem.h"

#include "Direct2DDevice.h"

using namespace std;

namespace BJEngine
{
	using namespace GraphicsEngine;
	using namespace Math;

	void GraphicsSystem::Initialize(HWND hwnd)
	{
		EngineSetting* engineSetting = &EngineSetting::GetInstance();

		if (graphics == nullptr)
		{
			if (engineSetting->engineMode == EngineSetting::Mode::MODE_2D)
			{
				if (engineSetting->graphicsEngine == EngineSetting::GraphicsEngine::DIRECT2D_GRAPHICS)
				{
					graphics = new Direct2DGraphics(hwnd);
				}
				else if((engineSetting->graphicsEngine == EngineSetting::GraphicsEngine::DIRECT2D_DC))
				{
					graphics = new Direct2DDevice(hwnd);
				}
			}
		}

		RECT rect;
		GetWindowRect(hwnd, &rect);
		windowSize = Vector2D(rect.right - rect.left, rect.bottom - rect.top);
	}

	Vector2D GraphicsSystem::GetWindowSize()
	{
		return windowSize;
	}

	void GraphicsSystem::ResizeWindow(unsigned int width, unsigned int height)
	{
		windowSize.x = width;
		windowSize.y = height;
		graphics->Resize(width, height);
	}

	void GraphicsSystem::Render()
	{
		if (isRendererSorted == false)
		{
			isRendererSorted = true;

			sort(rendererList.begin(), rendererList.end(), [](Renderer* a, Renderer* b)
				{
					return (a->GetLayer()) < (b->GetLayer());
				});
		}

		if (isUIRendererSorted == false)
		{
			isUIRendererSorted = true;

			sort(uiRendererList.begin(), uiRendererList.end(), [](UIRenderer* a, UIRenderer* b)
				{
					return (a->GetLayer()) < (b->GetLayer());
				});
		}

		graphics->BeginDraw();

		for (int i = 0; i < rendererList.size(); i++)
		{
			if (rendererList[i]->GetGameObject()->GetRootActive())
			{
				rendererList[i]->Render(graphics);
			}
		}

		if (EngineSetting::GetInstance().GetShowDebugInfo() == true)
		{
			PhysicsSystem::GetInstance().RenderDebuggingInfo();
		}

		for (int i = 0; i < uiRendererList.size(); i++)
		{
			if (uiRendererList[i]->GetGameObject()->GetRootActive())
			{
				uiRendererList[i]->Render(graphics);
			}
		}


		graphics->EndDraw();
	}

	void GraphicsSystem::AddRenderer(Renderer* renderer)
	{
		isRendererSorted = false;

		rendererList.push_back(renderer);
	}

	void GraphicsSystem::RemoveRenderer(Renderer* renderer)
	{
		for (vector<Renderer*>::iterator iter = rendererList.begin(); iter != rendererList.end(); iter++)
		{
			if (*iter == renderer)
			{
				rendererList.erase(iter);
				break;
			}
		}
	}

	void GraphicsSystem::AddUIRenderer(UIRenderer* uirenderer)
	{
		isUIRendererSorted = false;

		uiRendererList.push_back(uirenderer);
	}

	void GraphicsSystem::RemoveUIRenderer(UIRenderer* uirenderer)
	{
		for (vector<UIRenderer*>::iterator iter = uiRendererList.begin(); iter != uiRendererList.end(); iter++)
		{
			if (*iter == uirenderer)
			{
				uiRendererList.erase(iter);
				break;
			}
		}
	}

	GraphicsSystem& GraphicsSystem::GetInstance()
	{
		static GraphicsSystem instance;
		return instance;
	}

	GraphicsSystem::GraphicsSystem() : graphics(nullptr), isRendererSorted(false), isUIRendererSorted(false), windowSize(Vector2D())
	{
		
	}

	GraphicsSystem::~GraphicsSystem()
	{

	}
}