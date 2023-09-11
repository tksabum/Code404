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
* �ڽ� �ݶ��̴��� ��ӹ޾Ƽ�
* �ݶ��̴��� ������ OnTriggerEnter �Լ��� �����Ѵ�.
* OnTriggerEnter�� �������ؼ� �ȿ� SceneLoad �Լ��� ������Ѽ� ���� ���� �߰��ϵ��� �� �����̴�.
* 
* SceneLoad �Լ��� �����Ű�� ���� ���̵� �� / �ƿ��� �� �ؾ��Ѵ�.
* 
*/