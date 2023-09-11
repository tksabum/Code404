#pragma once

#ifdef PHYSICSSYSTEM_EXPORTS
#define PHYSICSSYSTEM_API __declspec(dllexport)
#else
#define PHYSICSSYSTEM_API __declspec(dllimport)
#endif

#include <vector>
#include <set>
#include <map>

#include "Vector2D.h"

namespace BJEngine
{
	class Collider;
	class CircleCollider;
	class LineCollider;
	class BoxCollider;
	class ConvexpolygonCollider;
	class Rigidbody2D;
	class TimeController;

	class SoftBody;
	class Element;

	class PhysicsSystem
	{
	public:
		void Clear();

		PHYSICSSYSTEM_API void Initialize(TimeController* timecontroller);

		// �浹 �̺�Ʈ �߻�
		PHYSICSSYSTEM_API void CollisionEvent();

		// �浹üũ
		PHYSICSSYSTEM_API void CollisionDetection();

		// ���� �̺�Ʈ
		PHYSICSSYSTEM_API void PrePhysicsEvent();
		PHYSICSSYSTEM_API void PhysicsEvent();

		// PhysicsSystem�� collider �߰�
		PHYSICSSYSTEM_API void AddCollider(Collider* collider);
		// PhysicsSystem���� collider ����
		PHYSICSSYSTEM_API void RemoveCollider(Collider* collider);

		// PhysicsSystem�� RigidBody2D �߰�
		PHYSICSSYSTEM_API void AddRigidBody2D(Rigidbody2D* rigidbody2d);
		// PhysicsSystem���� RigidBody2D ����
		PHYSICSSYSTEM_API void RemoveRigidBody2D(Rigidbody2D* rigidbody2d);

		PHYSICSSYSTEM_API void SetCollisionRepulsion(float collisionrepulsion);

		PHYSICSSYSTEM_API void RenderDebuggingInfo();

		PHYSICSSYSTEM_API void SetUsingVelocityOverlapped(bool value);
		PHYSICSSYSTEM_API bool GetUsingVelocityOverlapped();
		
		PHYSICSSYSTEM_API void SetUsingQTreeInCollisionDetection(bool value);
		PHYSICSSYSTEM_API bool GetUsingQTreeInCollisionDetection();

		PHYSICSSYSTEM_API void SetQTreeArea(Math::Vector2D lefttop, Math::Vector2D rightbottom);

		PHYSICSSYSTEM_API void SetQuadTreeDepth(int depth);
		PHYSICSSYSTEM_API int GetQuadTreeDepth();

		PHYSICSSYSTEM_API void AddSoftBody2D(SoftBody* softbody);
		PHYSICSSYSTEM_API void RemoveSoftBody2D(SoftBody* softbody);

	private:
		TimeController* timeController;
		bool isOldTable;
		bool isSoftOldTable;
		// map<rigidbodyPointer, colliderPointer>
		std::map<Rigidbody2D*, Collider*> colliderTable;
		// map<colliderPointer, rigidbodyPointer>
		std::map<Collider*, Rigidbody2D*> rigidbodyTable;
		
		std::map<Collider*, int> colliderIndexTable;

		// element*, (softbody index , element index)
		std::map<Element*, std::pair<int, int>> elementIndexTable;

		std::vector<Rigidbody2D*> rigidbodyList;
		std::vector<Collider*> colliderList;
		std::vector<std::set<int>> prevCollisionResult;
		std::vector<std::set<int>> currentCollisionResult;

		/// softbody List
		std::vector<SoftBody*> softbodyList;

		/// vector �ε����� softbody�� ������ �ϰ� softbody���� ������Ʈ ����Ʈ�� �ε����� �����ؼ�
		/// �ش� ������Ʈ �ε����� ��Ʈ�� �ݶ��̴� �ε����� ������ �� �ִ�.
		std::vector<std::vector<std::set<int>>> prevSoftResult;
		std::vector<std::vector<std::set<int>>> currentSoftResult;

		// key: �浹�� �� Collider������, value: collisionPoint(�浹��ġ), collisionVector(�浹����� ���̸� ��Ÿ���� ����)
		std::map<std::pair<Collider*, Collider*>, std::pair<Math::Vector2D, Math::Vector2D>> collisionInfo;
		std::map<std::pair<Element*, Collider*>, std::pair<Math::Vector2D, Math::Vector2D>> softcollisionInfo;

		bool collisionDetectionWithGJK(Collider* collider1, Collider* collider2);
		bool collisionDetectionWithGJK(Element* element, Collider* collider1);
		void collisionDetectionWithGJK(Element* softbody, std::set<int>& elementAndCollider);

		Math::Vector2D minkowskiDifference(Collider* collider1, Collider* collider2, Math::Vector2D directionvector);
		Math::Vector2D minkowskiDifference(Element* element, Collider* collider1, Math::Vector2D directionvector);

		void findDirectionAndDepthWithEPA(Collider* collider1, Collider* collider2, Math::Vector2D point1, Math::Vector2D point2, Math::Vector2D point3);
		void findDirectionAndDepthWithEPA(Element* element, Collider* collider1, Math::Vector2D point1, Math::Vector2D point2, Math::Vector2D point3);
		Math::Vector2D getVectorFromOriginToEdge(Math::Vector2D point1, Math::Vector2D point2);

		// Table�� ���� �����.
		void recreateTable();

		void recreateSoftTable();

		float collisionRepulsion;

		// ������ rigidbody�� ���� �о�� ������ velocity�� �ִ°����� ó����, (false�� ��� position�� �̵��ϴ� ������ ó����)
		bool usingVelocityOverlapped;

		bool usingQTreeInCollisionDetection;
		int qtreeDepth;
		Math::Vector2D qtreeAreaLeftTop;
		Math::Vector2D qtreeAreaRightBottom;

		const static int GJK_LOOP_MAX;
		const static int EPA_LOOP_MAX;
		const static float EPA_TOLERANCE;
		const static float EPA_DEPTH_TOLERANCE;
		const static float COLLISION_REPULSION_DEFAULT;
		const static bool USING_VELOCITY_OVERLAPPED_DEFAULT;
		const static float ALWAYS_REPULSION_DEPTH;
		const static int QUADTREE_DEPTH_DEFAULT;
		const static bool USING_QTREE_DEFAULT;
		const static Math::Vector2D QUADTREE_AREA_LEFT_TOP_DEFAULT;
		const static Math::Vector2D QUADTREE_AREA_RIGHT_BOTTOM_DEFAULT;

		//singleton
	public:
		PHYSICSSYSTEM_API static PhysicsSystem& GetInstance();

	private:
		PhysicsSystem();
		~PhysicsSystem();
		PhysicsSystem(const PhysicsSystem& ref);
		PhysicsSystem& operator=(const PhysicsSystem& ref) {}
	};
}