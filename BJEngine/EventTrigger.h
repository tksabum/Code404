#pragma once

#include <string>

#include "Component.h"

namespace BJEngine
{
	class GameObject;

	class EventTrigger : public Component
	{
	public:
		EventTrigger();
		virtual ~EventTrigger();

		virtual void OnTriggerEnter(Collider* other) override;
		// virtual void OnTriggerExit(Collider* other) override;

		void SetActiveObject(GameObject* gameobject);
	
	private:
		GameObject* object = nullptr;
		bool active;
	};
}