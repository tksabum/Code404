#include "SceneManager.h"

#include "Scene.h"

#include "GameObject.h"
#include "Camera.h"
#include "PhysicsSystem.h"

using namespace std;

namespace BJEngine
{
	const int SceneManager::DEFAULTSCENE_DEFAULT = 0;

	SceneManager::SceneManager() : timeController(nullptr), defaultScene(DEFAULTSCENE_DEFAULT), sceneList(vector<Scene*>()), currentScene(DEFAULTSCENE_DEFAULT)
	{

	}

	SceneManager::~SceneManager()
	{
		for (int i = 0; i < sceneList.size(); i++)
		{
			delete sceneList[i];
		}

		sceneList.clear();
	}

	//int SceneManager::CreateScene(std::wstring name)
	//{
	//	Scene* newscene = new Scene(name);
	//	if (timeController != nullptr)
	//	{
	//		newscene->Initialize(timeController);
	//	}
	//	sceneList.push_back(newscene);

	//	return sceneList.size() - 1;
	//}

	void SceneManager::DeleteScene(int sceneID)
	{
		delete sceneList[sceneID];
		sceneList.erase(sceneList.begin() + sceneID);
	}

	void SceneManager::DeleteScene(Scene* scene)
	{
		int sceneID = find(sceneList.begin(), sceneList.end(), scene) - sceneList.begin();
		if (sceneID < sceneList.size())
		{
			DeleteScene(sceneID);
		}
	}

	Scene* SceneManager::GetScene(int sceneID)
	{
		if (0 <= sceneID && sceneID < sceneList.size())
		{
			return sceneList[sceneID];
		}

		return nullptr;
	}

	Scene* SceneManager::GetCurrentScene()
	{
		return sceneList[currentScene];
	}

	void SceneManager::Initialize(TimeController* timecontroller)
	{
		timeController = timecontroller;

		currentScene = defaultScene;
		for (int i = 0; i < sceneList.size(); i++)
		{
			sceneList[i]->Initialize(timecontroller);
		}
	}

	void SceneManager::InitializeEvent()
	{
		sceneList[currentScene]->InitializeEvent();
	}

	void SceneManager::DestroyEvent()
	{
		sceneList[currentScene]->DestroyEvent();
	}

	void SceneManager::PhysicsEvent()
	{
		sceneList[currentScene]->PhysicsEvent();
	}

	void SceneManager::EarlyUpdateEvent()
	{
		sceneList[currentScene]->EarlyUpdateEvent();
	}

	void SceneManager::UpdateEvent()
	{
		sceneList[currentScene]->UpdateEvent();
	}

	void SceneManager::LateUpdateEvent()
	{
		sceneList[currentScene]->LateUpdateEvent();
	}

	void SceneManager::RenderEvent()
	{
		sceneList[currentScene]->RenderEvent();
	}

	int SceneManager::AddScene(Scene* scene)
	{
		if (sceneTable.count(scene->name) != 0)
		{
			return -1;
		}

		if (timeController != nullptr)
		{
			scene->Initialize(timeController);
		}
		sceneList.push_back(scene);

		int id = sceneList.size() - 1;

		sceneTable[scene->name] = id;
		
		return id;
	}
	
	void SceneManager::LoadScene(std::wstring scenename)
	{
		if (sceneTable.count(scenename) == 0)
		{
			return;
		}
		nextSceneQueue.push(sceneTable[scenename]);
	}
	
	void SceneManager::LoadScene(int sceneID)
	{
		nextSceneQueue.push(sceneID);
	}

	void SceneManager::ChangeScene(int sceneID)
	{
		// ���� ���� ������Ʈ�� ��� ����
		sceneList[currentScene]->DeleteAllObject();

		currentScene = sceneID;

		// �⺻���� �����Ǵ� ī�޶� ������Ʈ�� �����ϱ� ���� �ڵ�
		sceneList[currentScene]->DeleteAllObject();

		GameObject* cameraobject = sceneList[currentScene]->CreateGameObject(L"Camera");
		Camera* camera = cameraobject->AddComponent<Camera>();
		sceneList[currentScene]->SetMainCamera(camera);

		sceneList[sceneID]->Start();

		PhysicsSystem::GetInstance().Clear();
	}

	int SceneManager::GetCurrentSceneID()
	{
		return this->currentScene;
	}

	
}