#include "SpriteRenderer.h"

#include "GraphicsSystem.h"

#include "GraphicsResourceManager.h"
#include "Sprite.h"

#include "Direct2DGraphics.h"

#include "Vector2D.h"
#include "Transform.h"
#include "GameObject.h"

namespace BJEngine
{
	using namespace Math;

	SpriteRenderer::SpriteRenderer() : spriteName(L""), imageNumber(0), imageOpacity(1.0f), flipX(false), flipY(false)
	{

	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::Render(GraphicsEngine::Graphics* graphics)
	{
		Transform* transform = GetGameObject()->GetTransform();

		Vector2D screenPosition = transform->Get2DScreenPosition();
		Vector2D screenScale = transform->Get2DScreenScale();
		float screenRotation = transform->Get2DScreenRotation();

		Vector2D flipvector(1.0f, 1.0f);

		if (flipX)
		{
			flipvector.x = -1.0f;
		}

		if (flipY)
		{
			flipvector.y = -1.0f;
		}

		GraphicsEngine::Sprite* sprite = GetSprite();

		if (sprite != nullptr)
		{
			graphics->DrawSprite(sprite, GetImageNumber(), screenPosition.x, screenPosition.y, flipvector.x * screenScale.x, flipvector.y * screenScale.y, screenRotation, imageOpacity);
		}

	}

	void SpriteRenderer::SetSpriteName(std::wstring spritename)
	{
		spriteName = spritename;
	}

	GraphicsEngine::Sprite* SpriteRenderer::GetSprite()
	{
		return GraphicsResourceManager::GetInstance().GetSprite(spriteName);
	}

	void SpriteRenderer::SetImageNumber(int imagenumber)
	{
		imageNumber = imagenumber;
	}
	
	int SpriteRenderer::GetImageNumber()
	{
		return imageNumber;
	}
	
	bool SpriteRenderer::GetFlipX()
	{
		return flipX;
	}
	
	void SpriteRenderer::SetFlipX(bool flipx)
	{
		flipX = flipx;
	}
	
	bool SpriteRenderer::GetFlipY()
	{
		return flipY;
	}
	
	void SpriteRenderer::SetFlipY(bool flipy)
	{
		flipY = flipy;
	}

	void SpriteRenderer::SetOpacity(float opacity)
	{
		imageOpacity = opacity;
	}
	
	float SpriteRenderer::GetOpacity()
	{
		return imageOpacity;
	}
}