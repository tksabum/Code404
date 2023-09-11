#pragma once

#ifdef SCENECHANGER_EXPORTS
#define SCENECHANGER_API __declspec(dllexport)
#else
#define SCENECHANGER_API __declspec(dllimport)
#endif

#include "ConvexpolygonCollider.h"

#include <string>

namespace BJEngine
{
	class SceneChanger : public Component
	{
	public:
		SCENECHANGER_API SceneChanger();
		SCENECHANGER_API virtual ~SceneChanger();

		SCENECHANGER_API void OnTriggerEnter(Collider* other) override;

		SCENECHANGER_API void SetScene(std::wstring scene);

	private:
		std::wstring nextScene;
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