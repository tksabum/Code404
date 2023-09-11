#include "GameObject.h"

#include <string>
#include <cmath>
#include <typeinfo>

#include "BJMath.h"
#include "Transform.h"
#include "Camera.h"
#include "Scene.h"
#include "EngineSetting.h"

using namespace std;

namespace BJEngine
{
	void GameObject::OnTriggerEnter()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnTriggerEnter();
		}
	}

	void GameObject::OnTriggerStay()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnTriggerStay();
		}
	}

	void GameObject::OnTriggerExit()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnTriggerExit();
		}
	}

	void GameObject::OnTriggerEnter(Collider* other)
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnTriggerEnter(other);
		}
	}

	void GameObject::OnTriggerStay(Collider* other)
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnTriggerStay(other);
		}
	}

	void GameObject::OnTriggerExit(Collider* other)
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnTriggerExit(other);
		}
	}

	void GameObject::OnMouseLeftDownEvent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnMouseLeftDown();
		}
	}

	void GameObject::OnMouseRightDownEvent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnMouseRightDown();
		}
	}

	void GameObject::OnMouseLeftUpEvent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnMouseLeftUp();
		}
	}

	void GameObject::OnMouseRightUpEvent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnMouseRightUp();
		}
	}

	void GameObject::OnMouseEnterEvent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnMouseEnter();
		}
	}

	void GameObject::OnMouseExitEvent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnMouseExit();
		}
	}

	void GameObject::OnMouseOverEvent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnMouseOver();
		}
	}

	wstring GameObject::GetName()
	{
		return name;
	}

	void GameObject::SetActive(bool act)
	{
		if (destroy == true && act == true)
		{
			return;
		}

		active = act;
	}

	bool GameObject::GetActive()
	{
		return active;
	}

	bool GameObject::GetRootActive()
	{
		bool rootResult = true;

		if (parent != nullptr)
		{
			rootResult = parent->GetRootActive();
		}
		
		bool ownResult = GetActive();

		return rootResult && ownResult;
	}

	void GameObject::Destroy()
	{
		SetActive(false);
		destroy = true;
		if (state != State::ENABLE)
		{
			state = State::DESTROY;
		}

		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Destroy();
		}
	}

	GameObject* GameObject::GetParent()
	{
		return parent;
	}

	GameObject* GameObject::GetChild(std::wstring childname)
	{
		for (int i = 0; i < children.size(); i++)
		{
			if (childname == children[i]->name)
			{
				return children[i];
			}
		}

		for (int i = 0; i < children.size(); i++)
		{
			GameObject* childresult = children[i]->GetChild(childname);
			if (childresult != nullptr)
			{
				return childresult;
			}
		}

		return nullptr;
	}

	GameObject* GameObject::GetChild(int childnumber)
	{
		if (childnumber < 0 || children.size() <= childnumber)
		{
			return nullptr;
		}

		return children[childnumber];
	}

	std::vector<GameObject*> GameObject::GetChildren()
	{
		return children;
	}

	TimeController* GameObject::GetTimeController()
	{
		return scene->GetTimeController();
	}

	Scene* GameObject::GetScene()
	{
		return scene;
	}

	void GameObject::SetLayerID(int layerid)
	{
		layerID = layerid;
	}

	int GameObject::GetLayerID()
	{
		return layerID;
	}

	void GameObject::SetLayerName(std::wstring layername)
	{
		layerID = EngineSetting::GetInstance().GetLayerID(layername);
	}

	std::wstring GameObject::GetLayerName()
	{
		return EngineSetting::GetInstance().GetLayerName(layerID);
	}

	void GameObject::DeleteComponent(Component* component)
	{
		delete component;
	}

	GameObject::GameObject(Scene* basescene, std::wstring gameobjectName)
	{
		state = State::CREATED;
		active = true;
		destroy = false;

		layerID = EngineSetting::GetInstance().GetLayerID(L"Default");

		scene = basescene;

		name = gameobjectName;

		parent = nullptr;

		transform =	CreateComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		DeleteComponent(transform);

		for (int i = 0; i < components.size(); i++)
		{
			DeleteComponent(components[i]);
		}

		for (int i = 0; i < children.size(); i++)
		{
			delete children[i];
		}

		components.clear();
	}

	GameObject* GameObject::AddGameObject(Scene* basescene, GameObject* parentobject, std::wstring gameobjectname)
	{
		// 현재 객체가 parentobject인 경우
		if (this == parentobject)
		{
			GameObject* object = new GameObject(basescene, gameobjectname);
			object->parent = this;

			children.push_back(object);

			return object;
		}

		GameObject* result = nullptr;

		// 자식오브젝트 들에서 addgameobject 실행
		for (int i = 0; i < children.size(); i++)
		{
			result = children[i]->AddGameObject(basescene, parentobject, gameobjectname);
			if (result != nullptr)
			{
				// parentobject에 object를 추가하는데 성공
				break;
			}
		}

		return result;
	}

	void GameObject::DeleteChild(GameObject* childobject)
	{
		int childNumber = find(children.begin(), children.end(), childobject) - children.begin();
		if (childNumber < children.size())
		{
			delete children[childNumber];
			children.erase(children.begin() + childNumber);
		}
	}

	//void GameObject::InitializeEvent()
	//{
	//	InitializeEvent(false);
	//}

	//void GameObject::InitializeEvent(bool parentdisable)
	//{
	//	if (state == State::CREATED)
	//	{
	//		if (parentdisable == false && active == true)
	//		{
	//			state = State::ENABLE;

	//			for (int i = 0; i < components.size(); i++)
	//			{
	//				components[i]->Awake();
	//				components[i]->OnEnable();
	//				components[i]->Start();
	//			}

	//			for (int i = 0; i < children.size(); i++)
	//			{
	//				children[i]->InitializeEvent(false);
	//			}
	//		}
	//	}
	//	else if (state == State::ENABLE)
	//	{
	//		if (destroy == true)
	//		{
	//			state = State::DESTROY;

	//			for (int i = 0; i < components.size(); i++)
	//			{
	//				components[i]->OnDisable();
	//			}

	//			for (int i = 0; i < children.size(); i++)
	//			{
	//				children[i]->InitializeEvent(true);
	//			}
	//		}
	//		else if (parentdisable == false && active == true)
	//		{
	//			for (int i = 0; i < children.size(); i++)
	//			{
	//				children[i]->InitializeEvent(false);
	//			}
	//		}
	//		else
	//		{
	//			state = State::DISABLE;

	//			for (int i = 0; i < components.size(); i++)
	//			{
	//				components[i]->OnDisable();
	//			}

	//			for (int i = 0; i < children.size(); i++)
	//			{
	//				children[i]->InitializeEvent(true);
	//			}
	//		}
	//	}
	//	else if (state == State::DISABLE)
	//	{
	//		if (active == true)
	//		{
	//			state = State::ENABLE;

	//			for (int i = 0; i < components.size(); i++)
	//			{
	//				components[i]->OnEnable();
	//			}

	//			for (int i = 0; i < children.size(); i++)
	//			{
	//				children[i]->InitializeEvent(false);
	//			}
	//		}
	//	}
	//}

	void GameObject::DisableEvent(bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < children.size(); i++)
				{
					children[i]->DisableEvent(false);
				}
			}
		}
		else if (state == State::ENABLE)
		{
			if (destroy == true)
			{
				for (int i = 0; i < components.size(); i++)
				{
					components[i]->OnDisable();
				}

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->DisableEvent(true);
				}
			}
			else if (parentdisable == false && active == true)
			{
				for (int i = 0; i < children.size(); i++)
				{
					children[i]->DisableEvent(false);
				}
			}
			else
			{
				for (int i = 0; i < components.size(); i++)
				{
					components[i]->OnDisable();
				}

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->DisableEvent(true);
				}
			}
		}
		else if (state == State::DISABLE)
		{
			if (active == true)
			{
				for (int i = 0; i < children.size(); i++)
				{
					children[i]->DisableEvent(false);
				}
			}
		}
	}

	void GameObject::AwakeEvent(bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < components.size(); i++)
				{
					components[i]->Awake();
				}

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->AwakeEvent(false);
				}
			}
		}
	}

	void GameObject::EnableEvent(bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < components.size(); i++)
				{
					components[i]->OnEnable();
				}

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->EnableEvent(false);
				}
			}
		}
		else if (state == State::ENABLE)
		{
			if (destroy == true)
			{
				for (int i = 0; i < children.size(); i++)
				{
					children[i]->EnableEvent(true);
				}
			}
			else if (parentdisable == false && active == true)
			{
				for (int i = 0; i < children.size(); i++)
				{
					children[i]->EnableEvent(false);
				}
			}
			else
			{
				for (int i = 0; i < children.size(); i++)
				{
					children[i]->EnableEvent(true);
				}
			}
		}
		else if (state == State::DISABLE)
		{
			if (active == true)
			{
				for (int i = 0; i < components.size(); i++)
				{
					components[i]->OnEnable();
				}

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->EnableEvent(false);
				}
			}
		}
	}

	void GameObject::StartEvent(bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < components.size(); i++)
				{
					components[i]->Start();
				}

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->StartEvent(false);
				}
			}
		}
	}

	void GameObject::stateChangeEvent(bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				state = State::ENABLE;

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->stateChangeEvent(false);
				}
			}
		}
		else if (state == State::ENABLE)
		{
			if (destroy == true)
			{
				state = State::DESTROY;

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->stateChangeEvent(true);
				}
			}
			else if (parentdisable == false && active == true)
			{
				for (int i = 0; i < children.size(); i++)
				{
					children[i]->stateChangeEvent(false);
				}
			}
			else
			{
				state = State::DISABLE;

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->stateChangeEvent(true);
				}
			}
		}
		else if (state == State::DISABLE)
		{
			if (active == true)
			{
				state = State::ENABLE;

				for (int i = 0; i < children.size(); i++)
				{
					children[i]->stateChangeEvent(false);
				}
			}
		}
	}

	void GameObject::DestroyEvent()
	{
		if (state == State::DESTROY)
		{
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->OnDestroy();
			}
		}

		for (int i = 0; i < children.size(); i++)
		{
			children[i]->DestroyEvent();
		}

		if (state == State::DESTROY)
		{
			// 위쪽에서 삭제
			if (parent == nullptr)
			{
				scene->DeleteGameObject(this);
			}
			else
			{
				parent->DeleteChild(this);
			}
		}
	}

	void GameObject::EarlyUpdateEvent()
	{
		if (state == State::ENABLE)
		{
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->EarlyUpdate();
			}

			for (int i = 0; i < children.size(); i++)
			{
				children[i]->EarlyUpdateEvent();
			}
		}
	}

	void GameObject::UpdateEvent()
	{
		if (state == State::ENABLE)
		{
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->Update();
			}

			for (int i = 0; i < children.size(); i++)
			{
				children[i]->UpdateEvent();
			}
		}
	}

	void GameObject::LateUpdateEvent()
	{
		if (state == State::ENABLE)
		{
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->LateUpdate();
			}

			for (int i = 0; i < children.size(); i++)
			{
				children[i]->LateUpdateEvent();
			}
		}
	}

	Transform* GameObject::GetTransform()
	{
		return transform;
	}
}