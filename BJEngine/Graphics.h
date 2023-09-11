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
			// 그리기를 시작한다.
			GRAPHICS_API virtual void BeginDraw() abstract;
			// 그리기를 끝낸다.
			GRAPHICS_API virtual void EndDraw() abstract;
			// 창 크기를 변경한다.
			GRAPHICS_API virtual void Resize(unsigned int width, unsigned int height) abstract;

			// ### 도형 ###
			// Fill
			
			// 타원을 내부를 채워서 그린다.
			GRAPHICS_API virtual void FillEllipse(float x, float y, float radiusX, float radiusY, Color color = Color::Black) abstract;
			// 각각의 변이 x, y축과 평행 또는 수직인 직사각형을 내부를 채워 그린다.
			GRAPHICS_API virtual void FillRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) abstract;

			// Draw
			
			// 선분을 그린다.
			GRAPHICS_API virtual void DrawLine(Math::Vector2D point0, Math::Vector2D point1, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) abstract;
			// 타원을 그린다.
			GRAPHICS_API virtual void DrawEllipse(float x, float y, float radiusX, float radiusY, float scaleX, float scaleY, float rotation, Color color = Color::Black) abstract;
			// 각각의 변이 x, y축과 평행 또는 수직인 직사각형을 그린다.
			GRAPHICS_API virtual void DrawRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) abstract;
			// 다각형을 그린다.
			GRAPHICS_API virtual void DrawPolygon(const std::vector<Math::Vector2D>& points, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) abstract;
			

			// ### 글자 ###

			// 글자를 그린다.
			GRAPHICS_API virtual void DrawString(std::wstring text, float x, float y, float sizeX, float sizeY, float scaleX, float scaleY, float rotation, int fontsize, Color color = Color::Black, std::wstring font = L"consolas") abstract;


			// ### 그림 ###

			// sprite를 그린다.
			GRAPHICS_API virtual void DrawSprite(Sprite* sprite, int spriteNumber, float x, float y, float scaleX, float scaleY, float rotation, float opacity = 1.0f) abstract;

		};
	}
}