
#include "SoftBody.h"
#include "Element.h"
#include "Spring.h"
#include "BJMath.h"

/// delta time 얻기위한 헤더 참조
#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

/// 그냥받아옴
#include "GameObject.h"
#include "Transform.h"
#include "Matrix3x3.h"
#include "Matrix3x1.h"

/// Physics 시스템에 있는 softbody 리스트에 넣기위한 헤더 참조
#include "PhysicsSystem.h"

#include "Input.h"

namespace BJEngine
{
	using namespace Math;

	SoftBody::SoftBody()
	{
		PhysicsSystem::GetInstance().AddSoftBody2D(this);
	}

	SoftBody::~SoftBody()
	{
		Clear();
		PhysicsSystem::GetInstance().RemoveSoftBody2D(this);
	}

	void SoftBody::Update()
	{
		Vector2D move = {0.f, 0.f};

		if (Input::GetInstance().PressedKey(VK_LEFT))
		{
			move.x = -2000.f;
		}
		if (Input::GetInstance().PressedKey(VK_RIGHT))
		{
			move.x = 2000.f;
		}
		if (Input::GetInstance().PressedKey(VK_UP))
		{
			move.y = -2000.f;
		}
		if (Input::GetInstance().PressedKey(VK_DOWN))
		{
			move.y = 2000.f;
		}
		if (Input::GetInstance().ReleasedKeyNow(VK_LEFT) ||
			Input::GetInstance().ReleasedKeyNow(VK_RIGHT))
		{
			move.x = 0.f;
		}
		if (Input::GetInstance().ReleasedKeyNow(VK_UP) ||
			Input::GetInstance().ReleasedKeyNow(VK_DOWN))
		{
			move.y = 0.f;
		}

		for (auto& e : elementList)
		{
			e->SetAccelerator(move);
		}
	}

	void SoftBody::SetSoftBall(size_t vertex, size_t spring, float radius, float springconstant, float damping)
	{
		if (vertex < 3)
		{
			return;
		}

		float theta = (2 * Math::PI) / vertex;

		if (vertex % 2 == 0)
		{
			for (size_t i = 0; i < vertex; i++)
			{
				Element* element = new Element(this);

				element->SetPosition(Matrix3x3::Rotation(-theta / 2 + theta * i) * Vector2D{ 0.f, -1.f * radius });
				elementList.push_back(element);
			}
		}
		else
		{
			for (size_t i = 0; i < vertex; i++)
			{
				Element* element = new Element(this);

				element->SetPosition(Matrix3x3::Rotation(theta * i) * Vector2D { 0.f, -1.f * radius });
				elementList.push_back(element);
			}
		}

		for (int i = 0; i < elementList.size(); i++)
		{
			for (int j = 1; ((vertex / j) != 0) && j < spring; j++)
			{
				/// 엘리멘트를 순서대로 연결해줌
				Spring* spring = new Spring(elementList[i], elementList[(i + j) % vertex]);
				spring->SetProperties(2 * (radius * cos((PI / 2) - (theta * 0.5f * j))), springconstant, damping);
				springList.push_back(spring);
			}
		}
	}

