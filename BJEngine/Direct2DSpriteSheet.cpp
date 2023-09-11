#include "Direct2DSpriteSheet.h"

#include "Direct2DGraphics.h"

#include "GraphicsSystem.h"



namespace BJEngine
{
	namespace GraphicsEngine
	{
		Direct2DSpriteSheet::Direct2DSpriteSheet(std::wstring sprite)
		{
			name = sprite;
		}

		void Direct2DSpriteSheet::LoadSpriteSheet(Graphics* graphics)
		{
			if (bitmap != nullptr)
			{
				return;
			}

			Direct2DGraphics* d2dgraphics = (Direct2DGraphics*)graphics;
			
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
						hr = d2dgraphics->renderTarget->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);

						if (SUCCEEDED(hr))
						{
							sizeX = bitmap->GetPixelSize().width;
							sizeY = bitmap->GetPixelSize().height;
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

		void Direct2DSpriteSheet::Test(IWICBitmapFrameDecode* frame)
		{
			D2D1_SIZE_U bitmapSize = bitmap->GetPixelSize();
			D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties();
			bitmap->GetDpi(&props.dpiX, &props.dpiY);
			props.pixelFormat = bitmap->GetPixelFormat();

			// Create a new bitmap with the same properties as the original bitmap
			ID2D1Bitmap* pModifiedBitmap = NULL;
			HRESULT hr = ((Direct2DGraphics*)GraphicsSystem::GetInstance().graphics)->renderTarget->CreateBitmap(bitmapSize, NULL, 0, props, &pModifiedBitmap);

			// Lock the original bitmap to get a pointer to its pixel data
			D2D1_MAPPED_RECT mappedBitmap;
			WICRect rect = { 0,0,bitmapSize.width,bitmapSize.height };
			BYTE* pv = new BYTE[bitmapSize.width * bitmapSize.height * 4];
			frame->CopyPixels(&rect, 4 * bitmapSize.width, 4 * bitmapSize.width * bitmapSize.height, pv);
			
			for (int i = 0; i < bitmapSize.height; i++)
			{
				for (int j = 0; j < bitmapSize.width; j++)
				{
					UINT index = (i * bitmapSize.width + j) * 4;
					BYTE b = pv[index];
					BYTE g = pv[index + 1];
					BYTE r = pv[index + 2];
					BYTE a = pv[index + 3];

					if (
						pv[index + 3] == 0
						)
					{
						pv[index] = 0;
						pv[index + 1] = 0;
						pv[index + 2] = 0;
						pv[index + 3] = 0;
					}
					else
					{
						static float light = 50.0f;
						
						//pv[index] = 255 * ((bitmapSize.height - i) / (float)bitmapSize.height) * ((bitmapSize.height - i) / (float)bitmapSize.height);
						//pv[index + 1] = 255 * ((bitmapSize.height - i) / (float)bitmapSize.height) * ((bitmapSize.height - i) / (float)bitmapSize.height);
						//pv[index + 2] = 255 * ((bitmapSize.height - i) / (float)bitmapSize.height) * ((bitmapSize.height - i) / (float)bitmapSize.height);
						//pv[index + 3] = 0;
						//
						//pv[index] = 255 * (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f) - ((pow(i, 2.0f) + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)))) / (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)) * 1.0f;
						//pv[index + 1] = 255 * (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f) - ((pow(i, 2.0f) + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)))) / (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)) * 1.0f;
						//pv[index + 2] = 255 * (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f) - ((pow(i, 2.0f) + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)))) / (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)) * 1.0f;
						//pv[index + 3] = 0;
						pv[index] = 255 * ((bitmapSize.height - i) / (float)bitmapSize.height) * ((bitmapSize.height - i) / (float)bitmapSize.height);
						pv[index + 1] = 255 * ((bitmapSize.height - i) / (float)bitmapSize.height) * ((bitmapSize.height - i) / (float)bitmapSize.height);
						pv[index + 2] = 255 * ((bitmapSize.height - i) / (float)bitmapSize.height) * ((bitmapSize.height - i) / (float)bitmapSize.height);
						pv[index + 3] = 0;

						pv[index] = 255 * (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f) - ((pow(i, 2.0f) + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)))) / (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)) * 0.6f;
						pv[index + 1] = 255 * (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f) - ((pow(i, 2.0f) + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)))) / (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)) * 0.6f;
						pv[index + 2] = 255 * (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f) - ((pow(i, 2.0f) + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)))) / (bitmapSize.height * bitmapSize.height + pow(abs(bitmapSize.width / 2.0f - j), 2.0f)) * 0.6f;
						pv[index + 3] = 0;
					}
				}
			}

			bitmap->CopyFromMemory(NULL, pv, bitmapSize.width * 4);

			//wicBitmap->CopyPixels()
			//// Loop through each pixel of the bitmap
			//for (UINT y = 0; y < bitmapSize.height; ++y) {
			//	for (UINT x = 0; x < bitmapSize.width; ++x) {
			//		UINT index = (y * bitmapSize.width + x) * 4;

			//		// Check if the color of the pixel is yellow
			//		if (pPixels[index + 0] == 255 && pPixels[index + 1] == 255 && pPixels[index + 2] == 0) {
			//			// Set the color of the pixel to black
			//			pPixels[index + 0] = 0;
			//			pPixels[index + 1] = 0;
			//			pPixels[index + 2] = 0;
			//		}
			//	}
			//}

			// Copy the modified pixel data to the new bitmap
			//pModifiedBitmap->CopyFromMemory(NULL, pPixels, bitmapSize.width * 4);
			//
			//// Draw the modified bitmap to the render target
			//pRenderTarget->DrawBitmap(pModifiedBitmap);
			//
			//// Clean up
			//delete[] pPixels;
			//pBitmap->Release();
			//pModifiedBitmap->Release();
			//pRenderTarget->Release();
			//pD2DFactory->Release();
			//
			//return 0;


		}
	}
}