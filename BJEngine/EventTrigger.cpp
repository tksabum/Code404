#include "EventTrigger.h"

/// 씬 전환을 위한 헤더 참조
#include "Collider.h"
#include "BlackMask.h"

/// 충돌체가 플레이어인지 확인하기 위한 헤더 참조
#include "GameObject.h"

/// 델타타임을 조절하기 위한 트리거
#include "TimeController.h"

/// 이벤트 도중에 키입력을 막기 위한 참조
#include "Input.h"


namespace BJEngine
{
	using namespace Math;

	EventTrigger::EventTrigger() : active(true)
	{

	}

	EventTrigger::~EventTrigger()
	{

	}

	void EventTrigger::OnTriggerEnter(Collider* other)
	{
		if (active != true)
			return;

		/// 트리거가 발동이 되면 델타타임을 0으로 만들고
		/// 게임 오브젝트를 활성화 시킨다.
		if ((other->GetGameObject()->GetLayerName() == L"Player" || other->GetGameObject()->GetLayerName() == L"StealthPlayer") 
			&& object != nullptr)
		{
			GetGameObject()->GetTimeController()->SetTimeScale(0.f);
			Input::GetInstance().SetKeyActive(false);
			object->SetActive(true);
			active = false;
		}
	}

	void EventTrigger::SetActiveObject(GameObject* gameobject)
	{
		this->object = gameobject;
	}
}