#include "Direct2DSpriteSheet.h"

#include "Direct2DDevice.h"

#include "DCSpriteSheet.h"

BJEngine::GraphicsEngine::DCSpriteSheet::DCSpriteSheet(std::wstring sprite)
{
	name = sprite;
}

void BJEngine::GraphicsEngine::DCSpriteSheet::LoadSpriteSheet(Graphics* graphics)
{
	if (bitmap != nullptr)
	{
		return;
	}

	Direct2DDevice* d2dgraphics = (Direct2DDevice*)graphics;

	// Decoder
	IWICBitmapDecoder* decoder = NULL;
	HRESULT hr = d2dgraphics->imagingFactory->CreateDecoderFromFilename(
		name.c_str(),                      // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&decoder                        // Pointer to the decoder
	);

	if (SUCCEEDED(hr))
	{
		// Frame
		IWICBitmapFrameDecode* frame = NULL;

		hr = decoder->GetFrame(0, &frame);

		if (SUCCEEDED(hr))
		{
			// Converter
			IWICFormatConverter* converter;
			d2dgraphics->imagingFactory->CreateFormatConverter(&converter);

			hr = converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

			if (SUCCEEDED(hr))
			{
				// Bitmap
				hr = d2dgraphics->d2dDc->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);

				if (SUCCEEDED(hr))
				{
					sizeX = bitmap->GetPixelSize().width;
					sizeY = bitmap->GetPixelSize().height;

					//if (name == L"playerTop.png" || name == L"playerBottom.png" || name == L"light.png")
					//if (name == L"light.png")
					//{
					//	Test(frame);
					//}
				}

				// converter Release
				converter->Release();
			}

			// frame Release
			frame->Release();
		}

		// decoder Release
		decoder->Release();
	}
}
