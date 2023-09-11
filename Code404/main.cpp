#include <Windows.h>

#include "Input.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CoreSystem.h"
#include "EngineSetting.h"
#include "GraphicsResourceManager.h"
#include "PhysicsSystem.h"
#include "AnimationResourceManager.h"
#include "SoundManager.h"
#include "MessageManager.h"
#include "UISoundController.h"
#include "SampleScene.h"
#include "SampleScene2.h"
#include "SampleScene3.h"
#include "SampleScene4.h"
#include "SampleScene5.h"
#include "SampleScene6.h"
#include "MainScene.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"
#include "Stage1_4.h"
#include "Stage1_5.h"
#include "Stage2_1.h"
#include "Stage2_2.h"
#include "Stage2_3.h"
#include "Stage2_4.h"
#include "Stage2_5.h"
#include "Stage2_6.h"
#include "Stage2_7.h"
#include "Stage2_8.h"
#include "Stage2_9.h"
#include "PrevBossScene.h"
#include "BossEndingCutScene.h"
#include "MainSceneAnimation.h"

using namespace BJEngine;
using namespace BJEngine::Math;

using namespace std;

///
/// 예제 1.
/// 가장 기본적인 형태의 Windows 프로그램 (유니코드 버전)
/// WinMain 디폴트
/// 2022.11.09 게임인재원 김범준

