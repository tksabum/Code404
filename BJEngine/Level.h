#pragma once
#pragma warning(disable : 4996)
#include <vector>
#include <string>
#include <fstream>
#define STAGE_ONE_ONE "stage/stage1-1.txt"
#define STAGE_ONE_TWO "stage/stage1-2.txt"
#define STAGE_ONE_THREE "stage/stage1-3.txt"
#define STAGE_ONE_FOUR_ONE "stage/stage1-4-1.txt"
#define STAGE_ONE_FOUR_TWO "stage/stage1-4-2.txt"
#define STAGE_ONE_FIVE "stage/stage1-5.txt"
#define STAGE_TWO_ONE "stage/stage2-1.txt"
#define STAGE_TWO_TWO "stage/stage2-2.txt"
#define STAGE_TWO_THREE_ONE "stage/stage2-3-1.txt"
#define STAGE_TWO_THREE_TWO "stage/stage2-3-2.txt"
#define STAGE_TWO_FOUR "stage/stage2-4.txt"
#define STAGE_TWO_FIVE "stage/stage2-5.txt"
#define STAGE_TWO_SIX_ONE "stage/stage2-6-1.txt"
#define STAGE_TWO_SIX_TWO "stage/stage2-6-2.txt"
#define STAGE_TWO_SEVEN "stage/stage2-7.txt"
#define STAGE_TWO_EIGHT_ONE "stage/stage2-8-1.txt"
#define STAGE_TWO_EIGHT_TWO "stage/stage2-8-2.txt"
#define STAGE_TWO_NINE "stage/stage2-9.txt"
#define STAGE_BOSS "stage/BossStage_2.txt"
#define STAGE_LONGMAP "stage/Longmap.txt"


struct Rectangle2
{
	int x;
	int y;
	int width;
	int height;
};

class Level
{
public:
	// void LoadLevel();
	Level(const char* filename);
	void Print();
	int GetData(int index);
	int GetTileData(int y,int x);
	int GetFlagData(int y,int x);

	int GetimageXData(int y,int x);
	int GetimageYData(int y,int x);

	int GetXPosition(int y, int x);
	int GetYPosition(int y, int x);

	void SetFlagData(int y, int x, int value);
	int GetSheetData(int y, int x);
	int GetStateData(int y, int x);
	void CreatePlatformFromLevelData(Level& level);
	void CreatePlatformFromLevelDataSecond(Level& level);
	void SoftBodyScene(Level& level);
	void CreateItem(Level& level);
	void Tiling(Level& level);
	void TilingSecond(Level& level);
private:
	std::vector<int> data;	// 타일의 정보를 불러올 가장 기초적인 data
	std::vector<std::vector<int>> tileData;	// 타일의 정보로 만들 맵 정보
	std::vector<std::vector<int>> FlagData;	// 타일이 있는지 판단하는 플래그 정보
	std::vector<std::vector<int>> imageXData;	// 어떤 X좌표 이미지가 있는지 판단하는 플래그 정보
	std::vector<std::vector<int>> imageYData;	// 어떤 Y좌표 이미지가 있는지 판단하는 플래그 정보
	std::vector<std::vector<int>> sheetData;
	std::vector<std::vector<int>> stateData;
	int xPos,yPos;
	int sheet;
	int width, height, cellSize;		// 가로,세로,셀사이즈
};

