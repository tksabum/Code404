
#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../Code404/AI.h"
#include "Animator.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "CoreSystem.h"
#include "SceneManager.h"
#include "Rigidbody2D.h"
#include "Transform.h"
#include "GameObject.h"
#include "ConvexpolygonCollider.h"
#include "Scene.h"
#include "Scene.h"
#include "Image.h"
#include "Renderer.h"
#include "../Code404/ParryingSystem.h"
#include "../Code404/ItemMove.h"
using namespace BJEngine;
using namespace BJEngine::Math;
using namespace BJEngine::GraphicsEngine;
using namespace std;
#include "../Code404/GameObjectFactory.h"
#include "Scene.h"
#include "CoreSystem.h"
#include "GameObject.h"
#include "ConvexpolygonCollider.h"
#include "Rigidbody2D.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Image.h"
#include "SpriteRenderer.h"
using namespace BJEngine;
using namespace BJEngine::Math;

Level::Level(const char* filename)
{
		FILE* f;
		f = fopen(filename, "rt");
		int i = 0;
		int value = 0;
		if (f == NULL)
		{
			std::cout << "Error opening file" << std::endl;
		}
		while (fread(&value, sizeof(int), 1, f) == 1)
		{
			data.push_back(value);
		}
		width = data[0];
		height = data[1];
		cellSize = data[2];
		// 타일의 정보로 만들 맵 정보
		int count = 3;
		int count2 = 3;
		tileData.resize(height, std::vector<int>(width * 13, 0));
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width * 13; i++)	// 한 타일에 들어있는 정보 개수 : 13
			{
				tileData[j][i] = data[count];
				count++;
			}
		}
		
		FlagData.resize(height, std::vector<int>(width, 0));
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				FlagData[j][i] = data[i * 13 + 5 + j * width * 13];

				// 5 18 31 44		 5 18 31 j * width * 13
				// 57 70 83 96		 44	
				// 109
			}
		}

        imageXData.resize(height, std::vector<int>(width, 0));
        imageYData.resize(height, std::vector<int>(width, 0));

        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                imageXData[j][i] = data[i * 13 + 8 + j * width * 13];
            }
        }
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                imageYData[j][i] = data[i * 13 + 9 + j * width * 13];
            }
        }
		sheetData.resize(height, std::vector<int>(width, 0));

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				sheetData[j][i] = data[width * 13 * j + 7 + 13 * i];
			}
		}
		stateData.resize(height, std::vector<int>(width, 0));
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				stateData[j][i] = data[width * 13 * j + 3+ 13 * i];
			}
		}
}

void Level::Print()
{
	for (int i = 0; i < data.size(); i++)
	{
		std::cout << data[i] << std::endl;
	}
}

int Level::GetData(int index)
{
	return this->data[index];
}

int Level::GetTileData(int y, int x)
{
	return this->tileData[y][x];
}

int Level::GetFlagData(int y, int x)
{
	return this->FlagData[y][x];
}

int Level::GetimageXData(int y, int x)
{
    return this->imageXData[y][x];
}

int Level::GetimageYData(int y, int x)
{
    return this->imageYData[y][x];
}

int Level::GetXPosition(int y, int x)
{
	xPos = 0;
	xPos = this->tileData[y][x * 13 + 5];
	// xPos /= 32;
	return xPos;
}

int Level::GetYPosition(int y, int x)
{
	yPos = 0;
	yPos = this->tileData[y][x * 13 + 6];

	return yPos;
}


void Level::SetFlagData(int y, int x, int value)
{
	this->FlagData[y][x] = value;
}

int Level::GetSheetData(int y, int x)
{
	return this->sheetData[y][x];
}

int Level::GetStateData(int y, int x)
{
	return this->stateData[y][x];
}