/// <summary>
/// Win32API를 이용한 데스크톱 어플리케이션
/// 프로그램의 엔트리 포인트
/// </summary>
/// <param name="hInstance">인스턴스의 핸들</param>
/// <param name="hPrevInstance">이전 인스턴스의 핸들(Win16의 잔재)</param>
/// <param name="lpCmdLine">실행시 인자</param>
/// <param name="nCmdShow">실행시 인자</param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// 엔진세팅
	EngineSetting::GetInstance().SetShowDebugInfo(false);
	EngineSetting::GetInstance().engineMode = EngineSetting::Mode::MODE_2D; // 2D모드
	EngineSetting::GetInstance().graphicsEngine = EngineSetting::GraphicsEngine::DIRECT2D_GRAPHICS; // Direct2D 그래픽스

	// Layer 설정
	EngineSetting::GetInstance().SetLayerName(1, L"Trigger");
	EngineSetting::GetInstance().SetLayerName(2, L"StealthPlayer");
	EngineSetting::GetInstance().SetLayerName(3, L"Player");


	EngineSetting::GetInstance().SetLayerName(4, L"Dagger");
	EngineSetting::GetInstance().SetLayerName(5, L"Platform");
	EngineSetting::GetInstance().SetLayerName(6, L"InvisiblePlatform");
	EngineSetting::GetInstance().SetLayerName(8, L"Bullet");
	EngineSetting::GetInstance().SetLayerName(9, L"Item");

	EngineSetting::GetInstance().SetLayerName(10, L"Enemy");
	EngineSetting::GetInstance().SetLayerName(11, L"Boss");

	EngineSetting::GetInstance().SetLayerName(12, L"CutSceneObject");

	EngineSetting::GetInstance().SetLayerName(13, L"Event");
	EngineSetting::GetInstance().SetLayerName(14, L"SoftBodyScene");

	// Layer간의 충돌설정
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Trigger"), EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Trigger"), EngineSetting::GetInstance().GetLayerID(L"Player"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Trigger"), EngineSetting::GetInstance().GetLayerID(L"Dagger"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Trigger"), EngineSetting::GetInstance().GetLayerID(L"Platform"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Trigger"), EngineSetting::GetInstance().GetLayerID(L"Bullet"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Trigger"), EngineSetting::GetInstance().GetLayerID(L"Enemy"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Trigger"), EngineSetting::GetInstance().GetLayerID(L"Boss"));

	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"), EngineSetting::GetInstance().GetLayerID(L"Dagger"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"), EngineSetting::GetInstance().GetLayerID(L"Platform"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"), EngineSetting::GetInstance().GetLayerID(L"InvisiblePlatform"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"), EngineSetting::GetInstance().GetLayerID(L"Item"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"), EngineSetting::GetInstance().GetLayerID(L"Boss"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"), EngineSetting::GetInstance().GetLayerID(L"Event"));
	
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Player"), EngineSetting::GetInstance().GetLayerID(L"Dagger"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Player"), EngineSetting::GetInstance().GetLayerID(L"Platform"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Player"), EngineSetting::GetInstance().GetLayerID(L"InvisiblePlatform"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Player"), EngineSetting::GetInstance().GetLayerID(L"Bullet"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Player"), EngineSetting::GetInstance().GetLayerID(L"Item"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Player"), EngineSetting::GetInstance().GetLayerID(L"Boss"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Player"), EngineSetting::GetInstance().GetLayerID(L"Event"));

	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Dagger"), EngineSetting::GetInstance().GetLayerID(L"Platform"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Dagger"), EngineSetting::GetInstance().GetLayerID(L"Enemy"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Dagger"), EngineSetting::GetInstance().GetLayerID(L"Boss"));

	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Platform"), EngineSetting::GetInstance().GetLayerID(L"Enemy"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Platform"), EngineSetting::GetInstance().GetLayerID(L"Boss"));

	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"InvisiblePlatform"), EngineSetting::GetInstance().GetLayerID(L"Enemy"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"InvisiblePlatform"), EngineSetting::GetInstance().GetLayerID(L"Boss"));

	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Bullet"), EngineSetting::GetInstance().GetLayerID(L"Platform"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Bullet"), EngineSetting::GetInstance().GetLayerID(L"Enemy"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"Bullet"), EngineSetting::GetInstance().GetLayerID(L"Boss"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"CutSceneObject"), EngineSetting::GetInstance().GetLayerID(L"CutSceneObject"));
	EngineSetting::GetInstance().SetCollisionSetting(true, EngineSetting::GetInstance().GetLayerID(L"SoftBodyScene"), EngineSetting::GetInstance().GetLayerID(L"StealthPlayer"));

	// 엔진 초기화
	CoreSystem::GetInstance().Initialize(hInstance, L"Code404", 1920, 1080, false);
	SceneManager* sceneManager = CoreSystem::GetInstance().GetSceneManager();

	// 사운드 객체 생성
	

	// Sprite 생성
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Death", L"images/character/player/character_spreadsheet.png", 0, 50, 512 * 4, 206, 362, 206, 150, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Stand", L"images/character/player/character_spreadsheet.png", 0, 306, 2048, 206, 412, 206, 100, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Attacked", L"images/character/player/character_spreadsheet.png", 0, 562, 512 * 3, 206, 412, 206, 100, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Dash", L"images/character/player/character_spreadsheet.png", 0, 818, 2048, 206, 512, 206, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Jump", L"images/character/player/character_spreadsheet.png", 0, 1024, 2560, 256, 412, 256, 100, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Run", L"images/character/player/character_spreadsheet.png", 0, 1330, 3072, 206, 462, 206, 50, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_WithdrawDagger_Run", L"images/character/player/character_spreadsheet.png", 0, 1536, 2560, 256, 462, 256, 50, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_ThrowDagger_Run", L"images/character/player/character_spreadsheet.png", 0, 1842, 2560, 206, 462, 206, 50, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_ThrowDagger_Jump", L"images/character/player/character_spreadsheet.png", 0, 2048, 2560, 256, 412, 256, 100, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_WithdrawDagger_Jump", L"images/character/player/character_spreadsheet.png", 0, 2304, 2560, 256, 412, 256, 100, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_WithdrawDagger_Stand", L"images/character/player/character_spreadsheet.png", 0, 2866, 2560, 206, 412, 206, 100, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_ThrowDagger_Stand", L"images/character/player/character_spreadsheet.png", 0, 3122, 2560, 206, 412, 206, 100, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_NormalAttack_Jump", L"images/character/player/character_spreadsheet.png", 0, 3840, 512 * 5, 256, 432, 256, 80, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_NormalAttack", L"images/character/player/character_spreadsheet.png",		512 * 0, 2610, 512 * 3, 206, 452, 206, 60, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Wait_ComboAttack", L"images/character/player/character_spreadsheet.png",	512 * 3, 2610, 512 * 3, 206, 452, 206, 60, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_ComboAttack", L"images/character/player/character_spreadsheet.png",		512 * 6, 2610, 512 * 3, 206, 432, 206, 80, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Wait_FinalAttack", L"images/character/player/character_spreadsheet.png",	512 * 9, 2610, 512 * 3, 206, 432, 206, 80, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_FinalAttack", L"images/character/player/character_spreadsheet.png",		512 * 12, 2610, 512 * 6, 206, 452, 206, 60, 0);

	//GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Flying", L"images/character/player/character_spreadsheet.png", 0, 3328, 2560, 256, 512, 256, 0, 0);
	//GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Stuck", L"images/character/player/character_spreadsheet.png", 2560, 3328, 512, 256, 512, 256, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Flying_A", L"images/character/player/character_spreadsheet.png", 0, 3328, 2560, 256, 512, 256, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Flying_D", L"images/character/player/character_spreadsheet.png", 0, 4096, 2560, 256, 512, 256, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Flying_S", L"images/character/player/character_spreadsheet.png", 0, 4608, 2560, 256, 512, 256, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Stuck_A", L"images/character/player/Knife/knife.png", 0, 0, 184, 81, 184, 81, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Stuck_S", L"images/character/player/Knife/knife2.png", 0, 0, 184, 81, 184, 81, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Stuck_D", L"images/character/player/Knife/knife3.png", 0, 0, 184, 81, 184, 81, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Effect", L"images/이펙트스프라이트시트.png", 0, 512 * 1, 2560, 512, 512, 512, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Effect_A", L"images/이펙트스프라이트시트.png", 0, 512 * 1, 2560, 512, 512, 512, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Effect_S", L"images/이펙트스프라이트시트.png", 0, 512 * 2, 2560, 512, 512, 512, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Effect_D", L"images/이펙트스프라이트시트.png", 0, 512 * 3, 2560, 512, 512, 512, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Dash_Effect", L"images/이펙트스프라이트시트.png", 0, 512 * 0, 512 * 7, 512, 512, 512, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Jump_Effect", L"images/이펙트스프라이트시트.png", 0, 512 * 7, 512 * 5, 512, 512, 512, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Land_Effect", L"images/이펙트스프라이트시트.png", 0, 512 * 8, 512 * 7, 512, 512, 512, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Dagger_Collision_Effect", L"images/이펙트스프라이트시트.png", 0, 512 * 4, 512 * 7, 512, 512, 512, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Parrying_Effect", L"images/character/player/effect2.png", 0, 0, 512 * 6, 512, 512, 512, 0, 0);

	// 몬스터 HP바s
	GraphicsResourceManager::GetInstance().CreateSprite(L"Item", L"images/Item/산소통.png", 0, 0, 64, 64, 64, 64, 0, 0);

	// 몬스터 HP바
	GraphicsResourceManager::GetInstance().CreateSprite(L"Mob_HPBar", L"images/mobhpbar.png", 0, 0, 20, 6, 20, 6, 0, 0);
	// 몬스터 HP바 테두리
	GraphicsResourceManager::GetInstance().CreateSprite(L"Mob_HPBar_OutLine", L"images/mobhpbaroutline.png", 0, 0, 20, 6, 20, 6, 0, 0);

	// 근접 몬스터 tpyeA Sprite
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeA_Enemy_Stand", L"images/근거리_시트_수정.png", 0, 0, 4800, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeA_Enemy_Walk", L"images/근거리_시트_수정.png", 0, 268, 2400, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeA_Enemy_Attack", L"images/근거리_시트_수정.png", 0, 268 * 2, 3200, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeA_Enemy_Hit", L"images/근거리_시트_수정.png", 0, 268 * 3, 2800, 268, 400, 268, 0, 0);
	// 근접 몬스터 tpyeB Sprite
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeB_Enemy_Stand", L"images/근거리2_시트_수정.png", 0, 0, 4800, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeB_Enemy_Walk", L"images/근거리2_시트_수정.png", 0, 268, 2400, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeB_Enemy_Attack", L"images/근거리2_시트_수정.png", 0, 268 * 2, 3200, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MeleeB_Enemy_Hit", L"images/근거리2_시트_수정.png", 0, 268 * 3, 2800, 268, 400, 268, 0, 0);
	// 원거리 몬스터 typeA Sprite
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeA_Enemy_Stand", L"images/원거리_시트1.png", 0, 0, 3200, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeA_Enemy_Walk", L"images/원거리_시트1.png", 0, 268, 2800, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeA_Enemy_Attack", L"images/원거리_시트1.png", 0, 268 * 2, 2400, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeA_Enemy_Hit", L"images/원거리_시트1.png", 0, 268 * 3, 2000, 268, 400, 268, 0, 0);
	// 원거리 몬스터 typeB Sprite
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeB_Enemy_Stand", L"images/원거리_시트2.png", 0, 0, 3200, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeB_Enemy_Walk", L"images/원거리_시트2.png", 0, 268, 2800, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeB_Enemy_Attack", L"images/원거리_시트2.png", 0, 268 * 2, 2400, 268, 400, 268, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeB_Enemy_Hit", L"images/원거리_시트2.png", 0, 268 * 3, 2000, 268, 400, 268, 0, 0);
	// 원거리 몬스터 총알
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeA_Bullet", L"images/RangeA_Bullet.png", 0, 0, 49, 21, 49, 21, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"RangeB_Bullet", L"images/RangeB_Bullet.png", 0, 0, 49, 21, 49, 21, 0, 0);
	// 원거리 몬스터 총구 이펙트
	GraphicsResourceManager::GetInstance().CreateSprite(L"Bullet_Effect", L"images/bullet_effect.png", 0, 0, 360, 36, 72, 36, 0, 0);

	// 몬스터 죽는 이펙트 Sprite
	GraphicsResourceManager::GetInstance().CreateSprite(L"Enemy_Die_Effect", L"images/이펙트스프라이트시트.png", 0, 512 * 5, 4096, 512, 512, 512, 0, 0);

	// 보스 맵 배경
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Scene_BackGround", L"images/Bossmap.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);

	// 보스 스프라이트
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Head_Blue", L"images/boss_head_blue.png", 0, 0, 432, 533, 432, 522, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Hand_Blue", L"images/boss_hand_blue.png", 0, 0, 434, 324, 434, 324, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Head_Red", L"images/boss_head_red.png", 0, 0, 432, 533, 432, 522, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Hand_Red", L"images/boss_hand_red.png", 0, 0, 434, 324, 434, 324, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Eye_Blue", L"images/eyes_blue.png", 0, 0, 96, 96, 96, 96, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Eye_Red", L"images/eyes_red.png", 0, 0, 96, 96, 96, 96, 0, 0);

	// 보스가 쏘는 총알 스프라이트
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Normal_Bullet", L"images/BossNormalBullet.png", 0, 0, 64, 64, 64, 64, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Trace_Bullet", L"images/BossTraceBullet.png", 0, 0, 512, 512, 512, 512, 0, 0);

	// 보스 눈에서 나오는 레이저 스프라이트
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Cross_Laser", L"images/BossCrossLaser.png", 0, 0, 8800, 50, 4400, 50, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Cross_Laser_Center", L"images/BossCrossLaserCenter.png", 0, 0, 512, 256, 256, 256, 0, 0);

	// 보스씬에서 사용하는 패트롤과 타임 레이저 스프라이트
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Yellow_Laser", L"images/BossPatrolLaser.png", 0, 0, 13200, 32, 4400, 32, 0, 0);

	// 보스 HP바, 테두리 스프라이트
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_HP_Bar", L"images/monsterbar.png", 0, 0, 1577, 40, 1577, 40, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_HP_Bar_OutLine", L"images/monsterbar_outline.png", 0, 0, 1577, 40, 1577, 40, 0, 0);

	// 보스 초상화
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Protrait_Blue", L"images/Boss_Protrait_Blue.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Protrait_Red", L"images/Boss_Protrait_Red.png", 0, 0, 128, 128, 128, 128, 0, 0);

	// 보스가 내려찍을 때 나오는 이펙트
	GraphicsResourceManager::GetInstance().CreateSprite(L"TakeDown_Effect", L"images/TakeDownEffect.png", 0, 0, 3072, 154, 512, 154, 0, 0);
	// 보스 죽을 때 눈에서 나오는 이펙트
	GraphicsResourceManager::GetInstance().CreateSprite(L"Boss_Die_Effect", L"images/BossDieEffect.png", 0, 0, 5120, 512, 512, 512, 0, 0);

	// 타임 레이저 CCTV
	GraphicsResourceManager::GetInstance().CreateSprite(L"Time_Laser_CCTV", L"images/CCTV.png", 0, 0, 64, 64, 64, 64, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Time_Laser_CCTV_Effect", L"images/CCTV_effect.png", 0, 0, 512, 128, 128, 128, 0, 0);

	// 맨 마지막 아무키나 누르세요
	GraphicsResourceManager::GetInstance().CreateSprite(L"Press_Any_Key", L"images/cutscene/press.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);

#pragma region BackGround
	GraphicsResourceManager::GetInstance().CreateSprite(L"Background3840x1080", L"images/background/Internal background/3840_1080.png", 0, 0, 3840, 1080, 3840, 1080, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"ExternalBackgroundMiddle", L"images/background/External background/External background_(middle).png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"ExternalBackgroundDistantView", L"images/background/External background/External background_distant view.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"ExternalBackgroundObject1", L"images/background/External background/External background_obj1.png", 0, 0, 1080, 1080, 1080, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"ExternalBackgroundObject2", L"images/background/External background/External background_obj2.png", 0, 0, 1080, 1080, 1080, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"ExternalBackgroundObject3", L"images/background/External background/External background_obj3.png", 0, 0, 1080, 1080, 1080, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"ExternalBackgroundObject4", L"images/background/External background/External background_obj4.png", 0, 0, 1080, 1080, 1080, 1080, 0, 0);
	
	
	GraphicsResourceManager::GetInstance().CreateSprite(L"InternalBackground1920x2160", L"images/background/Internal background/1920_2160.png", 0, 0, 1920, 2160, 1920, 2160, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"InternalBackground1920x4320", L"images/background/Internal background/1920_4320.png", 0, 0, 1920, 4320, 1920, 4320, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"InternalBackground3840x1080", L"images/background/Internal background/3840_1080.png", 0, 0, 3840, 1080, 3840, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"InternalBackgroundMiddle", L"images/background/Internal background/camera_move.png", 0, 0, 668, 886, 668, 886, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"InternalBackgroundOrigin", L"images/background/Internal background/Internal background.png", 0, 0, 2118, 1080, 2118, 1080, 0, 0);
#pragma endregion

#pragma region UI
	// UI
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Failed_Text", L"images/UI/fail/404failed.png", 0, 0, 800, 300, 800, 300, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Save_Button_Idle", L"images/UI/fail/save.png", 0, 0, 480, 120, 480, 120, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Save_Button_Click", L"images/UI/fail/save_click.png", 0, 0, 480, 120, 480, 120, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Save_Button_Enter", L"images/UI/fail/save_cursor.png", 0, 0, 480, 120, 480, 120, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Quit_Button_Idle", L"images/UI/fail/quit.png", 0, 0, 480, 120, 480, 120, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Quit_Button_Click", L"images/UI/fail/quit_click.png", 0, 0, 480, 120, 480, 120, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Quit_Button_Enter", L"images/UI/fail/quit_cursor.png", 0, 0, 480, 120, 480, 120, 0, 0);


	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Exit_Button_Idle", L"images/UI/main/exit.png", 0, 0, 200, 128, 200, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Exit_Button_Click", L"images/UI/main/exit_click.png", 0, 0, 200, 128, 200, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Exit_Button_Enter", L"images/UI/main/exit_cursor.png", 0, 0, 200, 128, 200, 128, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Setting_Button_Idle", L"images/UI/main/setting.png", 0, 0, 350, 128, 350, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Setting_Button_Click", L"images/UI/main/setting_click.png", 0, 0, 350, 128, 350, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Setting_Button_Enter", L"images/UI/main/setting_cursor.png", 0, 0, 350, 128, 350, 128, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Start_Button_Idle", L"images/UI/main/start.png", 0, 0, 350, 128, 350, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Start_Button_Click", L"images/UI/main/start_click.png", 0, 0, 350, 128, 350, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Start_Button_Enter", L"images/UI/main/start_cursor.png", 0, 0, 350, 128, 350, 128, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Team_Button_Idle", L"images/UI/main/team.png", 0, 0, 350, 128, 350, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Team_Button_Click", L"images/UI/main/team_click.png", 0, 0, 350, 128, 350, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Team_Button_Enter", L"images/UI/main/team_cursor.png", 0, 0, 350, 128, 350, 128, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_PopUp_Window", L"images/UI/popup/popup-100.png", 0, 0, 1000, 900, 1000, 900, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Close_Button_Idle", L"images/UI/popup/x.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Close_Button_Click", L"images/UI/popup/x_click.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Close_Button_Enter", L"images/UI/popup/x_cursor.png", 0, 0, 128, 128, 128, 128, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Team", L"images/UI/team/team.png", 0, 0, 800, 800, 800, 800, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_1080x720_Button_Idle", L"images/UI/display/1080.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_1080x720_Button_Click", L"images/UI/display/1080_click.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_1080x720_Button_Enter", L"images/UI/display/1080_cursor.png", 0, 0, 500, 100, 500, 100, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_1920x1080_Button_Idle", L"images/UI/display/1920.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_1920x1080_Button_Click", L"images/UI/display/1920_click.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_1920x1080_Button_Enter", L"images/UI/display/1920_cursor.png", 0, 0, 500, 100, 500, 100, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Toggle_Border", L"images/UI/display/select_1.png", 0, 0, 64, 64, 64, 64, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Toggle_Selector", L"images/UI/display/select_2.png", 0, 0, 64, 64, 64, 64, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Fullscreen_Button_Idle", L"images/UI/display/full.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Fullscreen_Button_Click", L"images/UI/display/full_click.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Fullscreen_Button_Enter", L"images/UI/display/full_cursor.png", 0, 0, 500, 100, 500, 100, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Window_Button_Idle", L"images/UI/display/window.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Window_Button_Click", L"images/UI/display/window_click.png", 0, 0, 500, 100, 500, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Window_Button_Enter", L"images/UI/display/window_cursor.png", 0, 0, 500, 100, 500, 100, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Back_Button_Idle", L"images/UI/setting/back.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Back_Button_Click", L"images/UI/setting/back_click.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Back_Button_Enter", L"images/UI/setting/back_cursor.png", 0, 0, 600, 140, 600, 140, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Menu_Button_Idle", L"images/UI/setting/menu.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Menu_Button_Click", L"images/UI/setting/menu_click.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Menu_Button_Enter", L"images/UI/setting/menu_cursor.png", 0, 0, 600, 140, 600, 140, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Display_Button_Idle", L"images/UI/setting/display.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Display_Button_Click", L"images/UI/setting/display_click.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Display_Button_Enter", L"images/UI/setting/display_cursor.png", 0, 0, 600, 140, 600, 140, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Sound_Button_Idle", L"images/UI/setting/sound.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Sound_Button_Click", L"images/UI/setting/sound_click.png", 0, 0, 600, 140, 600, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Sound_Button_Enter", L"images/UI/setting/sound_cursor.png", 0, 0, 600, 140, 600, 140, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_SoundController_Button_Idle", L"images/UI/sound/^.png", 0, 0, 35, 45, 35, 45, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_SoundController_Button_Click", L"images/UI/sound/^_click.png", 0, 0, 35, 45, 35, 45, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_SoundController_Button_Enter", L"images/UI/sound/^_cursor.png", 0, 0, 35, 45, 35, 45, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Soundbar", L"images/UI/sound/soundbar.png", 0, 0, 500, 140, 500, 140, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_Soundbar_Background", L"images/UI/sound/soundbar_background.png", 0, 0, 500, 140, 500, 140, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_BGM", L"images/UI/sound/BGM.png", 0, 0, 400, 100, 400, 100, 0, 0);


	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_EffectSound", L"images/UI/sound/soundeffect.png", 0, 0, 400, 100, 400, 100, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UI_MessageBox", L"images/UI/chat/chat.png", 0, 0, 850, 400, 850, 400, 0, 0);

	// 메인화면 애니메이션
	GraphicsResourceManager::GetInstance().CreateSprite(L"MainScreen_1", L"images/UI/main/MainScreen_1.png", 0, 0, 9600, 9720, 1920, 1080, 0, 0);


	GraphicsResourceManager::GetInstance().CreateSprite(L"MainScene_BackGround", L"images/new.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MainScene_Button", L"images/Test_MainScene_Button.png", 0, 0, 350, 128, 350, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MainScene_ExitButton", L"images/Test_MainScene_ExitButton.png", 0, 0, 200, 128, 200, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"MainScene_SettingWindow", L"images/Test_MainScene_SettingWindow.png", 0, 0, 1000, 900, 1000, 900, 0, 0);
	// 창 나가기 버튼
	GraphicsResourceManager::GetInstance().CreateSprite(L"MainScene_SettingCloseButton", L"images/Test_MainScene_SettingCloseButton.png", 0, 0, 128, 128, 128, 128, 0, 0);
	// 소리 설정창 들어가기 버튼
	GraphicsResourceManager::GetInstance().CreateSprite(L"SoundSettingButton", L"images/SettingButton.png", 0, 0, 600, 140, 600, 140, 0, 0);
	// 화면 설정창 들어가기 버튼
	GraphicsResourceManager::GetInstance().CreateSprite(L"ScreenSettingButton", L"images/SettingButton.png", 0, 0, 600, 140, 600, 140, 0, 0);
	// 키 셋팅 들어가기 버튼
	// GraphicsResourceManager::GetInstance().CreateSprite(L"KeySettingButton", L"images/SettingButton.png", 0, 0, 600, 140, 600, 140, 0, 0);
	// 돌아가기 버튼
	GraphicsResourceManager::GetInstance().CreateSprite(L"ReturnButton", L"images/SettingButton.png", 0, 0, 600, 140, 600, 140, 0, 0);

	// 소리 설정창
	// BGM textbox
	// GraphicsResourceManager::GetInstance().CreateSprite(L"BgmTextBox", L"images/SoundTextBox.png", 0, 0, 500, 140, 500, 140, 0, 0);
	// Sound Effect textbox
	// GraphicsResourceManager::GetInstance().CreateSprite(L"SoundEffectTextBox", L"images/SoundTextBox.png", 0, 0, 500, 140, 500, 140, 0, 0);
	// 소리 조절 버튼
	// GraphicsResourceManager::GetInstance().CreateSprite(L"SoundControlButton", L"images/SoundControlButton.png", 0, 0, 100, 100, 100, 100, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"BGMTextBox", L"images/SOUND_SETTING_IMAGE_BGM1.png", 0, 0, 400, 100, 400, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"EffectTextBox", L"images/SOUND_SETTING_IMAGE_BGM2.png", 0, 0, 400, 100, 400, 100, 0, 0);
	// Sound Effect textbox
	GraphicsResourceManager::GetInstance().CreateSprite(L"SoundSettingBar", L"images/SOUND_SETTING_IMAGE_BAR.png", 0, 0, 400, 15, 400, 15, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"SoundSettingBarBackground", L"images/SOUND_SETTING_IMAGE_BGM_BG.png", 0, 0, 440, 120, 440, 120, 0, 0);
	// 소리 조절 버튼
	GraphicsResourceManager::GetInstance().CreateSprite(L"SoundSettingController", L"images/SOUND_SETTING_IMAGE_CONTROLLER.png", 0, 0, 20, 70, 20, 70, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"BgmControlMIN", L"images/SOUND_SETTING_IMAGE_MIN.png", 0, 0, 100, 100, 100, 100, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"BgmControlMAX", L"images/SOUND_SETTING_IMAGE_MAX.png", 0, 0, 100, 100, 100, 100, 0, 0);


	// 화면 설정창
	// 해상도 설정 textbox
	// 1920 * 1080
	GraphicsResourceManager::GetInstance().CreateSprite(L"1920*1080", L"images/ResolutionTextBox.png", 0, 0, 500, 100, 500, 100, 0, 0);
	// 1080 * 700
	GraphicsResourceManager::GetInstance().CreateSprite(L"1080*700", L"images/ResolutionTextBox.png", 0, 0, 500, 100, 500, 100, 0, 0);
	// 전체화면
	GraphicsResourceManager::GetInstance().CreateSprite(L"FullScreen", L"images/ResolutionTextBox.png", 0, 0, 500, 100, 500, 100, 0, 0);
	// 창모드
	GraphicsResourceManager::GetInstance().CreateSprite(L"WindowMode", L"images/ResolutionTextBox.png", 0, 0, 500, 100, 500, 100, 0, 0);
	// SelectTogle
	GraphicsResourceManager::GetInstance().CreateSprite(L"SelectTogle", L"images/SellectTogle.png", 0, 0, 64, 64, 64, 64, 0, 0);
	// 구분선
	GraphicsResourceManager::GetInstance().CreateSprite(L"DividingLine", L"images/DividingLine.png", 0, 0, 600, 10, 600, 10, 0, 0);

	/// 씬 전환 시 사용되는 효과
	GraphicsResourceManager::GetInstance().CreateSprite(L"BlackMask1", L"images/BlackMask1.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"BlackMask2", L"images/BlackMask2.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"BlackMask3", L"images/BlackMask3.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"SwitchScreen", L"images/Switch Screen/Switch Screen.png", 0, 0, 2300, 1080, 2300, 1080, 0, 0);

	// 보스 엔딩 컷씬
	GraphicsResourceManager::GetInstance().CreateSprite(L"CutScene1", L"images/cutscene/cutscene1.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"CutScene2", L"images/cutscene/cutscene2.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"CutScene3", L"images/cutscene/cutscene3.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"CutScene4", L"images/cutscene/cutscene4.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"CutSceneEndLine", L"images/cutscene/cutsceneEndLine.png", 0, 0, 1920, 1080, 1920, 1080, 0, 0);

#pragma endregion

	// 플레이어 UI
	GraphicsResourceManager::GetInstance().CreateSprite(L"GasMask", L"images/maskUI.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"BarBackGround", L"images/BarBackGround.png", 0, 0, 320, 30, 320, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"PlayerHP", L"images/HP.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"PlayerHP_Outline", L"images/HP_outline.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"PlayerMP", L"images/MP.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"PlayerMP_Outline", L"images/MP_outline.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_A", L"images/knifeUI1.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_S", L"images/knifeUI2.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_D", L"images/knifeUI3.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Knife_Cover", L"images/UI/ingame/knifeCover.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Akey", L"images/UI/ingame/knifeUI1key.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Skey", L"images/UI/ingame/knifeUI2key.png", 0, 0, 128, 128, 128, 128, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Dkey", L"images/UI/ingame/knifeUI3key.png", 0, 0, 128, 128, 128, 128, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Stealth0", L"images/UI/ingame/eunsin0.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Stealth1", L"images/UI/ingame/eunsin1.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Stealth2", L"images/UI/ingame/eunsin2.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Stealth3", L"images/UI/ingame/eunsin3.png", 0, 0, 370, 30, 370, 30, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Player_Stealth4", L"images/UI/ingame/eunsin4.png", 0, 0, 370, 30, 370, 30, 0, 0);

	// 빛
	GraphicsResourceManager::GetInstance().CreateSprite(L"Light", L"images/light.png", 0, 0, 500, 500, 500, 500, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"Black", L"images/black.png", 0, 0, 500, 500, 500, 500, 0, 0);

	GraphicsResourceManager::GetInstance().CreateSprite(L"UITest", L"images/panel.png", 0, 0, 533, 300, 533, 300, 0, 0);



	// 타일
	GraphicsResourceManager::GetInstance().CreateSprite(L"TileSheet0", L"images/tile/Tile1.png", 0, 0, 256, 1024, 64, 64, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TileSheet1", L"images/tile/Tile2.png", 0, 0, 256, 1024, 64, 64, 0, 0);


	/// 튜토리얼 UI
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_A", L"images/UI/key/A.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_C", L"images/UI/key/C.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_D", L"images/UI/key/D.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_F", L"images/UI/key/F.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_Left", L"images/UI/key/Left.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_Right", L"images/UI/key/Right.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_S", L"images/UI/key/S.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_X", L"images/UI/key/X.png", 0, 0, 32, 32, 32, 32, 0, 0);
	GraphicsResourceManager::GetInstance().CreateSprite(L"TutorialUI_Z", L"images/UI/key/Z.png", 0, 0, 32, 32, 32, 32, 0, 0);


	// 건물 내부 배경
	GraphicsResourceManager::GetInstance().CreateSprite(L"back", L"images/Internal background.png", 0, 0, 2118, 1080, 2118, 1080, 0, 0);

	// Animation 생성
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Death", L"Player_Death", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Attacked", L"Player_Attacked", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Stand", L"Player_Stand", 0.15f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Run", L"Player_Run", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Jump", L"Player_Jump", 0.1f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Dash", L"Player_Dash", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_WithdrawDagger_Run", L"Player_WithdrawDagger_Run", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_ThrowDagger_Run", L"Player_ThrowDagger_Run", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_ThrowDagger_Jump", L"Player_ThrowDagger_Jump", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_WithdrawDagger_Jump", L"Player_WithdrawDagger_Jump", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_WithdrawDagger_Stand", L"Player_WithdrawDagger_Stand", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_ThrowDagger_Stand", L"Player_ThrowDagger_Stand", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_NormalAttack_Jump", L"Player_NormalAttack_Jump", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_NormalAttack", L"Player_NormalAttack", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_ComboAttack", L"Player_ComboAttack", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_FinalAttack", L"Player_FinalAttack", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Wait_ComboAttack", L"Player_Wait_ComboAttack", 0.05f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Wait_FinalAttack", L"Player_Wait_FinalAttack", 0.05f, false);
	//AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Flying", L"Dagger_Flying", 0.03f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Flying_A", L"Dagger_Flying_A", 0.03f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Flying_S", L"Dagger_Flying_S", 0.03f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Flying_D", L"Dagger_Flying_D", 0.03f, true);
	//AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Stuck", L"Dagger_Stuck", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Stuck_A", L"Dagger_Stuck_A", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Stuck_S", L"Dagger_Stuck_S", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Stuck_D", L"Dagger_Stuck_D", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Effect", L"Dagger_Effect", 0.04f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Effect_A", L"Dagger_Effect_A", 0.04f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Effect_S", L"Dagger_Effect_S", 0.04f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Effect_D", L"Dagger_Effect_D", 0.04f, false);

	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Dash_Effect", L"Player_Dash_Effect", 0.06f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Jump_Effect", L"Player_Jump_Effect", 0.06f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Player_Land_Effect", L"Player_Land_Effect", 0.06f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Dagger_Collision_Effect", L"Dagger_Collision_Effect", 0.03f, false);

	AnimationResourceManager::GetInstance().CreateAnimation(L"Animation_Parrying_Effect", L"Parrying_Effect", 0.05f, false);

	// Enemy 공통 죽기 이펙트
	AnimationResourceManager::GetInstance().CreateAnimation(L"Enemy_Die_Effect", L"Enemy_Die_Effect", 0.1f, false);

	// 근접 몬스터 Animation
	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeA_Enemy_Stand", L"MeleeA_Enemy_Stand", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeA_Enemy_Walk", L"MeleeA_Enemy_Walk", 0.07f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeA_Enemy_Attack", L"MeleeA_Enemy_Attack", 0.1f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeA_Enemy_Hit", L"MeleeA_Enemy_Hit", 0.1f, false);

	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeB_Enemy_Stand", L"MeleeB_Enemy_Stand", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeB_Enemy_Walk", L"MeleeB_Enemy_Walk", 0.07f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeB_Enemy_Attack", L"MeleeB_Enemy_Attack", 0.1f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"MeleeB_Enemy_Hit", L"MeleeB_Enemy_Hit", 0.1f, false);

	// 원거리 몬스터 Animation
	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeA_Enemy_Stand", L"RangeA_Enemy_Stand", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeA_Enemy_Walk", L"RangeA_Enemy_Walk", 0.07f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeA_Enemy_Attack", L"RangeA_Enemy_Attack", 0.1f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeA_Enemy_Hit", L"RangeA_Enemy_Hit", 0.1f, false);

	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeB_Enemy_Stand", L"RangeB_Enemy_Stand", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeB_Enemy_Walk", L"RangeB_Enemy_Walk", 0.07f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeB_Enemy_Attack", L"RangeB_Enemy_Attack", 0.1f, false);
	AnimationResourceManager::GetInstance().CreateAnimation(L"RangeB_Enemy_Hit", L"RangeB_Enemy_Hit", 0.1f, false);
	// 원거리 몬스터 총구 이펙트 애니메이션
	AnimationResourceManager::GetInstance().CreateAnimation(L"Bullet_Effect", L"Bullet_Effect", 0.05f, false);

	// 보스 눈에서 나오는 레이저 애니메이션
	AnimationResourceManager::GetInstance().CreateAnimation(L"Boss_Cross_Laser", L"Boss_Cross_Laser", 0.1f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Boss_Cross_Laser_Center", L"Boss_Cross_Laser_Center", 2.f, true);
	AnimationResourceManager::GetInstance().CreateAnimation(L"Boss_Yellow_Laser", L"Boss_Yellow_Laser", 0.1f, true);

	// 보스가 내려찍을 때 나오는 이펙트 애니메이션
	AnimationResourceManager::GetInstance().CreateAnimation(L"TakeDown_Effect", L"TakeDown_Effect", 0.1f, false);

	// 보스가 죽을 때 눈에서 나오는 이펙트 애니메이션
	AnimationResourceManager::GetInstance().CreateAnimation(L"Boss_Die_Effect", L"Boss_Die_Effect", 0.05f, false);
	// 보스 타임레이저 애니메이션
	AnimationResourceManager::GetInstance().CreateAnimation(L"Time_Laser_CCTV_Effect", L"Time_Laser_CCTV_Effect", 0.05f, true);
	// 메인화면 애니메이션
	AnimationResourceManager::GetInstance().CreateAnimation(L"MainScreen_1", L"MainScreen_1", 0.1f, false);

	/// 이벤트 메세지
	MessageManager::GetInstance().AddMessage(L"EventMessage1-1", L"에이전트 404라고 했나?", L"", 30);
	MessageManager::GetInstance().AddMessage(L"EventMessage1-2", L"이번 임무 문제 없지?", L"", 30);
	MessageManager::GetInstance().AddMessage(L"EventMessage1-3", L"기지 내부로 가서 모든 걸 없었던 일로 만들면 돼.", L"", 30);
	MessageManager::GetInstance().AddMessage(L"EventMessage1-4", L"자세한 건 내부에 진입하면 다시 연락하도록 하지.", L"", 30);
	MessageManager::GetInstance().AddMessage(L"EventMessage2-1", L"내부에 진입한 모양이군.", L"", 30);
	MessageManager::GetInstance().AddMessage(L"EventMessage2-2", L"좋아, 내부에 커다란 고철 덩어리를 처리하고 나면 돼. 그게 끝이야.", L"", 30);
	MessageManager::GetInstance().AddMessage(L"EventMessage2-3", L"행운을 빌지", L"", 30);



	// 씬 생성(scene이 sceneManager에 등록되면 Coresystem::Finalize에서 메모리가 해제된다.)
	Scene* scene001 = new SampleScene();
	Scene* scene002 = new SampleScene2();
	Scene* scene003 = new SampleScene3();
	Scene* scene004 = new SampleScene4();
	Scene* scene005 = new SampleScene5();
	Scene* scene006 = new SampleScene6();
	Scene* mainsceneAnimation = new MainSceneAnimation();
	Scene* mainscene = new MainScene();
	Scene* stage1_1 = new Stage1_1();
	Scene* stage1_2 = new Stage1_2();
	Scene* stage1_3 = new Stage1_3();
	Scene* stage1_4 = new Stage1_4();
	Scene* stage1_5 = new Stage1_5();
	Scene* stage2_1 = new Stage2_1();
	Scene* stage2_2 = new Stage2_2();
	Scene* stage2_3 = new Stage2_3();
	Scene* stage2_4 = new Stage2_4();
	Scene* stage2_5 = new Stage2_5();
	Scene* stage2_6 = new Stage2_6();
	Scene* stage2_7 = new Stage2_7();
	Scene* stage2_8 = new Stage2_8();
	Scene* stage2_9 = new Stage2_9();
	Scene* prevBossStage = new PrevBossScene();
	Scene* bossEndingCutScene = new BossEndingCutScene();
	int sceneid1 = sceneManager->AddScene(scene001);
	int sceneid2 = sceneManager->AddScene(scene002);
	int sceneid3 = sceneManager->AddScene(scene003);
	int sceneid4 = sceneManager->AddScene(scene004);
	int sceneid5 = sceneManager->AddScene(scene005);
	int sceneid6 = sceneManager->AddScene(scene006);
	int mainsceneanimationid = sceneManager->AddScene(mainsceneAnimation);
	int mainsceneid = sceneManager->AddScene(mainscene);
	int stage1_1id = sceneManager->AddScene(stage1_1);
	int stage1_2id = sceneManager->AddScene(stage1_2);
	int stage1_3id = sceneManager->AddScene(stage1_3);
	int stage1_4id = sceneManager->AddScene(stage1_4);
	int stage1_5id = sceneManager->AddScene(stage1_5);
	int stage2_1id = sceneManager->AddScene(stage2_1);
	int stage2_2id = sceneManager->AddScene(stage2_2);
	int stage2_3id = sceneManager->AddScene(stage2_3);
	int stage2_4id = sceneManager->AddScene(stage2_4);
	int stage2_5id = sceneManager->AddScene(stage2_5);
	int stage2_6id = sceneManager->AddScene(stage2_6);
	int stage2_7id = sceneManager->AddScene(stage2_7);
	int stage2_8id = sceneManager->AddScene(stage2_8);
	int stage2_9id = sceneManager->AddScene(stage2_9);
	int preveBossStageid = sceneManager->AddScene(prevBossStage);
	int bossEndingCutSceneid = sceneManager->AddScene(bossEndingCutScene);

	// 사운드 매니저 실행
	
	
	/*SoundManager::GetInstance().StopAll();
	SoundManager::GetInstance().Play(SOUND_BGM_MAIN404, SoundManager::GetInstance().GetBGMVolume());*/

	// Physics setting
	//PhysicsSystem::GetInstance().SetCollisionRepulsion(3.0f);
	//PhysicsSystem::GetInstance().SetCollisionRepulsion(3.0f);
	PhysicsSystem::GetInstance().SetCollisionRepulsion(1.0f);
	PhysicsSystem::GetInstance().SetQTreeArea(Vector2D(-500.0f, -200.0f), Vector2D(2000.0f, 1000.0f));
	PhysicsSystem::GetInstance().SetUsingQTreeInCollisionDetection(false);
	PhysicsSystem::GetInstance().SetQuadTreeDepth(2);
	PhysicsSystem::GetInstance().SetUsingVelocityOverlapped(false);

	// LoadScene을 해줘야 scene.Start가 호출됨
	sceneManager->LoadScene(mainsceneanimationid);

	CoreSystem::GetInstance().Run(nCmdShow);
	CoreSystem::GetInstance().Finalize();

	// 사운드 매니저 객체 삭제(소멸자가 아닌 finalize 함수를 통해 객체에 있는 정보 삭제)
	SoundManager::GetInstance().Finalize();
	return 0;
}