#include "PlayerStealthCounter.h"

#include "Player.h"

void PlayerStealthCounter::Awake()
{
	image = GetGameObject()->GetComponent<Image>();
}

void PlayerStealthCounter::SetPlayer(Player* target)
{
	player = target;
}

void PlayerStealthCounter::SetStealthCounterImage(wstring imagename1, wstring imagename2, wstring imagename3, wstring imagename4, wstring imagename5)
{
	imageName[0] = imagename1;
	imageName[1] = imagename2;
	imageName[2] = imagename3;
	imageName[3] = imagename4;
	imageName[4] = imagename5;
}

void PlayerStealthCounter::Update()
{
	image->SetSpriteName(imageName[player->currentStealthCount]);
	image->SetImageNumber(0);
}