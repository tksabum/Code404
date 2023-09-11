/// MessageBox�� �����ϱ� ���� "����" ������
#include "Text.h"
#include "Message.h"

#include "MessageEvent.h"

/// �ؽ�Ʈ ��� ��ġ�� ���� ��� ����
#include "Graphics.h"
#include "GameObject.h"
#include "Transform.h"

/// �ð����� �ΰ� �ؽ�Ʈ�� ����ϱ� ���� ����
#include "TimeController.h";
#include "Input.h"

using namespace BJEngine;
using namespace BJEngine::Math;

MessageEvent::MessageEvent()
{

}

MessageEvent::~MessageEvent()
{

}

void MessageEvent::SetRenderer(BJEngine::Text* renderer)
{
	render = renderer;
}

void MessageEvent::SetMessageEvent(Message* message, std::wstring fontname, float timegap, size_t width, size_t hight, BJEngine::Math::Vector2D area)
{
	this->msg = message;
	this->messageGap = timegap;
	this->margin = { (float)width, (float)hight };
	this->messageArea = area;

	GetGameObject()->GetTransform()->SetLocalPosition(margin);
	render->SetTextSize(msg->GetMessageFontSize());
	render->SetFont(fontname);
	render->SetTextColor(msg->GetMessageColor());
	render->SetTextAreaSize({messageArea.x - (margin.x * 2.f), messageArea.y - (margin.y * 2.f) });

	itr = message->GetMessageString().begin();
}

void MessageEvent::AddEventButton(BJEngine::GameObject* button)
{
	eventButtonList.push_back(button);
}

void MessageEvent::SetNextMessage(MessageEvent* button)
{
	nextEvent = button;
}

void MessageEvent::SetPrevMessage(MessageEvent* button)
{
	prevEvent = button;
}

void MessageEvent::Update()
{
	static float stack = 0.00001f;
	if (prevEvent != nullptr)
	{
		prevEvent->GetGameObject()->SetActive(false);
	}

	// ���콺 Ŭ���� ������ �ȴٸ�
	if (Input::GetInstance().ReleasedKeyNow(VK_LBUTTON) && itr != msg->GetMessageString().end())
	{
		buffer = msg->GetMessageString();
		itr = msg->GetMessageString().end();
		stack = 0.f;
		render->SetText(buffer); 

		if (!eventButtonList.empty())
		{
			for (auto& e : eventButtonList)
			{
				e->SetActive(true);
			}
		}
		return;
	}

	// ��Ÿ Ÿ���� �׿��� �ݺ����� ��ġ�� ���ڿ� ���� �ƴ϶��
	if (stack > messageGap && itr != msg->GetMessageString().end())
	{
		// �ݺ��ڰ� ����Ű�� ���ڸ� �ϳ��� ���ۿ� �ְ� �ݺ��ڸ� �̵���Ų ��
		// ���۸� ����Ѵ�.
		buffer.push_back(*itr);
		itr++;
		stack = 0.f;
		render->SetText(buffer);
	}

	// �ݺ��ڰ� ���ڿ� ���� ����Ű�� �ְ� ������ ��Ȱ��ȭ ���¶��
	else if (itr == msg->GetMessageString().end() && isLoop == false)
	{

		// ��ư�� Ȱ��ȭ ��Ű�� ���𰡸� �־�����.
		// ��ư�� ���� �޼����� Ȱ��ȭ ��Ű���� �ϸ� �ɵ� �ϴ�.
		if (!eventButtonList.empty())
		{
			for (auto& e : eventButtonList)
			{
				e->SetActive(true);
			}
		}

		// ��ư ����Ʈ�� ��ư�� ���ٸ�
		// ���� ����� �޼��� �̺�Ʈ
		else
		{
			if (Input::GetInstance().ReleasedKeyNow(VK_LBUTTON) && nextEvent != nullptr)
			{
				nextEvent->GetGameObject()->SetActive(true);
			}
		}
	}

	// �ݺ��ڰ� ���ڿ� ���� ����Ű�� �ְ� ������ Ȱ��ȭ ���¶��
	else if (itr == msg->GetMessageString().end() && isLoop == true)
	{
		// �ݺ��ڰ� ���ڿ� ���� �ٴٸ��� ������ Ȱ��ȭ �Ǿ��ִٸ�
		// ���۸� �ʱ�ȭ�ϰ�
		// �ݺ��ڸ� ���ڿ� ó������ �ǵ�����
		buffer.clear();
		itr = msg->GetMessageString().begin();
	}

	// �ƹ� �͵� �ƴ϶�� ��ŸŸ���� �״´�.
	else
	{
		stack += GetGameObject()->GetTimeController()->GetEventDeltaTime();
	}
}

/*
void MessageEvent::Render(BJEngine::GraphicsEngine::Graphics* graphics)
{
	Vector2D screenPosition = GetGameObject()->GetTransform()->Get2DScreenPosition();
	Vector2D screenScale = GetGameObject()->GetTransform()->Get2DScreenScale();
	float screenRotation = GetGameObject()->GetTransform()->Get2DScreenRotation();

	graphics->DrawString(
		buffer, 
		screenPosition.x + margin.x, screenPosition.y + margin.y,
		messageArea.x - margin.x, messageArea.y - margin.y,
		screenScale.x, screenScale.y, 
		screenRotation, 
		msg->GetMessageFontSize(),
		msg->GetMessageColor());
}
*/
