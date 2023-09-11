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
* 박스 콜라이더를 상속받아서
* 콜라이더에 닿으면 OnTriggerEnter 함수를 실행한다.
* OnTriggerEnter는 재정의해서 안에 SceneLoad 함수를 실행시켜서 다음 씬을 추가하도록 할 예정이다.
* 
* SceneLoad 함수를 실행시키기 전에 페이드 인 / 아웃도 꼭 해야한다.
* 
*/