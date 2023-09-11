#include "BlackMask.h"
#include "BJMath.h"
#include "Vector3D.h"

#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"

#include <cmath>

#include "Scene.h"
#include "EventTrigger.h"

/// �� ��ȯ�� ���� ��� ����
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
			/// ���ο� ������ ������ �� ���� ���
			case BlackMask::eMode::SCENE_IN:
			{
				// ���� ȭ�鿡�� �����ؼ�
				// ��ȯ �̹����� ������ ȭ�� ������ ���� ������
				if (pos.x > 2300.f)
				{
					pos.x = 2300.f;
					GetGameObject()->GetTimeController()->SetTimeScale(1.f);
					Input::GetInstance().SetKeyActive(true);
					isEnd = true;
				}

				/// ȭ�� ��ȯ�� �̺�Ʈ�̱� ������ ��ŸŸ���� 0���� ����� �����Ѵ�.
				else
				{
					GetGameObject()->GetTimeController()->SetTimeScale(0.f);
					Input::GetInstance().SetKeyActive(false);
					moveSpeed += accelerator * GetGameObject()->GetTimeController()->GetEventDeltaTime();
					pos.x += moveSpeed * GetGameObject()->GetTimeController()->GetEventDeltaTime();
				}
			}
				break;

			/// ���ο� ������ �̵��� �� ���� ���
			case BlackMask::eMode::SCENE_OUT:
			{
				// ���� ȭ�鿡�� �����ؼ�
				// ��ȯ �̹����� ������ ȭ���� ���� �� ����
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
