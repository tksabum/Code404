#include "GraphicsResourceManager.h"

#include "EngineSetting.h"
#include "Direct2DSpriteSheet.h"
#include "Direct2DSprite.h"
#include "GraphicsSystem.h"

#include "DCSpriteSheet.h"
#include "DCSprite.h"

namespace BJEngine
{
	using namespace GraphicsEngine;

	void GraphicsResourceManager::CreateSprite(std::wstring spritename, std::wstring sheetname, int orgPosX, int orgPosY, int orgSizeX, int orgSizeY, int cutSizeX, int cutSizeY, int spaceX, int spaceY)
	{
		if (spriteList.count(spritename) != 0)
		{
			throw "CreateSprite: This spritename is already exist";
		}

		if (spriteSheetList.count(sheetname) == 0)
		{
			// create Sheet
			CreateSheet(sheetname);
		}

		if (spriteSheetList.count(sheetname) == 0)
		{
			throw "CreateSprite: Fail Create Sprite Sheet";
		}

		EngineSetting* engineSetting = &EngineSetting::GetInstance();

		if (engineSetting->engineMode == EngineSetting::Mode::MODE_2D)
		{
			if (engineSetting->graphicsEngine == EngineSetting::GraphicsEngine::DIRECT2D_GRAPHICS)
			{
				Sprite* sprite = new Direct2DSprite();
				sprite->Cut(spriteSheetList[sheetname], orgPosX, orgPosY, orgSizeX, orgSizeY, cutSizeX, cutSizeY, spaceX, spaceY);
				spriteList[spritename] = sprite;
			}
			else if (engineSetting->graphicsEngine == EngineSetting::GraphicsEngine::DIRECT2D_DC)
			{
				Sprite* sprite = new DCSprite();
				sprite->Cut(spriteSheetList[sheetname], orgPosX, orgPosY, orgSizeX, orgSizeY, cutSizeX, cutSizeY, spaceX, spaceY);
				spriteList[spritename] = sprite;
			}
		}
	}

	GraphicsEngine::Sprite* GraphicsResourceManager::GetSprite(std::wstring spritename)
	{
		return spriteList[spritename];
	}

	GraphicsEngine::SpriteSheet* GraphicsResourceManager::CreateSheet(std::wstring sheetname)
	{
		if (spriteSheetList.count(sheetname) != 0)
		{
			throw "CreateSheet: This sheetname is already exist";
		}

		if (GraphicsSystem::GetInstance().graphics == nullptr)
		{
			throw "CreateSheet: GraphicsSystem is not Initialized";
		}

		EngineSetting* engineSetting = &EngineSetting::GetInstance();

		if (engineSetting->engineMode == EngineSetting::Mode::MODE_2D)
		{
			if (engineSetting->graphicsEngine == EngineSetting::GraphicsEngine::DIRECT2D_GRAPHICS)
			{
				SpriteSheet* spritesheet = new Direct2DSpriteSheet(sheetname);
				spritesheet->LoadSpriteSheet(GraphicsSystem::GetInstance().graphics);
				spriteSheetList[sheetname] = spritesheet;
				return spritesheet;
			}
			else if (engineSetting->graphicsEngine == EngineSetting::GraphicsEngine::DIRECT2D_DC)
			{
				SpriteSheet* spritesheet = new DCSpriteSheet(sheetname);
				spritesheet->LoadSpriteSheet(GraphicsSystem::GetInstance().graphics);
				spriteSheetList[sheetname] = spritesheet;
				return spritesheet;
			}
		}
	}

	GraphicsResourceManager& GraphicsResourceManager::GetInstance()
	{
		static GraphicsResourceManager instance;
		return instance;
	}

	GraphicsResourceManager::GraphicsResourceManager()
	{

	}

	GraphicsResourceManager::~GraphicsResourceManager()
	{

	}
}