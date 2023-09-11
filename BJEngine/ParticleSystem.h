#pragma once

#ifdef PARTICLESYSTEM_EXPORTS
#define PARTICLESYSTEM_API __declspec(dllexport)
#else
#define PARTICLESYSTEM_API __declspec(dllimport)
#endif

#include <vector>
#include "Renderer.h"
#include "Vector2D.h"
#include "Color.h"

#include "RandomNumberGenerator.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Sprite;
		class Graphics;
	}

	class Particle;
	class CoreSystem;
	class Animation;
	class AnimationSystem;
	class SpriteRenderer;
	class GameObject;

	constexpr float PARTICLE_DEFAULT_SIZE = 10.f;
	constexpr float PARTICLE_DEFAULT_GRAVITY = 0.f;
	constexpr float PARTICLE_PI = 3.141592f;

	class ParticleSystem : public Renderer
	{
	public:
		#pragma region 구성요소
		/// <summary>
		/// 파티클이 속력을 가지는지 아닌지
		/// </summary>
		enum class eParticleState
		{
			Dynamic,			// 동적
			Static,				// 정적
		};

		/// <summary>
		/// 파티클 생성 범위
		/// </summary>
		enum class eScope
		{
			Point,				// 점
			Line,				// 선
			Circle,				// 원
			Rectangle,			// 사각형
		};

		/// <summary>
		/// 파티클의 방향
		/// 
		/// eScope가 Point면 무조건 Diverges만 가능하다.
		/// </summary>
		enum class eActivity
		{
			Random,				// 무작위
			Regular,			// 규칙적인
			Diverges,			// 발산
			Convergence,		// 수렴
		};

		/// <summary>
		/// 파티클을 뿌리는 방식
		/// </summary>
		enum class eEmissionMode
		{
			Spray,				// 시간차
			Impact,				// 한번에
		};

		/// <summary>
		/// 파티클 그리기 타입
		/// </summary>
		enum class eRenderType
		{
			Ellipse,			// 원
			Rectangle,			// 사각형
			Sprite,				// 스프라이트
			Line,				// 선
		};
		#pragma endregion 구성요소
	
	private:
		eParticleState state;
		eScope scope;
		eActivity activity;
		eEmissionMode emission;
		eRenderType renderType;

	public:
		PARTICLESYSTEM_API ParticleSystem();
		PARTICLESYSTEM_API virtual ~ParticleSystem();

		#pragma region Particle Systam
		PARTICLESYSTEM_API void SystemSetting(
			eParticleState _state,
			eScope _scope,
			eActivity _activity,
			eEmissionMode _emission,
			eRenderType _type);

		// 파티클 시스템 초기화
		PARTICLESYSTEM_API void Initialize(
			size_t count,
			Math::Vector2D pos,
			bool isloop,
			float sDelay,
			float pDelay,
			float minLife,
			float minSpeed,
			float minSize,
			GraphicsEngine::Color 
			= GraphicsEngine::Color::Black);
		
		// 파티클 기본값으로 리셋
		PARTICLESYSTEM_API void ParticleReset(Particle*& particle, size_t index = 0);
		// PARTICLESYSTEM_API void ParticleReset(GameObject*& particle, size_t index = 0);

		// 파티클 시스템을 기본값으로 리셋
		PARTICLESYSTEM_API void ParticleSystemSet();

		// 파티클 배열 클리어
		PARTICLESYSTEM_API void ParticlePoolClear();
		
		// 위치 업데이트
		PARTICLESYSTEM_API virtual void Update() override;
		
		// 파티클 그리기
		PARTICLESYSTEM_API virtual void Render(GraphicsEngine::Graphics* graphics);
		PARTICLESYSTEM_API void RenderEllipse(GraphicsEngine::Graphics*& graphics);
		PARTICLESYSTEM_API void RenderRectangle(GraphicsEngine::Graphics*& graphics);
		PARTICLESYSTEM_API void RenderSprite(GraphicsEngine::Graphics*& graphics);
		PARTICLESYSTEM_API void RenderLine(GraphicsEngine::Graphics*& graphics);
		#pragma endregion Main Function


	/// Set 함수
	public:
		#pragma region Particle Systam
		// 파티클의 최대 갯수 설정
		PARTICLESYSTEM_API void SetParticleCount(size_t count);

		// 루프 여부를 설정
		PARTICLESYSTEM_API void SetLooping(bool isLoop);

		// 파티클이 실행될 때까지의 지연 시간 설정
		PARTICLESYSTEM_API void SetParticleDelay(float delay);
		PARTICLESYSTEM_API void SetSystemDelay(float delay);

		// 파티클 수명을 설정
		PARTICLESYSTEM_API void SetLifeTime(float _min, float _max = 0.f);

		// 파티클의 초기 속도를 설정
		PARTICLESYSTEM_API void SetMinSpeed(float _min, float _max = 0.f);
	
		// 시작 크기를 설정
		PARTICLESYSTEM_API void SetStartSize(float size = PARTICLE_DEFAULT_SIZE);

		// 파티클의 초기 회전 값을 설정
		PARTICLESYSTEM_API void SetStartRotation(float angle);

		// 파티클 생성 범위 상세 설정 (사각형)
		PARTICLESYSTEM_API void SetRectScope(float minScopeX, float maxScopeX, float minScopeY, float maxScopeY);

		// 파티클 생성 범위 상세 설정 (원)
		PARTICLESYSTEM_API void SetCircleScope(float minScope, float maxScope);

		// 파티클 색상 설정
		PARTICLESYSTEM_API void SetColor(GraphicsEngine::Color color);

		// 파티클 초기 방향
		PARTICLESYSTEM_API void SetDirection(Math::Vector2D dir);

		// Activity가 Regular로 되어있는 경우에 이용하자
		PARTICLESYSTEM_API void SetDirectionRangeRadian(float radian);
		PARTICLESYSTEM_API void SetDirectionRangeDegree(float degree);

		PARTICLESYSTEM_API void SetParticleLineScope(const Math::Vector2D& point);
		PARTICLESYSTEM_API void SetParticleLineScope(std::vector<Math::Vector2D>& _points);
		PARTICLESYSTEM_API void SetGameObjectPointer(BJEngine::GameObject* object);
		PARTICLESYSTEM_API void SetGameObjectPointer(const std::vector<BJEngine::GameObject*>& point);
		PARTICLESYSTEM_API void ClearLineScope();

		// 중력 설정
		PARTICLESYSTEM_API void SetGravity(float y = PARTICLE_DEFAULT_GRAVITY);

		PARTICLESYSTEM_API void SetScale(float x, float y);

		PARTICLESYSTEM_API void SetUsePoint(bool isUse);
		#pragma endregion Set Function


		#pragma region Particle System
	public:
		PARTICLESYSTEM_API void SetSpriteName(std::wstring spritename);
		PARTICLESYSTEM_API GraphicsEngine::Sprite* GetSprite();
		PARTICLESYSTEM_API void SetImageNumber(int imagenumber);
		PARTICLESYSTEM_API int GetImageNumber();
		PARTICLESYSTEM_API bool GetFlipX();
		PARTICLESYSTEM_API void SetFlipX(bool flipx);
		PARTICLESYSTEM_API bool GetFlipY();
		PARTICLESYSTEM_API void SetFlipY(bool flipy);

	private:
		std::wstring spriteName{L""};
		int imageNumber{0};
		bool flipX{false};
		bool flipY{false};
		#pragma endregion Sprite Setting

	private:
		#pragma region Particle System
		Math::Vector2D GetPosition();
		float GetParticleDelay(size_t index = 1);
		float GetLifeTime();
		Math::Vector2D GetVelocity(Math::Vector2D pos = (0.f, 0.f));
		Math::Vector2D GetAcceleration();
		#pragma endregion Get Function

	private:


	/// 멤버 변수
	private:
		std::vector<Particle*> particles;	// 파티클들의 모음
		std::vector<Math::Vector2D> points{};	// 생성될 점의 위치
		std::vector<BJEngine::GameObject*> objectList{};	// 생성될 점의 위치

		size_t maxParticle;					// 파티클 최대 갯수
		bool looping;						// 반복하는 파티클인지?
		
		float systemDelay;					// 파티클 시스템이 실행되기까지 지연 시간
		float stackTime;
		float particleDelay;				// 파티클 간의 팝업 간격						
		
		float minLifeTime;					// 파티클 최단 수명
		float maxLifeTime;					// 파티클 최장 수명
		
		float minSpeed;						// 최소 속력
		float maxSpeed{ 10.f };				// 최대 속력

		float minSize;						// 최소 크기
		float maxSize;						// 최대 크기

		float minScopeX{};					// 기본값 0.
		float minScopeY{};					// 기본값 0.
		float maxScopeX;					// 파티클이 생성되는 X범위 최댓값
		float maxScopeY;					// 파티클이 생성되는 Y범위 최댓값

		float startRotate;					// 초기 회전 값
		float angulerVelocity;				// 각속도

		float scaleX{1.f};
		float scaleY{1.f};
		
		GraphicsEngine::Color startColor;	// 초기 색상
		
		Math::Vector2D position;			// 초기 위치
		Math::Vector2D direction{0.f, 1.f};			// 초기 방향
		float range{};
		Math::Vector2D gravity;				// 중력 값

		bool usePoint = true;

	private:
		RandomNumberGenerator rn;
	};
}




