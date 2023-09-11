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
			// layer(key)���� ���� ���� ���
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
		

		// softbody �浹 üũ
		
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

					// ����Ʈ���� �����
					if (usingQTreeInCollisionDetection)
					{

					}

					// ����Ʈ���� ������� ����
					else
					{
						// softbody ����Ʈ�� collider ����Ʈ�� ��ȸ�ϸ鼭 �浹 ������ ��.
						for (int s = 0; s < softlist.size(); s++)
						{
							// ���� 
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
		

		// ���� Layer ���� �浹
		for (int i = 0; i < EngineSetting::LAYER_MAX_SIZE; i++)
		{
			if (EngineSetting::GetInstance().GetCollisionSetting(i, i))
			{
				// layer i�� colliderList
				vector<pair<int, Collider*>> clist = collisionMap[i];

				// ����Ʈ�� ���
				if (usingQTreeInCollisionDetection)
				{
					// ����Ʈ�� ����
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

							// �ڱ��ڽ� ����
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
				// ����Ʈ�� ������� ����
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

		// ���� �ٸ� Layer�� �浹
		for (int i = 0; i < EngineSetting::LAYER_MAX_SIZE - 1; i++)
		{
			for (int j = i + 1; j < EngineSetting::LAYER_MAX_SIZE; j++)
			{
				if (EngineSetting::GetInstance().GetCollisionSetting(i, j))
				{
					vector<pair<int, Collider*>> clist1 = collisionMap[i];
					vector<pair<int, Collider*>> clist2 = collisionMap[j];

					// ����Ʈ�� ���
					if (usingQTreeInCollisionDetection)
					{
						// ����Ʈ�� ����
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
					// ����Ʈ�� ������� ����
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

		// ����Ʈ �ٵ� ���ο� �ִ� ������ ����Ʈ�� ��
		for (int i = 0; i < softbodyList.size(); i++)
		{
			softbodyList[i]->SpringUpdate();
		}

		// ���������� ���� ���� �ӷ����� �ٲ�
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
			// Ȱ��ȭ�� ���� ������Ʈ���� ����
			if (rigidbodyList[i]->GetGameObject()->GetActive() == true)
			{
				Transform* transform = rigidbodyList[i]->GetGameObject()->GetTransform();

				// �浹�� ���þ��� ��� rigidbody�� ���Ͽ�
				// overlap velocity�� �̵�
				// �ӵ�, ���ӵ��� ���� �̵��� ȸ��
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

			// �浹����
			Vector2D collisionPoint = (*iter).second.first;

			// �浹���� (�浹����� ����)
			Vector2D collisionVector = (*iter).second.second;

			int softbodyIdx = elementIndexTable[element].first;
			int elementIdx = elementIndexTable[element].second;
			int colliderIdx = colliderIndexTable[collider];

			// collider�� rigidbody�� ������ �ִ� ���
			if (rigidbodyTable.count(collider) != 0)
			{
				Rigidbody2D* rigidbody = rigidbodyTable[collider];

				// collider�� trigger�� �ƴ� ���
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

					// �����߽ɿ��� ���� �浹���ͱ����� �Ÿ�
					// �� �浹�� ���������� c���� b�� ū ��찡 ������ ������ ���밪�� ���� ��, ��Ʈ ������ �����Ѵ�.
					float collsionlength1 = sqrt(abs((c1 * c1) - (b1 * b1)));
					float collsionlength2 = sqrt(abs((c2 * c2) - (b2 * b2)));

					// �浹 �� ȸ������
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

					// �浹�� �ڽſ��� ������ ��ݷ��� velocity�� �Ǵ� ����
					float velocityRate1 = 1.0f;
					float velocityRate2 = 1.0f;
					if (farthest1 != 0.0f) velocityRate1 = 1.0f - collsionlength1 / farthest1;
					if (farthest2 != 0.0f) velocityRate2 = 1.0f - collsionlength2 / farthest2;

					// �浹�� �ڽſ��� ������ ��ݷ��� rotationSpeed�� �Ǵ� ����
					float rotationRate1 = 1.0f - velocityRate1;
					float rotationRate2 = 1.0f - velocityRate2;

					// �ӵ�
					Vector2D velocity1 = rigidbody->GetVelocity();
					Vector2D velocity2 = element->GetVelocity();

					// ���ӵ�
					float rotationVelocity1 = rigidbody->GetRotationalVelocity();
					float rotationVelocity2 = 0.f;

					// �浹���� �ӵ�
					Vector2D collsionDirectionVelocity1 = collisionVector.GetUnitVector() * DotProduct(velocity1, collisionVector.GetUnitVector());
					Vector2D collsionDirectionVelocity2 = collisionVector.GetUnitVector() * DotProduct(velocity2, collisionVector.GetUnitVector());

					// ���ӵ��� �ӵ��� ���޵Ǵ� ����(unit vector)
					Vector2D rotationTransmissionDirection1 = -collisionVector.GetUnitVector();
					Vector2D rotationTransmissionDirection2 = collisionVector.GetUnitVector();

					// �浹������ ���� �ӵ�
					Vector2D collsionOrthogonalVelocity1 = velocity1 - collsionDirectionVelocity1;
					Vector2D collsionOrthogonalVelocity2 = velocity2 - collsionDirectionVelocity2;

					// �ݹ߰��
					float coefficientOfRestitution = min(rigidbody->GetRestitutionValue(), element->GetRestitutionValue());

					// ����
					float mass1 = rigidbody->GetMass();
					float mass2 = element->GetMass();

					// �浹�� ���� ��� ��ȯ
					Vector2D u1 = collsionDirectionVelocity1;
					Vector2D u2 = collsionDirectionVelocity2;
					float rotationSpeed1 = rotationVelocity1;
					if (rotationSpeed1 < 0.0f) rotationSpeed1 = -rotationSpeed1;
					float rotationSpeed2 = rotationVelocity2;
					if (rotationSpeed2 < 0.0f) rotationSpeed2 = -rotationSpeed2;

					// ȸ���� ����� ó��
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


					// Ư�� ��ǥ�� ����� ó��
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

					// �浹 ���̸�ŭ�� ��ġ�̵����� �ؼ��ϴ� ��찡 �ƴ϶�� ���� �����ӿ��� �浹���� ���� ��츸 �浹�� ������Ʈ���� �ӵ���ȯ
					// ������Ʈ / ����Ʈ �ٵ�
					if (!usingVelocityOverlapped || prevSoftResult[softbodyIdx][elementIdx].count(colliderIdx) == 0)
					{
						// ���ӵ��� ����� Ż������� �ݴ��� ��쿡�� �ӵ���ȯ
						if (DotProduct((collsionDirectionVelocity1 - collsionDirectionVelocity2), collisionVector.GetUnitVector()) < 0.0f)
						{
							rigidbody->SetVelocity(v1 + lockReflection1 + collsionOrthogonalVelocity1);
							element->SetVelocity(v2 + lockReflection2 + collsionOrthogonalVelocity2);
						}
					}


					// ������ ũ��� ���⿡ ���� ����
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

					// ������ ��ŭ �ӵ��� ����
					if (usingVelocityOverlapped)
					{
						rigidbody->ApplyForce(acceleration1 * collisionRepulsion * velocityRate1);
						element->ApplyForce(acceleration2 * collisionRepulsion * velocityRate2);
						if (rigidbody->GetRotationLock() == false)
						{
							rigidbody->ApplyTorqueForce(collisionVector.Size() * rotationDirectionAfterCollision1 * collisionRepulsion * rotationRate1);
						}
					}
					// ������ ��ŭ �̵�
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

			// ���� �浹�� ���� ������ �ι� ���� �ʱ� ���� �ڵ�
			if (collider1 < collider2)
			{
				continue;
			}

			int colliderIdx1 = colliderIndexTable[collider1];
			int colliderIdx2 = colliderIndexTable[collider2];

			// �浹����
			Vector2D collisionPoint = (*iter).second.first;

			// �浹���� (�浹����� ����)
			Vector2D collisionVector = (*iter).second.second;

			// �� �� rigidbody�� ������ �ִ� ���
			if (rigidbodyTable.count(collider1) != 0 && rigidbodyTable.count(collider2) != 0)
			{
				Rigidbody2D* rigidbody1 = rigidbodyTable[collider1];
				Rigidbody2D* rigidbody2 = rigidbodyTable[collider2];

				// �� �� trigger�� �ƴ� ���
				if (!collider1->GetTrigger() && !collider2->GetTrigger())
				{
					// collider1, collider2�� �����߽�
					Vector2D centroid1 = collider1->GetCentroid();
					Vector2D centroid2 = collider2->GetCentroid();

					// collider1, collider2�� ������ �����߽ɿ����� ���� �� ������ �Ÿ�
					float farthest1 = collider1->GetFarthestLength();
					float farthest2 = collider2->GetFarthestLength();

					float c1 = (centroid1 - collisionPoint).Size();
					float b1 = DotProduct((centroid1 - collisionPoint), collisionVector.GetUnitVector());
					float c2 = (centroid2 - collisionPoint).Size();
					float b2 = DotProduct((centroid2 - collisionPoint), collisionVector.GetUnitVector());

					// �����߽ɿ��� ���� �浹���ͱ����� �Ÿ�
					// �� �浹�� ���������� c���� b�� ū ��찡 ������ ������ ���밪�� ���� ��, ��Ʈ ������ �����Ѵ�.
					float collsionlength1 = sqrt(abs((c1 * c1) - (b1 * b1)));
					float collsionlength2 = sqrt(abs((c2 * c2) - (b2 * b2)));

					// �浹 �� ȸ������
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

					// �浹�� �ڽſ��� ������ ��ݷ��� velocity�� �Ǵ� ����
					float velocityRate1 = 1.0f;
					float velocityRate2 = 1.0f;
					if (farthest1 != 0.0f) velocityRate1 = 1.0f - collsionlength1 / farthest1;
					if (farthest2 != 0.0f) velocityRate2 = 1.0f - collsionlength2 / farthest2;

					// �浹�� �ڽſ��� ������ ��ݷ��� rotationSpeed�� �Ǵ� ����
					float rotationRate1 = 1.0f - velocityRate1;
					float rotationRate2 = 1.0f - velocityRate2;

					// �ӵ�
					Vector2D velocity1 = rigidbody1->GetVelocity();
					Vector2D velocity2 = rigidbody2->GetVelocity();

					// ���ӵ�
					float rotationVelocity1 = rigidbody1->GetRotationalVelocity();
					float rotationVelocity2 = rigidbody2->GetRotationalVelocity();

					// ���ӵ��� �ӵ��� ���޵Ǵ� ����(unit vector)
					Vector2D rotationTransmissionDirection1 = -collisionVector.GetUnitVector();
					Vector2D rotationTransmissionDirection2 = collisionVector.GetUnitVector();

					// �浹���� �ӵ�
					Vector2D collsionDirectionVelocity1 = collisionVector.GetUnitVector() * DotProduct(velocity1, collisionVector.GetUnitVector());
					Vector2D collsionDirectionVelocity2 = collisionVector.GetUnitVector() * DotProduct(velocity2, collisionVector.GetUnitVector());

					// �浹������ ���� �ӵ�
					Vector2D collsionOrthogonalVelocity1 = velocity1 - collsionDirectionVelocity1;
					Vector2D collsionOrthogonalVelocity2 = velocity2 - collsionDirectionVelocity2;

					// �ݹ߰��
					float coefficientOfRestitution = min(rigidbody1->GetRestitutionValue(), rigidbody2->GetRestitutionValue());

					// ����
					float mass1 = rigidbody1->GetMass();
					float mass2 = rigidbody2->GetMass();

					// �浹�� ���� ��� ��ȯ
					Vector2D u1 = collsionDirectionVelocity1;
					Vector2D u2 = collsionDirectionVelocity2;
					float rotationSpeed1 = rotationVelocity1;
					if (rotationSpeed1 < 0.0f) rotationSpeed1 = -rotationSpeed1;
					float rotationSpeed2 = rotationVelocity2;
					if (rotationSpeed2 < 0.0f) rotationSpeed2 = -rotationSpeed2;

					// ȸ���� ����� ó��
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

					// Ư�� ��ǥ�� ����� ó��
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

					// �浹 ���̸�ŭ�� ��ġ�̵����� �ؼ��ϴ� ��찡 �ƴ϶�� ���� �����ӿ��� �浹���� ���� ��츸 �浹�� ������Ʈ���� �ӵ���ȯ
					if (!usingVelocityOverlapped || prevCollisionResult[colliderIdx1].count(colliderIdx2) == 0)
					{
						// ���ӵ��� ����� Ż������� �ݴ��� ��쿡�� �ӵ���ȯ
						if (DotProduct((collsionDirectionVelocity1 - collsionDirectionVelocity2), collisionVector.GetUnitVector()) < 0.0f)
						{
							rigidbody1->SetVelocity(v1 + lockReflection1 + collsionOrthogonalVelocity1);
							rigidbody2->SetVelocity(v2 + lockReflection2 + collsionOrthogonalVelocity2);
							rigidbody1->SetRotationalVelocity(r1 + lockRotationReflection1);
							rigidbody2->SetRotationalVelocity(r2 + lockRotationReflection2);
						}
					}

					
					// ������ ũ��� ���⿡ ���� ����
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

					// ������ ��ŭ �ӵ��� ����
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
					// ������ ��ŭ �̵�
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
		
		// ��ġ�� �̵���Ŵ
		for (int i = 0; i < softbodyList.size(); i++)
		{
			softbodyList[i]->ElementPositionUpdate();
		}

		for (int i = 0; i < rigidbodyList.size(); i++)
		{
			// Ȱ��ȭ�� ���� ������Ʈ���� ����
			if (rigidbodyList[i]->GetGameObject()->GetActive() == true)
			{
				// �߷¿� ������ �޴� rigidbody�� �ӵ� ����
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
			// ��Ȱ��ȭ�� ���ӿ�����Ʈ�� ����
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

		// ����Ʈ���� ����
		// ���μ�
		for (int i = 0; i < pow(2, qtreeDepth - 1) + 1; i++)
		{
			Vector2D left(qtreeAreaLeftTop.x, qtreeAreaLeftTop.y + ((qtreeAreaRightBottom.y - qtreeAreaLeftTop.y) / pow(2, qtreeDepth - 1)) * i);
			Vector2D right(qtreeAreaRightBottom.x, qtreeAreaLeftTop.y + ((qtreeAreaRightBottom.y - qtreeAreaLeftTop.y) / pow(2, qtreeDepth - 1)) * i);

			GraphicsSystem::GetInstance().graphics->DrawLine(transformMatrix * left, transformMatrix * right, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		}

		// ���μ�
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

			// �浹����
			Vector2D collisionPoint = (*iter).second.first;

			// �浹���� (�浹����� ����)
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
		

		// �浹 ����� ����
		for (map<pair<Collider*, Collider*>, pair<Vector2D, Vector2D>>::iterator iter = collisionInfo.begin(); iter != collisionInfo.end(); iter++)
		{
			Collider* collider1 = (*iter).first.first;
			Collider* collider2 = (*iter).first.second;

			// ���� �浹�� ���� ������ �ι� ���� �ʱ� ���� �ڵ�
			if (collider1 < collider2)
			{
				continue;
			}

			int colliderIdx1 = colliderIndexTable[collider1];
			int colliderIdx2 = colliderIndexTable[collider2];

			// �浹����
			Vector2D collisionPoint = (*iter).second.first;

			// �浹���� (�浹����� ����)
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

		// rigidbody�� �ӵ��� ���ӵ�
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

		// ���ڽ�Ű���� �̷�� ù��° �� A�� ã��
		Vector2D minkowskiPointA = minkowskiDifference(collider1, collider2, directionVector);

		// A���� �������� ���� ���ͷ� ���⺤�͸� �����Ͽ� ���ο� �� B�� ã�´�.
		directionVector = -minkowskiPointA.GetUnitVector();
		Vector2D minkowskiPointB = minkowskiDifference(collider1, collider2, directionVector);

		// A�� ���⺤���� �������� �������� B�� �� ����� ��� �浹���� ����
		if (DotProduct(minkowskiPointB, directionVector) < 0.0f)
		{
			return false;
		}

		// AB x AO x AB ���� �������� AB�� �����̸鼭 ���������� ���͸� ���Ͽ� ���ο� ���⺤�ͷ� �Ѵ�.
		directionVector = CrossProduct(CrossProduct((minkowskiPointB - minkowskiPointA), -minkowskiPointA).GetUnitVector(), (minkowskiPointB - minkowskiPointA)).GetUnitVector();

		// A, B, O �� �� ������ �ִ� ���
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

			// ���ο� ���⺤�ͷ� ���ڽ�Ű���� �̷�� ������ ��C�� ã��
			Vector2D minkowskiPointC = minkowskiDifference(collider1, collider2, directionVector);

			// ���⺤���� �������� �������� C�� �� ����� ��� �浹���� ����
			if (DotProduct(minkowskiPointC, directionVector) < 0.0f)
			{
				return false;
			}

			if (minkowskiPointC == minkowskiPointA || minkowskiPointC == minkowskiPointB)
			{
				return false;
			}

			// ���� ������ ������ �� �ִ� ������
			// ABC�� �̷�� �ﰢ���� ����(Rabc),
			// B, C�� ������ BC�� ������ ������ �̷�� �ﰢ�� �ܺο���(Rbc),
			// A, C�� ������ AC�� ������ ������ �̷�� �ﰢ�� �ܺο���(Rac),
			// �̷��� 3���� ������ ������ �� ����

			Vector2D vAB = minkowskiPointB - minkowskiPointA;
			Vector2D vAC = minkowskiPointC - minkowskiPointA;
			Vector2D vBC = minkowskiPointC - minkowskiPointB;

			Vector2D bcDirection = CrossProduct(CrossProduct(-vAC, -vBC).GetUnitVector(), -vBC).GetUnitVector();
			Vector2D acDirection = CrossProduct(CrossProduct(-vBC, -vAC).GetUnitVector(), -vAC).GetUnitVector();

			// �� ���� �� ������ �ִ� ���
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

			// Rbc ������ ������ �����ϴ� ���
			if (originInRbc)
			{
				minkowskiPointA = minkowskiPointC;
				directionVector = bcDirection;
			}
			// Rac ������ ������ �����ϴ� ���
			else if (originInRac)
			{
				minkowskiPointB = minkowskiPointC;
				directionVector = acDirection;
			}
			// Rabc ������ ������ �����ϴ� ���
			else
			{
				// EPA�˰������� ���̿� ���� ã��
				findDirectionAndDepthWithEPA(collider1, collider2, minkowskiPointA, minkowskiPointB, minkowskiPointC);

				return true;
			}
		}
	}

	
	bool PhysicsSystem::collisionDetectionWithGJK(Element* element, Collider* collider1)
	{
		Vector2D directionVector(1.0f, 0.0f);

		// ���ڽ�Ű���� �̷�� ù��° �� A�� ã��
		Vector2D minkowskiPointA = minkowskiDifference(element, collider1, directionVector);

		// A���� �������� ���� ���ͷ� ���⺤�͸� �����Ͽ� ���ο� �� B�� ã�´�.
		directionVector = -minkowskiPointA.GetUnitVector();
		Vector2D minkowskiPointB = minkowskiDifference(element, collider1, directionVector);

		// A�� ���⺤���� �������� �������� B�� �� ����� ��� �浹���� ����
		if (DotProduct(minkowskiPointB, directionVector) < 0.0f)
		{
			return false;
		}

		// AB x AO x AB ���� �������� AB�� �����̸鼭 ���������� ���͸� ���Ͽ� ���ο� ���⺤�ͷ� �Ѵ�.
		directionVector = CrossProduct(CrossProduct((minkowskiPointB - minkowskiPointA), -minkowskiPointA).GetUnitVector(), (minkowskiPointB - minkowskiPointA)).GetUnitVector();

		// A, B, O �� �� ������ �ִ� ���
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

			// ���ο� ���⺤�ͷ� ���ڽ�Ű���� �̷�� ������ ��C�� ã��
			Vector2D minkowskiPointC = minkowskiDifference(element, collider1, directionVector);

			// ���⺤���� �������� �������� C�� �� ����� ��� �浹���� ����
			if (DotProduct(minkowskiPointC, directionVector) < 0.0f)
			{
				return false;
			}

			if (minkowskiPointC == minkowskiPointA || minkowskiPointC == minkowskiPointB)
			{
				return false;
			}

			// ���� ������ ������ �� �ִ� ������
			// ABC�� �̷�� �ﰢ���� ����(Rabc),
			// B, C�� ������ BC�� ������ ������ �̷�� �ﰢ�� �ܺο���(Rbc),
			// A, C�� ������ AC�� ������ ������ �̷�� �ﰢ�� �ܺο���(Rac),
			// �̷��� 3���� ������ ������ �� ����

			Vector2D vAB = minkowskiPointB - minkowskiPointA;
			Vector2D vAC = minkowskiPointC - minkowskiPointA;
			Vector2D vBC = minkowskiPointC - minkowskiPointB;

			Vector2D bcDirection = CrossProduct(CrossProduct(-vAC, -vBC).GetUnitVector(), -vBC).GetUnitVector();
			Vector2D acDirection = CrossProduct(CrossProduct(-vBC, -vAC).GetUnitVector(), -vAC).GetUnitVector();

			// �� ���� �� ������ �ִ� ���
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

			// Rbc ������ ������ �����ϴ� ���
			if (originInRbc)
			{
				minkowskiPointA = minkowskiPointC;
				directionVector = bcDirection;
			}
			// Rac ������ ������ �����ϴ� ���
			else if (originInRac)
			{
				minkowskiPointB = minkowskiPointC;
				directionVector = acDirection;
			}
			// Rabc ������ ������ �����ϴ� ���
			else
			{
				// EPA�˰������� ���̿� ���� ã��
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
		// pair<pair<(-������ ������ �Ÿ�), (�������� ���������� ����)>, pair<(������ �̷�� ����1), (������ �̷�� ����2)>>
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
				// ������ direction���� ������ 2���� supportpoint �߿���
				// ��� collider�� �浹 ���� ���� ã����
				// �� ��ġ���� direction �������� �浹�� �Ͼ ����

				Vector2D penetrationPoint1 = collider1->GetSupportPoint(direction);
				Vector2D penetrationPoint2 = collider2->GetSupportPoint(-direction);

				if (collider2->IsPointInside(penetrationPoint1))
				{
					// penetrationPoint1 ���� �浹�� �Ͼ
					collisionInfo.insert({ { collider1, collider2 }, { penetrationPoint1, -direction } });
					collisionInfo.insert({ { collider2, collider1 }, { penetrationPoint1, direction } });

					collider1->lastCollisionVector = -direction;
					collider2->lastCollisionVector = direction;
				}
				else if (collider1->IsPointInside(penetrationPoint2))
				{
					// penetrationPoint2 ���� �浹�� �Ͼ
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
						// penetrationPointcw1 ���� �浹�� �Ͼ
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointcw1, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointcw1, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else if (collider1->IsPointInside(penetrationPointcw2))
					{
						// penetrationPointcw2 ���� �浹�� �Ͼ
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointcw2, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointcw2, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else if (collider2->IsPointInside(penetrationPointccw1))
					{
						// penetrationPointccw1 ���� �浹�� �Ͼ
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointccw1, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointccw1, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else if (collider1->IsPointInside(penetrationPointccw2))
					{
						// penetrationPointccw2 ���� �浹�� �Ͼ
						collisionInfo.insert({ { collider1, collider2 }, { penetrationPointccw2, -direction } });
						collisionInfo.insert({ { collider2, collider1 }, { penetrationPointccw2, direction } });
						collider1->lastCollisionVector = -direction;
						collider2->lastCollisionVector = direction;
					}
					else
					{
						// �浹�� ����� ���ο� �ִ� Support Point�� �������� �ʴ� ���
						Vector2D orthogonalDirection(direction.y, -direction.x);

						Vector2D lap1 = collider1->GetSupportPoint(direction);
						Vector2D lap2 = collider1->GetSupportPoint(-direction);

						Vector2D lbp1 = collider2->GetSupportPoint(orthogonalDirection);
						Vector2D lbp2 = collider2->GetSupportPoint(-orthogonalDirection);

						if (DetermineLineOverlap(lap1, lap2, lbp1, lbp2))
						{
							// penetrationPoint1 ���� �浹�� �Ͼ
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
							// penetrationPoint2 ���� �浹�� �Ͼ
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
		// pair<pair<(-������ ������ �Ÿ�), (�������� ���������� ����)>, pair<(������ �̷�� ����1), (������ �̷�� ����2)>>
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
				// ������ direction���� ������ 2���� supportpoint �߿���
				// ��� collider�� �浹 ���� ���� ã����
				// �� ��ġ���� direction �������� �浹�� �Ͼ ����

				Vector2D penetrationPoint1 = collider1->GetSupportPoint(direction);
				Vector2D penetrationPoint2 = element->GetWorldPosition() - direction * element->GetRadius();

				if ((penetrationPoint1 - element->GetWorldPosition()).Size() <= element->GetRadius())
				{
					// penetrationPoint1 ���� �浹�� �Ͼ
					softcollisionInfo.insert({ { element, collider1 }, { penetrationPoint1, direction } });
				}
				else if (collider1->IsPointInside(penetrationPoint2))
				{
					// penetrationPoint2 ���� �浹�� �Ͼ
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
						// penetrationPointcw1 ���� �浹�� �Ͼ
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointcw1, direction } });
					}
					else if (collider1->IsPointInside(penetrationPointcw2))
					{
						// penetrationPointcw2 ���� �浹�� �Ͼ
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointcw2, direction } });
					}
					else if ((penetrationPointccw1 - element->GetWorldPosition()).Size() <= element->GetRadius())
					{
						// penetrationPointccw1 ���� �浹�� �Ͼ
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointccw1, direction } });
					}
					else if (collider1->IsPointInside(penetrationPointccw2))
					{
						// penetrationPointccw2 ���� �浹�� �Ͼ
						softcollisionInfo.insert({ { element, collider1 }, { penetrationPointccw2, direction } });
					}
					else
					{
						bool success = false;

						// �浹�� ����� ���ο� �ִ� Support Point�� �������� �ʴ� ���
						Vector2D orthogonalDirection(direction.y, -direction.x);

						Vector2D lap1 = collider1->GetSupportPoint(direction);
						Vector2D lap2 = collider1->GetSupportPoint(-direction);

						Vector2D lbp1 = element->GetWorldPosition() + orthogonalDirection * element->GetRadius();
						Vector2D lbp2 = element->GetWorldPosition() - orthogonalDirection * element->GetRadius();

						if (DetermineLineOverlap(lap1, lap2, lbp1, lbp2))
						{
							// penetrationPoint1 ���� �浹�� �Ͼ
							softcollisionInfo.insert({ { element, collider1 }, { penetrationPoint1, direction } });
							success = true;
						}

						lap1 = collider1->GetSupportPoint(orthogonalDirection);
						lap2 = collider1->GetSupportPoint(-orthogonalDirection);

						lbp1 = element->GetWorldPosition() + direction * element->GetRadius();
						lbp2 = element->GetWorldPosition() - direction * element->GetRadius();

						if (DetermineLineOverlap(lap1, lap2, lbp1, lbp2))
						{
							// penetrationPoint2 ���� �浹�� �Ͼ
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

	// recreateTable������ gameobject�� �����͸� ���Ͽ� ���̺��� �����ϴµ�
	// �����ڿ��� AddCollider, AddRigidbody�� ȣ���ϴ� �ͺ��� ������Ʈ�� gameobject�ּҸ� �־��ִ� ���� ������ ������
	// ��ü������ ������ ���� �ڿ� ���̺��� �����ؾ��Ѵ�.
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