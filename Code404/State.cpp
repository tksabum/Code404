#include "State.h"

#include "AI.h"

#include "GameObject.h"
#include "Animator.h"
#include "Enemy.h"

using namespace BJEngine;

State::State(AI* ai)
	: aiComponent(ai)
{
	animator = ai->GetGameObject()->GetComponent<Animator>();
	enemyComponent = ai->GetGameObject()->GetComponent<Enemy>();
}

State::~State()
{

}

AI* State::GetAI()
{
	return aiComponent;
}

BJEngine::Animator* State::GetAnimator()
{
	return animator;
}

void State::Update()
{
	if (enemyComponent->GetCurHP() <= 0.f)
	{
		aiComponent->ChangeState(AI_STATE::DIE);
	}

	//if (enemyComponent->IsAttacked())
	//{
	//	aiComponent->ChangeState(AI_STATE::HIT);
	//}
}

void State::AlwaysUpdate()
{

}

bool State::IsParryingTime()
{
	return false;
}

void State::Enter()
{

}

void State::Exit()
{

}
