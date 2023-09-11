#include "PhysicsSystem.h"
#include <typeinfo>
#include <map>
#include <queue>
#include <string>

#include "BJMath.h"
#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "ConvexpolygonCollider.h"
#include "LineCollider.h"
#include "Rigidbody2D.h"
#include "GameObject.h"
#include "EngineSetting.h"
#include "TimeController.h"
#include "Transform.h"
#include "Matrix3x1.h"
#include "Matrix3x3.h"
#include "GraphicsSystem.h"
#include "Graphics.h"
#include "Camera.h"
#include "QuadTree.h"

#include "SoftBody.h"
#include "Element.h"
#include "Spring.h"

using namespace std;


namespace BJEngine
{
	using namespace Math;

	const int PhysicsSystem::GJK_LOOP_MAX = 1000;
	const int PhysicsSystem::EPA_LOOP_MAX = 1000;
	const float PhysicsSystem::EPA_TOLERANCE = 0.1f;
	const float PhysicsSystem::EPA_DEPTH_TOLERANCE = 0.001f;
	const float PhysicsSystem::COLLISION_REPULSION_DEFAULT = 1.0f;
	const bool PhysicsSystem::USING_VELOCITY_OVERLAPPED_DEFAULT = true;
	const float PhysicsSystem::ALWAYS_REPULSION_DEPTH = 5.0f;
	const int PhysicsSystem::QUADTREE_DEPTH_DEFAULT = 10;
	const bool PhysicsSystem::USING_QTREE_DEFAULT = true;
	const Vector2D PhysicsSystem::QUADTREE_AREA_LEFT_TOP_DEFAULT(-5000.0f, -5000.0f);
	const Vector2D PhysicsSystem::QUADTREE_AREA_RIGHT_BOTTOM_DEFAULT(5000.0f, 5000.0f);

	void PhysicsSystem::Clear()
	{
		prevCollisionResult.clear();
		currentCollisionResult.clear();
		prevSoftResult.clear();
		currentSoftResult.clear();
	}

	void PhysicsSystem::Initialize(TimeController* timecontroller)
	{
		timeController = timecontroller;
	}

	void PhysicsSystem::CollisionEvent()
	{
		if (isOldTable)
		{
			isOldTable = false;
			recreateTable();
		}
		
		if (isSoftOldTable)
		{
			isSoftOldTable = false;
			recreateSoftTable();
		}

		for (auto& e : softbodyList)
		{
			for (auto a : e->elementList)
			{
				a->RemoveForce();
			}
		}

		for (int i = 0; i < colliderList.size(); i++)
		{
			Collider* currentcollider = colliderList[i];

			for (set<int>::iterator iter = currentCollisionResult[i].begin(); iter != currentCollisionResult[i].end(); iter++)
			{
				int othercolliderid = *iter;
				Collider* othercollider = colliderList[*iter];

				if (prevCollisionResult[i].count(othercolliderid) == 0)
				{
					currentcollider->GetGameObject()->OnTriggerEnter(othercollider);
				}
				else
				{
					currentcollider->GetGameObject()->OnTriggerStay(othercollider);
				}
			}
		}

		for (int i = 0; i < colliderList.size(); i++)
		{
			Collider* currentcollider = colliderList[i];

			for (set<int>::iterator iter = prevCollisionResult[i].begin(); iter != prevCollisionResult[i].end(); iter++)
			{
				int othercolliderid = *iter;
				Collider* othercollider = colliderList[*iter];

				if (currentCollisionResult[i].count(othercolliderid) == 0)
				{
					currentcollider->GetGameObject()->OnTriggerExit(othercollider);
				}
			}
		}

		for (int i = 0; i < colliderList.size(); i++)
		{
			Collider* currentcollider = colliderList[i];
			bool prevcollision = (prevCollisionResult[i].size() > 0);
			bool currentcollision = (currentCollisionResult[i].size() > 0);

			if (!prevcollision && currentcollision)
			{
				currentcollider->GetGameObject()->OnTriggerEnter();
			}
			else if (prevcollision && currentcollision)
			{
				currentcollider->GetGameObject()->OnTriggerStay();
			}
			else if (prevcollision && !currentcollision)
			{
				currentcollider->GetGameObject()->OnTriggerExit();
			}
		}
	}

	void PhysicsSystem::CollisionDetection()
	{
		if (isOldTable)
		{
			isOldTable = false;
			recreateTable();
		}

		
		if (isSoftOldTable)
		{
			isSoftOldTable = false;
			recreateSoftTable();
		}
		

		collisionInfo.clear();
 		softcollisionInfo.clear();

		if (currentCollisionResult.size() == 0)
		{
			prevCollisionResult.clear();
			prevCollisionResult.resize(colliderList.size(), set<int>());
		}
		else
		{
			prevCollisionResult = currentCollisionResult;
		}
		
		if (currentSoftResult.size() == 0)
		{
			prevSoftResult.clear();
			prevSoftResult.resize(softbodyList.size());

			for (int i = 0; i < prevSoftResult.size(); i++)
			{
				prevSoftResult[i].resize(softbodyList[i]->elementList.size(), set<int>());
			}
		}
		else
		{
			prevSoftResult = currentSoftResult;
		}
		

		currentCollisionResult.clear();
		currentCollisionResult.resize(colliderList.size(), set<int>());

		
		currentSoftResult.clear();
		currentSoftResult.resize(softbodyList.size());
		for (int i = 0; i < currentSoftResult.size(); i++)
		{
			currentSoftResult[i].resize(softbodyList[i]->elementList.size(), set<int>());
		}
		

		while (prevCollisionResult.size() < currentCollisionResult.size())
		{
			prevCollisionResult.push_back(set<int>());
		}

		
		for (int i = 0; i < prevSoftResult.size(); i++)
		{
			while (prevSoftResult[i].size() < currentSoftResult[i].size())
			{
				prevSoftResult[i].push_back(set<int>());
			}
		}
		

		// <layer, pair<idx, obj>>
		map<int, vector<pair<int, Collider*>>> collisionMap;

		map<int, vector<pair<int, SoftBody*>>> softbodyMap;

		for (int i = 0; i < colliderList.size(); i++)
		{
			// layer(key)값이 아직 없는 경우
			if (collisionMap.count(colliderList[i]->GetGameObject()->GetLayerID()) == 0)
			{
				collisionMap.insert({ colliderList[i]->GetGameObject()->GetLayerID(), vector<pair<int, Collider*>>(1, make_pair(i, colliderList[i])) });
			}
			else
			{
				collisionMap[colliderList[i]->GetGameObject()->GetLayerID()].push_back(make_pair(i, colliderList[i]));
			}
		}

		
		for (int i = 0; i < softbodyList.size(); i++)
		{
			if (softbodyMap.count(softbodyList[i]->GetGameObject()->GetLayerID() == 0))
			{
				softbodyMap.insert({ softbodyList[i]->GetGameObject()->GetLayerID(), vector<pair<int, SoftBody*>>(1, make_pair(i, softbodyList[i])) });
			}
			else
			{
				softbodyMap[softbodyList[i]->GetGameObject()->GetLayerID()].push_back(make_pair(i, softbodyList[i]));
			}
		}
		

		// softbody 충돌 체크
		
		for (int i = 0; i < softbodyList.size(); i++)
		{
			for (int j = 0; j < rigidbodyList.size(); j++)
			{
				int softID = softbodyList[i]->GetGameObject()->GetLayerID();
				int rigidID = rigidbodyList[j]->GetGameObject()->GetLayerID();

				if (EngineSetting::GetInstance().GetCollisionSetting(softID, rigidID))
				{
					vector<pair<int, Collider*>> clist = collisionMap[rigidID];
					vector<pair<int, SoftBody*>> softlist = softbodyMap[softID];

					// 쿼드트리를 사용함
					if (usingQTreeInCollisionDetection)
					{

					}

					// 쿼드트리를 사용하지 않음
					else
					{
						// softbody 리스트와 collider 리스트를 순회하면서 충돌 감지를 함.
						for (int s = 0; s < softlist.size(); s++)
						{
							// 맵의 
							SoftBody* softbody = softlist[s].second;
							int softID = softlist[s].first;

							if (!(softbody->GetGameObject()->GetRootActive()))
							{
								continue;
							}

							for (int e = 0; e < softbody->elementList.size(); e++)
							{
								set<int> elementAndCollider;
								collisionDetectionWithGJK(softbody->elementList[e], elementAndCollider);
								currentSoftResult[softID][e] = elementAndCollider;
							}
						}
					}
				}
			}
		}
		

		// 같은 Layer 내의 충돌
		for (int i = 0; i < EngineSetting::LAYER_MAX_SIZE; i++)
		{
			if (EngineSetting::GetInstance().GetCollisionSetting(i, i))
			{
				// layer i의 colliderList
				vector<pair<int, Collider*>> clist = collisionMap[i];

				// 쿼드트리 사용
				if (usingQTreeInCollisionDetection)
				{
					// 쿼드트리 생성
					QuadTree qtree(qtreeAreaLeftTop, qtreeAreaRightBottom, qtreeDepth);
					for (int i = 0; i < clist.size(); i++)
					{
						qtree.Push(clist[i].second);
					}

					for (int c1 = 0; c1 < clist.size(); c1++)
					{
						Collider* c1collider = clist[c1].second;
						int c1id = clist[c1].first;

						vector<Collider*> PCElist = qtree.FindPossibleCollisionElements(c1collider->GetGameObject()->GetTransform()->GetWorldPosition(), c1collider->GetFarthestLengthFromWorldPosition());

						for (vector<Collider*>::iterator iter = PCElist.begin(); iter != PCElist.end(); iter++)
						{
							Collider* c2collider = *iter;
							int c2id = colliderIndexTable[*iter];

							// 자기자신 제외
							if (c1collider == c2collider)
							{
								continue;
							}

							if (!(c1collider->GetGameObject()->GetRootActive() && c2collider->GetGameObject()->GetRootActive()))
							{
								continue;
							}

							bool result = collisionDetectionWithGJK(c1collider, c2collider);

							if (result)
							{
								currentCollisionResult[c1id].insert(c2id);
								currentCollisionResult[c2id].insert(c1id);
							}
						}
					}
				}
				// 쿼드트리 사용하지 않음
				else
				{
					for (int c1 = 0; c1 < (int)clist.size() - 1; c1++)
					{
						for (int c2 = c1 + 1; c2 < clist.size(); c2++)
						{
							Collider* c1collider = clist[c1].second;
							Collider* c2collider = clist[c2].second;
							int c1id = clist[c1].first;
							int c2id = clist[c2].first;

							if (!(c1collider->GetGameObject()->GetRootActive() && c2collider->GetGameObject()->GetRootActive()))
							{
								continue;
							}

							bool result = collisionDetectionWithGJK(c1collider, c2collider);

							if (result)
							{
								currentCollisionResult[c1id].insert(c2id);
								currentCollisionResult[c2id].insert(c1id);
							}
						}
					}
				}
			}
		}

		// 서로 다른 Layer의 충돌
		for (int i = 0; i < EngineSetting::LAYER_MAX_SIZE - 1; i++)
		{
			for (int j = i + 1; j < EngineSetting::LAYER_MAX_SIZE; j++)
			{
				if (EngineSetting::GetInstance().GetCollisionSetting(i, j))
				{
					vector<pair<int, Collider*>> clist1 = collisionMap[i];
					vector<pair<int, Collider*>> clist2 = collisionMap[j];

					// 쿼드트리 사용
					if (usingQTreeInCollisionDetection)
					{
						// 쿼드트리 생성
						QuadTree qtree(qtreeAreaLeftTop, qtreeAreaRightBottom, qtreeDepth);
						for (int i = 0; i < clist2.size(); i++)
						{
							qtree.Push(clist2[i].second);
						}

						for (int c1 = 0; c1 < clist1.size(); c1++)
						{
							Collider* c1collider = clist1[c1].second;
							int c1id = clist1[c1].first;

							vector<Collider*> PCElist = qtree.FindPossibleCollisionElements(c1collider->GetGameObject()->GetTransform()->GetWorldPosition(), c1collider->GetFarthestLengthFromWorldPosition());

							for (vector<Collider*>::iterator iter = PCElist.begin(); iter != PCElist.end(); iter++)
							{
								Collider* c2collider = *iter;
								int c2id = colliderIndexTable[*iter];

								if (!(c1collider->GetGameObject()->GetRootActive() && c2collider->GetGameObject()->GetRootActive()))
								{
									continue;
								}

								bool result = collisionDetectionWithGJK(c1collider, c2collider);

								if (result)
								{
									currentCollisionResult[c1id].insert(c2id);
									currentCollisionResult[c2id].insert(c1id);
								}
							}
						}
					}
					// 쿼드트리 사용하지 않음
					else
					{
						for (int c1 = 0; c1 < clist1.size(); c1++)
						{
							for (int c2 = 0; c2 < clist2.size(); c2++)
							{
								Collider* c1collider = clist1[c1].second;
								Collider* c2collider = clist2[c2].second;
								int c1id = clist1[c1].first;
								int c2id = clist2[c2].first;

								if (!(c1collider->GetGameObject()->GetRootActive() && c2collider->GetGameObject()->GetRootActive()))
								{
									continue;
								}

								bool result = collisionDetectionWithGJK(c1collider, c2collider);

								if (result)
								{
									currentCollisionResult[c1id].insert(c2id);
									currentCollisionResult[c2id].insert(c1id);
								}
							}
						}
					}
				}
			}
		}
	}

