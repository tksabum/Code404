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
	/// 파티클을 만들기 시작함
	/// 
	/// 2023.02.04	정기훈
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

		Math::Vector2D position;				// 입자의 위치
		Math::Vector3D scale;				// 입자의 위치
		Math::Vector2D velocity;				// 입자의 속력, 뱡항도 포함됨
		Math::Vector2D acceleration;			// 입자 가속도 / 중력도 포함됨

		GraphicsEngine::Color particleColor;	// 입자의 색깔

		float angle;							 // 입자의 각도
		float angularVelocity;					 // 입자의 각속도

		float age;								 // 입자 현재 나이
		float life;								 // 입자 현재 나이

		float particleDelay;
		float stackTime;

		bool disable{ false };
	};
}