#include "SceneChanger.h"

/// 씬 전환을 위한 헤더 참조
#include "Scene.h"
#include "CoreSystem.h"
#include "SceneManager.h"

/// 충돌체가 플레이어인지 확인하기 위한 헤더 참조
#include "GameObject.h"


namespace BJEngine
{
	using namespace Math;

	SceneChanger::SceneChanger()
	{

	}

	SceneChanger::~SceneChanger()
	{

	}

	void SceneChanger::OnTriggerEnter(Collider* other)
	{
		if (other->GetGameObject()->GetLayerName() == L"Player")
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(nextScene);
		}
	}

	void SceneChanger::SetScene(std::wstring scene)
	{
		this->nextScene = scene;
	}

}