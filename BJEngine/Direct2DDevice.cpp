#pragma comment (lib, "user32.lib")
#pragma comment(lib, "dxgi.lib") 
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")

#include "Direct2DDevice.h"

#include "DCSprite.h"
#include "DCSpriteSheet.h"

#include "Input.h"

using namespace std;
using namespace D2D1;
namespace BJEngine
{
	namespace GraphicsEngine
	{
		template<class T> void Direct2DDevice::safeRelease(T** ppT)
		{
			if (*ppT)
			{
				(*ppT)->Release();
				*ppT = NULL;
			}
		}

		Direct2DDevice::Direct2DDevice(HWND hWnd, Color backcolor)
			: factory(nullptr)
		{
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(&writeFactory));
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imagingFactory));

			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				(&factory)
			);

			UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				creationFlags,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				&device,
				nullptr,
				&context
			);

			ComPtr<IDXGIDevice> dxgiDevice;

			device.As(&dxgiDevice);

			factory->CreateDevice(dxgiDevice.Get(), &d2dDevice);

			d2dDevice->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				&d2dDc
			);

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = 0;                           // use automatic sizing
			swapChainDesc.Height = 0;
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
			swapChainDesc.Scaling = DXGI_SCALING_NONE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
			swapChainDesc.Flags = 0;

			// Identify the physical adapter (GPU or card) this device is runs on.
			ComPtr<IDXGIAdapter> dxgiAdapter;

			dxgiDevice->GetAdapter(&dxgiAdapter);

			ComPtr<IDXGIFactory2> dxgiFactory;
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

			dxgiFactory->CreateSwapChainForHwnd(
				device.Get(),
				hWnd,
				&swapChainDesc,
				0,
				nullptr,    // allow on all displays
				&swapChain
			);

			ComPtr<ID3D11Texture2D> backBuffer;
			swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

			D2D1_BITMAP_PROPERTIES1 bitmapProperties =
				BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
					1920,
					1080
				);

			ComPtr<IDXGISurface> dxgiBackBuffer;
			swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
			ComPtr<ID2D1Bitmap1> targetBitmap;

			d2dDc->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				&bitmapProperties,
				&targetBitmap
			);

			d2dDc->SetTarget(targetBitmap.Get());
		}

		Direct2DDevice::~Direct2DDevice()
		{

		}

		void Direct2DDevice::DrawRect(float x, float y, float sizeX, float sizeY, Color color /*= Color::Black*/)
		{
			ComPtr<ID2D1SolidColorBrush> pBlackBrush;

			d2dDc->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&pBlackBrush);

			//d2dDc->BeginDraw();

			D2D1_RECT_F rectangle;
			rectangle.left = x - sizeX / 2;
			rectangle.top = y - sizeY / 2;
			rectangle.right = x + sizeX / 2;
			rectangle.bottom = y + sizeY / 2;

			d2dDc->DrawRectangle(rectangle, pBlackBrush.Get());
		}

		void Direct2DDevice::BeginDraw()
		{
			d2dDc->BeginDraw();
			d2dDc->Clear(ColorF(1, 1, 1));
		}

		void Direct2DDevice::EndDraw()
		{
			d2dDc->EndDraw();
			DXGI_PRESENT_PARAMETERS parameter = { 0, NULL, NULL, NULL };
			swapChain->Present1(1, 0, &parameter);
		}

		void Direct2DDevice::Resize(unsigned int width, unsigned int height)
		{

		}

		void Direct2DDevice::FillEllipse(float x, float y, float radiusX, float radiusY, Color color /*= Color::Black*/)
		{
			D2D1_ELLIPSE ellipseRegion;
			ellipseRegion.point.x = x;
			ellipseRegion.point.y = y;
			ellipseRegion.radiusX = radiusX;
			ellipseRegion.radiusY = radiusY;

			ID2D1SolidColorBrush* brush;
			HRESULT hr = d2dDc->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				d2dDc->FillEllipse(ellipseRegion, brush);
				brush->Release();
			}
		}

		void Direct2DDevice::FillRect(float x, float y, float sizeX, float sizeY, Color color /*= Color::Black*/)
		{
			D2D1_RECT_F rectRegion;
			rectRegion.left = x;
			rectRegion.top = y;
			rectRegion.right = x + sizeX;
			rectRegion.bottom = y + sizeY;

			ID2D1SolidColorBrush* brush;
			HRESULT hr = d2dDc->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				d2dDc->FillRectangle(rectRegion, brush);
				brush->Release();
			}
		}

		void Direct2DDevice::DrawLine(Math::Vector2D point0, Math::Vector2D point1, float x, float y, float scaleX, float scaleY, float rotation, Color color /*= Color::Black*/)
		{
			D2D1_POINT_2F d2dpoint0 = Point2F(point0.x, point0.y);
			D2D1_POINT_2F d2dpoint1 = Point2F(point1.x, point1.y);

			ID2D1SolidColorBrush* brush;
			HRESULT hr = d2dDc->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				d2dDc->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				d2dDc->DrawLine(d2dpoint0, d2dpoint1, brush);

				d2dDc->SetTransform(Matrix3x2F::Identity());

				brush->Release();
			}
		}

		void Direct2DDevice::DrawEllipse(float x, float y, float radiusX, float radiusY, float scaleX, float scaleY, float rotation, Color color /*= Color::Black*/)
		{
			D2D1_ELLIPSE ellipseRegion;
			ellipseRegion.point.x = 0.0f;
			ellipseRegion.point.y = 0.0f;
			ellipseRegion.radiusX = radiusX;
			ellipseRegion.radiusY = radiusY;

			ID2D1SolidColorBrush* brush;
			HRESULT hr = d2dDc->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				d2dDc->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				d2dDc->DrawEllipse(ellipseRegion, brush);

				d2dDc->SetTransform(Matrix3x2F::Identity());

				brush->Release();
			}
		}

		void Direct2DDevice::DrawPolygon(const std::vector<Math::Vector2D>& points, float x, float y, float scaleX, float scaleY, float rotation, Color color /*= Color::Black*/)
		{
			vector<D2D1_POINT_2F> vpoint;
			for (int i = 0; i < points.size(); i++)
			{
				vpoint.push_back(Point2F(points[i].x, points[i].y));
			}

			ID2D1PathGeometry* pRightMountainGeometry;
			HRESULT hr = factory->CreatePathGeometry(&pRightMountainGeometry);
			if (SUCCEEDED(hr))
			{
				ID2D1GeometrySink* pSink = NULL;

				hr = pRightMountainGeometry->Open(&pSink);
				if (SUCCEEDED(hr))
				{
					pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

					pSink->BeginFigure(
						vpoint[0],
						D2D1_FIGURE_BEGIN_FILLED
					);

					pSink->AddLines(&vpoint[0], vpoint.size());
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				}
				hr = pSink->Close();

				safeRelease(&pSink);
			}

			ID2D1SolidColorBrush* brush;
			HRESULT hr2 = d2dDc->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr) && SUCCEEDED(hr2))
			{
				d2dDc->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				d2dDc->DrawGeometry(pRightMountainGeometry, brush, 1.0f);

				d2dDc->SetTransform(Matrix3x2F::Identity());
			}

			if (SUCCEEDED(hr2))
			{
				brush->Release();
			}

			if (SUCCEEDED(hr))
			{
				pRightMountainGeometry->Release();
			}
		}

		void Direct2DDevice::DrawString(std::wstring text, float x, float y, float sizeX, float sizeY, float scaleX, float scaleY, float rotation, int fontsize, Color color /*= Color::Black*/, std::wstring font)
		{
			IDWriteTextFormat* textformat;
			HRESULT hr1 = writeFactory->CreateTextFormat(
				font.c_str(),
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				fontsize,
				L"",
				&textformat);

			ID2D1SolidColorBrush* brush;
			HRESULT hr2 = d2dDc->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
			{
				d2dDc->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				d2dDc->DrawText(text.c_str(), text.size(), textformat, RectF(-sizeX / 2.0f, -sizeY / 2.0f, sizeX / 2.0f, sizeY / 2.0f), brush);

				d2dDc->SetTransform(Matrix3x2F::Identity());
			}

			if (SUCCEEDED(hr1))
			{
				textformat->Release();
			}

			if (SUCCEEDED(hr2))
			{
				brush->Release();
			}
		}

		void Direct2DDevice::DrawSprite(Sprite* sprite, int spriteNumber, float x, float y, float scaleX, float scaleY, float rotation, float opacity /*= 1.0f*/)
		{
			D2D1_RECT_F destinationRect;
			destinationRect.top = -((DCSprite*)sprite)->cutSizeY / 2.0f;
			destinationRect.left = -((DCSprite*)sprite)->cutSizeX / 2.0f;
			destinationRect.bottom = ((DCSprite*)sprite)->cutSizeY / 2.0f;
			destinationRect.right = ((DCSprite*)sprite)->cutSizeX / 2.0f;

			int spriteNumberX = spriteNumber % ((DCSprite*)sprite)->countX;
			int spriteNumberY = spriteNumber / ((DCSprite*)sprite)->countX;


			D2D1_RECT_F sourceRect;
			sourceRect.top = ((DCSprite*)sprite)->orgPosY + spriteNumberY * ((DCSprite*)sprite)->cutSizeY + spriteNumberY * ((DCSprite*)sprite)->spaceY;
			sourceRect.left = ((DCSprite*)sprite)->orgPosX + spriteNumberX * ((DCSprite*)sprite)->cutSizeX + spriteNumberX * ((DCSprite*)sprite)->spaceX;
			sourceRect.bottom = sourceRect.top + ((DCSprite*)sprite)->cutSizeY;
			sourceRect.right = sourceRect.left + ((DCSprite*)sprite)->cutSizeX;

			d2dDc->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

			Test(((DCSpriteSheet*)((DCSprite*)sprite)->targetSheet)->bitmap);
			d2dDc->DrawBitmap(((DCSpriteSheet*)((DCSprite*)sprite)->targetSheet)->bitmap, destinationRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sourceRect);

			d2dDc->SetTransform(Matrix3x2F::Identity());
		}

		void Direct2DDevice::Test(ID2D1Bitmap* p_bitmap)
		{
			//// 조명 원 생성
			//D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(centerX, centerY), radius, radius);

			//// 브러시 생성
			//ID2D1RadialGradientBrush* pBrush = NULL;
			//D2D1_GRADIENT_STOP gradientStops[2];
			//gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White, 1);
			//gradientStops[0].position = 0;
			//gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::White, 0);
			//gradientStops[1].position = 1;
			//d2dDc->CreateRadialGradientBrush(
			//	D2D1::RadialGradientBrushProperties(
			//		D2D1::Point2F(centerX, centerY),
			//		D2D1::Point2F(0, 0),
			//		radius,
			//		radius),
			//	gradientStops,
			//	2,
			//	&pBrush);

			//// 렌더링
			//pRenderTarget->BeginDraw();
			//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
			//pRenderTarget->FillEllipse(&ellipse, pBrush);
			//pRenderTarget->EndDraw();


			//// 1. 조명 효과를 담을 레이어를 만듭니다.
			//ID2D1Layer* lightLayer;
			//d2dDc->CreateLayer(NULL, &lightLayer);

			//// 2. 레이어에 조명을 그립니다.
			//D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(100, 100), 50, 50);
			//ID2D1RadialGradientBrush* gradientBrush;
			//d2dDc->CreateRadialGradientBrush(
			//	D2D1::RadialGradientBrushProperties(
			//		ellipse.point,
			//		ellipse.radiusX,
			//		ellipse.radiusY),
			//	D2D1::GradientStop(D2D1::ColorF(D2D1::ColorF::White, 0), 0),
			//	D2D1::GradientStop(D2D1::ColorF(D2D1::ColorF::Transparent, 1), 1),
			//	&gradientBrush);
			//deviceContext->PushLayer(
			//	D2D1::LayerParameters(D2D1::InfiniteRect(), gradientBrush),
			//	lightLayer);

			//// 3. 원래의 콘텐츠를 레이어와 결합합니다.
			//deviceContext->BeginDraw();
			//deviceContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
			//deviceContext->DrawText(L"Hello, World!", 13, textFormat.Get(), D2D1::RectF(0, 0, 200, 100), textBrush.Get());
			//deviceContext->PopLayer();
			//deviceContext->EndDraw();


			//ID2D1SolidColorBrush* pLightBrush = NULL;
			//ID2D1SolidColorBrush* pSurfaceBrush = NULL;

			//const D2D1_COLOR_F LightColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
			//const D2D1_COLOR_F SurfaceColor = D2D1::ColorF(0.8f, 0.8f, 0.8f, 1.0f);
			//const D2D1_POINT_2F LightPosition = D2D1::Point2F(150.0f, 150.0f);

			//d2dDc->CreateSolidColorBrush(LightColor, &pLightBrush);
			//d2dDc->CreateSolidColorBrush(SurfaceColor, &pSurfaceBrush);

			//D2D1_RECT_F lightRectangle = D2D1::RectF(50.0f, 50.0f, 250.0f, 250.0f);
			//d2dDc->FillRectangle(lightRectangle, pLightBrush);

			//// 표면이 있는 사각형 그리기
			//D2D1_RECT_F surfaceRectangle = D2D1::RectF(100.0f, 100.0f, 200.0f, 200.0f);
			//d2dDc->FillRectangle(surfaceRectangle, pSurfaceBrush);

			//D2D1_POINT_2F center = D2D1::Point2F((surfaceRectangle.left + surfaceRectangle.right) / 2, (surfaceRectangle.top + surfaceRectangle.bottom) / 2);
			//D2D1_ELLIPSE ellipse = D2D1::Ellipse(center, (surfaceRectangle.right - surfaceRectangle.left) / 2, (surfaceRectangle.bottom - surfaceRectangle.top) / 2);

			//D2D1_GRADIENT_STOP gradientStops[2];
			//gradientStops[0].color = LightColor;
			//gradientStops[0].position = 0.0f;
			//gradientStops[1].color = SurfaceColor;
			//gradientStops[1].position = 1.0f;

			//ID2D1GradientStopCollection* pGradientStops = NULL;
			//d2dDc->CreateGradientStopCollection(
			//	gradientStops,
			//	2,
			//	D2D1_GAMMA_2_2,
			//	D2D1_EXTEND_MODE_CLAMP,
			//	&pGradientStops
			//);

			//ID2D1RadialGradientBrush* pGradientBrush = NULL;
			//d2dDc->CreateRadialGradientBrush(
			//	D2D1::RadialGradientBrushProperties((D2D1_POINT_2F)ellipse.point, D2D1::Point2F(LightPosition.x, LightPosition.y), (float)(surfaceRectangle.right - surfaceRectangle.left) / 2, (float)(surfaceRectangle.bottom - surfaceRectangle.top) / 2),
			//	pGradientStops,
			//	&pGradientBrush
			//);
			//d2dDc->FillEllipse(ellipse, pGradientBrush);
			/*ComPtr<ID2D1Effect> gaussianBlurEffect;

			(d2dDc->CreateEffect(CLSID_D2D1GaussianBlur, &gaussianBlurEffect));

			gaussianBlurEffect->SetInput(0, p_bitmap);

			(gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 6.0f));

			D2D1_POINT_2F pos = { -158,-128 };
			D2D1_RECT_F rect = {0,0,316,256};
			d2dDc->DrawImage(gaussianBlurEffect.Get(), pos, rect);*/

			static float center1=100;
			static float center2=100;
			static float center3 = -700;
			if (Input::GetInstance().PressedKeyNow('Q'))
			{
				center1 += 100;
			}

			if (Input::GetInstance().PressedKeyNow('W'))
			{
				center3 -= 5;
			}

			if (Input::GetInstance().PressedKeyNow('E'))
			{
				center3 += 5;
			}

			ID2D1RadialGradientBrush* pRadialGradientBrush = nullptr;
			ID2D1GradientStopCollection* pGradientStops = nullptr;

			D2D1_GRADIENT_STOP gradientStops[] =
			{
				{0.0f, D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.4f)},
				{0.3f, D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.2f)},
				{1.0f, D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f)},
			};
			UINT gradientStopsCount = ARRAYSIZE(gradientStops);

			d2dDc->CreateGradientStopCollection(
				gradientStops, gradientStopsCount,
				D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP,
				&pGradientStops);

			// Radial gradient brush 설정
			D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES brushProperties =
				D2D1::RadialGradientBrushProperties(
					D2D1::Point2F(0.0f, center3),		// 그라데이션 중심
					D2D1::Point2F(0.0f, 0.0f),		// 그라데이션 점의 반지름
					600.0f,							// 그라데이션 외부 원의 반지름
					1000.0f);						// 그라데이션 내부 원의 반지름

			// 그라데이션 브러쉬 만들기
			d2dDc->CreateRadialGradientBrush(
				brushProperties,
				pGradientStops,
				&pRadialGradientBrush);

			D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(0.0f, 0.0f), 200.0f, 2000.0f);

			

			// 타원 그리기
			d2dDc->FillEllipse(ellipse, pRadialGradientBrush);

			pRadialGradientBrush->Release();
			pGradientStops->Release();

			/*ID2D1Bitmap* blankBitmap;
			D2D1_SIZE_U size = D2D1::SizeU(100, 100);
			D2D1_BITMAP_PROPERTIES bitmapProps = D2D1::BitmapProperties();
			d2dDc->CreateBitmap(size, NULL, 0, &bitmapProps, &blankBitmap);

			ID2D1Effect* effect;
			d2dDc->CreateEffect(CLSID_D2D1GaussianBlur, &effect);
			effect->SetInput(0, blankBitmap);

			D2D1_RECT_F rect = { 0,0,100,100 };

			ID2D1ImageBrush* imageBrush;
			D2D1_IMAGE_BRUSH_PROPERTIES brushProps = D2D1::ImageBrushProperties(rect,D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP);
			d2dDc->CreateImageBrush(effect, &brushProps, D2D1::BrushProperties(), &imageBrush);
			d2dDc->BeginDraw();
			d2dDc->FillRectangle(D2D1::RectF(0, 0, 100, 100), imageBrush);
			d2dDc->EndDraw();*/

			//ID2D1GradientStopCollection* pGradientStops;
			//D2D1_GRADIENT_STOP gradientStops[2];
			//gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White, 1.0f);
			//gradientStops[0].position = 0.0f;
			//gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black, 0.0f);
			//gradientStops[1].position = 1.0f;
			//d2dDc->CreateGradientStopCollection(
			//	gradientStops,
			//	2,
			//	D2D1_GAMMA_2_2,
			//	D2D1_EXTEND_MODE_CLAMP,
			//	&pGradientStops
			//);
			//const int kRenderWidth = 800;
			//const int kRenderHeight = 600;
			//ID2D1RadialGradientBrush* pRadialGradientBrush;
			//d2dDc->CreateRadialGradientBrush(
			//	D2D1::RadialGradientBrushProperties(
			//		D2D1::Point2F(kRenderWidth / 2.0f, kRenderHeight / 2.0f),
			//		D2D1::Point2F(0, 0),
			//		kRenderWidth / 4.0f,
			//		kRenderHeight / 4.0f),
			//	pGradientStops,
			//	&pRadialGradientBrush
			//);

			//ID2D1Bitmap* blankBitmap;
			//D2D1_SIZE_U size = D2D1::SizeU(100, 100);
			//D2D1_BITMAP_PROPERTIES bitmapProps = D2D1::BitmapProperties();
			//HRESULT hr = d2dDc->CreateBitmap(size, NULL, 0, &bitmapProps, &blankBitmap);

			//ID2D1Effect* pLightEffect;
			//d2dDc->CreateEffect(CLSID_D2D1GaussianBlur, &pLightEffect);
			//pLightEffect->SetInput(0, blankBitmap);
			//pLightEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 20.0f);

			//// Draw the light effect
			//d2dDc->DrawImage(pLightEffect);

			//ID2D1RadialGradientBrush* pRadialGradientBrush;

			//D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES radialProps =
			//	D2D1::RadialGradientBrushProperties(D2D1_POINT_2F{100,100}, D2D1_POINT_2F{ 0,0 }, 100, 100);

			//ID2D1GradientStopCollection* pGradientStops;

			//
			//D2D1_GRADIENT_STOP gradientStops[2];
			//gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White, 1.0f);
			//gradientStops[0].position = 0.0f;
			//gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black, 0.0f);
			//gradientStops[1].position = 1.0f;
			//d2dDc->CreateGradientStopCollection(
			//	gradientStops,
			//	2,
			//	D2D1_GAMMA_2_2,
			//	D2D1_EXTEND_MODE_CLAMP,
			//	&pGradientStops
			//);

			//d2dDc->CreateGradientStopCollection(
			//	gradientStops, 2,
			//	D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP,
			//	&pGradientStops);

			//d2dDc->CreateRadialGradientBrush(
			//	radialProps, pGradientStops, &pRadialGradientBrush);

			//// Draw ellipse with radial gradient brush
			//D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1_POINT_2F{ 100,100 }, 100, 100);
			//d2dDc->DrawEllipse(ellipse, pRadialGradientBrush, 1);
		}
	}
}


