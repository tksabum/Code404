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
		0.0f,		// �⺻ �����Ҹ�
		0.0f,		// ���� ���
		0.0f,		// ���� �¼�
		0.0f,		// ���ӻ�� ���
		0.0f,		// ���ӻ�� �¼�
		1,			// ����
		1,			// �ִ� ����
		0.3f,		// ��Ÿ��
		5.0f,		// �ݺ���� �ʱ�ȭ �ð�
		false		// ���� ���� ��� ���
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
