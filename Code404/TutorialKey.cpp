#include "TutorialKey.h"

void TutorialKey::Awake()
{
	sprite = GetGameObject()->GetComponent<SpriteRenderer>();
	sprite->SetImageNumber(0);
}

void TutorialKey::OnTriggerEnter(Collider* collider)
{

}

void TutorialKey::ChangeSprite(wstring str)
{
	sprite->SetSpriteName(str);
}

void TutorialKey::ShowSprite()
{
	sprite->SetOpacity(1.0f);
}

void TutorialKey::HideSprite()
{
	sprite->SetOpacity(0.0f);
}
