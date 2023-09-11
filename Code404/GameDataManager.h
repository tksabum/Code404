#pragma once
#include <map>
#include <string>
class GameDataManager
{
public:
	// ������ ���� ������ �� �ʿ��� �ʱ�ȭ
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

