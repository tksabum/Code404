#include "Direct2DGraphics.h"

#include "Direct2DSprite.h"
#include "Direct2DSpriteSheet.h"

using namespace std;
using namespace D2D1;

namespace BJEngine
{
	namespace GraphicsEngine
	{
		using namespace Math;

		template<class T> void Direct2DGraphics::safeRelease(T** ppT)
		{
			if (*ppT)
			{
				(*ppT)->Release();
				*ppT = NULL;
			}
		}

		Direct2DGraphics::Direct2DGraphics(HWND hWnd, Color backcolor) : backColor(backcolor)
		{
			HRESULT hr;

			// D2D 컴포넌트를 사용할 수 있도록 초기화
			hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			if (!SUCCEEDED(hr))
			{
				throw "D2D 초기화 실패";
			}

			// D2D Factory 객체 생성
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
			if (!SUCCEEDED(hr))
			{
				throw "Factory 객체 생성 실패";
			}

			// D2D WriteFactory 생성
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(&writeFactory));
			if (!SUCCEEDED(hr))
			{
				throw "WriteFactory 생성 실패";
			}

			// imagingFactory 생성
			hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imagingFactory));
			if (!SUCCEEDED(hr))
			{
				throw "imagingFactory 생성 실패";
			}

			RECT r;
			GetClientRect(hWnd, &r);

			// D2D RenderTarget 생성
			hr = factory->CreateHwndRenderTarget(RenderTargetProperties(), HwndRenderTargetProperties(hWnd, SizeU(r.right, r.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY), &renderTarget);
			if (!SUCCEEDED(hr))
			{
				throw "RenderTarget 생성 실패";
			}
		}

		Direct2DGraphics::~Direct2DGraphics()
		{
			// D2D Factory 객체 제거
			factory->Release();

			// D2D WriteFactory 제거
			writeFactory->Release();

			// imagingFactory 제거
			imagingFactory->Release();

			// D2D RenderTarget 제거
			renderTarget->Release();

			// D2D 컴포넌트 사용해제
			CoUninitialize();
		}

		void Direct2DGraphics::BeginDraw()
		{
			renderTarget->BeginDraw();
			renderTarget->Clear(backColor.ConvertD2DColor());
		}

		void Direct2DGraphics::EndDraw()
		{
			renderTarget->EndDraw();
		}

		void Direct2DGraphics::Resize(unsigned int width, unsigned int height)
		{
			renderTarget->Resize({ width, height });
		}

		void Direct2DGraphics::FillEllipse(float x, float y, float radiusX, float radiusY, Color color)
		{
			D2D1_ELLIPSE ellipseRegion;
			ellipseRegion.point.x = x;
			ellipseRegion.point.y = y;
			ellipseRegion.radiusX = radiusX;
			ellipseRegion.radiusY = radiusY;

			ID2D1SolidColorBrush* brush;
			HRESULT hr = renderTarget->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				renderTarget->FillEllipse(ellipseRegion, brush);
				brush->Release();
			}
		}

		void Direct2DGraphics::FillRect(float x, float y, float sizeX, float sizeY, Color color)
		{
			D2D1_RECT_F rectRegion;
			rectRegion.left = x;
			rectRegion.top = y;
			rectRegion.right = x + sizeX;
			rectRegion.bottom = y + sizeY;

			ID2D1SolidColorBrush* brush;
			HRESULT hr = renderTarget->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				renderTarget->FillRectangle(rectRegion, brush);
				brush->Release();
			}
		}

		void Direct2DGraphics::DrawLine(Math::Vector2D point0, Math::Vector2D point1, float x, float y, float scaleX, float scaleY, float rotation, Color color)
		{
			D2D1_POINT_2F d2dpoint0 = Point2F(point0.x, point0.y);
			D2D1_POINT_2F d2dpoint1 = Point2F(point1.x, point1.y);

			ID2D1SolidColorBrush* brush;
			HRESULT hr = renderTarget->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				renderTarget->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				renderTarget->DrawLine(d2dpoint0, d2dpoint1, brush);

				renderTarget->SetTransform(Matrix3x2F::Identity());

				brush->Release();
			}
		}

		void Direct2DGraphics::DrawEllipse(float x, float y, float radiusX, float radiusY, float scaleX, float scaleY, float rotation, Color color)
		{
			D2D1_ELLIPSE ellipseRegion;
			ellipseRegion.point.x = 0.0f;
			ellipseRegion.point.y = 0.0f;
			ellipseRegion.radiusX = radiusX;
			ellipseRegion.radiusY = radiusY;

			ID2D1SolidColorBrush* brush;
			HRESULT hr = renderTarget->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				renderTarget->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				renderTarget->DrawEllipse(ellipseRegion, brush);

				renderTarget->SetTransform(Matrix3x2F::Identity());

				brush->Release();
			}
		}

		void Direct2DGraphics::DrawRect(float x, float y, float sizeX, float sizeY, Color color)
		{
			D2D1_RECT_F rectRegion;
			rectRegion.left = x;
			rectRegion.top = y;
			rectRegion.right = x + sizeX;
			rectRegion.bottom = y + sizeY;

			ID2D1SolidColorBrush* brush;
			HRESULT hr = renderTarget->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr))
			{
				renderTarget->DrawRectangle(rectRegion, brush);
				brush->Release();
			}
		}

		void Direct2DGraphics::DrawPolygon(const std::vector<Math::Vector2D>& points, float x, float y, float scaleX, float scaleY, float rotation, Color color)
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
			HRESULT hr2 = renderTarget->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr) && SUCCEEDED(hr2))
			{
				renderTarget->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				renderTarget->DrawGeometry(pRightMountainGeometry, brush, 1.0f);

				renderTarget->SetTransform(Matrix3x2F::Identity());
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

		void Direct2DGraphics::DrawString(std::wstring text, float x, float y, float sizeX, float sizeY, float scaleX, float scaleY, float rotation, int fontsize, Color color, std::wstring font)
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
			HRESULT hr2 = renderTarget->CreateSolidColorBrush(color.ConvertD2DColor(), &brush);

			if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
			{
				renderTarget->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

				renderTarget->DrawText(text.c_str(), text.size(), textformat, RectF(-sizeX / 2.0f, -sizeY / 2.0f, sizeX / 2.0f, sizeY / 2.0f), brush);

				renderTarget->SetTransform(Matrix3x2F::Identity());
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

		void Direct2DGraphics::DrawSprite(Sprite* sprite, int spriteNumber, float x, float y, float scaleX, float scaleY, float rotation, float opacity)
		{
			D2D1_RECT_F destinationRect;
			destinationRect.top = -((Direct2DSprite*)sprite)->cutSizeY / 2.0f;
			destinationRect.left = -((Direct2DSprite*)sprite)->cutSizeX / 2.0f;
			destinationRect.bottom = ((Direct2DSprite*)sprite)->cutSizeY / 2.0f;
			destinationRect.right = ((Direct2DSprite*)sprite)->cutSizeX / 2.0f;

			int spriteNumberX = spriteNumber % ((Direct2DSprite*)sprite)->countX;
			int spriteNumberY = spriteNumber / ((Direct2DSprite*)sprite)->countX;


			D2D1_RECT_F sourceRect;
			sourceRect.top = ((Direct2DSprite*)sprite)->orgPosY + spriteNumberY * ((Direct2DSprite*)sprite)->cutSizeY + spriteNumberY * ((Direct2DSprite*)sprite)->spaceY;
			sourceRect.left = ((Direct2DSprite*)sprite)->orgPosX + spriteNumberX * ((Direct2DSprite*)sprite)->cutSizeX + spriteNumberX * ((Direct2DSprite*)sprite)->spaceX;
			sourceRect.bottom = sourceRect.top + ((Direct2DSprite*)sprite)->cutSizeY;
			sourceRect.right = sourceRect.left + ((Direct2DSprite*)sprite)->cutSizeX;

			renderTarget->SetTransform(Matrix3x2F::Scale(D2D1_SIZE_F{ scaleX, scaleY }) * Matrix3x2F::Rotation(rotation) * Matrix3x2F::Translation(x, y));

			renderTarget->DrawBitmap(((Direct2DSpriteSheet*)((Direct2DSprite*)sprite)->targetSheet)->bitmap, destinationRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sourceRect);

			renderTarget->SetTransform(Matrix3x2F::Identity());
		}

		void Direct2DGraphics::Test()
		{
			ID2D1GradientStopCollection* pGradientStops = NULL;
			D2D1_GRADIENT_STOP gradientStops[2];
			gradientStops[0].position = 0.0f;
			gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White, 1.0f);
			gradientStops[1].position = 1.0f;
			gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::White, 0.0f);

			renderTarget->CreateGradientStopCollection(
				gradientStops,
				2,
				D2D1_GAMMA_2_2,
				D2D1_EXTEND_MODE_CLAMP,
				&pGradientStops
			);

			ID2D1RadialGradientBrush* pRadialGradientBrush = NULL;
			renderTarget->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(
					D2D1::Point2F(100.0f, 100.0f),
					D2D1::Point2F(),
					400.0f,
					400.0f
				),
				pGradientStops,
				&pRadialGradientBrush
			);

			D2D1_POINT_2F points[3];
			points[0].x = 500.0f;
			points[0].y = 100.0f;
			points[1].x = 200.0f;
			points[1].y = 500.0f;
			points[2].x = 900.0f;
			points[2].y = 500.0f;

			ID2D1PathGeometry* pPathGeometry = NULL;
			factory->CreatePathGeometry(&pPathGeometry);
			ID2D1GeometrySink* pSink = NULL;
			pPathGeometry->Open(&pSink);
			pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLines(points + 1, 2);
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			pSink->Close();

			renderTarget->FillGeometry(pPathGeometry, pRadialGradientBrush);

			pGradientStops->Release();
			pPathGeometry->Release();
			pSink->Release();
			pRadialGradientBrush->Release();
		}

		//void Direct2DGraphics::imageTest0()
		//{
		//	// Create a decoder
		//	IWICBitmapDecoder* pDecoder = NULL;

		//	HRESULT hr = imagingFactory->CreateDecoderFromFilename(
		//		L"Sample.png",                      // Image to be decoded
		//		NULL,                            // Do not prefer a particular vendor
		//		GENERIC_READ,                    // Desired read access to the file
		//		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		//		&pDecoder                        // Pointer to the decoder
		//	);

		//	// Retrieve the first frame of the image from the decoder
		//	IWICBitmapFrameDecode* pFrame = NULL;

		//	pDecoder->GetFrame(0, &pFrame);

		//	pDecoder->Release();




		//	IWICFormatConverter* converter;
		//	imagingFactory->CreateFormatConverter(&converter);

		//	converter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);


		//	pFrame->Release();







		//	ID2D1Bitmap* bitmap;
		//	renderTarget->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);

		//	converter->Release();







		//	D2D1_RECT_F rectangle;
		//	rectangle.top = 200.0f;
		//	rectangle.left = 500.0f;
		//	rectangle.bottom = rectangle.top + bitmap->GetPixelSize().height;
		//	rectangle.right = rectangle.left + bitmap->GetPixelSize().width;


		//	D2D1_RECT_F sourceRect;
		//	sourceRect.top = 0.0f;
		//	sourceRect.left = 0.0f;
		//	sourceRect.bottom = bitmap->GetPixelSize().height;
		//	sourceRect.right = bitmap->GetPixelSize().width;

		//	renderTarget->DrawBitmap(bitmap, rectangle, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sourceRect);


		//	bitmap->Release();


		//}
	}
}