	void PhysicsSystem::PrePhysicsEvent()
	{
		float deltaTime = timeController->GetPhysicsDeltaTime();

		// 소프트 바디 내부에 있는 스프링 리스트를 돔
		for (int i = 0; i < softbodyList.size(); i++)
		{
			softbodyList[i]->SpringUpdate();
		}

		// 스프링으로 쌓인 힘을 속력으로 바꿈
		for (int i = 0; i < softbodyList.size(); i++)
		{
			for (int j = 0; j < softbodyList[i]->elementList.size(); j++)
			{
				Vector2D gravity{ 0.f, 200.f };
				softbodyList[i]->elementList[j]->ForceToVelocity(gravity, deltaTime);
			}
		}

		for (int i = 0; i < rigidbodyList.size(); i++)
		{
			// 활성화된 게임 오브젝트에만 적용
			if (rigidbodyList[i]->GetGameObject()->GetActive() == true)
			{
				Transform* transform = rigidbodyList[i]->GetGameObject()->GetTransform();

				// 충돌과 관련없이 모든 rigidbody에 대하여
				// overlap velocity로 이동
				// 속도, 각속도에 따라서 이동과 회전
				Vector2D prevPosition = transform->GetLocalPosition();
				Vector3D prevRotation = transform->GetLocalRotation();

				Matrix3x3 transformMatrix = Matrix3x3::Translation(rigidbodyList[i]->GetVelocity() * deltaTime);
				Vector2D nextPosition = transformMatrix * prevPosition;
				Vector3D nextRotation = Vector3D(prevRotation.x, prevRotation.y, prevRotation.z + rigidbodyList[i]->GetRotationalVelocity() * deltaTime);

				transform->SetLocalPosition(nextPosition);
				transform->SetLocalRotation(nextRotation);
			}
		}
	}

