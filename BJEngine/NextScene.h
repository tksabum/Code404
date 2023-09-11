#pragma once

#include <string>

#include "Component.h"

namespace BJEngine
{
	class GameObject;

	class NextScene : public Component
	{
	public:
		NextScene();
		virtual ~NextScene();

		virtual void OnTriggerEnter(Collider* other) override;
		virtual void OnTriggerExit(Collider* other) override;

		void SetBlackMask(GameObject* gameobject);
	
	private:
		GameObject* blackmask = nullptr;

	};
}



/*
* �ڽ� �ݶ��̴��� ��ӹ޾Ƽ�
* �ݶ��̴��� ������ OnTriggerEnter �Լ��� �����Ѵ�.
* OnTriggerEnter�� �������ؼ� �ȿ� SceneLoad �Լ��� ������Ѽ� ���� ���� �߰��ϵ��� �� �����̴�.
* 
* SceneLoad �Լ��� �����Ű�� ���� ���̵� �� / �ƿ��� �� �ؾ��Ѵ�.
* 
*/