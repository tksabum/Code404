#include "SoundManager.h"
#include "../Code404/UISoundController.h"
using namespace BJEngine;
using namespace BJEngine::Math;
SoundManager::SoundManager()
{
	lastBGMNumber = -1;

	volume = 0.5f;
	bgmVolume = 0.5f;
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	// BGM
	system->createSound(NAME_BGM_MAIN404, FMOD_LOOP_NORMAL, 0, &sound[SOUND_BGM_MAIN404]);
	system->createSound(NAME_BGM_STAGE1, FMOD_LOOP_NORMAL, 0, &sound[SOUND_BGM_STAGE1]);
	system->createSound(NAME_BGM_STAGE2, FMOD_LOOP_NORMAL, 0, &sound[SOUND_BGM_STAGE2]);
	system->createSound(NAME_BGM_BOSSSTAGE, FMOD_LOOP_NORMAL, 0, &sound[SOUND_BGM_BOSSSTAGE]);
	system->createSound(NAME_BGM_GAMEOVER, FMOD_LOOP_NORMAL, 0, &sound[SOUND_BGM_GAMEOVER]);
	system->createSound(NAME_BGM_CLEAR, FMOD_LOOP_NORMAL, 0, &sound[SOUND_BGM_CLEAR ]);

	// EFFECT

	system->createSound(NAME_EFFECT_WALK, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_WALK]);
	system->createSound(NAME_EFFECT_HIT, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_HIT]);
	system->createSound(NAME_EFFECT_DASH, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_DASH]);
	system->createSound(NAME_EFFECT_JUMP, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_JUMP]);
	system->createSound(NAME_EFFECT_THROWING, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_THROWING]);
	system->createSound(NAME_EFFECT_OBJECTHIT, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_OBJECTHIT]);
	system->createSound(NAME_EFFECT_LANDING, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_LANDING]);
	system->createSound(NAME_EFFECT_PARRING, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_PARRYING]);
	system->createSound(NAME_EFFECT_DAMAGED, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_DAMAGED]);
	system->createSound(NAME_EFFECT_GUNLASER, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_GUNLASER]);
	system->createSound(NAME_EFFECT_ENEMYDIE, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_ENEMYDIE]);
	system->createSound(NAME_EFFECT_BOSSHANDMASH, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSHANDMASH]);
	system->createSound(NAME_EFFECT_BOSSHANDATTACK, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSHANDATTACK]);
	system->createSound(NAME_EFFECT_BOSSCROSSMETALHARD, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSCROSSMETALHARD]);
	system->createSound(NAME_EFFECT_BOSSCAMERASHAKE, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSCAMERASHAKE]);
	system->createSound(NAME_EFFECT_BOSSEXPLOSION, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSEXPLOSION]);
	system->createSound(NAME_EFFECT_BOSSLAZER, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSLAZER]);
	system->createSound(NAME_EFFECT_BOSSHEADATTACK, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSHEADATTACK]);
	system->createSound(NAME_EFFECT_BOSSHIT, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_BOSSHIT]);
	system->createSound(NAME_EFFECT_HITAIR, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_HITAIR]);
	system->createSound(NAME_EFFECT_CHARACTER_DAMAGE, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_CHARACTER_DAMAGE]);
	system->createSound(NAME_EFFECT_CLICK, FMOD_LOOP_OFF, 0, &sound[SOUND_EFFECT_CLICK]);
}

SoundManager::~SoundManager()
{
	StopAll();
	system->close();
	system->release();
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::StopAll()
{
	bgmChannel->stop();
	for (int i = 0; i < 31; i++) 
	{
		effectChannel[i]->stop();
	}
}

void SoundManager::StopAllBGM()
{
	bgmChannel->stop();
}

void SoundManager::Finalize()
{
	StopAll();
	//sound[32]->release();
	bgmChannel->stop();
	effectChannel[31]->stop();
	sound[32]->release();
	system->close();
	system->release();
}

float SoundManager::GetVolume()
{
	return this->volume;
}

float SoundManager::GetBGMVolume()
{
	return this->bgmVolume;
}

void SoundManager::SetVolume(float volume2)
{
	this->volume = volume2;
	UpdateAllVolume();
}

void SoundManager::SetBGM(float volume2)
{
	this->bgmVolume = volume2;
	UpdateAllVolume();
}


void SoundManager::UpdateVolume(float volume)
{
	this->volume = volume;
	UpdateAllVolume();
}

void SoundManager::UpdateAllVolume()
{
	bgmChannel->setVolume(bgmVolume);
	for (int i = 0; i < 31; i++)
	{
		effectChannel[i]->setVolume(volume);
	}
}

void SoundManager::Play(int soundNum, bool samebgmkeep)
{

	if (lastBGMNumber != soundNum && 0 <= soundNum && soundNum <= 9)
	{
		bool isPlaying;
		bgmChannel->isPlaying(&isPlaying);
		if (isPlaying)
		{
			bgmChannel->stop();
		}
		system->playSound(sound[soundNum], 0, false, &bgmChannel);
		bgmChannel->setVolume(bgmVolume);

		lastBGMNumber = soundNum;
	}
	
	

	if (SOUND_EFFECT_WALK <= soundNum && soundNum <= SOUND_EFFECT_CLICK)
	{
		for (int i = 0; i < 31; i++)
		{
			bool isPlaying;
			effectChannel[i]->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				
				system->playSound(sound[soundNum], 0, false, &effectChannel[i]);
				effectChannel[i]->setVolume(volume);
				break;
			}
			else
			{
				effectChannel[i]->setVolume(volume);
			}
		}
	}
}
