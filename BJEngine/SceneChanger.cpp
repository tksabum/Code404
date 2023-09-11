#include "SceneChanger.h"

/// �� ��ȯ�� ���� ��� ����
#include "Scene.h"
#include "CoreSystem.h"
#include "SceneManager.h"

/// �浹ü�� �÷��̾����� Ȯ���ϱ� ���� ��� ����
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