	void PhysicsSystem::PhysicsEvent()
	{
		if (isOldTable)
		{
			isOldTable = false;
			recreateTable();
		}

		
		if (isSoftOldTable)
		{
			isSoftOldTable = false;
			recreateSoftTable();
		}

		/// 
		
		for (map<pair<Element*, Collider*>, pair<Vector2D, Vector2D>>::iterator iter = softcollisionInfo.begin(); iter != softcollisionInfo.end(); iter++)
		{
			Element* element = (*iter).first.first;
			Collider* collider = (*iter).first.second;

			// 충돌지점
			Vector2D collisionPoint = (*iter).second.first;

			// 충돌벡터 (충돌방향과 깊이)
			Vector2D collisionVector = (*iter).second.second;

			int softbodyIdx = elementIndexTable[element].first;
			int elementIdx = elementIndexTable[element].second;
			int colliderIdx = colliderIndexTable[collider];

			// collider가 rigidbody를 가지고 있는 경우
			if (rigidbodyTable.count(collider) != 0)
			{
				Rigidbody2D* rigidbody = rigidbodyTable[collider];

				// collider의 trigger가 아닌 경우
				if (!collider->GetTrigger())
				{
					Vector2D centroid1 = collider->GetCentroid();
					Vector2D centroid2 = element->GetWorldPosition();

					float farthest1 = collider->GetFarthestLength();
					float farthest2 = element->GetRadius();

					float c1 = (centroid1 - collisionPoint).Size();
					float b1 = DotProduct((centroid1 - collisionPoint), collisionVector.GetUnitVector());
					float c2 = (centroid2 - collisionPoint).Size();
					float b2 = DotProduct((centroid2 - collisionPoint), collisionVector.GetUnitVector());

					// 무게중심에서 부터 충돌벡터까지의 거리
					// 원 충돌시 오차로인해 c보다 b가 큰 경우가 나오기 때문에 절대값을 구한 후, 루트 연산을 진행한다.
					float collsionlength1 = sqrt(abs((c1 * c1) - (b1 * b1)));
					float collsionlength2 = sqrt(abs((c2 * c2) - (b2 * b2)));

					// 충돌 후 회전방향
					float rotationDirectionAfterCollision1 = 1.0f;
					if (CCW(centroid1, collisionPoint, collisionPoint + collisionVector) < 0.0f)
					{
						rotationDirectionAfterCollision1 = -1.0f;
					}

					float rotationDirectionAfterCollision2 = 1.0f;
					if (CCW(centroid2, collisionPoint, collisionPoint - collisionVector) < 0.0f)
					{
						rotationDirectionAfterCollision2 = -1.0f;
					}

					// 충돌시 자신에게 가해진 충격량이 velocity가 되는 비율
					float velocityRate1 = 1.0f;
					float velocityRate2 = 1.0f;
					if (farthest1 != 0.0f) velocityRate1 = 1.0f - collsionlength1 / farthest1;
					if (farthest2 != 0.0f) velocityRate2 = 1.0f - collsionlength2 / farthest2;

					// 충돌시 자신에게 가해진 충격량이 rotationSpeed가 되는 비율
					float rotationRate1 = 1.0f - velocityRate1;
					float rotationRate2 = 1.0f - velocityRate2;

					// 속도
					Vector2D velocity1 = rigidbody->GetVelocity();
					Vector2D velocity2 = element->GetVelocity();

					// 각속도
					float rotationVelocity1 = rigidbody->GetRotationalVelocity();
					float rotationVelocity2 = 0.f;

					// 충돌방향 속도
					Vector2D collsionDirectionVelocity1 = collisionVector.GetUnitVector() * DotProduct(velocity1, collisionVector.GetUnitVector());
					Vector2D collsionDirectionVelocity2 = collisionVector.GetUnitVector() * DotProduct(velocity2, collisionVector.GetUnitVector());

					// 각속도가 속도로 전달되는 방향(unit vector)
					Vector2D rotationTransmissionDirection1 = -collisionVector.GetUnitVector();
					Vector2D rotationTransmissionDirection2 = collisionVector.GetUnitVector();

					// 충돌방향의 수직 속도
					Vector2D collsionOrthogonalVelocity1 = velocity1 - collsionDirectionVelocity1;
					Vector2D collsionOrthogonalVelocity2 = velocity2 - collsionDirectionVelocity2;

					// 반발계수
					float coefficientOfRestitution = min(rigidbody->GetRestitutionValue(), element->GetRestitutionValue());

					// 질량
					float mass1 = rigidbody->GetMass();
					float mass2 = element->GetMass();

					// 충돌로 인한 운동량 교환
					Vector2D u1 = collsionDirectionVelocity1;
					Vector2D u2 = collsionDirectionVelocity2;
					float rotationSpeed1 = rotationVelocity1;
					if (rotationSpeed1 < 0.0f) rotationSpeed1 = -rotationSpeed1;
					float rotationSpeed2 = rotationVelocity2;
					if (rotationSpeed2 < 0.0f) rotationSpeed2 = -rotationSpeed2;

					// 회전이 잠긴경우 처리
					if (rigidbody->GetRotationLock())
					{
						velocityRate1 = 1.0f;
						rotationRate1 = 0.0f;
					}

					float e = coefficientOfRestitution;
					float pr1 = rotationSpeed1;
					float pr2 = rotationSpeed2;
					Vector2D v1 = ((u1 * mass1) + (u2 * mass2) + (u2 - u1) * mass2 * e) / (mass1 + mass2);
					Vector2D v2 = ((u1 * mass1) + (u2 * mass2) + (u1 - u2) * mass1 * e) / (mass1 + mass2);

					Vector2D lockReflection1;
					Vector2D lockReflection2;
					float lockRotationReflection1 = 0.0f;
					float lockRotationReflection2 = 0.0f;


					// 특정 좌표가 잠긴경우 처리
					if (rigidbody->GetPositionXLock())
					{
						lockReflection2 = lockReflection2 - Vector2D(v1.x, 0.0f) * velocityRate2;

						lockRotationReflection2 = lockRotationReflection2 + rotationDirectionAfterCollision2 * abs(v1.x) * rotationRate2;

						v1.x = 0.0f;
					}

					if (rigidbody->GetPositionYLock())
					{
						lockReflection2 = lockReflection2 - Vector2D(0.0f, v1.y) * velocityRate2;

						lockRotationReflection2 = lockRotationReflection2 + rotationDirectionAfterCollision2 * abs(v1.y) * rotationRate2;
						
						v1.y = 0.0f;
					}

					// 충돌 깊이만큼을 위치이동으로 해소하는 경우가 아니라면 이전 프레임에는 충돌하지 않은 경우만 충돌한 오브젝트간의 속도교환
					// 엘리멘트 / 소프트 바디
					if (!usingVelocityOverlapped || prevSoftResult[softbodyIdx][elementIdx].count(colliderIdx) == 0)
					{
						// 상대속도의 방향과 탈출방향이 반대인 경우에만 속도교환
						if (DotProduct((collsionDirectionVelocity1 - collsionDirectionVelocity2), collisionVector.GetUnitVector()) < 0.0f)
						{
							rigidbody->SetVelocity(v1 + lockReflection1 + collsionOrthogonalVelocity1);
							element->SetVelocity(v2 + lockReflection2 + collsionOrthogonalVelocity2);
						}
					}


					// 겹쳐진 크기과 방향에 대한 벡터
					Vector2D acceleration1 = collisionVector;
					if (rigidbody->GetPositionXLock())
					{
						acceleration1.x = 0.0f;
					}
					if (rigidbody->GetPositionYLock())
					{
						acceleration1.y = 0.0f;
					}

					Vector2D acceleration2 = -collisionVector;

					// 겹쳐진 만큼 속도를 증가
					if (usingVelocityOverlapped)
					{
						rigidbody->ApplyForce(acceleration1 * collisionRepulsion * velocityRate1);
						element->ApplyForce(acceleration2 * collisionRepulsion * velocityRate2);
						if (rigidbody->GetRotationLock() == false)
						{
							rigidbody->ApplyTorqueForce(collisionVector.Size() * rotationDirectionAfterCollision1 * collisionRepulsion * rotationRate1);
						}
					}
					// 겹쳐진 만큼 이동
					else
					{
						Transform* transform1 = rigidbody->GetGameObject()->GetTransform();

						Vector2D move1 = acceleration1 * collisionRepulsion;
						Vector2D move2 = acceleration2 * collisionRepulsion;

						if (rigidbody->GetPositionXLock() == true)
						{
							move1.x = 0.0f;
						}
						if (rigidbody->GetPositionYLock() == true)
						{
							move1.y = 0.0f;
						}

						transform1->SetLocalPosition(transform1->GetLocalPosition() + move1);
						element->MovePosition(move2);
					}
				}
			}
		}
		
		/// 
	
		for (map<pair<Collider*, Collider*>, pair<Vector2D, Vector2D>>::iterator iter = collisionInfo.begin(); iter != collisionInfo.end(); iter++)
		{
			Collider* collider1 = (*iter).first.first;
			Collider* collider2 = (*iter).first.second;

			// 같은 충돌에 대한 연산을 두번 하지 않기 위한 코드
			if (collider1 < collider2)
			{
				continue;
			}

			int colliderIdx1 = colliderIndexTable[collider1];
			int colliderIdx2 = colliderIndexTable[collider2];

			// 충돌지점
			Vector2D collisionPoint = (*iter).second.first;

			// 충돌벡터 (충돌방향과 깊이)
			Vector2D collisionVector = (*iter).second.second;

			// 둘 다 rigidbody를 가지고 있는 경우
			if (rigidbodyTable.count(collider1) != 0 && rigidbodyTable.count(collider2) != 0)
			{
				Rigidbody2D* rigidbody1 = rigidbodyTable[collider1];
				Rigidbody2D* rigidbody2 = rigidbodyTable[collider2];

				// 둘 다 trigger가 아닌 경우
				if (!collider1->GetTrigger() && !collider2->GetTrigger())
				{
					// collider1, collider2의 무게중심
					Vector2D centroid1 = collider1->GetCentroid();
					Vector2D centroid2 = collider2->GetCentroid();

					// collider1, collider2의 각각의 무게중심에서의 가장 먼 점과의 거리
					float farthest1 = collider1->GetFarthestLength();
					float farthest2 = collider2->GetFarthestLength();

					float c1 = (centroid1 - collisionPoint).Size();
					float b1 = DotProduct((centroid1 - collisionPoint), collisionVector.GetUnitVector());
					float c2 = (centroid2 - collisionPoint).Size();
					float b2 = DotProduct((centroid2 - collisionPoint), collisionVector.GetUnitVector());

					// 무게중심에서 부터 충돌벡터까지의 거리
					// 원 충돌시 오차로인해 c보다 b가 큰 경우가 나오기 때문에 절대값을 구한 후, 루트 연산을 진행한다.
					float collsionlength1 = sqrt(abs((c1 * c1) - (b1 * b1)));
					float collsionlength2 = sqrt(abs((c2 * c2) - (b2 * b2)));

					// 충돌 후 회전방향
					float rotationDirectionAfterCollision1 = 1.0f;
					if (CCW(centroid1, collisionPoint, collisionPoint + collisionVector) < 0.0f)
					{
						rotationDirectionAfterCollision1 = -1.0f;
					}

					float rotationDirectionAfterCollision2 = 1.0f;
					if (CCW(centroid2, collisionPoint, collisionPoint - collisionVector) < 0.0f)
					{
						rotationDirectionAfterCollision2 = -1.0f;
					}

					// 충돌시 자신에게 가해진 충격량이 velocity가 되는 비율
					float velocityRate1 = 1.0f;
					float velocityRate2 = 1.0f;
					if (farthest1 != 0.0f) velocityRate1 = 1.0f - collsionlength1 / farthest1;
					if (farthest2 != 0.0f) velocityRate2 = 1.0f - collsionlength2 / farthest2;

					// 충돌시 자신에게 가해진 충격량이 rotationSpeed가 되는 비율
					float rotationRate1 = 1.0f - velocityRate1;
					float rotationRate2 = 1.0f - velocityRate2;

					// 속도
					Vector2D velocity1 = rigidbody1->GetVelocity();
					Vector2D velocity2 = rigidbody2->GetVelocity();

					// 각속도
					float rotationVelocity1 = rigidbody1->GetRotationalVelocity();
					float rotationVelocity2 = rigidbody2->GetRotationalVelocity();

					// 각속도가 속도로 전달되는 방향(unit vector)
					Vector2D rotationTransmissionDirection1 = -collisionVector.GetUnitVector();
					Vector2D rotationTransmissionDirection2 = collisionVector.GetUnitVector();

					// 충돌방향 속도
					Vector2D collsionDirectionVelocity1 = collisionVector.GetUnitVector() * DotProduct(velocity1, collisionVector.GetUnitVector());
					Vector2D collsionDirectionVelocity2 = collisionVector.GetUnitVector() * DotProduct(velocity2, collisionVector.GetUnitVector());

					// 충돌방향의 수직 속도
					Vector2D collsionOrthogonalVelocity1 = velocity1 - collsionDirectionVelocity1;
					Vector2D collsionOrthogonalVelocity2 = velocity2 - collsionDirectionVelocity2;

					// 반발계수
					float coefficientOfRestitution = min(rigidbody1->GetRestitutionValue(), rigidbody2->GetRestitutionValue());

					// 질량
					float mass1 = rigidbody1->GetMass();
					float mass2 = rigidbody2->GetMass();

					// 충돌로 인한 운동량 교환
					Vector2D u1 = collsionDirectionVelocity1;
					Vector2D u2 = collsionDirectionVelocity2;
					float rotationSpeed1 = rotationVelocity1;
					if (rotationSpeed1 < 0.0f) rotationSpeed1 = -rotationSpeed1;
					float rotationSpeed2 = rotationVelocity2;
					if (rotationSpeed2 < 0.0f) rotationSpeed2 = -rotationSpeed2;

					// 회전이 잠긴경우 처리
					if (rigidbody1->GetRotationLock())
					{
						velocityRate1 = 1.0f;
						rotationRate1 = 0.0f;
					}

					if (rigidbody2->GetRotationLock())
					{
						velocityRate2 = 1.0f;
						rotationRate2 = 0.0f;
					}

					float e = coefficientOfRestitution;
					float pr1 = rotationSpeed1;
					float pr2 = rotationSpeed2;
					Vector2D v1 = (((u1 * mass1) + (u2 * mass2) + (u2 - u1) * mass2 * e) / (mass1 + mass2) + (rotationTransmissionDirection2 * rotationSpeed2 * rotationRate2) * (mass2 / mass1)) * velocityRate1;
					Vector2D v2 = (((u1 * mass1) + (u2 * mass2) + (u1 - u2) * mass1 * e) / (mass1 + mass2) + (rotationTransmissionDirection1 * rotationSpeed1 * rotationRate1) * (mass1 / mass2)) * velocityRate2;
					float r1 = rotationVelocity1 * (1.0f - rotationRate1) + rotationDirectionAfterCollision1 * ((((u1 * mass1) + (u2 * mass2) + (u2 - u1) * mass2 * e) / (mass1 + mass2) + (rotationTransmissionDirection2 * rotationSpeed2) * (mass2 / mass1)) * rotationRate1).Size();
					float r2 = rotationVelocity2 * (1.0f - rotationRate2) + rotationDirectionAfterCollision2 * ((((u1 * mass1) + (u2 * mass2) + (u1 - u2) * mass1 * e) / (mass1 + mass2) + (rotationTransmissionDirection1 * rotationSpeed1) * (mass1 / mass2)) * rotationRate2).Size();

					Vector2D lockReflection1;
					Vector2D lockReflection2;
					float lockRotationReflection1 = 0.0f;
					float lockRotationReflection2 = 0.0f;

					// 특정 좌표가 잠긴경우 처리
					if (rigidbody1->GetPositionXLock())
					{
						if (!rigidbody2->GetPositionXLock())
						{
							lockReflection2 = lockReflection2 - Vector2D(v1.x, 0.0f) * velocityRate2;

							lockRotationReflection2 = lockRotationReflection2 + rotationDirectionAfterCollision2 * abs(v1.x) * rotationRate2;
						}

						v1.x = 0.0f;
					}

					if (rigidbody1->GetPositionYLock())
					{
						if (!rigidbody2->GetPositionYLock())
						{
							lockReflection2 = lockReflection2 - Vector2D(0.0f, v1.y) * velocityRate2;

							lockRotationReflection2 = lockRotationReflection2 + rotationDirectionAfterCollision2 * abs(v1.y) * rotationRate2;
						}

						v1.y = 0.0f;
					}

					if (rigidbody2->GetPositionXLock())
					{
						if (!rigidbody1->GetPositionXLock())
						{
							lockReflection1 = lockReflection1 - Vector2D(v2.x, 0.0f) * velocityRate1;

							lockRotationReflection1 = lockRotationReflection1 + rotationDirectionAfterCollision1 * abs(v2.x) * rotationRate1;
						}

						v2.x = 0.0f;
					}

					if (rigidbody2->GetPositionYLock())
					{
						if (!rigidbody1->GetPositionYLock())
						{
							lockReflection1 = lockReflection1 - Vector2D(0.0f, v2.y) * velocityRate1;

							lockRotationReflection1 = lockRotationReflection1 + rotationDirectionAfterCollision1 * abs(v2.y) * rotationRate1;
						}

						v2.y = 0.0f;
					}

					// 충돌 깊이만큼을 위치이동으로 해소하는 경우가 아니라면 이전 프레임에는 충돌하지 않은 경우만 충돌한 오브젝트간의 속도교환
					if (!usingVelocityOverlapped || prevCollisionResult[colliderIdx1].count(colliderIdx2) == 0)
					{
						// 상대속도의 방향과 탈출방향이 반대인 경우에만 속도교환
						if (DotProduct((collsionDirectionVelocity1 - collsionDirectionVelocity2), collisionVector.GetUnitVector()) < 0.0f)
						{
							rigidbody1->SetVelocity(v1 + lockReflection1 + collsionOrthogonalVelocity1);
							rigidbody2->SetVelocity(v2 + lockReflection2 + collsionOrthogonalVelocity2);
							rigidbody1->SetRotationalVelocity(r1 + lockRotationReflection1);
							rigidbody2->SetRotationalVelocity(r2 + lockRotationReflection2);
						}
					}

					
					// 겹쳐진 크기과 방향에 대한 벡터
					Vector2D acceleration1 = collisionVector;
					if (rigidbody1->GetPositionXLock())
					{
						acceleration1.x = 0.0f;
					}
					if (rigidbody1->GetPositionYLock())
					{
						acceleration1.y = 0.0f;
					}

					Vector2D acceleration2 = -collisionVector;
					if (rigidbody2->GetPositionXLock())
					{
						acceleration2.x = 0.0f;
					}
					if (rigidbody2->GetPositionYLock())
					{
						acceleration2.y = 0.0f;
					}

					// 겹쳐진 만큼 속도를 증가
					if (usingVelocityOverlapped)
					{
						rigidbody1->ApplyForce(acceleration1* collisionRepulsion* velocityRate1);
						rigidbody2->ApplyForce(acceleration2* collisionRepulsion* velocityRate2);
						if (rigidbody1->GetRotationLock() == false)
						{
							rigidbody1->ApplyTorqueForce(collisionVector.Size() * rotationDirectionAfterCollision1 * collisionRepulsion * rotationRate1);
						}
						if (rigidbody2->GetRotationLock() == false)
						{
							rigidbody2->ApplyTorqueForce(collisionVector.Size() * rotationDirectionAfterCollision2 * collisionRepulsion * rotationRate2);
						}
					}
					// 겹쳐진 만큼 이동
					else
					{
						Transform* transform1 = rigidbody1->GetGameObject()->GetTransform();
						Transform* transform2 = rigidbody2->GetGameObject()->GetTransform();

						Vector2D move1 = acceleration1 * collisionRepulsion;
						Vector2D move2 = acceleration2 * collisionRepulsion;

						if (rigidbody1->GetPositionXLock() == true)
						{
							move1.x = 0.0f;
						}
						if (rigidbody1->GetPositionYLock() == true)
						{
							move1.y = 0.0f;
						}
						if (rigidbody2->GetPositionXLock() == true)
						{
							move2.x = 0.0f;
						}
						if (rigidbody2->GetPositionYLock() == true)
						{
							move2.y = 0.0f;
						}

						transform1->SetLocalPosition(transform1->GetLocalPosition() + move1);
						transform2->SetLocalPosition(transform2->GetLocalPosition() + move2);
					}
				}
			}
		}
		
		// 위치를 이동시킴
		for (int i = 0; i < softbodyList.size(); i++)
		{
			softbodyList[i]->ElementPositionUpdate();
		}

		for (int i = 0; i < rigidbodyList.size(); i++)
		{
			// 활성화된 게임 오브젝트에만 적용
			if (rigidbodyList[i]->GetGameObject()->GetActive() == true)
			{
				// 중력에 영향을 받는 rigidbody는 속도 가속
				if (rigidbodyList[i]->GetAffectedByGravity())
				{
					Vector2D gravity = rigidbodyList[i]->GetGravity();
					rigidbodyList[i]->SetVelocity(rigidbodyList[i]->GetVelocity() + (gravity * timeController->GetPhysicsDeltaTime()));
				}
			}
		}
	}


