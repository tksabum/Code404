#include "Twinkling.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"
#include "Vector2D.h"

#include "TimeController.h"

#include <random>

using namespace std;
using namespace BJEngine;
using namespace BJEngine::Math;

void Twinkling::Awake()
{
	originScaleY = GetGameObject()->GetTransform()->GetLocalScale().y;
	curScaleY = originScaleY;
	originPosY = GetGameObject()->GetTransform()->GetLocalPosition().y;
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<float> range(0.7, 1);
	targetScaleY = range(mt);
}

void Twinkling::Update()
{
	curScaleY = GetGameObject()->GetTransform()->GetLocalScale().y;
	if (abs(targetScaleY - curScaleY) >= 0.1f)
	{
		if (targetScaleY - curScaleY < 0)
		{
			curScaleY -= GetGameObject()->GetTimeController()->GetDeltaTime() * 3;
		}
		else
		{
			curScaleY += GetGameObject()->GetTimeController()->GetDeltaTime() * 3;
		}
	}
	else
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<float> range(0.7, 1);
		targetScaleY = range(mt);
	}

	GetGameObject()->GetTransform()->SetLocalScale(Vector3D(1, curScaleY * originScaleY,1));
	GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(GetGameObject()->GetTransform()->GetLocalPosition().x,originPosY + (curScaleY - originScaleY)*250, GetGameObject()->GetTransform()->GetLocalPosition().z));
}
