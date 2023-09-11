#pragma once

#include "fmod.hpp" 

#define SOUND_BGM_STAGE1 1
#define SOUND_BGM_STAGE2 2
#define SOUND_BGM_BOSSSTAGE 3
#define SOUND_BGM_GAMEOVER 4
#define SOUND_BGM_MAIN404 5
#define SOUND_BGM_CLEAR 6

#define SOUND_EFFECT_WALK 10
#define SOUND_EFFECT_HIT 11
#define SOUND_EFFECT_DASH 12
#define SOUND_EFFECT_JUMP 13
#define SOUND_EFFECT_THROWING 14
#define SOUND_EFFECT_OBJECTHIT 15
#define SOUND_EFFECT_LANDING 16
#define SOUND_EFFECT_PARRYING 17
#define SOUND_EFFECT_DAMAGED 18
#define SOUND_EFFECT_GUNLASER 19
#define SOUND_EFFECT_ENEMYDIE 20
#define SOUND_EFFECT_BOSSHANDMASH 21
#define SOUND_EFFECT_BOSSHANDATTACK 22
#define SOUND_EFFECT_BOSSCROSSMETALHARD 23
#define SOUND_EFFECT_BOSSCAMERASHAKE 24
#define SOUND_EFFECT_BOSSEXPLOSION 25
#define SOUND_EFFECT_BOSSLAZER 26
#define SOUND_EFFECT_BOSSHEADATTACK 27
#define SOUND_EFFECT_BOSSHIT 28
#define SOUND_EFFECT_HITAIR 29
#define SOUND_EFFECT_CHARACTER_DAMAGE 30
#define SOUND_EFFECT_CLICK 31

/// 더미^
#define NAME_BGM_MAIN404 "sound/BGM-Main404.mp3"
#define NAME_BGM_STAGE1 "sound/BGM-Stage1.mp3"
#define NAME_BGM_STAGE2 "sound/BGM-Stage2.mp3"
#define NAME_BGM_BOSSSTAGE "sound/BGM-StageBoss.mp3"
#define NAME_BGM_GAMEOVER "sound/BGM-Gameover.ogg"
#define NAME_BGM_CLEAR "sound/BGM-GameClear.ogg"


//#define SOUND_EFFECT_CLICK 10
//#define SOUND_EFFECT_GAMEOVER 11
//#define SOUND_EFFECT_JUMP 12
//#define SOUND_EFFECT_DASH 13
//#define SOUND_EFFECT_BREAK 14
//#define NAME_EFFECT_BUTTON "sound/608432__plasterbrain-ui-select-enter.flac"
//#define NAME_EFFECT_GAMEOVER "sound/Kirbys_Dream_Land_2A03_8-Bit_OST_-_Game_Over.mp3"
//#define NAME_EFFECT_JUMP "sound/518130__se2001__8-bit-jump-2.wav"
//#define NAME_EFFECT_DASH "sound/521999__kastenfrosch__whoosh-dash.wav"
//#define NAME_EFFECT_BREAK "sound/391961__ssierra1202__wood-destruction.wav"
/// 더미 ^
#define NAME_EFFECT_WALK "sound/Character-Walk.mp3"
#define NAME_EFFECT_HIT "sound/Character-Hit.mp3"
#define NAME_EFFECT_DASH "sound/Character-Dash.ogg"
#define NAME_EFFECT_JUMP "sound/Character-Jump.mp3"
#define NAME_EFFECT_THROWING "sound/Character-Throwing.mp3"
#define NAME_EFFECT_OBJECTHIT "sound/Object-Wallhit.ogg"
#define NAME_EFFECT_LANDING "sound/Character-Landing.mp3"
#define NAME_EFFECT_PARRING "sound/Character-Parring.mp3"
#define NAME_EFFECT_DAMAGED "sound/Enemy-Damage.ogg"
#define NAME_EFFECT_GUNLASER "sound/Enemy-GunLaser.ogg"
#define NAME_EFFECT_ENEMYDIE "sound/Enemy-Die.ogg"
#define NAME_EFFECT_BOSSHANDMASH "sound/Boss-Handsmash.mp3"
#define NAME_EFFECT_BOSSHANDATTACK "sound/Boss-handattack.mp3"
#define NAME_EFFECT_BOSSCROSSMETALHARD "sound/Boss-cross-metal-hard.ogg"
#define NAME_EFFECT_BOSSCAMERASHAKE "sound/Boss-camerashake.ogg"
#define NAME_EFFECT_BOSSEXPLOSION "sound/Boss-Explosion.ogg"
#define NAME_EFFECT_BOSSLAZER "sound/Boss-Lazer-fix.mp3"
#define NAME_EFFECT_BOSSHEADATTACK "sound/Boss-Headattack.ogg"
#define NAME_EFFECT_BOSSHIT "sound/Bosshit.mp3"
#define NAME_EFFECT_HITAIR "sound/Character-Hitair.ogg"
#define NAME_EFFECT_CHARACTER_DAMAGE "sound/Character-Damage.ogg"
#define NAME_EFFECT_CLICK "sound/UI-Click.ogg"


#ifdef SOUNDMANAGER_EXPORTS
#define SOUNDMANAGER_API __declspec(dllexport)
#else
#define SOUNDMANAGER_API __declspec(dllimport)
#endif

class SoundManager
{
private:
    SoundManager();
    SoundManager(const SoundManager& ref) {}
    SoundManager& operator = (const SoundManager& ref) {}
    ~SoundManager();

    FMOD::System* system;
    FMOD::Sound* sound[32];
    FMOD::Channel* bgmChannel;
    FMOD::Channel* effectChannel[31];
    void* extradriverdata;
    float volume;
    float bgmVolume;

    int lastBGMNumber;

public:
    SOUNDMANAGER_API static SoundManager& GetInstance();
    
    SOUNDMANAGER_API void StopAll();
    SOUNDMANAGER_API void StopAllBGM();
    SOUNDMANAGER_API void Play(int soundNum, bool samebgmkeep = true);
    SOUNDMANAGER_API void Finalize();
    SOUNDMANAGER_API float GetVolume();
    SOUNDMANAGER_API float GetBGMVolume();
    SOUNDMANAGER_API void SetVolume(float volume2);
    SOUNDMANAGER_API void SetBGM(float volume2);
    SOUNDMANAGER_API void UpdateVolume(float volume);
    SOUNDMANAGER_API void UpdateAllVolume();

};

