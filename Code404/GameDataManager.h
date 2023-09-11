#pragma once
#include <map>
#include <string>
class GameDataManager
{
public:
	// 게임을 새로 시작할 때 필요한 초기화
	void Clear();
	void ClearCheat();
	void CheatOn();


	// player
	float hp;
	float mp;
	int stealthCount;
	//std::map<std::wstring, std::wstring> map2;

	// On	: EHDHKWNTPDYQJAWNSTJSTODSLA
	// Off	: RKATKGKQSLEKTJSTODSLA
	bool runningCheat;

	bool debugmode;

	//singleton
public:
	static GameDataManager& GetInstance();

private:
	GameDataManager();
	~GameDataManager();
	GameDataManager(const GameDataManager& ref);
	GameDataManager& operator=(const GameDataManager& ref) {}
};

