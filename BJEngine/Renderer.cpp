#include "Renderer.h"

#include "GraphicsSystem.h"

namespace BJEngine
{
	Renderer::Renderer() : drawLayer(0)
	{
		GraphicsSystem::GetInstance().AddRenderer(this);
	}

	Renderer::~Renderer()
	{
		GraphicsSystem::GetInstance().RemoveRenderer(this);
	}

	void Renderer::SetLayer(int layer)
	{
		drawLayer = layer;
	}

	int Renderer::GetLayer() const
	{
		return drawLayer;
	}
}