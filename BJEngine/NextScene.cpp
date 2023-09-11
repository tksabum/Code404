#include "NextScene.h"

/// 씬 전환을 위한 헤더 참조
#include "Collider.h"
#include "BlackMask.h"

/// 충돌체가 플레이어인지 확인하기 위한 헤더 참조
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