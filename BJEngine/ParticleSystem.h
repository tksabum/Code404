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
		#pragma region �������
		/// <summary>
		/// ��ƼŬ�� �ӷ��� �������� �ƴ���
		/// </summary>
		enum class eParticleState
		{
			Dynamic,			// ����
			Static,				// ����
		};

		/// <summary>
		/// ��ƼŬ ���� ����
		/// </summary>
		enum class eScope
		{
			Point,				// ��
			Line,				// ��
			Circle,				// ��
			Rectangle,			// �簢��
		};

		/// <summary>
		/// ��ƼŬ�� ����
		/// 
		/// eScope�� Point�� ������ Diverges�� �����ϴ�.
		/// </summary>
		enum class eActivity
		{
			Random,				// ������
			Regular,			// ��Ģ����
			Diverges,			// �߻�
			Convergence,		// ����
		};

		/// <summary>
		/// ��ƼŬ�� �Ѹ��� ���
		/// </summary>
		enum class eEmissionMode
		{
			Spray,				// �ð���
			Impact,				// �ѹ���
		};

		/// <summary>
		/// ��ƼŬ �׸��� Ÿ��
		/// </summary>
		enum class eRenderType
		{
			Ellipse,			// ��
			Rectangle,			// �簢��
			Sprite,				// ��������Ʈ
			Line,				// ��
		};
		#pragma endregion �������
	
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

		// ��ƼŬ �ý��� �ʱ�ȭ
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
		
		// ��ƼŬ �⺻������ ����
		PARTICLESYSTEM_API void ParticleReset(Particle*& particle, size_t index = 0);
		// PARTICLESYSTEM_API void ParticleReset(GameObject*& particle, size_t index = 0);

		// ��ƼŬ �ý����� �⺻������ ����
		PARTICLESYSTEM_API void ParticleSystemSet();

		// ��ƼŬ �迭 Ŭ����
		PARTICLESYSTEM_API void ParticlePoolClear();
		
		// ��ġ ������Ʈ
		PARTICLESYSTEM_API virtual void Update() override;
		
		// ��ƼŬ �׸���
		PARTICLESYSTEM_API virtual void Render(GraphicsEngine::Graphics* graphics);
		PARTICLESYSTEM_API void RenderEllipse(GraphicsEngine::Graphics*& graphics);
		PARTICLESYSTEM_API void RenderRectangle(GraphicsEngine::Graphics*& graphics);
		PARTICLESYSTEM_API void RenderSprite(GraphicsEngine::Graphics*& graphics);
		PARTICLESYSTEM_API void RenderLine(GraphicsEngine::Graphics*& graphics);
		#pragma endregion Main Function


	/// Set �Լ�
	public:
		#pragma region Particle Systam
		// ��ƼŬ�� �ִ� ���� ����
		PARTICLESYSTEM_API void SetParticleCount(size_t count);

		// ���� ���θ� ����
		PARTICLESYSTEM_API void SetLooping(bool isLoop);

		// ��ƼŬ�� ����� �������� ���� �ð� ����
		PARTICLESYSTEM_API void SetParticleDelay(float delay);
		PARTICLESYSTEM_API void SetSystemDelay(float delay);

		// ��ƼŬ ������ ����
		PARTICLESYSTEM_API void SetLifeTime(float _min, float _max = 0.f);

		// ��ƼŬ�� �ʱ� �ӵ��� ����
		PARTICLESYSTEM_API void SetMinSpeed(float _min, float _max = 0.f);
	
		// ���� ũ�⸦ ����
		PARTICLESYSTEM_API void SetStartSize(float size = PARTICLE_DEFAULT_SIZE);

		// ��ƼŬ�� �ʱ� ȸ�� ���� ����
		PARTICLESYSTEM_API void SetStartRotation(float angle);

		// ��ƼŬ ���� ���� �� ���� (�簢��)
		PARTICLESYSTEM_API void SetRectScope(float minScopeX, float maxScopeX, float minScopeY, float maxScopeY);

		// ��ƼŬ ���� ���� �� ���� (��)
		PARTICLESYSTEM_API void SetCircleScope(float minScope, float maxScope);

		// ��ƼŬ ���� ����
		PARTICLESYSTEM_API void SetColor(GraphicsEngine::Color color);

		// ��ƼŬ �ʱ� ����
		PARTICLESYSTEM_API void SetDirection(Math::Vector2D dir);

		// Activity�� Regular�� �Ǿ��ִ� ��쿡 �̿�����
		PARTICLESYSTEM_API void SetDirectionRangeRadian(float radian);
		PARTICLESYSTEM_API void SetDirectionRangeDegree(float degree);

		PARTICLESYSTEM_API void SetParticleLineScope(const Math::Vector2D& point);
		PARTICLESYSTEM_API void SetParticleLineScope(std::vector<Math::Vector2D>& _points);
		PARTICLESYSTEM_API void SetGameObjectPointer(BJEngine::GameObject* object);
		PARTICLESYSTEM_API void SetGameObjectPointer(const std::vector<BJEngine::GameObject*>& point);
		PARTICLESYSTEM_API void ClearLineScope();

		// �߷� ����
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


	/// ��� ����
	private:
		std::vector<Particle*> particles;	// ��ƼŬ���� ����
		std::vector<Math::Vector2D> points{};	// ������ ���� ��ġ
		std::vector<BJEngine::GameObject*> objectList{};	// ������ ���� ��ġ

		size_t maxParticle;					// ��ƼŬ �ִ� ����
		bool looping;						// �ݺ��ϴ� ��ƼŬ����?
		
		float systemDelay;					// ��ƼŬ �ý����� ����Ǳ���� ���� �ð�
		float stackTime;
		float particleDelay;				// ��ƼŬ ���� �˾� ����						
		
		float minLifeTime;					// ��ƼŬ �ִ� ����
		float maxLifeTime;					// ��ƼŬ ���� ����
		
		float minSpeed;						// �ּ� �ӷ�
		float maxSpeed{ 10.f };				// �ִ� �ӷ�

		float minSize;						// �ּ� ũ��
		float maxSize;						// �ִ� ũ��

		float minScopeX{};					// �⺻�� 0.
		float minScopeY{};					// �⺻�� 0.
		float maxScopeX;					// ��ƼŬ�� �����Ǵ� X���� �ִ�
		float maxScopeY;					// ��ƼŬ�� �����Ǵ� Y���� �ִ�

		float startRotate;					// �ʱ� ȸ�� ��
		float angulerVelocity;				// ���ӵ�

		float scaleX{1.f};
		float scaleY{1.f};
		
		GraphicsEngine::Color startColor;	// �ʱ� ����
		
		Math::Vector2D position;			// �ʱ� ��ġ
		Math::Vector2D direction{0.f, 1.f};			// �ʱ� ����
		float range{};
		Math::Vector2D gravity;				// �߷� ��

		bool usePoint = true;

	private:
		RandomNumberGenerator rn;
	};
}




