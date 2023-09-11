#pragma once

#include <string>

enum class ENEMYATTACK_TYPE
{
	MELEE_A,
	MELEE_B,
	RANGE_A,
	RANGE_B,
	BOSSHEAD,
	BOSSHAND,
	BOSSCROSSLASER,
	BOSSSCENELASER,
	BOSSBULLET
};

enum class PLAYERATTACK_TYPE
{
	COMBO1,
	COMBO2,
	COMBO3,
	DAGGER,
	PARRYING_BULLET
};

namespace Code404
{
	namespace SceneName
	{
		const std::wstring stage1_1 = L"Stage1_1";
		const std::wstring stage1_2 = L"Stage1_2";
		const std::wstring stage1_3 = L"Stage1_3";
		const std::wstring stage1_4 = L"Stage1_4";
		const std::wstring stage1_5 = L"Stage1_5";
		const std::wstring stage2_1 = L"Stage2_1";
		const std::wstring stage2_2 = L"Stage2_2";
		const std::wstring stage2_3 = L"Stage2_3";
		const std::wstring stage2_4 = L"Stage2_4";
		const std::wstring stage2_5 = L"Stage2_5";
		const std::wstring stage2_6 = L"Stage2_6";
		const std::wstring stage2_7 = L"Stage2_7";
		const std::wstring stage2_8 = L"Stage2_8";
		const std::wstring stage2_9 = L"Stage2_9";
		const std::wstring BossStage = L"SampleScene4";
	}

	const float MELEE_A_DAMAGE = 20.0f;
	const float MELEE_A_HP = 250.0f;
	const float MELEE_A_PATROL_SPEED = 50.f;
	const float MELEE_A_TRACE_SPEED = 100.f;
	const float MELEE_A_ATTACK_DELAY = 4.f;
	
	const float MELEE_B_DAMAGE = 50.0f;
	const float MELEE_B_HP = 450.0f;
	const float MELEE_B_PATROL_SPEED = 50.f;
	const float MELEE_B_TRACE_SPEED = 100.f;
	const float MELEE_B_ATTACK_DELAY = 4.f;
	
	const float RANGE_A_DAMAGE = 20.0f;
	const float RANGE_A_HP = 250.0f;
	const float RANGE_A_PATROL_SPEED = 50.f;
	const float RANGE_A_TRACE_SPEED = 100.f;
	const float RANGE_A_ATTACK_DELAY = 4.f;
	const float RANGE_A_BULLET_SPEED = 1000.f;

	const float RANGE_B_DAMAGE = 50.0f;
	const float RANGE_B_HP = 450.0f;
	const float RANGE_B_PATROL_SPEED = 50.f;
	const float RANGE_B_TRACE_SPEED = 100.f;
	const float RANGE_B_ATTACK_DELAY = 4.f;
	const float RANGE_B_BULLET_SPEED = 1500.f;

	const float BOSS_HEAD_MAX_HP = 2000.f;
	const float BOSS_HAND_MAX_HP = 1000.f;
	const float BOSS_NORMAL_BULLET_DAMAGE = 50.f;
	const float BOSS_TRACE_BULLET_DAMAGE = 80.f;
	const float BOSS_CROSS_LASER_DAMAGE = 50.f;
	const float BOSS_PATROL_LASER_DAMAGE = 20.f;
	const float BOSS_TIME_LASER_DAMAGE = 100.f;

	namespace Layer
	{
		const int BackGround = 0;
		const int TileMap = 1;
		const int Player = 2;
		const int Item = 3;
		const int EnemyBullet = 4;
		const int Dagger = 5;
		const int Effect = 105;
		const int Tile = 100;

		// 보스씬 레이어
		const int BossPatrolLaser = 10;
		const int BossTimeLaser = 10;
		const int BossCCTV = 11;
		const int BossCCTVEffect = 12;
		const int BossHead = 13;
		const int BossHand = 14;
		const int BossChildHand = 15;
		const int BossEye = 16;
		const int BossCrossLaser = 17;
		const int BossCrossLaserCenter = 18;


		// InGame UI
		const int UILow = 99;
		const int UIDefault = 100;
		const int UIHigh= 101;

		// Menu UI
		const int UI0 = 0;
		const int UI1 = 1;
		const int UI2 = 2;
		const int UI3 = 3;
		const int UI4 = 4;
		const int UI5 = 5;

		const int BOSS_BLACKMASK = 200;

		const int SuperTop = 99999999;
	};


}