void Level::CreatePlatformFromLevelData(Level& level)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	std::vector<Rectangle2> rectangles;

	for (int j = 0; j < level.GetData(1); j++)
	{
		for (int i = 0; i < level.GetData(0); i++)
		{
			int vertical_count = 1;
			if (level.GetFlagData(j, i) == 1 && level.GetStateData(j,i)==0)
			{
				int horizontal_count = 1;

				// 수평으로 확장할 수 있는지 확인
				while (i + horizontal_count < level.GetData(0) && level.GetFlagData(j, i + horizontal_count) == 1) {
					horizontal_count++;
				}
				// 수직으로 확장할 수 있는지 확인
				while (j + vertical_count < level.GetData(1) && level.GetFlagData(j + vertical_count, i) == 1) {
					vertical_count++;
				}
				// 가로 및 세로 직사각형이 교차하는지 확인
				bool overlaps = false;
				for (const Rectangle2& existing_rect : rectangles)
				{
					if (i < existing_rect.x + existing_rect.width && i + horizontal_count > existing_rect.x &&
						j < existing_rect.y + existing_rect.height && j + vertical_count > existing_rect.y) {
						overlaps = true;
						break;
					}
				}
				if (!overlaps)
				{
					Rectangle2 rect;
					if (vertical_count >= horizontal_count) {
						rect = { i, j, 1, vertical_count };
					}
					else {
						rect = { i, j, horizontal_count, 1 };
					}
					rectangles.push_back(rect);

					GameObject* platform = currentScene->CreateGameObject(L"Platform");
					platform->SetLayerName(L"Platform");
					platform->GetTransform()->SetLocalPosition(Vector2D(-400.0f + rect.x * 64, 200.0f + rect.y * 64));
					ConvexpolygonCollider* platformcollider = platform->AddComponent<ConvexpolygonCollider>();
					if (vertical_count >= horizontal_count)
					{
						platformcollider->SetPolygon({ Vector2D(-32.0f, 32.0f + (vertical_count - 1) * 64),
													   Vector2D(-32.0f, -32.0f),
													   Vector2D(32.0f, -32.0f),
													   Vector2D(32.0f, 32.0f + (vertical_count - 1) * 64) });
					}
					else
					{
						platformcollider->SetPolygon({ Vector2D(-32.0f, 32.0f), Vector2D(-32.0f, -32.0f),
														Vector2D(32.0f + (horizontal_count - 1) * 64, -32.0f),
														Vector2D(32.0f + (horizontal_count - 1) * 64, 32.0f) });
					}
					Rigidbody2D* platformrigidbody = platform->AddComponent<Rigidbody2D>();
					platformrigidbody->SetPositionXLock(true);
					platformrigidbody->SetPositionYLock(true);
					platformrigidbody->SetRotationLock(true);
					platformrigidbody->SetRestitutionValue(0.0f);
					platformrigidbody->SetAffectedByGravity(false);
					// 이미 작성한 타일 Flag 지우기
					for (int y = rect.y; y < rect.y + rect.height; y++)
					{
						for (int x = rect.x; x < rect.x + rect.width; x++)
						{
							level.SetFlagData(y, x, 0);
						}
					}

				}
			}
			/*else if (level.GetFlagData(j, i) == 1 && level.GetStateData(j, i) == 1)
			{
				GameObject* platform = currentScene->CreateGameObject(L"Platform");
				platform->GetTransform()->SetLocalPosition(Vector2D(-400.0f+i*64, 200.0f+j*64));
				platform->SetLayerName(L"Platform");
				Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
				ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
				collider->SetPolygon({ Vector2D(-32, 32), Vector2D(-32, 32), Vector2D(32, -32), Vector2D(32, 32) });
				rigid->SetPositionXLock(true);
				rigid->SetPositionYLock(true);
				rigid->SetRotationLock(true);
				rigid->SetAffectedByGravity(false);
				rigid->SetRestitutionValue(0.f);
			}*/
			

			
		}
	}
}

