#pragma once

#ifdef NAVIGATION_EXPORTS
#define NAVIGATION_API __declspec(dllexport)
#else
#define NAVIGATION_API __declspec(dllimport)
#endif

#include "Component.h"

#include <vector>

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class Navigation : public BJEngine::Component
	{
	public:
		NAVIGATION_API Navigation();
		NAVIGATION_API ~Navigation();

		// �ִܰŸ� �˻�
		NAVIGATION_API std::vector<Math::Vector2D> FindShortestPath(Math::Vector2D destination);

		NAVIGATION_API void SetIsMoving(bool value);
		NAVIGATION_API bool GetIsMoving();

	private:
		// �����̴� ���� ������Ʈ (true�� ��� ���Ž������ ���� ���� �ʴ� ������Ʈ�� �ȴ�.)
		bool isMoving;

	};
}