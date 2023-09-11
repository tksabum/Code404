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

		// 최단거리 검색
		NAVIGATION_API std::vector<Math::Vector2D> FindShortestPath(Math::Vector2D destination);

		NAVIGATION_API void SetIsMoving(bool value);
		NAVIGATION_API bool GetIsMoving();

	private:
		// 움직이는 중인 오브젝트 (true인 경우 경로탐색에서 길을 막지 않는 오브젝트가 된다.)
		bool isMoving;

	};
}