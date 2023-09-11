#include "ThrowDagger.h"

#include "EffectPlayer.h"

#include "ObjectPoolSystem.h"

const float ThrowDagger::PROJECTILE_SPEED = 1500.0f;
const float ThrowDagger::KNIFE_SIZE = 0.7f;
const float ThrowDagger::KNIFE_EFFECT_SIZE = 1.0f;

ThrowDagger::ThrowDagger(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1, bool* uniquecost2) :
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
		0.3f,		// 쿨타임
		5.0f,		// 반복사용 초기화 시간
		false		// 음수 마나 비용 허용
	)
{

}

void ThrowDagger::Use()
{
	UseCost();

	repeatUse++;
	elapsedTime = 0.0f;

	GameObject* dagger = ObjectPoolSystem::GetInstance().PopFromPool(L"Dagger");
	dagger->SetActive(true);
	dagger->GetTransform()->SetLocalPosition(userTransform->GetWorldPosition());

	GameObject* daggereffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
	daggereffect->SetActive(true);

	EffectPlayer* animator = daggereffect->GetComponent<EffectPlayer>();
	animator->Reset(L"Animation_Dagger_Effect");

	if (userDirection->x < 0.0f)
	{
		dagger->GetTransform()->SetLocalScale(Vector3D(-KNIFE_SIZE, KNIFE_SIZE, 1.0f));
		daggereffect->GetTransform()->SetLocalScale(Vector3D(-KNIFE_EFFECT_SIZE, KNIFE_EFFECT_SIZE, 1.0f));

		daggereffect->GetTransform()->SetLocalPosition(userTransform->GetWorldPosition() + Vector2D(-100.0f, 0.0f));
	}
	else
	{
		dagger->GetTransform()->SetLocalScale(Vector3D(KNIFE_SIZE, KNIFE_SIZE, 1.0f));
		daggereffect->GetTransform()->SetLocalScale(Vector3D(KNIFE_EFFECT_SIZE, KNIFE_EFFECT_SIZE, 1.0f));

		daggereffect->GetTransform()->SetLocalPosition(userTransform->GetWorldPosition() + Vector2D(100.0f, 0.0f));
	}

	dagger->GetComponent<Rigidbody2D>()->SetVelocity((*userDirection) * PROJECTILE_SPEED);
}
