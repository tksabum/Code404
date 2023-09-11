#pragma once

#ifdef DIRECT2DGRAPHICS_EXPORTS
#define DIRECT2DGRAPHICS_API __declspec(dllexport)
#else
#define DIRECT2DGRAPHICS_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <dwrite.h>
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "Dwrite.lib")

#include <vector>
#include "Graphics.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Sprite;
		class Direct2DSprite;
		class Direct2DSpriteSheet;

		class Direct2DGraphics : public Graphics
		{
		private:
			Color backColor; // 배경색
			ID2D1Factory* factory; // Direct2D factory
			ID2D1HwndRenderTarget* renderTarget; // Direct2D renderTarget
			IDWriteFactory* writeFactory; // Direct write factory
			IWICImagingFactory* imagingFactory;

			// 포인터가 nullptr이 아닌경우 Release
			template<class T> void safeRelease(T** ppT);

		public:
			DIRECT2DGRAPHICS_API Direct2DGraphics(HWND hWnd, Color backcolor = Color::White);
			DIRECT2DGRAPHICS_API ~Direct2DGraphics();

			// 그리기를 시작한다.
			DIRECT2DGRAPHICS_API void BeginDraw() override;
			// 그리기를 끝낸다.
			DIRECT2DGRAPHICS_API void EndDraw() override;
			// 창 크기를 변경한다.
			DIRECT2DGRAPHICS_API void Resize(unsigned int width, unsigned int height);

			// 타원을 내부를 채워서 그린다.
			DIRECT2DGRAPHICS_API void FillEllipse(float x, float y, float radiusX, float radiusY, Color color = Color::Black) override;
			// 각각의 변이 x, y축과 평행 또는 수직인 직사각형을 내부를 채워 그린다.
			DIRECT2DGRAPHICS_API void FillRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) override;

			// 선분을 그린다.
			DIRECT2DGRAPHICS_API void DrawLine(Math::Vector2D point0, Math::Vector2D point1, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			// 타원을 그린다.
			DIRECT2DGRAPHICS_API void DrawEllipse(float x, float y, float radiusX, float radiusY, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			// 각각의 변이 x, y축과 평행 또는 수직인 직사각형을 그린다.
			DIRECT2DGRAPHICS_API void DrawRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) override;
			// 다각형을 그린다.
			DIRECT2DGRAPHICS_API void DrawPolygon(const std::vector<Math::Vector2D>& points, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			
			// 글자를 그린다.
			DIRECT2DGRAPHICS_API void DrawString(std::wstring text, float x, float y, float sizeX, float sizeY, float scaleX, float scaleY, float rotation, int fontsize, Color color = Color::Black, std::wstring font = L"consolas") override;


			DIRECT2DGRAPHICS_API void DrawSprite(Sprite* sprite, int spriteNumber, float x, float y, float scaleX, float scaleY, float rotation, float opacity = 1.0f) override;

			DIRECT2DGRAPHICS_API void Test();

			friend Direct2DSprite;
			friend Direct2DSpriteSheet;
		};
	}
}