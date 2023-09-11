#pragma once

#include "Component.h"

#include "Collider.h"
#include "GameObject.h"

using namespace BJEngine;

class Player;

class JumpTrigger : public Component
{
public:
	void Awake();
	void OnTriggerEnter(Collider* collider);
	void OnTriggerStay(Collider* collider);
	
private:
	Player* player;


};

