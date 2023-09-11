#pragma once

#ifdef DIRECT2DDEVICE_EXPORTS
#define DIRECT2DGRAPHICSDC_API __declspec(dllexport)
#else
#define DIRECT2DGRAPHICSDC_API __declspec(dllimport)
#endif

//#include <Windows.h>
//
//#include <wrl.h>
//#include <d2d1.h>
//#include <d2d1_1.h>
//#include <d2d1_1helper.h>
//#include <d3d11.h>
//#include <dxgi1_2.h>
//#pragma comment(lib, "D2D1.lib")

#include <windows.h>
#include <vector>
#include <string>
#include <d2d1.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <dxgi1_2.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl.h>
#include <d2d1effectauthor.h>
#include <d2d1effects_2.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <d2d1_3.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <fstream>
#include <DirectXMath.h>

#include "Graphics.h"

using namespace Microsoft::WRL;

namespace BJEngine
{
    namespace GraphicsEngine
    {
		class Sprite;
		class Direct2DSprite;
		class Direct2DSpriteSheet;
		class DCSpriteSheet;

		class Direct2DDevice :
			public Graphics
		{
		private:
			ID2D1Factory1* factory;
			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11DeviceContext> context;
			ComPtr<ID2D1Device> d2dDevice;
			ComPtr<ID2D1DeviceContext> d2dDc;
			ComPtr<IDXGISwapChain1> swapChain;

			IDWriteFactory* writeFactory; // Direct write factory
			IWICImagingFactory* imagingFactory;

			// �����Ͱ� nullptr�� �ƴѰ�� Release
			template<class T> void safeRelease(T** ppT);

		public:
			// ������ ���� x, y��� ���� �Ǵ� ������ ���簢���� �׸���.
			DIRECT2DGRAPHICSDC_API void DrawRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black);

			// �׸��⸦ �����Ѵ�.
			DIRECT2DGRAPHICSDC_API void BeginDraw() override;
			// �׸��⸦ ������.
			DIRECT2DGRAPHICSDC_API void EndDraw() override;
			// â ũ�⸦ �����Ѵ�.
			DIRECT2DGRAPHICSDC_API void Resize(unsigned int width, unsigned int height);

			// Ÿ���� ���θ� ä���� �׸���.
			DIRECT2DGRAPHICSDC_API void FillEllipse(float x, float y, float radiusX, float radiusY, Color color = Color::Black) override;
			// ������ ���� x, y��� ���� �Ǵ� ������ ���簢���� ���θ� ä�� �׸���.
			DIRECT2DGRAPHICSDC_API void FillRect(float x, float y, float sizeX, float sizeY, Color color = Color::Black) override;

			// ������ �׸���.
			DIRECT2DGRAPHICSDC_API void DrawLine(Math::Vector2D point0, Math::Vector2D point1, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			// Ÿ���� �׸���.
			DIRECT2DGRAPHICSDC_API void DrawEllipse(float x, float y, float radiusX, float radiusY, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;
			// �ٰ����� �׸���.
			DIRECT2DGRAPHICSDC_API void DrawPolygon(const std::vector<Math::Vector2D>& points, float x, float y, float scaleX, float scaleY, float rotation, Color color = Color::Black) override;

			// ���ڸ� �׸���.
			DIRECT2DGRAPHICSDC_API void DrawString(std::wstring text, float x, float y, float sizeX, float sizeY, float scaleX, float scaleY, float rotation, int fontsize, Color color = Color::Black, std::wstring font = L"consolas") override;


			DIRECT2DGRAPHICSDC_API void DrawSprite(Sprite* sprite, int spriteNumber, float x, float y, float scaleX, float scaleY, float rotation, float opacity = 1.0f) override;

			DIRECT2DGRAPHICSDC_API void Test(ID2D1Bitmap* p_bitmap);

		public:
			DIRECT2DGRAPHICSDC_API Direct2DDevice(HWND hWnd, Color backcolor = Color::White);
			DIRECT2DGRAPHICSDC_API ~Direct2DDevice();

			friend Direct2DSprite;
			friend Direct2DSpriteSheet;
			friend DCSpriteSheet;
		};
    }
}
