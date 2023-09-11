#pragma once

#ifdef GAMEOBJECT_EXPORTS
#define GAMEOBJECT_API __declspec(dllexport)
#else
#define GAMEOBJECT_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

// #include "Component.h"

namespace BJEngine
{
	class Component;
	class Scene;
	class Transform;
	class TimeController;
	class GraphicsSystem;
	class Collider;

	class GameObject
	{
	public:
		//GAMEOBJECT_API void Awake();
		//GAMEOBJECT_API void Start();

		//GAMEOBJECT_API void OnEnable();
		//GAMEOBJECT_API void OnDisable();

		GAMEOBJECT_API void OnTriggerEnter();
		GAMEOBJECT_API void OnTriggerStay();
		GAMEOBJECT_API void OnTriggerExit();

		GAMEOBJECT_API void OnTriggerEnter(Collider* other);
		GAMEOBJECT_API void OnTriggerStay(Collider* other);
		GAMEOBJECT_API void OnTriggerExit(Collider* other);

		//GAMEOBJECT_API void OnCollisionEnter();
		//GAMEOBJECT_API void OnCollisionExit();

		// ������Ʈ ������ ���콺�� ������ ȣ��
		GAMEOBJECT_API virtual void OnMouseLeftDownEvent();
		GAMEOBJECT_API virtual void OnMouseRightDownEvent();
		// ������Ʈ ������ ���콺�� ���� ȣ��
		GAMEOBJECT_API virtual void OnMouseLeftUpEvent();
		GAMEOBJECT_API virtual void OnMouseRightUpEvent();


		// ���콺�� ������Ʈ ���� �ö��� �� ȣ��
		GAMEOBJECT_API virtual void OnMouseEnterEvent();
		// ���콺�� ������Ʈ���� ����� �� ȣ��
		GAMEOBJECT_API virtual void OnMouseExitEvent();
		// ���콺�� ������Ʈ ���� ������ ��� ȣ��
		GAMEOBJECT_API virtual void OnMouseOverEvent();

		//GAMEOBJECT_API void FixedUpdate();
		//GAMEOBJECT_API void Update();
		//GAMEOBJECT_API void LateUpdate();

		GAMEOBJECT_API std::wstring GetName();

		GAMEOBJECT_API void SetActive(bool act);
		GAMEOBJECT_API bool GetActive();
		// parent�� ���󰡸� active�� Ȯ���Ͽ� ��� active�� true�� ��쿡�� true�� ��ȯ�Ѵ�.
		GAMEOBJECT_API bool GetRootActive();

		// ���ӿ�����Ʈ�� �����ϵ��� ����Ѵ�. (������ ������Ʈ�� ���ŵǴ°��� DestroyEvent�� �߻��� �� �����ȴ�.)
		GAMEOBJECT_API void Destroy();

		GAMEOBJECT_API GameObject* GetParent();

		// �̸��� childname�� ������Ʈ�� ã���ϴ�. child�� child���� Ȯ���մϴ�.
		GAMEOBJECT_API GameObject* GetChild(std::wstring childname);
		// childnumber��°�� �ڽĿ�����Ʈ�� ����ϴ�.
		GAMEOBJECT_API GameObject* GetChild(int childnumber);
		// ��� �ڽĿ�����Ʈ�� ����ϴ�.
		GAMEOBJECT_API std::vector<GameObject*> GetChildren();

		GAMEOBJECT_API TimeController* GetTimeController();

		GAMEOBJECT_API Scene* GetScene();

		GAMEOBJECT_API void SetLayerID(int layerid);
		GAMEOBJECT_API int GetLayerID();

		GAMEOBJECT_API void SetLayerName(std::wstring layername);
		GAMEOBJECT_API std::wstring GetLayerName();

		template <typename componentT>
		componentT* AddComponent()
		{
			Component* component = CreateComponent<componentT>();
			components.push_back(component);
			return (componentT*)component;
		}

		// ���� ���ӿ�����Ʈ�� ������Ʈ ��Ͽ��� ����
		void RemoveComponent(Component* component)
		{
			components.erase(remove(components.begin(), components.end(), component), components.end());
		}

		template <typename componentT>
		componentT* GetComponent()
		{
			for (int i = 0; i < components.size(); i++)
			{
				Component* component = dynamic_cast<componentT*>(components[i]);
				if (component != nullptr)
				{
					return (componentT*)component;
				}
			}

			return nullptr;
		}

		// ������Ʈ ����
		void DeleteComponent(Component* component);

	private:
		enum class State
		{
			CREATED,
			ENABLE,
			DISABLE,
			DESTROY
		};
		State state;
		bool active;
		bool destroy;

		int layerID;

		Scene* scene;
		Transform* transform;
		std::wstring name;

		GameObject* parent;
		std::vector<GameObject*> children;
		std::vector<Component*> components;

		GameObject(Scene* basescene, std::wstring gameobjectName = L"");
		virtual ~GameObject();

		/// <summary>
		/// ���� GameObject�� ParentGameObject��� GameObject�� �����Ͽ� children���� �߰��մϴ�.
		/// </summary>
		/// <returns> ������ GameObject�� ������ </returns>
		GameObject* AddGameObject(Scene* basescene, GameObject* parentobject, std::wstring gameobjectname);
		
		void DeleteChild(GameObject* childobject);

		void DisableEvent(bool parentdisable = false);
		void AwakeEvent(bool parentdisable = false);
		void EnableEvent(bool parentdisable = false);
		void StartEvent(bool parentdisable = false);
		void stateChangeEvent(bool parentdisable = false);

		void DestroyEvent();

		void EarlyUpdateEvent();
		void UpdateEvent();
		void LateUpdateEvent();

		/// <summary>
		/// <para>������Ʈ ��ü ����</para>
		/// <para>�� �Լ��� new�� ���� ������ �ϹǷ� ����</para>
		/// </summary>
		/// <typeparam name="componentT"> ������ ������Ʈ Ÿ�� </typeparam>
		/// <returns> ������ ������Ʈ ������ </returns>
		template<typename componentT>
		componentT* CreateComponent()
		{
			static_assert(std::is_base_of<Component, componentT>::value, "The class you are trying to create is not Component");

		#ifdef _DEBUG
			Component::SetAddComponentFlag(true);
		#endif // _DEBUG

			componentT* component = new componentT();

		#ifdef _DEBUG
			Component::SetAddComponentFlag(false);
		#endif // _DEBUG
			
			component->gameObject = this;
			return component;
		}

		friend Scene;
		friend GraphicsSystem;
		friend Transform;

	public:
		GAMEOBJECT_API Transform* GetTransform();
	};
}