#include "ObjectPoolSystem.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"

#include "GameObject.h"

using namespace BJEngine;

ObjectPoolSystem::ObjectPoolSystem()
{

}

ObjectPoolSystem::~ObjectPoolSystem()
{

}

void ObjectPoolSystem::CreatePrefab(wstring prefabname)
{
	// 프리펩을 저장하고 기본 size인 10개을 생성하고 삽입한다.
	stack<GameObject*> tempStack;
	for (size_t i = 0; i < size; ++i)
	{
		GameObject* newObj = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->CreateGameObject(prefabname);
		prefabComponentTable[prefabname](newObj);
		prefabInitializeTable[prefabname](newObj);
		newObj->SetActive(false);
		tempStack.push(newObj);
	}
	pool.insert({ prefabname, tempStack });
}

void ObjectPoolSystem::Clear()
{
	for (auto iter = pool.begin(); iter != pool.end(); iter++)
	{
		while (iter->second.empty() == false)
		{
			iter->second.pop();
		}
	}
}

void ObjectPoolSystem::SetPrefab(wstring prefabname, function<void(GameObject*)> componentfunction, function<void(GameObject*)> initializefunction)
{
	if (prefabComponentTable.count(prefabname) || prefabInitializeTable.count(prefabname))
	{
		return;
	}

	prefabComponentTable[prefabname] = componentfunction;
	prefabInitializeTable[prefabname] = initializefunction;
	CreatePrefab(prefabname);
}

GameObject* ObjectPoolSystem::PopFromPool(wstring prefabname)
{
	if (pool[prefabname].empty())
	{
		GameObject* newObj = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->CreateGameObject(prefabname);
		prefabComponentTable[prefabname](newObj);
		prefabInitializeTable[prefabname](newObj);
		newObj->SetActive(false);
		pool[prefabname].push(newObj);
	}
	GameObject* temp = pool[prefabname].top();
	pool[prefabname].pop();
	return temp;
}

void ObjectPoolSystem::GiveBackPool(GameObject* prefab)
{
	prefab->SetActive(false);
	prefabInitializeTable[prefab->GetName()](prefab);
	pool[prefab->GetName()].push(prefab);
}

ObjectPoolSystem& ObjectPoolSystem::GetInstance()
{
	static ObjectPoolSystem inst;
	return inst;
}