#pragma once

#ifdef NAVIGATIONSYSTEM_EXPORTS
#define NAVIGATIONSYSTEM_API __declspec(dllexport)
#else
#define NAVIGATIONSYSTEM_API __declspec(dllimport)
#endif

#include <vector>
#include <map>

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class Collider;
	class Navigation;

	class NavigationSystem
	{
	public:
		// NavigationSystem에 collider 추가
		NAVIGATIONSYSTEM_API void AddCollider(Collider* collider);
		// NavigationSystem에서 collider 제거
		NAVIGATIONSYSTEM_API void RemoveCollider(Collider* collider);

		// NavigationSystem에 Navigation 추가
		NAVIGATIONSYSTEM_API void AddNavigation(Navigation* navigation);
		// NavigationSystem에서 Navigation 제거
		NAVIGATIONSYSTEM_API void RemoveNavigation(Navigation* navigation);

		//void MakeGraph();

		NAVIGATIONSYSTEM_API std::vector<Math::Vector2D> FindShortestPath(Navigation* navigation, Math::Vector2D destination);

	private:
		bool isOldTable;
		unsigned long long lastFrameNumber;

		std::map<Collider*, Navigation*> navigationTable;
		std::map<Navigation*, Collider*> colliderTable;

		std::vector<Collider*> colliderList;
		std::vector<Navigation*> navigationList;

		// 경로 탐색 범위
		Math::Vector2D navigationAreaSize;

		// 가로, 세로의 노드 수
		Math::Vector2D navigationAreaCount;

		// node
		std::vector<Math::Vector2D> node;

		// edge
		std::vector<std::vector<int>> edge;

		// cost
		std::vector<std::vector<float>> cost;

		// dist
		std::vector<float> dist;
		
		void resetNodeEdge();
		void resetCost();

		// Table을 새로 만든다.
		void recreateTable();


		//singleton
	public:
		NAVIGATIONSYSTEM_API static NavigationSystem& GetInstance();

	private:
		NavigationSystem();
		~NavigationSystem();
		NavigationSystem(const NavigationSystem& ref);
		NavigationSystem& operator=(const NavigationSystem& ref) {}
	};
}