/*
�ٴڿ� Ƣ��� ������� �����Ϸ��� �� ��.

�ϰ������� �ʱ�ȭ�� �� ��
��ƼŬ�� �� ���� �������� ��ƼŬ ���� Ÿ���� �ʿ� ����.


*/



/* 
*	>> ����Ƽ ��ƼŬ �ý����� �Ӽ� << 
* 
* Duration			: float ��ƼŬ ��� �ð�
* Looping			: bool �ݺ� ���� (Duration �ݺ�)
* Prewarm			: bool ���� �Ⱓ
* 
* Start Delay		: float �ý����� Ȱ��ȭ �� �� ������ �����ϱ� ������ ���� �ð�
* Start Lifetime	: float ��ƼŬ ���� �ֱ�
* Start Speed		: float ��ƼŬ �ʱ� �ӵ�
* 
* Start Size		: size_t ��ƼŬ ���� ũ��
* 
* Start Rotation	: float ���� ȸ�� ��
* Flip Rotation		: float 
* 
* Start Color		: color ��ƼŬ ���� �� ���� ����
* 
* Gravity Modifier	: float ��ƼŬ�� �߷� ��
* 
* Deleta Time		: string?
*			- Scaled	: 
*			- Unscaled	: 
* 
* Scaling Mode		: string?
*			- Hierarchy	: �θ� ������Ʈ�� Scale ���� ������ ����
*			- Local		: ��ƼŬ �ý��� ���� Scale ���� ����
*			- Shape		: ��ƼŬ�� ���� �����ǿ� �������� ����� / ũ�⿡�� ���� X
* 
* Play on Awake
* 
* Emitter Velocity	: string?
*			- Transform	: 
*			- RigidBody	: 
* 
* Max Particle		: size_t ��ƼŬ ��
* 
* Auto Random Seed	: bool ��ƼŬ ����� ����
* 
* Stop Action		: string? ��ƼŬ �ý����� ������ ���� �ൿ
*			- Disable	:
*			- Destroy	:
*			- Callback	:
*			- None		:
* 
* Culling Mode		: string? ��ũ���� ����� ���� �ൿ
*			- Pause				 :
*			- Pause and Catch-up :
*			- Always Simulate	 : ī�޶� �ȿ� ���� �ʾƵ� �����
*			- Automatic			 : �ݺ� ������ ��쿡�� Pause / �ƴϸ� Always Simulate
* 
* Ring Buffer Mode	: string? ��ƼŬ�� Max Particle ���� ������ �� ���� Ȱ��ȭ ��
*			- Disable				: ������ ���� ��ƼŬ�� ������
*			- Pause Until Replaced	: ������ ���� ��ƼŬ�� ��Ȱ�� ��.
*			- Loop Until Replaced	: ������ ���� ��ƼŬ�� ��Ȱ�� ��.
*			(�̰� ���� ������..?)
*/