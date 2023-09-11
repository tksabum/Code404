#pragma once

#ifdef COMPONENT_EXPORTS
#define COMPONENT_API __declspec(dllexport)
#else
#define COMPONENT_API __declspec(dllimport)
#endif

namespace BJEngine
{
	class GameObject;
	class ComponentSystem;
	class Collider;

	class Component
	{
	public:
		COMPONENT_API virtual ~Component();

		COMPONENT_API virtual void Awake();
		COMPONENT_API virtual void Start();
		COMPONENT_API virtual void OnEnable();
		COMPONENT_API virtual void OnDisable();

		COMPONENT_API virtual void OnDestroy();

		COMPONENT_API virtual void OnTriggerEnter();
		COMPONENT_API virtual void OnTriggerStay();
		COMPONENT_API virtual void OnTriggerExit();
		COMPONENT_API virtual void OnTriggerEnter(Collider* other);
		COMPONENT_API virtual void OnTriggerStay(Collider* other);
		COMPONENT_API virtual void OnTriggerExit(Collider* other);

		//COMPONENT_API virtual void OnCollisionEnter();
		//COMPONENT_API virtual void OnCollisionExit();

		// 오브젝트 위에서 마우스를 누르면 호출
		COMPONENT_API virtual void OnMouseLeftDown();
		COMPONENT_API virtual void OnMouseRightDown();
		// 오브젝트 위에서 마우스를 떼면 호출
		COMPONENT_API virtual void OnMouseLeftUp();
		COMPONENT_API virtual void OnMouseRightUp();

		// 마우스가 오브젝트 위로 올라갔을 때 호출
		COMPONENT_API virtual void OnMouseEnter();
		// 마우스가 오브젝트에서 벗어났을 때 호출
		COMPONENT_API virtual void OnMouseExit();
		// 마우스가 오브젝트 위에 있으면 계속 호출
		COMPONENT_API virtual void OnMouseOver();

		//COMPONENT_API virtual void FixedUpdate();
		COMPONENT_API virtual void EarlyUpdate();
		COMPONENT_API virtual void Update();
		COMPONENT_API virtual void LateUpdate();

		COMPONENT_API GameObject* GetGameObject();

	protected:
		GameObject* gameObject;

		COMPONENT_API Component();

	private:
	#ifdef _DEBUG
		static bool addComponentFlag;
		COMPONENT_API static void SetAddComponentFlag(bool value);
		COMPONENT_API static bool GetAddComponentFlag();
	#endif // _DEBUG
		

		friend GameObject;
		friend ComponentSystem;
	};
}