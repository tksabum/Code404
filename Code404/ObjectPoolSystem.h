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
	// Key : ������ �̸�, Value : �������� ����
	map<wstring, stack<GameObject*>> pool;

	map<wstring, function<void(GameObject*)>> prefabComponentTable;
	map<wstring, function<void(GameObject*)>> prefabInitializeTable;

	// �������� �����ϰ� �⺻������ 10���� ����� ����.
	void CreatePrefab(wstring prefabname);

public:
	void Clear();

	// �������� �����Ѵ�.
	void SetPrefab(wstring prefabname, function<void(GameObject*)> componentfunction, function<void(GameObject*)> initializefunction);

	// ���ÿ��� ��Ȱ��ȭ�� ������Ʈ ������ ��ȯ
	GameObject* PopFromPool(wstring prefabname);
	// ������Ʈ�� ��Ȱ��ȭ ���·� �ٲٰ� ���ÿ� �ٽ� push
	void GiveBackPool(GameObject* prefab);

	// singleton
	static ObjectPoolSystem& GetInstance();

private:
	// �⺻������ ó�� ���� �� 10���� �����.
	size_t size = 10;

private:
	ObjectPoolSystem();
	~ObjectPoolSystem();
};

