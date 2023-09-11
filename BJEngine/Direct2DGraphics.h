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
			Color backColor; // ����
			ID2D1Factory* factory; // Direct2D factory
			ID2D1HwndRenderTarget* renderTarget; // Direct2D renderTarget
			IDWriteFactory* writeFactory; // Direct write factory
			IWICImagingFactory* imagingFactory;

			// �����Ͱ� nullptr�� �ƴѰ�� Release
			template<class T> void safeRelease(T** ppT);

		public:
			DIRECT2DGRAPHICS_API Direct2DGraphics(HWND hWnd, Color backcolor = Color::White);
			DIRECT2DGRAPHICS_API ~Direct2DGraphics();

			// �׸��⸦ �����Ѵ�.
			DIRECT2DGRAPHICS_API void BeginDraw() override;
			// �׸��⸦ ������.
			DIRECT2DGRAPHICS_API void EndDraw() override;
			// â ũ�⸦ �����Ѵ�.
			DIRECT2DGRAPHICS_API void Resize(unsigned int width, unsigned int height);

			// Ÿ���� ���θ� ä���� �׸���.
			DIRECT2DGRAPHICS_API void FillEllipse(float x, float y, float radiusX, float radiusY, Color color = Color::Black) override;
			// ������ ���� x, y��� ���� �Ǵ� ������ ���簢���� ���θ� ä�� �׸���.
			DIRECT2DGRAPHICS_API void FillRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) override;

			// ������ �׸���.
			DIRECT2DGRAPHICS_API void DrawLine(Math::Vector2D point0, Math::Vector2D point1, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			// Ÿ���� �׸���.
			DIRECT2DGRAPHICS_API void DrawEllipse(float x, float y, float radiusX, float radiusY, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			// ������ ���� x, y��� ���� �Ǵ� ������ ���簢���� �׸���.
			DIRECT2DGRAPHICS_API void DrawRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) override;
			// �ٰ����� �׸���.
			DIRECT2DGRAPHICS_API void DrawPolygon(const std::vector<Math::Vector2D>& points, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			
			// ���ڸ� �׸���.
			DIRECT2DGRAPHICS_API void DrawString(std::wstring text, float x, float y, float sizeX, float sizeY, float scaleX, float scaleY, float rotation, int fontsize, Color color = Color::Black, std::wstring font = L"consolas") override;


			DIRECT2DGRAPHICS_API void DrawSprite(Sprite* sprite, int spriteNumber, float x, float y, float scaleX, float scaleY, float rotation, float opacity = 1.0f) override;

			DIRECT2DGRAPHICS_API void Test();

			friend Direct2DSprite;
			friend Direct2DSpriteSheet;
		};
	}
}