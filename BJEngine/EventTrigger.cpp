#include "EventTrigger.h"

/// �� ��ȯ�� ���� ��� ����
#include "Collider.h"
#include "BlackMask.h"

/// �浹ü�� �÷��̾����� Ȯ���ϱ� ���� ��� ����
#include "GameObject.h"

/// ��ŸŸ���� �����ϱ� ���� Ʈ����
#include "TimeController.h"

/// �̺�Ʈ ���߿� Ű�Է��� ���� ���� ����
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

		/// Ʈ���Ű� �ߵ��� �Ǹ� ��ŸŸ���� 0���� �����
		/// ���� ������Ʈ�� Ȱ��ȭ ��Ų��.
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