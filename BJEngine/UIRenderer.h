#pragma once

#ifdef UIRENDERER_EXPORTS
#define UIRENDERER_API __declspec(dllexport)
#else
#define UIRENDERER_API __declspec(dllimport)
#endif

#include "Component.h"

namespace BJEngine
{
    namespace GraphicsEngine
    {
        class Graphics;
    }

    class UIRenderer : public Component
    {
    public:
        UIRENDERER_API UIRenderer();
        UIRENDERER_API virtual ~UIRenderer();

        UIRENDERER_API virtual void Render(GraphicsEngine::Graphics* graphics) abstract;

        UIRENDERER_API void SetLayer(int layer);
        UIRENDERER_API int GetLayer() const;

    private:
        int drawLayer;
    };
}