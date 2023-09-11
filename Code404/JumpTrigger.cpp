#include "JumpTrigger.h"

#include "Player.h"

void JumpTrigger::Awake()
{
	GetGameObject()->SetLayerName(L"Trigger");

	if (player == nullptr)
	{
		player = GetGameObject()->GetParent()->GetComponent<Player>();
	}
}

void JumpTrigger::OnTriggerEnter(Collider* collider)
{
	if (collider->GetTrigger() == false && (collider->GetGameObject()->GetLayerName() == L"Platform" || collider->GetGameObject()->GetLayerName() == L"Dagger"))
	{
		player->ReadyJump();
	}
}

void JumpTrigger::OnTriggerStay(Collider* collider)
{
	if (collider->GetTrigger() == false && (collider->GetGameObject()->GetLayerName() == L"Platform" || collider->GetGameObject()->GetLayerName() == L"Dagger"))
	{
		player->ReadyJump();
	}
}
