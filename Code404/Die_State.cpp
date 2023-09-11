#include "Die_State.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "GameObject.h"

#include "AI.h"

#include "ObjectPoolSystem.h"

#include "GameObject.h"
#include "Transform.h"

#include "EffectPlayer.h"

using namespace BJEngine;

Die_State::Die_State(AI* ai, float dietime)
	: State(ai)
	, dieTime(dietime)
{

}

Die_State::~Die_State()
{

}

bool Die_State::GetIsDie()
{
	return isDie;
}

void Die_State::Update()
{
	//totalDieTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	//if (dieTime <= totalDieTime)
	//{
	//	GameObject* owner = GetAI()->GetGameObject();
	//	isDie = true;
	//	owner->SetActive(false);
	//}
}

void Die_State::Enter()
{
	GetAI()->GetGameObject()->SetActive(false);
	totalDieTime = 0.f;
	GameObject* effect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
	effect->GetTransform()->SetLocalPosition(GetAI()->GetGameObject()->GetTransform()->GetLocalPosition());
	effect->SetActive(true);
	effect->GetComponent<EffectPlayer>()->Reset(L"Enemy_Die_Effect");
}

void Die_State::Exit()
{

}
