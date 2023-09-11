#include "NextScene.h"

/// �� ��ȯ�� ���� ��� ����
#include "Collider.h"
#include "BlackMask.h"

/// �浹ü�� �÷��̾����� Ȯ���ϱ� ���� ��� ����
#include "GameObject.h"


namespace BJEngine
{
	using namespace Math;

	NextScene::NextScene()
	{

	}

	NextScene::~NextScene()
	{

	}

	void NextScene::OnTriggerEnter(Collider* other)
	{
		if (other->GetGameObject()->GetLayerName() == L"Player" && blackmask != nullptr)
		{
			blackmask->GetComponent<BlackMask>()->SetIsLoading(true);
		}
	}

	void NextScene::OnTriggerExit(Collider* other)
	{

	}

	void NextScene::SetBlackMask(GameObject* gameobject)
	{
		this->blackmask = gameobject;
	}
}