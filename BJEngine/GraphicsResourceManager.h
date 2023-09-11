#pragma once

#ifdef GRAPHICSRESOURCEMANAGER_EXPORTS
#define GRAPHICSRESOURCEMANAGER_API __declspec(dllexport)
#else
#define GRAPHICSRESOURCEMANAGER_API __declspec(dllimport)
#endif

#include <string>
#include <unordered_map>

#include "Sprite.h"
#include "SpriteSheet.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Sprite;
		class SpriteSheet;
	}

	class GraphicsResourceManager
	{
	public:
		GRAPHICSRESOURCEMANAGER_API void CreateSprite(std::wstring spritename, std::wstring sheetname, int orgPosX, int orgPosY, int orgSizeX, int orgSizeY, int cutSizeX, int cutSizeY, int spaceX, int spaceY);
		GRAPHICSRESOURCEMANAGER_API GraphicsEngine::Sprite* GetSprite(std::wstring spritename);

	private:
		std::unordered_map<std::wstring, GraphicsEngine::SpriteSheet*> spriteSheetList;
		std::unordered_map<std::wstring, GraphicsEngine::Sprite*> spriteList;

		GraphicsEngine::SpriteSheet* CreateSheet(std::wstring sheetname);

		//singleton
	public:
		GRAPHICSRESOURCEMANAGER_API static GraphicsResourceManager& GetInstance();

	private:
		GraphicsResourceManager();
		~GraphicsResourceManager();
		GraphicsResourceManager(const GraphicsResourceManager& ref);
		GraphicsResourceManager& operator=(const GraphicsResourceManager& ref) {}
	};
}