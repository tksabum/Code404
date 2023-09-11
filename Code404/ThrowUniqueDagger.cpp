#include "ThrowUniqueDagger.h"

#include "EffectPlayer.h"
#include "UniqueDagger.h"

#include "ObjectPoolSystem.h"

const float ThrowUniqueDagger::KNIFE_EFFECT_SIZE = 1.0f;
const float ThrowUniqueDagger::KNIFE_EFFECT_DISTANCE = 0.0f;

ThrowUniqueDagger::ThrowUniqueDagger(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1, bool* uniquecost2) :
	Skill(
		userdirection,
		usertransform,
		uniquecost1,
		uniquecost2,
		usermp,
		0.0f,		// 기본 마나소모량
		0.0f,		// 레벨 계수
		0.0f,		// 레벨 승수
		0.0f,		// 연속사용 계수
		0.0f,		// 연속사용 승수
		1,			// 레벨
		1,			// 최대 레벨
		12.0f,		// 쿨타임
		5.0f,		// 반복사용 초기화 시간.
		false		// 음수 마나 비용 허용
	),
	uniqueDagger(nullptr)
{

}

void ThrowUniqueDagger::SetUniqueDagger(GameObject* dagger, wstring effectname)
{
	uniqueDagger = dagger;
	effectName = effectname;
}

void ThrowUniqueDagger::Use()
{
	UseCost();

	repeatUse++;
	elapsedTime = 0.0f;

	uniqueDagger->SetActive(true);

	GameObject* daggereffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
	daggereffect->SetActive(true);

	EffectPlayer* animator = daggereffect->GetComponent<EffectPlayer>();
	animator->Reset(effectName);

	float playerscale = userTransform->GetLocalScale().x;

	if (userDirection->x < 0.0f)
	{
		daggereffect->GetTransform()->SetLocalScale(Vector3D(-KNIFE_EFFECT_SIZE * playerscale, KNIFE_EFFECT_SIZE * playerscale, 1.0f));
		daggereffect->GetTransform()->SetLocalPosition(userTransform->GetWorldPosition() + Vector2D(-KNIFE_EFFECT_DISTANCE, 0.0f));
	}
	else
	{
		daggereffect->GetTransform()->SetLocalScale(Vector3D(KNIFE_EFFECT_SIZE * playerscale, KNIFE_EFFECT_SIZE * playerscale, 1.0f));
		daggereffect->GetTransform()->SetLocalPosition(userTransform->GetWorldPosition() + Vector2D(KNIFE_EFFECT_DISTANCE, 0.0f));
	}
}
