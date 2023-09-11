#pragma once

#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Color.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Sprite;

		class Graphics
		{
		public:
			// �׸��⸦ �����Ѵ�.
			GRAPHICS_API virtual void BeginDraw() abstract;
			// �׸��⸦ ������.
			GRAPHICS_API virtual void EndDraw() abstract;
			// â ũ�⸦ �����Ѵ�.
			GRAPHICS_API virtual void Resize(unsigned int width, unsigned int height) abstract;

			// ### ���� ###
			// Fill
			
			// Ÿ���� ���θ� ä���� �׸���.
			GRAPHICS_API virtual void FillEllipse(float x, float y, float radiusX, float radiusY, Color color = Color::Black) abstract;
			// ������ ���� x, y��� ���� �Ǵ� ������ ���簢���� ���θ� ä�� �׸���.
			GRAPHICS_API virtual void FillRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) abstract;

			// Draw
			
			// ������ �׸���.
			GRAPHICS_API virtual void DrawLine(Math::Vector2D point0, Math::Vector2D point1, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) abstract;
			// Ÿ���� �׸���.
			GRAPHICS_API virtual void DrawEllipse(float x, float y, float radiusX, float radiusY, float scaleX, float scaleY, float rotation, Color color = Color::Black) abstract;
			// ������ ���� x, y��� ���� �Ǵ� ������ ���簢���� �׸���.
			GRAPHICS_API virtual void DrawRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) abstract;
			// �ٰ����� �׸���.
			GRAPHICS_API virtual void DrawPolygon(const std::vector<Math::Vector2D>& points, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) abstract;
			

			// ### ���� ###

			// ���ڸ� �׸���.
			GRAPHICS_API virtual void DrawString(std::wstring text, float x, float y, float sizeX, float sizeY, float scaleX, float scaleY, float rotation, int fontsize, Color color = Color::Black, std::wstring font = L"consolas") abstract;


			// ### �׸� ###

			// sprite�� �׸���.
			GRAPHICS_API virtual void DrawSprite(Sprite* sprite, int spriteNumber, float x, float y, float scaleX, float scaleY, float rotation, float opacity = 1.0f) abstract;

		};
	}
}