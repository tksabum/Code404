#include "Element.h"
#include "SoftBody.h"

#include "Transform.h"
#include "GameObject.h"
#include "Matrix3x3.h"
#include "Matrix3x1.h"

#include "Scene.h"
#include "Camera.h"
#include "CoreSystem.h"

#include <cmath>


namespace BJEngine
{
	using namespace Math;

	Element::Element(SoftBody* p) :
		mass(2.f), radius(3.f),
		Force(), position(), velocity(), accelerator(),
		restitution(1.f),
		parent(p)
	{

	}

	Element::~Element()
	{

	}


	void Element::ApplyForce(Vector2D force)
	{
		Force += force;
	}

	void Element::RemoveForce()
	{
		Force = {};
	}

	void Element::ForceToVelocity(Math::Vector2D gravity, float deltaTime)
	{
		if (mass == 0.f)
		{
			return;
		}

		this->velocity += (Force / mass + gravity + accelerator) * deltaTime;

		velocity = velocity * std::exp(-(0.1f)* deltaTime);
	}

	Vector2D Element::PositionUpdate(float deltaTime)
	{
		float rotation = parent->GetGameObject()->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformmatrix = Matrix3x3::Rotation(-rotation);

		Vector2D transvelocity = transformmatrix * velocity;

		position = position + transvelocity * deltaTime;

		return velocity * deltaTime;
	}

	void Element::MovePosition(Vector2D pos)
	{
		float rotation = parent->GetGameObject()->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformmatrix = Matrix3x3::Rotation(-rotation);

		Vector2D transpos = transformmatrix * pos;

		this->position = this->position + transpos;

		parent->FixPosition(pos);
	}

	void Element::SetPosition(Vector2D pos)
	{
		this->position = pos;
	}

	void Element::SetVelocity(Math::Vector2D velo)
	{
		this->velocity = velo;
	}

	void Element::SetAccelerator(Math::Vector2D accel)
	{
		accelerator = accel;
	}

	void Element::SetRadius(float r)
	{
		this->radius = r;
	}

	void Element::SetMass(float m)
	{
		this->mass = m;
	}

	void Element::SetRestitutionValue(float e)
	{
		this->restitution = e;
	}

	Vector2D Element::GetPosition()
	{
		return position;
	}

	Vector2D Element::Get2DScreenPosition()
	{
		Transform* transform = parent->GetGameObject()->GetTransform();
		Vector2D wPosition = transform->GetWorldPosition();
		Vector2D wScale = transform->GetWorldScale();
		float wRotate = transform->GetWorldRotation().z;

		Camera* maincamera = parent->GetGameObject()->GetScene()->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		Vector2D cameraPosition = cameraTransform->GetWorldPosition();
		float cameraRotation = cameraTransform->GetWorldRotation().z;
		Vector2D cameraScale = cameraTransform->GetWorldScale();

		Vector2D screenSize = maincamera->GetScreenSize();

		Vector2D currentsize = CoreSystem::GetInstance().GetCurrentWindowSize();
		Vector2D originsize = CoreSystem::GetInstance().GetOriginWindowSize();
		float currentratio = currentsize.x / currentsize.y;
		float originratio = originsize.x / originsize.y;

		Vector2D windowscale(1.0f, 1.0f);

		// x 기준
		if (currentratio < originratio)
		{
			windowscale = windowscale * (currentsize.x / originsize.x);
		}
		else
		{
			windowscale = windowscale * (currentsize.y / originsize.y);
		}

		Matrix3x3 transformMatrix = Matrix3x3::Translation(screenSize / 2.0f) * Matrix3x3::RotationByDegree(cameraRotation) * Matrix3x3::Scale(cameraScale) * Matrix3x3::Scale(windowscale) * Matrix3x3::Translation(-cameraPosition);

		Vector2D worldPosition = GetWorldPosition();

		return transformMatrix * worldPosition;
	}

	Vector2D Element::Get2DScreenScale()
	{
		Camera* maincamera = parent->GetGameObject()->GetScene()->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		Vector2D worldScale = GetWorldScale();
		Vector2D cameraScale = cameraTransform->GetWorldScale();

		// windowsize
		Vector2D currentsize = CoreSystem::GetInstance().GetCurrentWindowSize();
		Vector2D originsize = CoreSystem::GetInstance().GetOriginWindowSize();
		float currentratio = currentsize.x / currentsize.y;
		float originratio = originsize.x / originsize.y;

		Vector2D windowscale(1.0f, 1.0f);

		// x 기준
		if (currentratio < originratio)
		{
			windowscale = windowscale * (currentsize.x / originsize.x);
		}
		else
		{
			windowscale = windowscale * (currentsize.y / originsize.y);
		}

		return Vector2D(worldScale.x * cameraScale.x * windowscale.x, worldScale.y * cameraScale.y * windowscale.y);
	}

	float Element::Get2DScreenRotation()
	{
		Camera* maincamera = parent->GetGameObject()->GetScene()->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		float cameraRotation = cameraTransform->GetWorldRotation().z;

		return GetWorldRotation() + cameraRotation;
	}

	Vector2D Element::GetWorldPosition()
	{
		Transform* transform = parent->GetGameObject()->GetTransform();
		Vector2D wPosition = transform->GetWorldPosition();
		Vector2D wScale = transform->GetWorldScale();
		float wRotate = transform->GetWorldRotation().z;

		return Matrix3x3::Translation(wPosition) * Matrix3x3::Rotation(wRotate) * Matrix3x3::Scale(wScale) * position;
	}

	Vector2D Element::GetWorldScale()
	{
		Transform* transform = parent->GetGameObject()->GetTransform();

		return transform->GetWorldScale();
	}

	float Element::GetWorldRotation()
	{
		Transform* transform = parent->GetGameObject()->GetTransform();

		return transform->GetWorldRotation().z;
	}

	Vector2D Element::GetVelocity()
	{
		return velocity;
	}

	float Element::GetRadius()
	{
		return radius;
	}

	float Element::GetMass()
	{
		return mass;
	}

	float Element::GetRestitutionValue()
	{
		return restitution;
	}

}
