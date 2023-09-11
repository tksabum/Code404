#include "Dash.h"

#include "Player.h"

Dash::Dash(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1, bool* uniquecost2) :
	Skill(
		userdirection,
		usertransform,
		uniquecost1,
		uniquecost2,
		usermp,
		50.0f,		// �⺻ �����Ҹ�
		0.0f,		// ���� ���
		0.0f,		// ���� �¼�
		0.0f,		// ���ӻ�� ���
		0.0f,		// ���ӻ�� �¼�
		1,			// ����
		1,			// �ִ� ����
		0.0f,		// ��Ÿ��
		5.0f,		// �ݺ���� �ʱ�ȭ �ð�.
		false		// ���� ���� ��� ���
	),
	user(nullptr)
{

}

void Dash::SetPlayer(Player* player)
{
	user = player;
}

void Dash::Use()
{
	UseCost();

	repeatUse++;
	elapsedTime = 0.0f;

	user->DashMove();
}
