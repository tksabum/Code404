#pragma once

#ifdef RENDERER_EXPORTS
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif

#include "Component.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Graphics;
	}

	class Renderer : public Component
	{
	public:
		RENDERER_API Renderer();
		RENDERER_API virtual ~Renderer();

		RENDERER_API virtual void Render(GraphicsEngine::Graphics* graphics) abstract;

		RENDERER_API void SetLayer(int layer);
		RENDERER_API int GetLayer() const;

	private:
		int drawLayer;

	};
}