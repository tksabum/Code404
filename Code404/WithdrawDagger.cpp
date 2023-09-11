#include "WithdrawDagger.h"

#include "UniqueDagger.h"

WithdrawDagger::WithdrawDagger(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1, bool* uniquecost2) :
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
	),
	targetDagger(nullptr)
{

}

void WithdrawDagger::SetTargetDagger(GameObject* dagger)
{
	targetDagger = dagger;
}

void WithdrawDagger::Use()
{
	targetDagger->GetComponent<UniqueDagger>()->Withdraw();
}
