#include "Image.h"

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

	Image::Image() : spriteName(L""), imageNumber(0), imageOpacity(1.0f), flipX(false), flipY(false)
	{

	}

	Image::~Image()
	{

	}

	void Image::Render(GraphicsEngine::Graphics* graphics)
	{
		Transform* transform = GetGameObject()->GetTransform();

		Vector2D screenScale = transform->GetUIScale();
		Vector2D screenPosition = transform->GetUIPosition() + Vector2D((GetSprite()->GetCutSizeX() * screenScale.x) / 2.0f, (GetSprite()->GetCutSizeY() * screenScale.y) / 2.0f);
		float screenRotation = transform->GetUIRotation();

		Vector2D flipvector(1.0f, 1.0f);

		if (flipX)
		{
			flipvector.x = -1.0f;
		}

		if (flipY)
		{
			flipvector.y = -1.0f;
		}

		graphics->DrawSprite(GetSprite(), GetImageNumber(), screenPosition.x, screenPosition.y, flipvector.x * screenScale.x, flipvector.y * screenScale.y, screenRotation, imageOpacity);
	}

	void Image::SetSpriteName(std::wstring spritename)
	{
		spriteName = spritename;
	}

	GraphicsEngine::Sprite* Image::GetSprite()
	{
		return GraphicsResourceManager::GetInstance().GetSprite(spriteName);
	}

	void Image::SetImageNumber(int imagenumber)
	{
		imageNumber = imagenumber;
	}

	int Image::GetImageNumber()
	{
		return imageNumber;
	}

	bool Image::GetFlipX()
	{
		return flipX;
	}

	void Image::SetFlipX(bool flipx)
	{
		flipX = flipx;
	}

	bool Image::GetFlipY()
	{
		return flipY;
	}

	void Image::SetFlipY(bool flipy)
	{
		flipY = flipy;
	}

	void Image::SetOpacity(float opacity)
	{
		imageOpacity = opacity;
	}

	float Image::GetOpacity()
	{
		return imageOpacity;
	}
}