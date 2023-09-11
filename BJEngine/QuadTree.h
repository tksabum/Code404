#pragma once

#ifdef QUADTREE_EXPORTS
#define QUADTREE_API __declspec(dllexport)
#else
#define QUADTREE_API __declspec(dllimport)
#endif

#include <vector>

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class Collider;

	struct QuadTreeNode
	{
		Math::Vector2D areaLeftTop;
		Math::Vector2D areaRightBottom;

		QuadTreeNode* nodeLeftTop = nullptr;
		QuadTreeNode* nodeLeftBottom = nullptr;
		QuadTreeNode* nodeRightTop = nullptr;
		QuadTreeNode* nodeRightBottom = nullptr;

		std::vector<std::pair<float, Collider*>> elements;
	};

	class QuadTree
	{
	public:
		QUADTREE_API QuadTree(Math::Vector2D areaLeftTop, Math::Vector2D areaRightBottom, int treedepth);
		//QuadTree(Math::Vector2D areaLeftTop, Math::Vector2D areaRightBottom, float elementMaxSize);
		
		QUADTREE_API ~QuadTree();

		// Push elements
		QUADTREE_API void Push(Collider* collider);

		// Find possible collision elements
		QUADTREE_API std::vector<Collider*> FindPossibleCollisionElements(Math::Vector2D position, float size);

	private:
		void makeQuadTree(Math::Vector2D areaLeftTop, Math::Vector2D areaRightBottom, QuadTreeNode* parentnode, int treedepth);
		void deleteQuadTree(QuadTreeNode* node);

		// position이 속한 영역을 찾습니다.
		QuadTreeNode* find(QuadTreeNode* target, Math::Vector2D position);

		// Find possible collision elements
		void findPCE(QuadTreeNode* target, Math::Vector2D position, float size, std::vector<Collider*>& result);

		// Check for possible collisions
		bool checkForPossibleCollision(Math::Vector2D areaLeftTop, Math::Vector2D areaRightBottom, Math::Vector2D position, float size);

		int depth;
		Math::Vector2D leafAreaSize;

		float elementMaxSize;

		QuadTreeNode* head;
	};
}