void Level::CreatePlatformFromLevelDataSecond(Level& level)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	std::vector<Rectangle2> rectangles;

	for (int j = 0; j < level.GetData(1); j++)
	{
		for (int i = 0; i < level.GetData(0); i++)
		{
			int vertical_count = 1;
			if (level.GetFlagData(j, i) == 1 && level.GetStateData(j, i) == 0)
			{
				int horizontal_count = 1;

				// 수평으로 확장할 수 있는지 확인
				while (i + horizontal_count < level.GetData(0) && level.GetFlagData(j, i + horizontal_count) == 1) {
					horizontal_count++;
				}
				// 수직으로 확장할 수 있는지 확인
				while (j + vertical_count < level.GetData(1) && level.GetFlagData(j + vertical_count, i) == 1) {
					vertical_count++;
				}
				// 가로 및 세로 직사각형이 교차하는지 확인
				bool overlaps = false;
				for (const Rectangle2& existing_rect : rectangles)
				{
					if (i < existing_rect.x + existing_rect.width && i + horizontal_count > existing_rect.x &&
						j < existing_rect.y + existing_rect.height && j + vertical_count > existing_rect.y) {
						overlaps = true;
						break;
					}
				}
				if (!overlaps)
				{
					Rectangle2 rect;
					if (vertical_count >= horizontal_count) {
						rect = { i, j, 1, vertical_count };
					}
					else {
						rect = { i, j, horizontal_count, 1 };
					}
					rectangles.push_back(rect);

					GameObject* platform = currentScene->CreateGameObject(L"Platform");
					platform->SetLayerName(L"Platform");
					platform->GetTransform()->SetLocalPosition(Vector2D(3440.0f + rect.x * 64, 200.0f + rect.y * 64));
					ConvexpolygonCollider* platformcollider = platform->AddComponent<ConvexpolygonCollider>();
					if (vertical_count >= horizontal_count)
					{
						platformcollider->SetPolygon({ Vector2D(-32.0f, 32.0f + (vertical_count - 1) * 64),
													   Vector2D(-32.0f, -32.0f),
													   Vector2D(32.0f, -32.0f),
													   Vector2D(32.0f, 32.0f + (vertical_count - 1) * 64) });
					}
					else
					{
						platformcollider->SetPolygon({ Vector2D(-32.0f, 32.0f), Vector2D(-32.0f, -32.0f),
														Vector2D(32.0f + (horizontal_count - 1) * 64, -32.0f),
														Vector2D(32.0f + (horizontal_count - 1) * 64, 32.0f) });
					}
					Rigidbody2D* platformrigidbody = platform->AddComponent<Rigidbody2D>();
					platformrigidbody->SetPositionXLock(true);
					platformrigidbody->SetPositionYLock(true);
					platformrigidbody->SetRotationLock(true);
					platformrigidbody->SetRestitutionValue(0.0f);
					platformrigidbody->SetAffectedByGravity(false);
					// 이미 작성한 타일 Flag 지우기
					for (int y = rect.y; y < rect.y + rect.height; y++)
					{
						for (int x = rect.x; x < rect.x + rect.width; x++)
						{
							level.SetFlagData(y, x, 0);
						}
					}

				}
			}
			/*else if (level.GetFlagData(j, i) == 1 && level.GetStateData(j, i) == 1)
			{
				GameObject* platform = currentScene->CreateGameObject(L"Platform");
				platform->GetTransform()->SetLocalPosition(Vector2D(-400.0f+i*64, 200.0f+j*64));
				platform->SetLayerName(L"Platform");
				Rigidbody2D* rigid = platform->AddComponent<Rigidbody2D>();
				ConvexpolygonCollider* collider = platform->AddComponent<ConvexpolygonCollider>();
				collider->SetPolygon({ Vector2D(-32, 32), Vector2D(-32, 32), Vector2D(32, -32), Vector2D(32, 32) });
				rigid->SetPositionXLock(true);
				rigid->SetPositionYLock(true);
				rigid->SetRotationLock(true);
				rigid->SetAffectedByGravity(false);
				rigid->SetRestitutionValue(0.f);
			}*/



		}
	}
}

void Level::SoftBodyScene(Level& level)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();
	for (int j = 0; j < level.GetData(1); j++)
	{
		for (int i = 0; i < level.GetData(0); i++)
		{
			if (level.GetStateData(j, i) == 2 || level.GetStateData(j, i) == 3)
			{
				GameObject* platform = currentScene->CreateGameObject(L"SoftBodyScene");
				platform->SetLayerName(L"SoftBodyScene");
				platform->GetTransform()->SetLocalPosition(Vector2D(-400.0f + i * 64, 200.0f + j * 64));
				ConvexpolygonCollider* platformcollider = platform->AddComponent<ConvexpolygonCollider>();
			}
		
		}
	}
}

