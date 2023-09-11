#include "EffectPlayer.h"

#include "ObjectPoolSystem.h"

void EffectPlayer::Reset(wstring effectname)
{
	isPlaying = false;
	if (sprite == nullptr)
	{
		sprite = GetGameObject()->GetComponent<SpriteRenderer>();
		animator = GetGameObject()->GetComponent<Animator>();
	}

	isPlaying = true;

	animator->Play(effectname);
}

void EffectPlayer::Update()
{
	if (isPlaying == true && animator != nullptr && animator->IsWait())
	{
		isPlaying = false;
		GetGameObject()->SetActive(false);

		ObjectPoolSystem::GetInstance().GiveBackPool(GetGameObject());
	}
}
