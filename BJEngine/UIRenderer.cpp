#include "UIRenderer.h"

#include "GraphicsSystem.h"

namespace BJEngine
{
	UIRenderer::UIRenderer() : drawLayer(0)
	{
		GraphicsSystem::GetInstance().AddUIRenderer(this);
	}

	UIRenderer::~UIRenderer()
	{
		GraphicsSystem::GetInstance().RemoveUIRenderer(this);
	}

	void UIRenderer::SetLayer(int layer)
	{
		drawLayer = layer;
	}

	int UIRenderer::GetLayer() const
	{
		return drawLayer;
	}
}