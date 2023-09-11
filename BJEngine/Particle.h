#pragma once

#ifdef PARTICLE_EXPORTS
#define PARTICLE_API __declspec(dllexport)
#else
#define PARTICLE_API __declspec(dllimport)
#endif

#include "Vector2D.h"
#include "Vector3D.h"
#include "Color.h"

namespace BJEngine
{
	/// <summary>
	/// ��ƼŬ�� ����� ������
	/// 
	/// 2023.02.04	������
	/// </summary>
	
	class ParticleSystem;
	
	class Particle
	{
	public:
		PARTICLE_API Particle(ParticleSystem* system);
		PARTICLE_API ~Particle();

		PARTICLE_API void SetAcceleration(Math::Vector2D accel);
		PARTICLE_API void SetVelocity(Math::Vector2D velo);

		PARTICLE_API void VelocityUpdate(float dt);
		PARTICLE_API void Update(float dt);

		Math::Vector2D GetPosition();
		Math::Vector2D GetWorldPosition();
		Math::Vector2D GetWorldScale();
		Math::Vector2D Get2DScreenScale();
		Math::Vector2D Get2DScreenPosition();

	public:
		ParticleSystem* parent;

		Math::Vector2D position;				// ������ ��ġ
		Math::Vector3D scale;				// ������ ��ġ
		Math::Vector2D velocity;				// ������ �ӷ�, ���׵� ���Ե�
		Math::Vector2D acceleration;			// ���� ���ӵ� / �߷µ� ���Ե�

		GraphicsEngine::Color particleColor;	// ������ ����

		float angle;							 // ������ ����
		float angularVelocity;					 // ������ ���ӵ�

		float age;								 // ���� ���� ����
		float life;								 // ���� ���� ����

		float particleDelay;
		float stackTime;

		bool disable{ false };
	};
}