	void SoftBody::SetSoftBlock(float gap, int column, int row, float springconstant, float damping)
	{
		if (column < 2)
		{
			column = 2;
		}
		if (row < 2)
		{
			row = 2;
		}

		for (size_t i = 0; i < row; i++)
		{
			for (size_t j = 0; j < column; j++)
			{
				Element* element = new Element(this);

				Vector2D pos{ j * gap, i * gap };

				element->SetPosition(pos);
				elementList.push_back(element);
			}
		}

		Vector2D temp{};
		for (auto& e : elementList)
		{
			temp += e->GetPosition();
		}
		temp = temp / elementList.size();
		for (auto& e : elementList)
		{
			e->SetPosition(e->GetPosition() - temp);
		}

		// [row][column], [행][열]
		for (int i = 0; i < elementList.size(); i++)
		{
			// i와 i + 1. 오른쪽 방향으로 스프링으로 연결해준다.
			if (((i + 1) < elementList.size()) && ((i + 1) % column != 0))
			{
				Spring* spring = new Spring(elementList[i], elementList[i + 1]);
				spring->SetProperties(gap, springconstant, damping);
				springList.push_back(spring);
			}
			// i와 i + column. 아래쪽 방향으로 스프링을 연결해준다.
			if (i + column < elementList.size())
			{
				Spring* spring = new Spring(elementList[i], elementList[i + column]);
				spring->SetProperties(gap, springconstant, damping);
				springList.push_back(spring);
			}
			// i와 i + column + 1. 오른쪽 아래 대각선 방향으로 연결해준다.
			if ((i + column + 1) < elementList.size() && ((i + 1) % column != 0))
			{
				Spring* spring = new Spring(elementList[i], elementList[i + column + 1]);
				spring->SetProperties(gap * 1.4142135623f, springconstant, damping);
				springList.push_back(spring);
			}
			// i와 i - column + 1. 오른쪽 위 대각산 방향으로 연결해준다.
			if (((i - column + 1) > 0) && ((i + 1) % column != 0))
			{
				Spring* spring = new Spring(elementList[i], elementList[1 - column + i]);
				spring->SetProperties(gap * 1.4142135623f, springconstant, damping);
				springList.push_back(spring);
			}


			// 좀 더 촘촘하게 스프링을 연결하고 싶다면
			// i + column + 2
			// i + column * 2 + 1
			// i - column * 2 + 1
			
			if ((i + column + 2) < elementList.size() && (i + 2) % column != 0 && (i + 1) % column != 0)
			{
				Spring* spring = new Spring(elementList[i], elementList[i + column + 2]);
				spring->SetProperties(gap * 2.236067f, springconstant, damping);
				springList.push_back(spring);
			}
			if ((i - column + 2) > 0 && (i + 2) % column != 0 && (i + 1) % column != 0)
			{
				Spring* spring = new Spring(elementList[i], elementList[i - column + 2]);
				spring->SetProperties(gap * 2.236067f, springconstant, damping);
				springList.push_back(spring);
			}
			if ((i + 2 * column + 1) < elementList.size() && (i + 1) % column != 0)
			{
				Spring* spring = new Spring(elementList[i], elementList[i + 2 * column + 1]);
				spring->SetProperties(gap * 2.236067f, springconstant, damping);
				springList.push_back(spring);
			}
			if (((i - 2 * column + 1) > 0) && (i + 1) % column != 0)
			{
				Spring* spring = new Spring(elementList[i], elementList[i - 2 * column + 1]);
				spring->SetProperties(gap * 2.236067f, springconstant, damping);
				springList.push_back(spring);
			}
		
		}
	}

	void SoftBody::Clear()
	{
		for (auto& e : elementList)
		{
			delete e;
			e = nullptr;
		}
		for (auto& e : springList)
		{
			delete e;
			e = nullptr;
		}
		elementList.clear();
		springList.clear();
	}

	void SoftBody::SpringUpdate()
	{
		float dt = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetPhysicsDeltaTime();

		for (auto& e : springList)
		{
			e->PhysicsUpdate(dt);
		}
	}

	void SoftBody::ElementPositionUpdate()
	{
		float dt = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetPhysicsDeltaTime();

		Vector2D temp{};

		for (auto& e : elementList)
		{
			temp += e->PositionUpdate(dt);
		}

		temp = temp / elementList.size();

		Transform* transform = GetGameObject()->GetTransform();

		transform->SetLocalPosition(transform->GetLocalPosition() + temp);


		float rotation = GetGameObject()->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformmatrix = Matrix3x3::Rotation(-rotation);

		Vector2D transtemp = transformmatrix * temp;

		for (auto& e : elementList)
		{
			e->SetPosition(e->GetPosition() - transtemp);
		}
	}

	void SoftBody::FixPosition(Vector2D pos)
	{
		Vector2D temp{};
		temp = pos / elementList.size();

		Transform* transform = GetGameObject()->GetTransform();

		transform->SetLocalPosition(transform->GetLocalPosition() + temp);

		float rotation = GetGameObject()->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformmatrix = Matrix3x3::Rotation(-rotation);

		Vector2D transtemp = transformmatrix * temp;

		for (auto& e : elementList)
		{
			e->SetPosition(e->GetPosition() - transtemp);
		}
	}
}
