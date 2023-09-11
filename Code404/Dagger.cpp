#include "Dagger.h"

void Dagger::Initialize()
{
	GetGameObject()->GetComponent<ConvexpolygonCollider>()->SetTrigger(true);
	rigidbody = GetGameObject()->GetComponent<Rigidbody2D>();
	isHold = false;

	animator = GetGameObject()->GetComponent<Animator>();
}

void Dagger::Update()
{

}

void Dagger::OnTriggerEnter(Collider* collider)
{
	if (isHold == false && collider->GetGameObject()->GetLayerName() != L"Player" && collider->GetGameObject()->GetLayerName() != L"Dagger" && collider->GetGameObject()->GetLayerName() != L"Trigger")
	{
		rigidbody->SetVelocity(Vector2D(0.0f, 0.0f));
		GetGameObject()->GetComponent<ConvexpolygonCollider>()->SetTrigger(false);

		isHold = true;

		animator->Play(L"Animation_Dagger_Stuck");
	}
}