#include "BlackMask.h"
#include "BJMath.h"
#include "Vector3D.h"

#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"

#include <cmath>

#include "Scene.h"
#include "EventTrigger.h"

/// 씬 전환을 위한 헤더 참조
#include "Scene.h"
#include "CoreSystem.h"
#include "SceneManager.h"
#include "BlackMask.h"
#include "Input.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void BlackMask::Awake()
{
	moveSpeed = 2000.f;
	accelerator = 0.f;
	isEnd = false;
}

void BlackMask::Update()
{
	BJEngine::Math::Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();
	
	if (!isEnd)
	{
		switch (sceneMode)
		{
			/// 새로운 씬에서 시작할 때 쓰는 모드
			case BlackMask::eMode::SCENE_IN:
			{
				// 검은 화면에서 시작해서
				// 전환 이미지가 완전히 화면 밖으로 나갈 때까지
				if (pos.x > 2300.f)
				{
					pos.x = 2300.f;
					GetGameObject()->GetTimeController()->SetTimeScale(1.f);
					Input::GetInstance().SetKeyActive(true);
					isEnd = true;
				}

				/// 화면 전환은 이벤트이기 때문에 델타타임을 0으로 만들고 시작한다.
				else
				{
					GetGameObject()->GetTimeController()->SetTimeScale(0.f);
					Input::GetInstance().SetKeyActive(false);
					moveSpeed += accelerator * GetGameObject()->GetTimeController()->GetEventDeltaTime();
					pos.x += moveSpeed * GetGameObject()->GetTimeController()->GetEventDeltaTime();
				}
			}
				break;

			/// 새로운 씬으로 이동할 때 쓰는 모드
			case BlackMask::eMode::SCENE_OUT:
			{
				// 게임 화면에서 시작해서
				// 전환 이미지가 완전히 화면을 가릴 때 까지
				if (pos.x > -190.f)
				{
					pos.x = -190.f;
					CoreSystem::GetInstance().GetSceneManager()->LoadScene(nextScene);
					isEnd = true;
				}
				else
				{
					moveSpeed += accelerator * GetGameObject()->GetTimeController()->GetEventDeltaTime();
					pos.x += moveSpeed * GetGameObject()->GetTimeController()->GetEventDeltaTime();
				}
			}
				break;

			default:
				throw("BlackMask Error!!");
				break;
		}
	}

	GetGameObject()->GetTransform()->SetLocalPosition(pos);
}

void BlackMask::SetScene(std::wstring scene)
{
	this->nextScene = scene;
}

void BlackMask::SetScene(int scene)
{
	this->nextSceneID = scene;
}

void BlackMask::SetMode(eMode mode)
{
	sceneMode = mode;
}

void BlackMask::SetStartPoint(Vector2D start)
{
	startPoint = start;
}

void BlackMask::SetEndPoint(Vector2D end)
{
	endPoint = end;
}

void BlackMask::SetSpeed(float speed)
{
	moveSpeed = speed;
}

void BlackMask::SetAccelerator(float accel)
{
	accelerator = accel;
}
