#include "Component.h"

#include <cassert>

namespace BJEngine
{
#ifdef _DEBUG
	bool Component::addComponentFlag = false;
#endif // _DEBUG
	

	Component::Component() : gameObject(nullptr)
	{
	#ifdef _DEBUG
		assert(addComponentFlag);
	#endif // _DEBUG
	}

#ifdef _DEBUG
	void Component::SetAddComponentFlag(bool value)
	{
		addComponentFlag = value;
	}
#endif // _DEBUG

#ifdef _DEBUG
	bool Component::GetAddComponentFlag()
	{
		return addComponentFlag;
	}
#endif // _DEBUG

	Component::~Component()
	{

	}

	void Component::Awake()
	{

	}

	void Component::Start()
	{

	}

	void Component::OnEnable()
	{

	}

	void Component::OnDisable()
	{

	}

	void Component::OnDestroy()
	{

	}

	void Component::OnTriggerEnter()
	{

	}

	void Component::OnTriggerStay()
	{

	}

	void Component::OnTriggerExit()
	{

	}

	void Component::OnTriggerEnter(Collider* other)
	{

	}

	void Component::OnTriggerStay(Collider* other)
	{

	}

	void Component::OnTriggerExit(Collider* other)
	{

	}

	void Component::OnMouseLeftDown()
	{
		
	}

	void Component::OnMouseRightDown()
	{
		
	}

	void Component::OnMouseLeftUp()
	{
		
	}

	void Component::OnMouseRightUp()
	{
		
	}

	void Component::OnMouseEnter()
	{

	}

	void Component::OnMouseExit()
	{

	}

	void Component::OnMouseOver()
	{

	}

	void Component::EarlyUpdate()
	{

	}

	void Component::Update()
	{

	}

	void Component::LateUpdate()
	{

	}

	GameObject* Component::GetGameObject()
	{
		return gameObject;
	}
}