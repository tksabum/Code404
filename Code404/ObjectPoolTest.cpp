#include "ObjectPoolTest.h"

#include "ObjectPoolSystem.h"

void ObjectPoolTest::Update()
{
	if (Input::GetInstance().PressedKeyNow('C'))
	{
		GameObject* gameobject = ObjectPoolSystem::GetInstance().PopFromPool(L"TestPrefab");
		gameobject->GetTransform()->SetLocalPosition(Vector2D((rand() % 600 - 300), (rand() % 600 - 300)));
		gameobject->SetActive(true);
		objectQueue.push(gameobject);
	}

	if (Input::GetInstance().PressedKeyNow('V'))
	{
		if (!objectQueue.empty())
		{
			ObjectPoolSystem::GetInstance().GiveBackPool(objectQueue.front());
			objectQueue.pop();
		}
	}
}
