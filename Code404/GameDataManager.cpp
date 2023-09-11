#include "GameDataManager.h"

#include "Player.h"

void GameDataManager::Clear()
{
	hp = Player::MAX_HP_DEFAULT;
	mp = Player::MAX_MP_DEFAULT;
	stealthCount = Player::STEALTH_COUNT_MAX;
}

void GameDataManager::ClearCheat()
{
	runningCheat = false;
}

void GameDataManager::CheatOn()
{
	runningCheat = true;
}

GameDataManager& GameDataManager::GetInstance()
{
	static GameDataManager instance;
	return instance;
}

GameDataManager::GameDataManager()
{
	runningCheat = false;
	debugmode = false;

	// 처음 인자 : 죽은 위치 KMJ
	// 뒤에 인자 : 생성될 위치
	/*map2.insert((L"SampleScene"), (L"SampleScene"));
	map2.insert((L"SampleScene2"), (L"SampleScene2"));
	map2.insert((L"SampleScene3"), (L"SampleScene3"));
	map2.insert((L"SampleScene4"), (L"SampleScene4"));
	map2.insert((L"Stage1_1"), (L"Stage1_1"));
	map2.insert((L"Stage1_2"), (L"Stage1_2"));
	map2.insert((L"Stage1_3"), (L"Stage1_3"));
	map2.insert((L"Stage1_4"), (L"Stage1_4"));
	map2.insert((L"Stage1_5"), (L"Stage1_5"));
	map2.insert((L"Stage2_1"), (L"Stage2_1"));
	map2.insert((L"Stage2_2"), (L"Stage2_2"));
	map2.insert((L"Stage2_3"), (L"Stage2_3"));
	map2.insert((L"Stage2_4"), (L"Stage2_4"));
	map2.insert((L"Stage2_5"), (L"Stage2_5"));
	map2.insert((L"Stage2_6"), (L"Stage2_6"));
	map2.insert((L"Stage2_7"), (L"Stage2_7"));
	map2.insert((L"Stage2_8"), (L"Stage2_8"));
	map2.insert((L"Stage2_9"), (L"Stage2_9"));*/

}

GameDataManager::~GameDataManager()
{

}