void Level::CreateItem(Level& level)
{
	for (int j = 0; j < level.GetData(1); j++)
	{
		for (int i = 0; i < level.GetData(0); i++)
		{
			if (level.GetStateData(j, i) == 9)
			{
				Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();
				GameObject* object = currentScene->CreateGameObject(L"Item");
				object->SetLayerName(L"Item");
				object->AddComponent<ItemMove>();
				
				ConvexpolygonCollider* collider = object->AddComponent<ConvexpolygonCollider>();
				collider->SetRegularPolygon(4, 50.0f);
				object->GetTransform()->SetLocalPosition(Vector2D( - 400 + 64 * i, 200 + 64 * j));
				SpriteRenderer* sprite = object->AddComponent<SpriteRenderer>();
				sprite->SetSpriteName(L"Item");
				sprite->SetImageNumber(0);
				sprite->SetLayer(200);
			}
		}
	}
}


void Level::Tiling(Level& level)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	for (int j = 0; j < level.GetData(1); j++)
	{
		for (int i = 0; i < level.GetData(0); i++)
		{
			if (level.GetFlagData(j, i) == 1)
			{
				if (level.GetSheetData(j, i) == 0 && level.GetStateData(j,i)<2)
				{
					int xPosition = level.GetXPosition(j, i);
					int yPosition = level.GetYPosition(j, i);
					GameObject* platform = currentScene->CreateGameObject(L"Platform");
					platform->GetTransform()->SetLocalPosition(Vector2D(-400.0f + i * 64, 200.0f + j * 64));
					
					SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();

					platformImage->SetSpriteName(L"TileSheet0");
					platformImage->SetImageNumber(4 * yPosition + xPosition);
					platformImage->SetLayer(Code404::Layer::Tile);
					platform->SetLayerName(L"Platform");
				}
				else if (level.GetSheetData(j, i) == 1 && level.GetStateData(j, i) < 2)
				{
					int xPosition = level.GetXPosition(j, i);
					int yPosition = level.GetYPosition(j, i);
					GameObject* platform = currentScene->CreateGameObject(L"Platform");
					platform->GetTransform()->SetLocalPosition(Vector2D(-400.0f + i * 64, 200.0f + j * 64));
					platform->GetTransform()->SetLocalScale(Vector3D(1.01f, 1.01f, 1.f));
					SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();

					platformImage->SetSpriteName(L"TileSheet1");
					platformImage->SetImageNumber(4 * yPosition + xPosition);
					platformImage->SetLayer(Code404::Layer::Tile);
					platform->SetLayerName(L"Platform");
				}
			}
		}
	}
}

void Level::TilingSecond(Level& level)
{
	Scene* currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene();

	for (int j = 0; j < level.GetData(1); j++)
	{
		for (int i = 0; i < level.GetData(0); i++)
		{
			if (level.GetFlagData(j, i) == 1)
			{
				if (level.GetSheetData(j, i) == 0)
				{
					int xPosition = level.GetXPosition(j, i);
					int yPosition = level.GetYPosition(j, i);
					GameObject* platform = currentScene->CreateGameObject(L"Platform");
					platform->GetTransform()->SetLocalPosition(Vector2D(3440.0f + i * 64, 200.0f + j * 64));
					SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();

					platformImage->SetSpriteName(L"TileSheet0");
					platformImage->SetImageNumber(4 * yPosition + xPosition);
					platformImage->SetLayer(Code404::Layer::Tile);
					platform->SetLayerName(L"Platform");
				}
				else if (level.GetSheetData(j, i) == 1)
				{
					int xPosition = level.GetXPosition(j, i);
					int yPosition = level.GetYPosition(j, i);
					GameObject* platform = currentScene->CreateGameObject(L"Platform");
					platform->GetTransform()->SetLocalPosition(Vector2D(3440.0f + i * 64, 200.0f + j * 64));
					SpriteRenderer* platformImage = platform->AddComponent<SpriteRenderer>();

					platformImage->SetSpriteName(L"TileSheet1");
					platformImage->SetImageNumber(4 * yPosition + xPosition);
					platformImage->SetLayer(Code404::Layer::Tile);
					platform->SetLayerName(L"Platform");
				}
			}
		}
	}
}

