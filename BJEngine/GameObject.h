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

		// 오브젝트 위에서 마우스를 누르면 호출
		GAMEOBJECT_API virtual void OnMouseLeftDownEvent();
		GAMEOBJECT_API virtual void OnMouseRightDownEvent();
		// 오브젝트 위에서 마우스를 떼면 호출
		GAMEOBJECT_API virtual void OnMouseLeftUpEvent();
		GAMEOBJECT_API virtual void OnMouseRightUpEvent();


		// 마우스가 오브젝트 위로 올라갔을 때 호출
		GAMEOBJECT_API virtual void OnMouseEnterEvent();
		// 마우스가 오브젝트에서 벗어났을 때 호출
		GAMEOBJECT_API virtual void OnMouseExitEvent();
		// 마우스가 오브젝트 위에 있으면 계속 호출
		GAMEOBJECT_API virtual void OnMouseOverEvent();

		//GAMEOBJECT_API void FixedUpdate();
		//GAMEOBJECT_API void Update();
		//GAMEOBJECT_API void LateUpdate();

		GAMEOBJECT_API std::wstring GetName();

		GAMEOBJECT_API void SetActive(bool act);
		GAMEOBJECT_API bool GetActive();
		// parent를 따라가며 active를 확인하여 모든 active가 true인 경우에만 true를 반환한다.
		GAMEOBJECT_API bool GetRootActive();

		// 게임오브젝트를 제거하도록 명령한다. (실제로 오브젝트가 제거되는것은 DestroyEvent가 발생할 때 삭제된다.)
		GAMEOBJECT_API void Destroy();

		GAMEOBJECT_API GameObject* GetParent();

		// 이름이 childname인 오브젝트를 찾습니다. child의 child까지 확인합니다.
		GAMEOBJECT_API GameObject* GetChild(std::wstring childname);
		// childnumber번째의 자식오브젝트를 얻습니다.
		GAMEOBJECT_API GameObject* GetChild(int childnumber);
		// 모든 자식오브젝트를 얻습니다.
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

		// 현재 게임오브젝트의 컴포넌트 목록에서 제거
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

		// 컴포넌트 삭제
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
		/// 현재 GameObject가 ParentGameObject라면 GameObject를 생성하여 children으로 추가합니다.
		/// </summary>
		/// <returns> 생성된 GameObject의 포인터 </returns>
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
		/// <para>컴포넌트 객체 생성</para>
		/// <para>이 함수는 new와 같은 역할을 하므로 주의</para>
		/// </summary>
		/// <typeparam name="componentT"> 생성할 컴포넌트 타입 </typeparam>
		/// <returns> 생성된 컴포넌트 포인터 </returns>
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