#pragma once

#include <stack>
#include <map>
#include <string>
#include <functional>
using namespace std;

namespace BJEngine
{
	class GameObject;
}

using namespace BJEngine;

class ObjectPoolSystem
{
private:
	// Key : 프리펩 이름, Value : 프리펩의 묶음
	map<wstring, stack<GameObject*>> pool;

	map<wstring, function<void(GameObject*)>> prefabComponentTable;
	map<wstring, function<void(GameObject*)>> prefabInitializeTable;

	// 프리펩을 셋팅하고 기본적으로 10개을 만들어 낸다.
	void CreatePrefab(wstring prefabname);

public:
	void Clear();

	// 프리펩을 셋팅한다.
	void SetPrefab(wstring prefabname, function<void(GameObject*)> componentfunction, function<void(GameObject*)> initializefunction);

	// 스택에서 비활성화된 오브젝트 꺼내서 반환
	GameObject* PopFromPool(wstring prefabname);
	// 오브젝트를 비활성화 상태로 바꾸고 스택에 다시 push
	void GiveBackPool(GameObject* prefab);

	// singleton
	static ObjectPoolSystem& GetInstance();

private:
	// 기본적으로 처음 만들 때 10개를 만든다.
	size_t size = 10;

private:
	ObjectPoolSystem();
	~ObjectPoolSystem();
};

