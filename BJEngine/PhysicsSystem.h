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

		// 충돌 이벤트 발생
		PHYSICSSYSTEM_API void CollisionEvent();

		// 충돌체크
		PHYSICSSYSTEM_API void CollisionDetection();

		// 물리 이벤트
		PHYSICSSYSTEM_API void PrePhysicsEvent();
		PHYSICSSYSTEM_API void PhysicsEvent();

		// PhysicsSystem에 collider 추가
		PHYSICSSYSTEM_API void AddCollider(Collider* collider);
		// PhysicsSystem에서 collider 제거
		PHYSICSSYSTEM_API void RemoveCollider(Collider* collider);

		// PhysicsSystem에 RigidBody2D 추가
		PHYSICSSYSTEM_API void AddRigidBody2D(Rigidbody2D* rigidbody2d);
		// PhysicsSystem에서 RigidBody2D 제거
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

		/// vector 인덱스로 softbody에 접근을 하고 softbody에서 엘리멘트 리스트의 인덱스에 접근해서
		/// 해당 엘리멘트 인덱스와 세트인 콜라이더 인덱스를 가져올 수 있다.
		std::vector<std::vector<std::set<int>>> prevSoftResult;
		std::vector<std::vector<std::set<int>>> currentSoftResult;

		// key: 충돌한 두 Collider포인터, value: collisionPoint(충돌위치), collisionVector(충돌방향과 깊이를 나타내는 벡터)
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

		// Table을 새로 만든다.
		void recreateTable();

		void recreateSoftTable();

		float collisionRepulsion;

		// 겹쳐진 rigidbody가 서로 밀어내는 동작을 velocity를 주는것으로 처리함, (false인 경우 position을 이동하는 것으로 처리함)
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