#include "QuadTree.h"

#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

using namespace std;

namespace BJEngine
{
	using namespace Math;

	QuadTree::QuadTree(Vector2D areaLeftTop, Vector2D areaRightBottom, int treedepth)
	{
		depth = treedepth;

		head = new QuadTreeNode();

		makeQuadTree(areaLeftTop, areaRightBottom, head, treedepth);
	}

	QuadTree::~QuadTree()
	{
		deleteQuadTree(head);
	}

	void QuadTree::Push(Collider* collider)
	{
		Vector2D position = collider->GetGameObject()->GetTransform()->GetWorldPosition();
		float size = collider->GetFarthestLengthFromWorldPosition();

		if (elementMaxSize < size)
		{
			elementMaxSize = size;
		}

		find(head, position)->elements.push_back({ size, collider });
	}

	vector<Collider*> QuadTree::FindPossibleCollisionElements(Math::Vector2D position, float size)
	{
		vector<Collider*> result;

		findPCE(head, position, size + elementMaxSize, result);

		return result;
	}

	void QuadTree::makeQuadTree(Vector2D areaLeftTop, Vector2D areaRightBottom, QuadTreeNode* parentnode, int treedepth)
	{
		parentnode->areaLeftTop = areaLeftTop;
		parentnode->areaRightBottom = areaRightBottom;

		if (treedepth == 1)
		{
			leafAreaSize = areaRightBottom - areaLeftTop;
			return;
		}

		Vector2D center((areaLeftTop.x + areaRightBottom.x) / 2.0f, (areaLeftTop.y + areaRightBottom.y) / 2.0f);

		parentnode->nodeLeftTop = new QuadTreeNode();
		parentnode->nodeLeftBottom = new QuadTreeNode();
		parentnode->nodeRightTop = new QuadTreeNode();
		parentnode->nodeRightBottom = new QuadTreeNode();

		makeQuadTree(areaLeftTop, center, parentnode->nodeLeftTop, treedepth - 1);
		makeQuadTree(Vector2D(areaLeftTop.x, center.y), Vector2D(center.x, areaRightBottom.y), parentnode->nodeLeftBottom, treedepth - 1);
		makeQuadTree(Vector2D(center.x, areaLeftTop.y), Vector2D(areaRightBottom.x, center.y), parentnode->nodeRightTop, treedepth - 1);
		makeQuadTree(center, areaRightBottom, parentnode->nodeRightBottom, treedepth - 1);
	}

	void QuadTree::deleteQuadTree(QuadTreeNode* node)
	{
		if (node == nullptr)
		{
			return;
		}

		deleteQuadTree(node->nodeLeftTop);
		deleteQuadTree(node->nodeLeftBottom);
		deleteQuadTree(node->nodeRightBottom);
		deleteQuadTree(node->nodeRightTop);

		delete node;
	}

	QuadTreeNode* QuadTree::find(QuadTreeNode* target, Math::Vector2D position)
	{
		if (target->nodeLeftTop == nullptr || target->nodeLeftBottom == nullptr || target->nodeRightTop == nullptr || target->nodeRightBottom == nullptr)
		{
			return target;
		}

		Vector2D center((target->areaLeftTop.x + target->areaRightBottom.x) / 2.0f, (target->areaLeftTop.y + target->areaRightBottom.y) / 2.0f);

		// left
		if (position.x < center.x)
		{
			// left top
			if (position.y < center.y)
			{
				return find(target->nodeLeftTop, position);
			}
			// left bottom
			else
			{
				return find(target->nodeLeftBottom, position);
			}
		}
		// right
		else
		{
			// right top
			if (position.y < center.y)
			{
				return find(target->nodeRightTop, position);
			}
			// right bottom
			else
			{
				return find(target->nodeRightBottom, position);
			}
		}
	}

	void QuadTree::findPCE(QuadTreeNode* target, Vector2D position, float size, vector<Collider*>& result)
	{
		if (target->nodeLeftTop == nullptr || target->nodeLeftBottom == nullptr || target->nodeRightTop == nullptr || target->nodeRightBottom == nullptr)
		{
			for (int i = 0; i < target->elements.size(); i++)
			{
				result.push_back(target->elements[i].second);
			}
			return;
		}

		if (checkForPossibleCollision(target->nodeLeftTop->areaLeftTop, target->nodeLeftTop->areaRightBottom, position, size))
		{
			findPCE(target->nodeLeftTop, position, size, result);
		}

		if (checkForPossibleCollision(target->nodeLeftBottom->areaLeftTop, target->nodeLeftBottom->areaRightBottom, position, size))
		{
			findPCE(target->nodeLeftBottom, position, size, result);
		}

		if (checkForPossibleCollision(target->nodeRightTop->areaLeftTop, target->nodeRightTop->areaRightBottom, position, size))
		{
			findPCE(target->nodeRightTop, position, size, result);
		}

		if (checkForPossibleCollision(target->nodeRightBottom->areaLeftTop, target->nodeRightBottom->areaRightBottom, position, size))
		{
			findPCE(target->nodeRightBottom, position, size, result);
		}
	}

	bool QuadTree::checkForPossibleCollision(Vector2D areaLeftTop, Vector2D areaRightBottom, Vector2D position, float size)
	{
		float closestX = max(areaLeftTop.x, min(position.x, areaRightBottom.x));
		float closestY = max(areaLeftTop.y, min(position.y, areaRightBottom.y));

		float distX = position.x - closestX;
		float distY = position.y - closestY;

		float dist = sqrt((distX * distX) + (distY * distY));

		return dist <= size;
	}
}