#include "ParticleSystem.h"
#include "Particle.h"

#include "Graphics.h"

/// 델타타임
#include "Scene.h"
#include "CoreSystem.h"
#include "SceneManager.h"
#include "TimeController.h"

///
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody2D.h"

/// 그림 그리기 위해 만든 것
#include "GraphicsResourceManager.h"
#include "AnimationResourceManager.h"
#include "AnimationSystem.h"
#include "Animation.h"

#include <cmath>


namespace BJEngine
{
	ParticleSystem::ParticleSystem() :
		particles(),
		maxParticle(),
		looping(),
		systemDelay(), particleDelay(),
		stackTime(),
		minLifeTime(), maxLifeTime(),
		minSpeed(),
		minSize(), maxSize(),
		minScopeX(), minScopeY(),
		maxScopeX(), maxScopeY(),
		startRotate(), angulerVelocity(),
		gravity({0.f, 0.f}),
		startColor(GraphicsEngine::Color::Black),
		state(eParticleState::Dynamic),
		scope(eScope::Circle),
		activity(eActivity::Random),
		emission(eEmissionMode::Spray),
		renderType(eRenderType::Ellipse)
	{

	}

	ParticleSystem::~ParticleSystem()
	{
		ParticlePoolClear();
	}

	/// <summary>
	/// 시스템 설정
	/// </summary>
	/// <param name="_active"></param>
	/// <param name="_scope"></param>
	/// <param name="_dir"></param>
	/// <param name="_type"></param>
	void ParticleSystem::SystemSetting(
		eParticleState _state,
		eScope _scope, 
		eActivity _activity,
		eEmissionMode _emission,
		eRenderType _type)
	{
		this->state = _state;
		this->scope		 = _scope;
		this->activity = _activity;
		this->emission	 = _emission;
		this->renderType = _type;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="count"></param>
	/// <param name="pos"></param>
	/// <param name="isloop"></param>
	/// <param name="sDelay"></param>
	/// <param name="pDelay"></param>
	/// <param name="minLife"></param>
	/// <param name="_minSpeed"></param>
	/// <param name="_minSize"></param>
	/// <param name="color"></param>
	void ParticleSystem::Initialize(
		size_t count,
		Math::Vector2D pos,
		bool isloop,
		float sDelay,
		float pDelay,
		float minLife,
		float _minSpeed,
		float _minSize,
		GraphicsEngine::Color color)
	{
		this->maxParticle = count;
		this->position = pos;
		this->looping = isloop;
		this->systemDelay = sDelay;
		this->particleDelay = pDelay;
		SetLifeTime(minLife);
		SetMinSpeed(_minSpeed);
		this->minSize = _minSize;
		this->startColor = color;
	}

	/// <summary>
	/// 파티클을 시스템의 기본값으로 초기화
	/// </summary>
	void ParticleSystem::ParticleReset(Particle*& particle, size_t index)
	{
		// 파티클 초기 위치 설정
		particle->position = GetPosition();

		// 파티클 초기 속력 설정
		particle->velocity = GetVelocity(particle->position);
		
		particle->acceleration = GetAcceleration();

		// 파티클 회전 각도와 각속도 설정
		// particle->angle = rn.GetRandomFloat(0.f, 3.141592f * 2.f);
		particle->angle = 0.f;
		particle->angularVelocity = angulerVelocity;

		// 파티클 초기 색상 설정
		particle->particleColor = startColor;

		// 파티클 나이 초기화
		particle->age = 0.f;

		//파티클 수명 설정
		particle->life = GetLifeTime();

		particle->particleDelay = GetParticleDelay(index);
		particle->stackTime = 0.f;

		particle->disable = false;
	}

	/// <summary>
	/// 파티클 시스템을 초기화
	/// </summary>
	void ParticleSystem::ParticleSystemSet()
	{
		// 파티클 배열의 사이즈가 0보다 크면 클리어
		if (particles.size() > 0)
		{
			ParticlePoolClear();
		}
		// 쌓은 델타타임 초기화
		stackTime = 0.f;

		// 초기 속력 ~ 10.f 에서 랜덤 값으로 날라가지만
		// 초기 속력 값이 클 경우 최대 속력을 키움
		if (minSpeed > maxSpeed)
		{
			maxSpeed = minSpeed * 1.4f;
		}

		for (size_t i = 0; i < maxParticle; ++i)
		{
			Particle* particle = new Particle(this);
			ParticleReset(particle, i);

			particles.push_back(particle);
		}
	}

	void ParticleSystem::ParticlePoolClear()
	{
		for (auto& e : particles)
		{
			delete e;
		}
		particles.clear();
	}

	void ParticleSystem::Update()
	{
		float dt = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();

		// 쌓인 델타 타임과 딜레이를 비교함.
		if ((systemDelay != 0.f) && (stackTime < systemDelay))
		{
			stackTime += dt;
			return;
		}

		for (auto& e : particles)
		{
			// 파티클 비활성화 상태면 넘김
			if (e->disable == true)
			{
				// 루프 상태면 비활성화된 파티클을 초기화한다.
				if (looping)
				{
					ParticleReset(e, 1);
				}
				else 
					continue;
			}

			e->Update(dt);
		}
	}

	void ParticleSystem::Render(GraphicsEngine::Graphics* graphics)
	{
		if ((systemDelay != 0.f) && (stackTime < systemDelay))
		{
			return;
		}

		switch (renderType)
		{
			case BJEngine::ParticleSystem::eRenderType::Ellipse:
				RenderEllipse(graphics);
				break;

			case BJEngine::ParticleSystem::eRenderType::Rectangle:
				RenderRectangle(graphics);
				break;

			case BJEngine::ParticleSystem::eRenderType::Sprite:
				RenderSprite(graphics);
				break;

			case BJEngine::ParticleSystem::eRenderType::Line:
				RenderLine(graphics);
				break;

			default:
				throw("Particle System Error! Plz refer to the <BJEngine/Temporary/JKH/Particle System/Render()>");
				break;
		}
	}

	void ParticleSystem::RenderEllipse(GraphicsEngine::Graphics*& graphics)
	{
		for (auto& e : particles)
		{
			// 쌓인 델타 타임과 딜레이를 비교함.
			if ((e->disable == true) || (e->stackTime < e->particleDelay))
			{
				continue;
			}

			graphics->DrawEllipse(e->Get2DScreenPosition().x, e->Get2DScreenPosition().y, minSize, minSize, e->Get2DScreenScale().x, e->Get2DScreenScale().y, e->angle, e->particleColor);
		}
	}

	void ParticleSystem::RenderRectangle(GraphicsEngine::Graphics*& graphics)
	{
		for (auto& e : particles)
		{
			// 쌓인 델타 타임과 딜레이를 비교함.
			if ((e->disable == true) || (e->stackTime < e->particleDelay))
			{
				continue;
			}

			graphics->DrawRect(e->Get2DScreenPosition().x, e->Get2DScreenPosition().y, minSize * e->Get2DScreenScale().x, minSize * e->Get2DScreenScale().y, e->particleColor);
		}
	}

	void ParticleSystem::RenderSprite(GraphicsEngine::Graphics*& graphics)
	{
		for (auto& e : particles)
		{
			// 쌓인 델타 타임과 딜레이를 비교함.
			if ((e->disable == true) || (e->stackTime < e->particleDelay))
			{
				continue;
			}

			Math::Vector2D flipvector(1.0f, 1.0f);

			if (flipX)
			{
				flipvector.x = -1.0f;
			}

			if (flipY)
			{
				flipvector.y = -1.0f;
			}

			graphics->DrawSprite(GetSprite(), GetImageNumber(), e->Get2DScreenPosition().x, e->Get2DScreenPosition().y, flipvector.x * e->Get2DScreenScale().x, flipvector.y * e->Get2DScreenScale().y, e->angle);
		}
	}

	void ParticleSystem::RenderLine(GraphicsEngine::Graphics*& graphics)
	{
		for (auto& e : particles)
		{
			// 쌓인 델타 타임과 딜레이를 비교함.
			if ((e->disable == true) || (e->stackTime < e->particleDelay))
			{
				continue;
			}

			graphics->DrawLine(
				e->Get2DScreenPosition(), e->Get2DScreenPosition() + (Math::Vector2D(0.f, rn.GetRandomFloat(minSize, minSize * 1.5f)) * e->Get2DScreenScale().x),
				0.f, 0.f, 
				e->Get2DScreenScale().x, e->Get2DScreenScale().y,
				e->angle, e->particleColor);
		}
	}

	#pragma region Particle System
	void ParticleSystem::SetParticleCount(size_t count)
	{
		this->maxParticle = count;
	}

	void ParticleSystem::SetLooping(bool isLoop)
	{
		this->looping = isLoop;
	}

	void ParticleSystem::SetSystemDelay(float delay)
	{
		this->systemDelay = delay;
	}

	void ParticleSystem::SetParticleDelay(float delay)
	{
		this->particleDelay = delay;
	}

	void ParticleSystem::SetLifeTime(float _min, float _max)
	{
		this->minLifeTime = _min;
		this->maxLifeTime = _max;
		if (minLifeTime > maxLifeTime)
		{
			maxLifeTime = this->minLifeTime + rn.GetRandomFloat(0.001f, 0.1f);
		}
	}

	void ParticleSystem::SetMinSpeed(float _min, float _max)
	{
		this->minSpeed = _min;
		this->maxSpeed = _max;
		if (minSpeed > maxSpeed)
		{
			maxSpeed = minSpeed * 1.5f;
		}
	}

	void ParticleSystem::SetStartSize(float size)
	{
		this->minSize = size;
	}

	void ParticleSystem::SetStartRotation(float angle)
	{
		this->startRotate = angle;
	}

	void ParticleSystem::SetColor(GraphicsEngine::Color color)
	{
		this->startColor = color;
	}

	void ParticleSystem::SetDirection(Math::Vector2D dir)
	{
		this->direction = dir;
	}

	void ParticleSystem::SetDirectionRangeRadian(float radian)
	{
		this->range = radian;
	}

	void ParticleSystem::SetDirectionRangeDegree(float degree)
	{
		float temp = degree * (PARTICLE_PI / 180);
		this->range = temp;
	}

	void ParticleSystem::SetRectScope(float minX, float maxX, float minY, float maxY)
	{
		this->minScopeX = minX;
		this->maxScopeX = maxX;
		this->minScopeY = minY;
		this->maxScopeY = maxY;
	}

	void ParticleSystem::SetCircleScope(float minScope, float maxScope)
	{
		this->minScopeX = minScope;
		this->maxScopeX = maxScope;
	}

	void ParticleSystem::SetParticleLineScope(const Math::Vector2D& point)
	{
		points.push_back(point);
	}

	void ParticleSystem::SetParticleLineScope(std::vector<Math::Vector2D>& _points)
	{
		points = _points;
	}

	void ParticleSystem::SetGameObjectPointer(BJEngine::GameObject* object)
	{
		objectList.push_back(object);
	}

	void ParticleSystem::SetGameObjectPointer(const std::vector<BJEngine::GameObject*>& objectlist)
	{
		objectList = objectlist;
	}

	void ParticleSystem::ClearLineScope()
	{
		particles.clear();
	}

	void ParticleSystem::SetGravity(float y)
	{
		this->gravity.x = 0.f;
		this->gravity.y = y;
	}

	void ParticleSystem::SetScale(float x, float y)
	{
		this->scaleX = x;
		this->scaleY = y;
	}

	void ParticleSystem::SetUsePoint(bool isUse)
	{
		usePoint = isUse;
	}

#pragma endregion Set Function

	#pragma region Particle System
	void ParticleSystem::SetSpriteName(std::wstring spritename)
	{
		spriteName = spritename;
	}

	GraphicsEngine::Sprite* ParticleSystem::GetSprite()
	{
		return GraphicsResourceManager::GetInstance().GetSprite(spriteName);
	}

	void ParticleSystem::SetImageNumber(int imagenumber)
	{
		imageNumber = imagenumber;
	}

	int ParticleSystem::GetImageNumber()
	{
		return imageNumber;
	}

	bool ParticleSystem::GetFlipX()
	{
		return flipX;
	}

	void ParticleSystem::SetFlipX(bool flipx)
	{
		flipX = flipx;
	}

	bool ParticleSystem::GetFlipY()
	{
		return flipY;
	}

	void ParticleSystem::SetFlipY(bool flipy)
	{
		flipY = flipy;
	}

	#pragma endregion Sprite Setting
	
	/*#pragma region Particle System
	void ParticleSystem::Play(std::wstring animationname)
	{
		animation = AnimationResourceManager::GetInstance().GetAnimation(animationname);

		playTime = 0.0f;
		frameNumber = 0;
	}

	void ParticleSystem::Execute()
	{
		if (animation->GetTotalDuration(frameNumber) < playTime)
		{
			frameNumber++;

			if (animation->GetTotalDuration(animation->GetFrameCount() - 1) < playTime)
			{
				if (animation->GetLoop())
				{
					playTime = 0.0f;
					frameNumber = 0;
				}
				else
				{
					frameNumber--;
				}
			}
		}

		SetImageNumber(animation->GetSpriteNumber(frameNumber));
		SetSpriteName(animation->GetSpriteName(frameNumber));

		playTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	}*/

	Math::Vector2D ParticleSystem::GetPosition()
	{
		if (scope == eScope::Point)
		{
			if (points.size() == 0)
			{
				return Math::Vector2D();
			}
			else
			{
				size_t index = rn.GetRandomIntiger(0, points.size() - 1);
				return /*GetGameObject()->GetTransform()->GetWorldPosition() +*/ points[index];
			}
		}
		else if (scope == eScope::Line)
		{
			// if (usePoint)
			// {
				if (points.size() < 2)
				{
					return Math::Vector2D();
				}
				else
				{
					size_t index = rn.GetRandomIntiger(0, points.size() - 2);
					Math::Vector2D point1 = points[index];
					Math::Vector2D point2 = points[index + 1];

					float min;
					float max;
					if (point1.x > point2.x)
					{
						min = point2.x;
						max = point1.x;
					}
					else
					{
						min = point1.x;
						max = point2.x;
					}

					if ((point1.x - point2.x) != 0.f)
					{
						float slope = (point1.y - point2.y) / (point1.x - point2.x);
						float x = rn.GetRandomFloat(min, max);
						float y = slope * (x - (point1.x)) + point1.y;
						return Math::Vector2D(x, y);
					}
					else
					{
						return Math::Vector2D(rn.GetRandomFloat(min, max), point1.y);
					}
				}
				/*
			}
			else
			{
				if (objectList.size() < 2)
				{
					return Math::Vector2D();
				}
				else
				{
					size_t index = rn.GetRandomIntiger(0, objectList.size() - 2);
					BJEngine::GameObject* object1 = objectList[index];
					BJEngine::GameObject* object2 = objectList[index + 1];

					float min;
					float max;
					if (object1->GetTransform()->GetWorldPosition().x > object2->GetTransform()->GetWorldPosition().x)
					{
						min = object2->GetTransform()->GetWorldPosition().x;
						max = object1->GetTransform()->GetWorldPosition().x;
					}
					else
					{
						min = object1->GetTransform()->GetWorldPosition().x;
						max = object2->GetTransform()->GetWorldPosition().x;
					}

					if ((object1->GetTransform()->GetWorldPosition().x - object2->GetTransform()->GetWorldPosition().x) != 0.f)
					{
						float slope = (object1->GetTransform()->GetWorldPosition().y - object2->GetTransform()->GetWorldPosition().y) / (object1->GetTransform()->GetWorldPosition().x - object2->GetTransform()->GetWorldPosition().x);
						float x = rn.GetRandomFloat(min, max);
						float y = slope * (x - (object1->GetTransform()->GetWorldPosition().x)) + object1->GetTransform()->GetWorldPosition().y;
						return Math::Vector2D(x, y);
					}
					else
					{
						return Math::Vector2D(rn.GetRandomFloat(min, max), object1->GetTransform()->GetWorldPosition().y);
					}
				}
			}
				*/
		}
		else if (scope == eScope::Circle)
		{
			return (rn.GetRandomUnitVector() * rn.GetRandomFloat(minScopeX, maxScopeX));
		}
		else if (scope == eScope::Rectangle)
		{
			float x = rn.GetRandomFloat(minScopeX, maxScopeX) + rn.GetRandomFloat(-maxScopeX, -minScopeX);

			float y = rn.GetRandomFloat(minScopeY, maxScopeY) + rn.GetRandomFloat(-maxScopeY, -minScopeY);
			
			return Math::Vector2D(x, y);
		}
		else
		{
			throw("Particle System Error! Plz refer to the <BJEngine/Temporary/JKH/Particle System/GetPosition()>");
		}
	}

	float ParticleSystem::GetParticleDelay(size_t index)
	{
		if (emission == eEmissionMode::Impact)
		{
			return particleDelay;
		}
		else if (emission == eEmissionMode::Spray)
		{
			return rn.GetRandomFloat(particleDelay, particleDelay * 1.2f) * index;
		}
		else
		{
			throw("Particle System Error! Plz refer to the <BJEngine/Temporary/JKH/Particle System/GetParticleDelay()>");
		}
	}

	float ParticleSystem::GetLifeTime()
	{
		if (emission == eEmissionMode::Impact)
		{
			return minLifeTime;
		}
		else if (emission == eEmissionMode::Spray)
		{
			return rn.GetRandomFloat(minLifeTime, maxLifeTime);
		}
		else
		{
			throw("Particle System Error! Plz refer to the <BJEngine/Temporary/JKH/Particle System/GetLifeTime()>");
		}
	}
	
	Math::Vector2D ParticleSystem::GetVelocity(Math::Vector2D pos)
	{
		if (state == eParticleState::Static)
		{
			return Math::Vector2D(0.f, 0.f);
		}

		Math::Vector2D temp{};
		float scalar{};
		
		if (emission == eEmissionMode::Impact)
		{
			scalar = minSpeed;
		}
		else
		{
			scalar = rn.GetRandomFloat(minSpeed, maxSpeed);
		}
		Math::Vector2D dir{};

		// 수렴
		if (activity == eActivity::Diverges)
		{
			temp = GetGameObject()->GetTransform()->Get2DScreenPosition();
			dir = (pos - temp).GetUnitVector();
		}
		// 발산
		else if (activity == eActivity::Convergence)
		{
			temp = GetGameObject()->GetTransform()->Get2DScreenPosition();
			dir = (temp - pos).GetUnitVector();
		}
		// 랜덤
		else if (activity == eActivity::Random)
		{
			dir = rn.GetRandomUnitVector();
		}
		else if (activity == eActivity::Regular)
		{
			// 디렉션 벡터로부터 범위에서 랜덤으로?
			// 아니면 디렉션 벡터를 향해서만? 그것도 설정 가능하게?
			dir = rn.GetRangeUnitVector(direction, range);
		}
		else
		{
			throw("Particle System Error! Plz refer to the <BJEngine/Temporary/JKH/Particle System/GetVelocity()>");
		}

		return dir * scalar;
	}

	Math::Vector2D ParticleSystem::GetAcceleration()
	{
		return gravity;
	}
	#pragma endregion Get Function
}