	void PhysicsSystem::AddCollider(Collider* collider)
	{
		colliderList.push_back(collider);

		isOldTable = true;
	}

	void PhysicsSystem::RemoveCollider(Collider* collider)
	{
		colliderList.erase(remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());

		isOldTable = true;
	}

	void PhysicsSystem::AddRigidBody2D(Rigidbody2D* rigidbody2d)
	{
		rigidbodyList.push_back(rigidbody2d);

		isOldTable = true;
	}

	void PhysicsSystem::RemoveRigidBody2D(Rigidbody2D* rigidbody2d)
	{
		rigidbodyList.erase(remove(rigidbodyList.begin(), rigidbodyList.end(), rigidbody2d), rigidbodyList.end());

		isOldTable = true;
	}

	void PhysicsSystem::SetCollisionRepulsion(float collisionrepulsion)
	{
		collisionRepulsion = collisionrepulsion;
	}

	void PhysicsSystem::RenderDebuggingInfo()
	{
		Camera* maincamera = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		Vector2D cameraPosition = cameraTransform->GetWorldPosition();
		float cameraRotation = cameraTransform->GetWorldRotation().z;
		Vector2D cameraScale = cameraTransform->GetWorldScale();

		Vector2D screenSize = maincamera->GetScreenSize();

		Matrix3x3 transformMatrix = Matrix3x3::Translation(screenSize / 2.0f) * Matrix3x3::RotationByDegree(cameraRotation) * Matrix3x3::Scale(cameraScale) * Matrix3x3::Translation(-cameraPosition);

		
		for (int i = 0; i < softbodyList.size(); i++)
		{
			if (softbodyList[i]->GetGameObject()->GetRootActive() == false)
			{
				continue;
			}

			for (int j = 0; j < softbodyList[i]->elementList.size(); j++)
			{
				Transform* transform = softbodyList[i]->GetGameObject()->GetTransform();

				Vector2D screenPosition = softbodyList[i]->elementList[j]->Get2DScreenPosition();
				Vector2D screenScale = softbodyList[i]->elementList[j]->Get2DScreenScale();
				float screenRotation = softbodyList[i]->elementList[j]->Get2DScreenRotation();

				GraphicsSystem::GetInstance().graphics->DrawEllipse(screenPosition.x, screenPosition.y, (softbodyList[i])->elementList[j]->GetRadius(), (softbodyList[i])->elementList[j]->GetRadius(), screenScale.x, screenScale.y, screenRotation, GraphicsEngine::Color::Black);
			}
			for (int j = 0; j < softbodyList[i]->springList.size(); j++)
			{
				Vector2D p1 = softbodyList[i]->springList[j]->GetElementAScreenPosition();
				Vector2D p2 = softbodyList[i]->springList[j]->GetElementBScreenPosition();

				GraphicsSystem::GetInstance().graphics->DrawLine(p1, p2, 0, 0, 1, 1, 0);
			}
		}
		

		for (int i = 0; i < colliderList.size(); i++)
		{
			// 비활성화된 게임오브젝트는 무시
			if (colliderList[i]->GetGameObject()->GetRootActive() == false)
			{
				continue;
			}

			const static string circlecolliderType = typeid(CircleCollider).name();
			const static string linecolliderType = typeid(LineCollider).name();
			const static string convexpolygoncolliderType = typeid(ConvexpolygonCollider).name();

			string collidername = typeid(*colliderList[i]).name();
			if (collidername == circlecolliderType)
			{
				Transform* transform = colliderList[i]->GetGameObject()->GetTransform();

				Vector2D screenPosition = transform->Get2DScreenPosition();
				Vector2D screenScale = transform->Get2DScreenScale();
				float screenRotation = transform->Get2DScreenRotation();

				GraphicsSystem::GetInstance().graphics->DrawEllipse(screenPosition.x, screenPosition.y, ((CircleCollider*)colliderList[i])->GetRadius(), ((CircleCollider*)colliderList[i])->GetRadius(), screenScale.x, screenScale.y, screenRotation, GraphicsEngine::Color::Black);
			}
			else if (collidername == convexpolygoncolliderType)
			{
				Transform* transform = colliderList[i]->GetGameObject()->GetTransform();

				Vector2D screenPosition = transform->Get2DScreenPosition();
				Vector2D screenScale = transform->Get2DScreenScale();
				float screenRotation = transform->Get2DScreenRotation();

				GraphicsSystem::GetInstance().graphics->DrawPolygon(((ConvexpolygonCollider*)colliderList[i])->GetPoints(), screenPosition.x, screenPosition.y, screenScale.x, screenScale.y, screenRotation, GraphicsEngine::Color::Black);
			}
			else if (collidername == linecolliderType)
			{
				Transform* transform = colliderList[i]->GetGameObject()->GetTransform();

				Vector2D screenPosition = transform->Get2DScreenPosition();
				Vector2D screenScale = transform->Get2DScreenScale();
				float screenRotation = transform->Get2DScreenRotation();

				GraphicsSystem::GetInstance().graphics->DrawLine(((LineCollider*)colliderList[i])->GetPoint1(), ((LineCollider*)colliderList[i])->GetPoint2(), screenPosition.x, screenPosition.y, screenScale.x, screenScale.y, screenRotation, GraphicsEngine::Color::Black);
			}
		}

		// 쿼드트리의 영역
		// 가로선
		for (int i = 0; i < pow(2, qtreeDepth - 1) + 1; i++)
		{
			Vector2D left(qtreeAreaLeftTop.x, qtreeAreaLeftTop.y + ((qtreeAreaRightBottom.y - qtreeAreaLeftTop.y) / pow(2, qtreeDepth - 1)) * i);
			Vector2D right(qtreeAreaRightBottom.x, qtreeAreaLeftTop.y + ((qtreeAreaRightBottom.y - qtreeAreaLeftTop.y) / pow(2, qtreeDepth - 1)) * i);

			GraphicsSystem::GetInstance().graphics->DrawLine(transformMatrix * left, transformMatrix * right, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		}

		// 세로선
		for (int i = 0; i < pow(2, qtreeDepth - 1) + 1; i++)
		{
			Vector2D up(qtreeAreaLeftTop.x + ((qtreeAreaRightBottom.x - qtreeAreaLeftTop.x) / pow(2, qtreeDepth - 1)) * i, qtreeAreaLeftTop.y);
			Vector2D down(qtreeAreaLeftTop.x + ((qtreeAreaRightBottom.x - qtreeAreaLeftTop.x) / pow(2, qtreeDepth - 1)) * i, qtreeAreaRightBottom.y);

			GraphicsSystem::GetInstance().graphics->DrawLine(transformMatrix * up, transformMatrix * down, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		}

		
		for (auto iter = softcollisionInfo.begin(); iter != softcollisionInfo.end(); iter++)
		{
			Element* element = (*iter).first.first;
			Collider* collider = (*iter).first.second;

			int softIdx = elementIndexTable[element].first;
			int elementIdx = elementIndexTable[element].second;
			int colliderIdx = colliderIndexTable[collider];

			// 충돌지점
			Vector2D collisionPoint = (*iter).second.first;

			// 충돌벡터 (충돌방향과 깊이)
			Vector2D collisionVector = (*iter).second.second;

			Vector2D screenPosition1 = transformMatrix * collisionPoint;
			Vector2D screenPosition2 = transformMatrix * (collisionPoint + collisionVector * 2.0f);

			Vector2D elementscreenPos = element->Get2DScreenPosition();
			Vector2D colliderscreenPos = collider->GetGameObject()->GetTransform()->Get2DScreenPosition();

			GraphicsSystem::GetInstance().graphics->DrawRect(screenPosition1.x - 5, screenPosition1.y - 5, 10, 10, GraphicsEngine::Color::Red);
			GraphicsSystem::GetInstance().graphics->DrawLine(elementscreenPos - collisionVector.GetUnitVector() * 50, elementscreenPos + collisionVector.GetUnitVector() * 50, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, GraphicsEngine::Color::Red);
			GraphicsSystem::GetInstance().graphics->DrawLine(colliderscreenPos - collisionVector.GetUnitVector() * 50, colliderscreenPos + collisionVector.GetUnitVector() * 50, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, GraphicsEngine::Color::Red);
			GraphicsSystem::GetInstance().graphics->DrawLine(screenPosition1, screenPosition2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, GraphicsEngine::Color::Blue);
		}
		

		// 충돌 방향과 깊이
		for (map<pair<Collider*, Collider*>, pair<Vector2D, Vector2D>>::iterator iter = collisionInfo.begin(); iter != collisionInfo.end(); iter++)
		{
			Collider* collider1 = (*iter).first.first;
			Collider* collider2 = (*iter).first.second;

			// 같은 충돌에 대한 연산을 두번 하지 않기 위한 코드
			if (collider1 < collider2)
			{
				continue;
			}

			int colliderIdx1 = colliderIndexTable[collider1];
			int colliderIdx2 = colliderIndexTable[collider2];

			// 충돌지점
			Vector2D collisionPoint = (*iter).second.first;

			// 충돌벡터 (충돌방향과 깊이)
			Vector2D collisionVector = (*iter).second.second;

			Vector2D screenPosition1 = transformMatrix * collisionPoint;
			Vector2D screenPosition2 = transformMatrix * (collisionPoint + collisionVector * 2.0f);

			Vector2D colliderscreenPos1 = collider1->GetGameObject()->GetTransform()->Get2DScreenPosition();
			Vector2D colliderscreenPos2 = collider2->GetGameObject()->GetTransform()->Get2DScreenPosition();

			GraphicsSystem::GetInstance().graphics->DrawRect(screenPosition1.x - 5, screenPosition1.y -5, 10, 10, GraphicsEngine::Color::Red);
			GraphicsSystem::GetInstance().graphics->DrawLine(colliderscreenPos1 - collisionVector.GetUnitVector() * 50, colliderscreenPos1 + collisionVector.GetUnitVector() * 50, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, GraphicsEngine::Color::Red);
			GraphicsSystem::GetInstance().graphics->DrawLine(colliderscreenPos2 - collisionVector.GetUnitVector() * 50, colliderscreenPos2 + collisionVector.GetUnitVector() * 50, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, GraphicsEngine::Color::Red);
			GraphicsSystem::GetInstance().graphics->DrawLine(screenPosition1, screenPosition2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, GraphicsEngine::Color::Blue);
		}

		// rigidbody의 속도와 각속도
		for (int i = 0; i < rigidbodyList.size(); i++)
		{
			Transform* transform = rigidbodyList[i]->GetGameObject()->GetTransform();

			Vector2D screenPosition = transform->Get2DScreenPosition();
			Vector2D screenScale = transform->Get2DScreenScale();
			float screenRotation = transform->Get2DScreenRotation();

			wstring str;
			str += L"vX: ";
			str += to_wstring(rigidbodyList[i]->GetVelocity().x) + L"\n";
			str += L"vY: ";
			str += to_wstring(rigidbodyList[i]->GetVelocity().y) + L"\n";
			str += L"R: ";
			str += to_wstring(rigidbodyList[i]->GetRotationalVelocity()) + L"\n";
			GraphicsSystem::GetInstance().graphics->DrawString(str, screenPosition.x, screenPosition.y, 80.0f, 80.0f, 1.0f, 1.0f, 0.0f, 10, GraphicsEngine::Color::Black);
		}
	}

	void PhysicsSystem::SetUsingVelocityOverlapped(bool value)
	{
		usingVelocityOverlapped = value;
	}

	bool PhysicsSystem::GetUsingVelocityOverlapped()
	{
		return usingVelocityOverlapped;
	}

	void PhysicsSystem::SetUsingQTreeInCollisionDetection(bool value)
	{
		usingQTreeInCollisionDetection = value;
	}

	bool PhysicsSystem::GetUsingQTreeInCollisionDetection()
	{
		return usingQTreeInCollisionDetection;
	}

	void PhysicsSystem::SetQTreeArea(Vector2D lefttop, Vector2D rightbottom)
	{
		qtreeAreaLeftTop = lefttop;
		qtreeAreaRightBottom = rightbottom;
	}

	void PhysicsSystem::SetQuadTreeDepth(int depth)
	{
		qtreeDepth = depth;
	}

	int PhysicsSystem::GetQuadTreeDepth()
	{
		return qtreeDepth;
	}

	
	void PhysicsSystem::AddSoftBody2D(SoftBody* softbody)
	{
		softbodyList.push_back(softbody);

		isSoftOldTable = true;
	}
	

	
	void PhysicsSystem::RemoveSoftBody2D(SoftBody* softbody)
	{
		softbodyList.erase(remove(softbodyList.begin(), softbodyList.end(), softbody));

		isSoftOldTable = true;
	}
	

	bool PhysicsSystem::collisionDetectionWithGJK(Collider* collider1, Collider* collider2)
	{
		Vector2D directionVector(1.0f, 0.0f);

		// 민코스키차가 이루는 첫번째 점 A를 찾음
		Vector2D minkowskiPointA = minkowskiDifference(collider1, collider2, directionVector);

		// A에서 원점으로 가는 벡터로 방향벡터를 변경하여 새로운 점 B를 찾는다.
		directionVector = -minkowskiPointA.GetUnitVector();
		Vector2D minkowskiPointB = minkowskiDifference(collider1, collider2, directionVector);

		// A가 방향벡터의 방향으로 원점보다 B에 더 가까운 경우 충돌하지 않음
		if (DotProduct(minkowskiPointB, directionVector) < 0.0f)
		{
			return false;
		}

		// AB x AO x AB 벡터 외적으로 AB에 수직이면서 원점방향의 벡터를 구하여 새로운 방향벡터로 한다.
		directionVector = CrossProduct(CrossProduct((minkowskiPointB - minkowskiPointA), -minkowskiPointA).GetUnitVector(), (minkowskiPointB - minkowskiPointA)).GetUnitVector();

		// A, B, O 가 한 직선상에 있는 경우
		if (directionVector == Vector2D(0.0f, 0.0f))
		{
			directionVector = Vector2D(minkowskiPointA.y, -minkowskiPointA.x);

			if (directionVector == Vector2D(0.0f, 0.0f))
			{
				directionVector = Vector2D(minkowskiPointB.y, -minkowskiPointB.x);
			}
		}

		int loopcount = 0;

		while (true)
		{
			loopcount++;
			if (loopcount > GJK_LOOP_MAX)
			{
				return false;
			}

			// 새로운 방향벡터로 민코스키차가 이루는 도형의 점C를 찾고
			Vector2D minkowskiPointC = minkowskiDifference(collider1, collider2, directionVector);

			// 방향벡터의 방향으로 원점보다 C에 더 가까운 경우 충돌하지 않음
			if (DotProduct(minkowskiPointC, directionVector) < 0.0f)
			{
				return false;
			}

			if (minkowskiPointC == minkowskiPointA || minkowskiPointC == minkowskiPointB)
			{
				return false;
			}

			// 이후 원점이 존재할 수 있는 영역은
			// ABC가 이루는 삼각형의 내부(Rabc),
			// B, C를 지나며 BC에 수직인 직선이 이루는 삼각형 외부영역(Rbc),
			// A, C를 지나며 AC에 수직인 직선이 이루는 삼각형 외부영역(Rac),
			// 이렇게 3가지 영역에 존재할 수 있음

			Vector2D vAB = minkowskiPointB - minkowskiPointA;
			Vector2D vAC = minkowskiPointC - minkowskiPointA;
			Vector2D vBC = minkowskiPointC - minkowskiPointB;

			Vector2D bcDirection = CrossProduct(CrossProduct(-vAC, -vBC).GetUnitVector(), -vBC).GetUnitVector();
			Vector2D acDirection = CrossProduct(CrossProduct(-vBC, -vAC).GetUnitVector(), -vAC).GetUnitVector();

			// 세 점이 한 직선상에 있는 경우
			if (directionVector == Vector2D(0.0f, 0.0f))
			{
				bcDirection = Vector2D(vBC.y, -vBC.x);
			}
			if (directionVector == Vector2D(0.0f, 0.0f))
			{
				acDirection = Vector2D(vAC.y, -vAC.x);
			}

			bool originInRbc = DotProduct(bcDirection, -minkowskiPointC) >= 0.0f;
			bool originInRac = DotProduct(acDirection, -minkowskiPointC) >= 0.0f;

			// Rbc 영역에 원점이 존재하는 경우
			if (originInRbc)
			{
				minkowskiPointA = minkowskiPointC;
				directionVector = bcDirection;
			}
			// Rac 영역에 원점이 존재하는 경우
			else if (originInRac)
			{
				minkowskiPointB = minkowskiPointC;
				directionVector = acDirection;
			}
			// Rabc 영역에 원점이 존재하는 경우
			else
			{
				// EPA알고리즘으로 깊이와 방향 찾기
				findDirectionAndDepthWithEPA(collider1, collider2, minkowskiPointA, minkowskiPointB, minkowskiPointC);

				return true;
			}
		}
	}

	
	bool PhysicsSystem::collisionDetectionWithGJK(Element* element, Collider* collider1)
	{
		Vector2D directionVector(1.0f, 0.0f);

		// 민코스키차가 이루는 첫번째 점 A를 찾음
		Vector2D minkowskiPointA = minkowskiDifference(element, collider1, directionVector);

		// A에서 원점으로 가는 벡터로 방향벡터를 변경하여 새로운 점 B를 찾는다.
		directionVector = -minkowskiPointA.GetUnitVector();
		Vector2D minkowskiPointB = minkowskiDifference(element, collider1, directionVector);

		// A가 방향벡터의 방향으로 원점보다 B에 더 가까운 경우 충돌하지 않음
		if (DotProduct(minkowskiPointB, directionVector) < 0.0f)
		{
			return false;
		}

		// AB x AO x AB 벡터 외적으로 AB에 수직이면서 원점방향의 벡터를 구하여 새로운 방향벡터로 한다.
		directionVector = CrossProduct(CrossProduct((minkowskiPointB - minkowskiPointA), -minkowskiPointA).GetUnitVector(), (minkowskiPointB - minkowskiPointA)).GetUnitVector();

		// A, B, O 가 한 직선상에 있는 경우
		if (directionVector == Vector2D(0.0f, 0.0f))
		{
			directionVector = Vector2D(minkowskiPointA.y, -minkowskiPointA.x);

			if (directionVector == Vector2D(0.0f, 0.0f))
			{
				directionVector = Vector2D(minkowskiPointB.y, -minkowskiPointB.x);
			}
		}

		int loopcount = 0;

		while (true)
		{
			loopcount++;
			if (loopcount > GJK_LOOP_MAX)
			{
				return false;
			}

			// 새로운 방향벡터로 민코스키차가 이루는 도형의 점C를 찾고
			Vector2D minkowskiPointC = minkowskiDifference(element, collider1, directionVector);

			// 방향벡터의 방향으로 원점보다 C에 더 가까운 경우 충돌하지 않음
			if (DotProduct(minkowskiPointC, directionVector) < 0.0f)
			{
				return false;
			}

			if (minkowskiPointC == minkowskiPointA || minkowskiPointC == minkowskiPointB)
			{
				return false;
			}

			// 이후 원점이 존재할 수 있는 영역은
			// ABC가 이루는 삼각형의 내부(Rabc),
			// B, C를 지나며 BC에 수직인 직선이 이루는 삼각형 외부영역(Rbc),
			// A, C를 지나며 AC에 수직인 직선이 이루는 삼각형 외부영역(Rac),
			// 이렇게 3가지 영역에 존재할 수 있음

			Vector2D vAB = minkowskiPointB - minkowskiPointA;
			Vector2D vAC = minkowskiPointC - minkowskiPointA;
			Vector2D vBC = minkowskiPointC - minkowskiPointB;

			Vector2D bcDirection = CrossProduct(CrossProduct(-vAC, -vBC).GetUnitVector(), -vBC).GetUnitVector();
			Vector2D acDirection = CrossProduct(CrossProduct(-vBC, -vAC).GetUnitVector(), -vAC).GetUnitVector();

			// 세 점이 한 직선상에 있는 경우
			if (directionVector == Vector2D(0.0f, 0.0f))
			{
				bcDirection = Vector2D(vBC.y, -vBC.x);
			}
			if (directionVector == Vector2D(0.0f, 0.0f))
			{
				acDirection = Vector2D(vAC.y, -vAC.x);
			}

			bool originInRbc = DotProduct(bcDirection, -minkowskiPointC) >= 0.0f;
			bool originInRac = DotProduct(acDirection, -minkowskiPointC) >= 0.0f;

			// Rbc 영역에 원점이 존재하는 경우
			if (originInRbc)
			{
				minkowskiPointA = minkowskiPointC;
				directionVector = bcDirection;
			}
			// Rac 영역에 원점이 존재하는 경우
			else if (originInRac)
			{
				minkowskiPointB = minkowskiPointC;
				directionVector = acDirection;
			}
			// Rabc 영역에 원점이 존재하는 경우
			else
			{
				// EPA알고리즘으로 깊이와 방향 찾기
				findDirectionAndDepthWithEPA(element, collider1, minkowskiPointA, minkowskiPointB, minkowskiPointC);

				return true;
			}
		}
	}
	


	void PhysicsSystem::collisionDetectionWithGJK(Element* element, set<int>& elementAndCollider)
	{ 
		for (int i = 0; i < colliderList.size(); i++)
		{
			if (collisionDetectionWithGJK(element, colliderList[i]))
			{
				int cIndex = colliderIndexTable[colliderList[i]];
				elementAndCollider.insert(cIndex);
			}
		}
	}


	Vector2D PhysicsSystem::minkowskiDifference(Collider* collider1, Collider* collider2, Vector2D directionvector)
	{
		Vector2D supportPoint1 = collider1->GetSupportPoint(directionvector);
		Vector2D supportPoint2 = collider2->GetSupportPoint(-directionvector);

		return supportPoint1 - supportPoint2;
	}

	
	Vector2D PhysicsSystem::minkowskiDifference(Element* element, Collider* collider1, Math::Vector2D directionvector)
	{
		Vector2D supportPoint1 = collider1->GetSupportPoint(-directionvector.GetUnitVector());
		Vector2D supportPoint2 = element->GetWorldPosition() + directionvector.GetUnitVector() * element->GetRadius();

		return supportPoint2 - supportPoint1;
	}
	

	void PhysicsSystem::findDirectionAndDepthWithEPA(Collider* collider1, Collider* collider2, Vector2D point1, Vector2D point2, Vector2D point3)
	{
		// pair<pair<(-원점과 간선의 거리), (원점에서 간선까지의 벡터)>, pair<(간선을 이루는 정점1), (간선을 이루는 정점2)>>
		priority_queue<pair<pair<float, Vector2D>, pair<Vector2D, Vector2D>>> edgequeue;

		Vector2D otoevector12 = getVectorFromOriginToEdge(point1, point2);
		Vector2D otoevector13 = getVectorFromOriginToEdge(point1, point3);
		Vector2D otoevector23 = getVectorFromOriginToEdge(point2, point3);

		if (otoevector12 == Vector2D(0.0f, 0.0f))
		{
			otoevector12 = Vector2D((point1 - point2).y, (point1 - point2).x).GetUnitVector();
			if (DotProduct(otoevector12, point3) > 0.0f)
			{
				otoevector12 = -otoevector12;
			}
		}

		if (otoevector13 == Vector2D(0.0f, 0.0f))
		{
			otoevector13 = Vector2D((point1 - point3).y, (point1 - point3).x).GetUnitVector();
			if (DotProduct(otoevector13, point2) > 0.0f)
			{
				otoevector13 = -otoevector13;
			}
		}

		if (otoevector23 == Vector2D(0.0f, 0.0f))
		{
			otoevector23 = Vector2D((point2 - point3).y, (point2 - point3).x).GetUnitVector();
			if (DotProduct(otoevector23, point1) > 0.0f)
			{
				otoevector23 = -otoevector23;
			}
		}

		edgequeue.push({ { -otoevector12.Size(), otoevector12 }, { point1, point2 } });
		edgequeue.push({ { -otoevector13.Size(), otoevector13 }, { point1, point3 } });
		edgequeue.push({ { -otoevector23.Size(), otoevector23 }, { point2, point3 } });

		int loopcount = 0;

		while (true)
		{
			loopcount++;
			if (loopcount > EPA_LOOP_MAX)
			{
				return;
			}

			float depth = -edgequeue.top().first.first;
			Vector2D direction = edgequeue.top().first.second;
			Vector2D p1 = edgequeue.top().second.first;
			Vector2D p2 = edgequeue.top().second.second;

			edgequeue.pop();

			Vector2D p3 = minkowskiDifference(collider1, collider2, direction);

			Vector2D newotoevector1 = getVectorFromOriginToEdge(p1, p3);
			Vector2D newotoevector2 = getVectorFromOriginToEdge(p2, p3);

			if ((newotoevector1 != Vector2D(0.0f, 0.0f) && newotoevector2 != Vector2D(0.0f, 0.0f)) && (p1 - p3).Size() > EPA_TOLERANCE && (p2 - p3).Size() > EPA_TOLERANCE)
			{
				edgequeue.push({ { -newotoevector1.Size(), newotoevector1 }, { p1, p3 } });
				edgequeue.push({ { -newotoevector2.Size(), newotoevector2 }, { p2, p3 } });
			}
			else
			{
				// 구해진 direction으로 나오는 2개의 supportpoint 중에서
				// 상대 collider와 충돌 중인 점을 찾으면
				// 그 위치에서 direction 방향으로 충돌이 일어난 것임

				Vector2D penetrationPoint1 = collider1->GetSupportPoint(direction);
				Vector2D penetrationPoint2 = collider2->GetSupportPoint(-direction);

				if (collider2->IsPointInside(penetrationPoint1))
				{
					// penetrationPoint1 에서 충돌이 일어남
					collisionInfo.insert({ { collider1, collider2 }, { penetrationPoint1, -direction } });
					collisionInfo.insert({ { collider2, collider1 }, { penetrationPoint1, direction } });

					collider1->lastCollisionVector = -direction;
					collider2->lastCollisionVector = direction;
				}
				else if (collider1->IsPointInside(penetrationPoint2))
				{
					// penetrationPoint2 에서 충돌이 일어남
					collisionInfo.insert({ { collider1, collider2 }, { penetrationPoint2, -direction } });
					collisionInfo.insert({ { collider2, collider1 }, { penetrationPoint2, direction } });
					collider1->lastCollisionVector = -direction;
					collider2->lastCollisionVector = direction;
				}
				else
				{
					Vector2D penetrationPointcw1 = collider1->GetSupportPoint(Matrix3x3::RotationByDegree(-0.01f) * direction);
					Vector2D penetrationPointcw2 = collider2->GetSupportPoint(Matrix3x3::RotationByDegree(-0.01f) * -direction);
					Vector2D penetrationPointccw1 = collider1->GetSupportPoint(Matrix3x3::RotationByDegree(0.01f) * direction);
					Vector2D penetrationPointccw2 = collider2->GetSupportPoint(Matrix3x3::RotationByDegree(0.01f) * -direction);

					if (collider2->IsPointInside(penetrationPointcw1))
					{
						// penetrationPointcw1 에서 충돌이 일어남
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointcw1, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointcw1, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else if (collider1->IsPointInside(penetrationPointcw2))
					{
						// penetrationPointcw2 에서 충돌이 일어남
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointcw2, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointcw2, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else if (collider2->IsPointInside(penetrationPointccw1))
					{
						// penetrationPointccw1 에서 충돌이 일어남
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointccw1, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointccw1, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else if (collider1->IsPointInside(penetrationPointccw2))
					{
						// penetrationPointccw2 에서 충돌이 일어남
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointccw2, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointccw2, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else
					{
						// 충돌한 상대의 내부에 있는 Support Point가 존재하지 않는 경우
						Vector2D orthogonalDirection(direction.y, -direction.x);

						Vector2D lap1 = collider1->GetSupportPoint(direction);
						Vector2D lap2 = collider1->GetSupportPoint(-direction);

						Vector2D lbp1 = collider2->GetSupportPoint(orthogonalDirection);
						Vector2D lbp2 = collider2->GetSupportPoint(-orthogonalDirection);

						if (DetermineLineOverlap(lap1, lap2, lbp1, lbp2))
						{
							// penetrationPoint1 에서 충돌이 일어남
							collisionInfo.insert({ { collider1, collider2 }, { penetrationPoint1, -direction } });
							collisionInfo.insert({ { collider2, collider1 }, { penetrationPoint1, direction } });
							collider1->lastCollisionVector = -direction;
							collider2->lastCollisionVector = direction;
						}

						lap1 = collider1->GetSupportPoint(orthogonalDirection);
						lap2 = collider1->GetSupportPoint(-orthogonalDirection);

						lbp1 = collider2->GetSupportPoint(direction);
						lbp2 = collider2->GetSupportPoint(-direction);

						if (DetermineLineOverlap(lap1, lap2, lbp1, lbp2))
						{
							// penetrationPoint2 에서 충돌이 일어남
							collisionInfo.insert({ { collider1, collider2 }, { penetrationPoint2, -direction } });
							collisionInfo.insert({ { collider2, collider1 }, { penetrationPoint2, direction } });
							collider1->lastCollisionVector = -direction;
							collider2->lastCollisionVector = direction;
						}
					}
				}

				return;
			}
		}

	}

	
	void PhysicsSystem::findDirectionAndDepthWithEPA(Element* element, Collider* collider1, Math::Vector2D point1, Math::Vector2D point2, Math::Vector2D point3)
	{
		// pair<pair<(-원점과 간선의 거리), (원점에서 간선까지의 벡터)>, pair<(간선을 이루는 정점1), (간선을 이루는 정점2)>>
		priority_queue<pair<pair<float, Vector2D>, pair<Vector2D, Vector2D>>> edgequeue;

		Vector2D otoevector12 = getVectorFromOriginToEdge(point1, point2);
		Vector2D otoevector13 = getVectorFromOriginToEdge(point1, point3);
		Vector2D otoevector23 = getVectorFromOriginToEdge(point2, point3);

		if (otoevector12 == Vector2D(0.0f, 0.0f))
		{
			otoevector12 = Vector2D((point1 - point2).y, (point1 - point2).x).GetUnitVector();
			if (DotProduct(otoevector12, point3) > 0.0f)
			{
				otoevector12 = -otoevector12;
			}
		}

		if (otoevector13 == Vector2D(0.0f, 0.0f))
		{
			otoevector13 = Vector2D((point1 - point3).y, (point1 - point3).x).GetUnitVector();
			if (DotProduct(otoevector13, point2) > 0.0f)
			{
				otoevector13 = -otoevector13;
			}
		}

		if (otoevector23 == Vector2D(0.0f, 0.0f))
		{
			otoevector23 = Vector2D((point2 - point3).y, (point2 - point3).x).GetUnitVector();
			if (DotProduct(otoevector23, point1) > 0.0f)
			{
				otoevector23 = -otoevector23;
			}
		}

		edgequeue.push({ { -otoevector12.Size(), otoevector12 }, { point1, point2 } });
		edgequeue.push({ { -otoevector13.Size(), otoevector13 }, { point1, point3 } });
		edgequeue.push({ { -otoevector23.Size(), otoevector23 }, { point2, point3 } });

		int loopcount = 0;

		while (true)
		{
			loopcount++;
			if (loopcount > EPA_LOOP_MAX)
			{
				return;
			}

			float depth = -edgequeue.top().first.first;
			Vector2D direction = edgequeue.top().first.second;
			Vector2D p1 = edgequeue.top().second.first;
			Vector2D p2 = edgequeue.top().second.second;

			edgequeue.pop();

			Vector2D p3 = minkowskiDifference(element, collider1, direction);

			Vector2D newotoevector1 = getVectorFromOriginToEdge(p1, p3);
			Vector2D newotoevector2 = getVectorFromOriginToEdge(p2, p3);

			if ((newotoevector1 != Vector2D(0.0f, 0.0f) && newotoevector2 != Vector2D(0.0f, 0.0f)) && (p1 - p3).Size() > EPA_TOLERANCE && (p2 - p3).Size() > EPA_TOLERANCE)
			{
				edgequeue.push({ { -newotoevector1.Size(), newotoevector1 }, { p1, p3 } });
				edgequeue.push({ { -newotoevector2.Size(), newotoevector2 }, { p2, p3 } });
			}
			else
			{
				// 구해진 direction으로 나오는 2개의 supportpoint 중에서
				// 상대 collider와 충돌 중인 점을 찾으면
				// 그 위치에서 direction 방향으로 충돌이 일어난 것임

				Vector2D penetrationPoint1 = collider1->GetSupportPoint(direction);
				Vector2D penetrationPoint2 = element->GetWorldPosition() - direction * element->GetRadius();

				if ((penetrationPoint1 - element->GetWorldPosition()).Size() <= element->GetRadius())
				{
					// penetrationPoint1 에서 충돌이 일어남
					softcollisionInfo.insert({ { element, collider1 }, { penetrationPoint1, direction } });
				}
				else if (collider1->IsPointInside(penetrationPoint2))
				{
					// penetrationPoint2 에서 충돌이 일어남
					softcollisionInfo.insert({ { element, collider1 }, { penetrationPoint2, direction } });
				}
				else
				{
					Vector2D penetrationPointcw1 = collider1->GetSupportPoint(Matrix3x3::RotationByDegree(-0.01f) * direction);
					Vector2D directionRotateCW2 = (Matrix3x3::RotationByDegree(-0.01f) * -direction);
					Vector2D penetrationPointcw2 = element->GetWorldPosition() + directionRotateCW2 * element->GetRadius();

					Vector2D penetrationPointccw1 = collider1->GetSupportPoint(Matrix3x3::RotationByDegree(0.01f) * direction);
					Vector2D directionRotateCCW2 = (Matrix3x3::RotationByDegree(0.01f) * -direction);
					Vector2D penetrationPointccw2 = element->GetWorldPosition() + directionRotateCCW2 * element->GetRadius();

					if ((penetrationPointcw1 - element->GetWorldPosition()).Size() <= element->GetRadius())
					{
						// penetrationPointcw1 에서 충돌이 일어남
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointcw1, direction } });
					}
					else if (collider1->IsPointInside(penetrationPointcw2))
					{
						// penetrationPointcw2 에서 충돌이 일어남
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointcw2, direction } });
					}
					else if ((penetrationPointccw1 - element->GetWorldPosition()).Size() <= element->GetRadius())
					{
						// penetrationPointccw1 에서 충돌이 일어남
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointccw1, direction } });
					}
					else if (collider1->IsPointInside(penetrationPointccw2))
					{
						// penetrationPointccw2 에서 충돌이 일어남
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointccw2, direction } });
					}
					else
					{
						bool success = false;

						// 충돌한 상대의 내부에 있는 Support Point가 존재하지 않는 경우
						Vector2D orthogonalDirection(direction.y, -direction.x);

						Vector2D lap1 = collider1->GetSupportPoint(direction);
						Vector2D lap2 = collider1->GetSupportPoint(-direction);

						Vector2D lbp1 = element->GetWorldPosition() + orthogonalDirection * element->GetRadius();
						Vector2D lbp2 = element->GetWorldPosition() - orthogonalDirection * element->GetRadius();

						if (DetermineLineOverlap(lap1, lap2, lbp1, lbp2))
						{
							// penetrationPoint1 에서 충돌이 일어남
							softcollisionInfo.insert({ { element, collider1 }, { penetrationPoint1, direction } });
							success = true;
						}

						lap1 = collider1->GetSupportPoint(orthogonalDirection);
						lap2 = collider1->GetSupportPoint(-orthogonalDirection);

						lbp1 = element->GetWorldPosition() + direction * element->GetRadius();
						lbp2 = element->GetWorldPosition() - direction * element->GetRadius();

						if (DetermineLineOverlap(lap1, lap2, lbp1, lbp2))
						{
							// penetrationPoint2 에서 충돌이 일어남
							softcollisionInfo.insert({ { element, collider1 }, { penetrationPoint2, direction } });
							success = true;
						}

						 if (!success)
						 { 
						 	softcollisionInfo.insert({ { element, collider1 }, { penetrationPoint2, direction } });
						 }
					}
				}
				
				return;
			}
		}
	}
	

	Vector2D PhysicsSystem::getVectorFromOriginToEdge(Vector2D point1, Vector2D point2)
	{
		Vector2D directionvector = CrossProduct(CrossProduct(point1, point2).GetUnitVector(), point1 - point2).GetUnitVector();

		return directionvector * DotProduct(directionvector, point1);
	}

	// recreateTable에서는 gameobject의 포인터를 비교하여 테이블을 생성하는데
	// 생성자에서 AddCollider, AddRigidbody를 호출하는 것보다 컴포넌트에 gameobject주소를 넣어주는 것이 느리기 때문에
	// 객체생성이 완전히 끝난 뒤에 테이블을 생성해야한다.
	void PhysicsSystem::recreateTable()
	{
		// rigidbodyTable
		colliderTable.clear();

		for (int i = 0; i < rigidbodyList.size(); i++)
		{
			for (int j = 0; j < colliderList.size(); j++)
			{
				if (rigidbodyList[i]->GetGameObject() == colliderList[j]->GetGameObject())
				{
					colliderTable[rigidbodyList[i]] = colliderList[j];
					break;
				}
			}
		}

		// colliderTable
		rigidbodyTable.clear();

		for (int i = 0; i < colliderList.size(); i++)
		{
			for (int j = 0; j < rigidbodyList.size(); j++)
			{
				if (colliderList[i]->GetGameObject() == rigidbodyList[j]->GetGameObject())
				{
					rigidbodyTable[colliderList[i]] = rigidbodyList[j];
					break;
				}
			}
		}

		// colliderIndexTable
		colliderIndexTable.clear();

		for (int i = 0; i < colliderList.size(); i++)
		{
			colliderIndexTable[colliderList[i]] = i;
		}
	}

	
	void PhysicsSystem::recreateSoftTable()
	{
		for (int i = 0; i < softbodyList.size(); i++)
		{
			for (int j = 0; j < softbodyList[i]->elementList.size(); j++)
			{
				elementIndexTable[softbodyList[i]->elementList[j]] = make_pair(i, j);
			}
		}

		// rigidbodyTable
		elementIndexTable.clear();

	}
	

	// singleton

	PhysicsSystem& PhysicsSystem::GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}

	PhysicsSystem::PhysicsSystem() : isOldTable(true), /*isSoftOldTable(true),*/ prevCollisionResult(), currentCollisionResult(), collisionRepulsion(COLLISION_REPULSION_DEFAULT), usingVelocityOverlapped(USING_VELOCITY_OVERLAPPED_DEFAULT), qtreeDepth(QUADTREE_DEPTH_DEFAULT), usingQTreeInCollisionDetection(USING_QTREE_DEFAULT), qtreeAreaLeftTop(QUADTREE_AREA_LEFT_TOP_DEFAULT), qtreeAreaRightBottom(QUADTREE_AREA_RIGHT_BOTTOM_DEFAULT)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{

	}
}