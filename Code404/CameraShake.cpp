#include "BJMath.h"
#include "Vector3D.h"
#include <vector>
#include "CameraShake.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "GameObject.h"
#include "Transform.h"

#include <random>

using namespace std;
using namespace BJEngine;
using namespace BJEngine::Math;


void CameraShake::Shake(float shaketime, float xdelta, float ydelta)
{
	startPos = GetGameObject()->GetTransform()->GetLocalPosition();
	shakeTime = shaketime;
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	xMin = pos.x - xdelta;
	xMax = pos.x + xdelta;
	yMin = pos.y - ydelta;
	yMax = pos.y + ydelta;
	isShake = true;
}

void CameraShake::Awake()
{
	
}

void CameraShake::Update()
{
	if (isShake)
	{
		totalShakeTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
		// 흔들기
		if (totalShakeTime <= shakeTime)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_real_distribution<float> xRange(xMin, xMax);
			float xRandom = xRange(mt);

			uniform_real_distribution<float> yRange(yMin, yMax);
			float yRandom = yRange(mt);

			BJEngine::Math::Vector3D randomPos;
			randomPos.x = xRandom;
			randomPos.y = yRandom;

			GetGameObject()->GetTransform()->SetLocalPosition(randomPos);
		}
		else
		{
			// 흔들기 끝남
			// 변수들 초기화
			isShake = false;
			totalShakeTime = 0.f;
			shakeTime = 0.f;
			GetGameObject()->GetTransform()->SetLocalPosition(startPos);
		}
	}
}
