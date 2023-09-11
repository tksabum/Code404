#include "Particle.h"

/// 파티클 시스템의 스크린 좌표를 얻기 위한 무언가
#include "ParticleSystem.h"
#include "GameObject.h"
#include "Transform.h"

#include "CoreSystem.h"
// #include "Vector2D.h"
// #include "Vector3D.h"
#include "Scene.h"
#include "Camera.h"
#include "Matrix3x3.h"
#include "Matrix3x1.h"

namespace BJEngine
{
	using namespace Math;

	Particle::Particle(ParticleSystem* system) :
		parent(system),
		position(), velocity(), acceleration(),
		angle(), angularVelocity(), age(), life(), particleDelay(), stackTime(),
		particleColor(GraphicsEngine::Color::Black)
	{
		scale = Vector3D{ 1.f, 1.f, 1.f };
	}

	Particle::~Particle()
	{

	}

	void Particle::SetAcceleration(Math::Vector2D accel)
	{
		this->acceleration = accel;
	}

	void Particle::SetVelocity(Math::Vector2D velo)
	{
		this->velocity = velo;
	}

	void Particle::VelocityUpdate(float dt)
	{
		if (acceleration.y > 600.f)
		{
			velocity.y = 200.f;
			acceleration.y = 0.f;
		}
		velocity = velocity + acceleration * dt;
	}

	void Particle::Update(float dt)
	{
		if (particleDelay != 0.f)
		{
			// 델타타임을 쌓음
			stackTime += dt;
			// 쌓은 델타타임과 딜레이를 비교함
			if (stackTime < particleDelay)
			{
				return;
			}
		}

		// 파티클의 위치, 속도를 업데이트함.
		VelocityUpdate(dt);
		position = position + velocity * dt;

		// 파티클의 각도를 업데이트함.
		angle += angularVelocity * dt;

		// 파티클의 나이를 업데이트함.
		age += dt;

		if (age > life)
		{
			disable = true;
		}
	}

	Math::Vector2D Particle::GetPosition()
	{
		// Math::Vector2D LOCAL = parent->GetGameObject()->GetTransform()->GetLocalPosition();
		// Math::Vector2D WORLD = parent->GetGameObject()->GetTransform()->GetWorldPosition();
		// Math::Vector2D SCREEN = parent->GetGameObject()->GetTransform()->Get2DScreenPosition();
		// 
		// Math::Vector2D temp = LOCAL + position;

		return position;
	}

	Math::Vector2D Particle::GetWorldPosition()
	{
		GameObject* parentobject = parent->GetGameObject()->GetParent();
		Vector2D objectworld = parent->GetGameObject()->GetTransform()->GetWorldPosition();
		Vector2D objectlocal = parent->GetGameObject()->GetTransform()->GetLocalPosition();

		if (parentobject == nullptr)
		{
			return objectworld + position;
		}

		Vector3D rootPosition = parentobject->GetTransform()->GetWorldPosition();

		return rootPosition + objectlocal + (position);
	}

	Math::Vector2D Particle::GetWorldScale()
	{
		GameObject* parentobject = parent->GetGameObject()->GetParent();

		if (parentobject == nullptr)
		{
			return scale;
		}

		Vector3D rootScale = parentobject->GetTransform()->GetWorldScale();

		return Vector3D(rootScale.x * scale.x, rootScale.y * scale.y, rootScale.z * scale.z);
	}

	Math::Vector2D Particle::Get2DScreenScale()
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

	Math::Vector2D Particle::Get2DScreenPosition()
	{
		
		Camera* maincamera = parent->GetGameObject()->GetScene()->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		Vector2D cameraPosition = cameraTransform->GetWorldPosition();
		float cameraRotation = cameraTransform->GetWorldRotation().z;
		Vector2D cameraScale = cameraTransform->GetWorldScale();

		Vector2D screenSize = maincamera->GetScreenSize();

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

		Matrix3x3 transformMatrix = Matrix3x3::Translation(screenSize / 2.0f) * Matrix3x3::RotationByDegree(cameraRotation) * Matrix3x3::Scale(cameraScale) * Matrix3x3::Scale(windowscale) * Matrix3x3::Translation(-cameraPosition);

		Vector2D worldPosition = GetWorldPosition();

		return transformMatrix * worldPosition;
	}
}
