#pragma once

#include "Component.h"

#include "Image.h"
#include "GameObject.h"

using namespace std;
using namespace BJEngine;

class Player;

class PlayerStealthCounter : public Component
{
public:
	void Awake();

	void SetPlayer(Player* target);
	void SetStealthCounterImage(wstring imagename1, wstring imagename2, wstring imagename3, wstring imagename4, wstring imagename5);

	void Update();

private:
	Image* image;
	Player* player;
	wstring imageName[5];
};

