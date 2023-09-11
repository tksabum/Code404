#include "Scene.h"

#include "GameObject.h"
#include "Camera.h"
#include "PhysicsSystem.h"

namespace BJEngine
{
	GameObject* Scene::CreateGameObject(std::wstring gameobjectname)
	{
		GameObject* object = new GameObject(this, gameobjectname);
		gameobjectList.push_back(object);
		return object;
	}

	GameObject* Scene::CreateGameObject(GameObject* parentobject, std::wstring gameobjectname)
	{
		GameObject* result = nullptr;

		for (int i = 0; i < gameobjectList.size(); i++)
		{
			result = gameobjectList[i]->AddGameObject(this, parentobject, gameobjectname);
			if (result != nullptr)
			{
				// parentobject에 object를 추가하는데 성공
				break;
			}
		}

		return result;
	}

	void Scene::CreateGameObject(int)
	{
		throw; // not implement
	}

	void Scene::DeleteGameObject(GameObject* gameobject)
	{
		int gameobjectNumber = find(gameobjectList.begin(), gameobjectList.end(), gameobject) - gameobjectList.begin();
		if (gameobjectNumber < gameobjectList.size())
		{
			delete gameobjectList[gameobjectNumber];
			gameobjectList.erase(gameobjectList.begin() + gameobjectNumber);
		}
	}

	GameObject* Scene::GetGameObject(std::wstring gameobjectname)
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			if (gameobjectname == gameobjectList[i]->name)
			{
				return gameobjectList[i];
			}
		}

		for (int i = 0; i < gameobjectList.size(); i++)
		{
			GameObject* childresult = gameobjectList[i]->GetChild(gameobjectname);
			if (childresult != nullptr)
			{
				return childresult;
			}
		}

		return nullptr;
	}

	TimeController* Scene::GetTimeController()
	{
		return timeController;
	}

	Camera* Scene::GetMainCamera()
	{
		return mainCamera;
	}

	void Scene::SetMainCamera(Camera* camera)
	{
		mainCamera = camera;
	}

	void Scene::InitializeEvent()
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->DisableEvent();
		}

		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->AwakeEvent();
		}

		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->EnableEvent();
		}

		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->StartEvent();
		}

		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->stateChangeEvent();
		}
	}

	void Scene::DestroyEvent()
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->DestroyEvent();
		}
	}

	void Scene::PhysicsEvent()
	{

	}

	void Scene::EarlyUpdateEvent()
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->EarlyUpdateEvent();
		}
	}

	void Scene::UpdateEvent()
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->UpdateEvent();
		}
	}

	void Scene::LateUpdateEvent()
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->LateUpdateEvent();
		}
	}

	void Scene::RenderEvent()
	{

	}

	void Scene::DeleteAllObject()
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			delete gameobjectList[i];
		}

		gameobjectList.clear();

		mainCamera = nullptr;
		defaultCamera = nullptr;
	}

	Scene::Scene(std::wstring scenename) : name(scenename)
	{
		name = scenename;

		GameObject* cameraobject = CreateGameObject(L"Camera");
		Camera* camera = cameraobject->AddComponent<Camera>();
		mainCamera = camera;
		defaultCamera = camera;
	}

	Scene::~Scene()
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			delete gameobjectList[i];
		}
		gameobjectList.clear();
	}

	void Scene::Initialize(TimeController* timecontroller)
	{
		timeController = timecontroller;
	}

	std::wstring Scene::GetName()
	{
		return this->name;
	}
}