/*
바닥에 튀기는 빗방울을 구현하려면 할 것.

일괄적으로 초기화를 한 뒤
파티클이 한 번에 터질려면 파티클 간의 타임이 필요 없음.


*/



/* 
*	>> 유니티 파티클 시스템의 속성 << 
* 
* Duration			: float 파티클 재생 시간
* Looping			: bool 반복 설정 (Duration 반복)
* Prewarm			: bool 예열 기간
* 
* Start Delay		: float 시스템이 활성화 된 후 방출을 시작하기 까지의 지연 시간
* Start Lifetime	: float 파티클 생명 주기
* Start Speed		: float 파티클 초기 속도
* 
* Start Size		: size_t 파티클 시작 크기
* 
* Start Rotation	: float 시작 회전 값
* Flip Rotation		: float 
* 
* Start Color		: color 파티클 시작 시 색상 설정
* 
* Gravity Modifier	: float 파티클의 중력 값
* 
* Deleta Time		: string?
*			- Scaled	: 
*			- Unscaled	: 
* 
* Scaling Mode		: string?
*			- Hierarchy	: 부모 오브젝트의 Scale 값에 영향을 받음
*			- Local		: 파티클 시스템 내의 Scale 값만 적용
*			- Shape		: 파티클의 시작 포지션에 스케일이 적용됨 / 크기에는 영향 X
* 
* Play on Awake
* 
* Emitter Velocity	: string?
*			- Transform	: 
*			- RigidBody	: 
* 
* Max Particle		: size_t 파티클 수
* 
* Auto Random Seed	: bool 파티클 모양이 랜덤
* 
* Stop Action		: string? 파티클 시스템이 정지한 후의 행동
*			- Disable	:
*			- Destroy	:
*			- Callback	:
*			- None		:
* 
* Culling Mode		: string? 스크린을 벗어나면 취할 행동
*			- Pause				 :
*			- Pause and Catch-up :
*			- Always Simulate	 : 카메라 안에 있지 않아도 실행됨
*			- Automatic			 : 반복 설정한 경우에는 Pause / 아니면 Always Simulate
* 
* Ring Buffer Mode	: string? 파티클이 Max Particle 수에 도달할 때 까지 활성화 됨
*			- Disable				: 수명을 다한 파티클을 제거함
*			- Pause Until Replaced	: 수명을 다한 파티클을 재활용 함.
*			- Loop Until Replaced	: 수명을 다한 파티클을 재활용 함.
*			(이게 무슨 차이임..?)
*/