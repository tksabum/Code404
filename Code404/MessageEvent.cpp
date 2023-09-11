/// MessageBox가 존재하기 위해 "먼저" 참조함
#include "Text.h"
#include "Message.h"

#include "MessageEvent.h"

/// 텍스트 출력 위치를 위한 헤더 참조
#include "Graphics.h"
#include "GameObject.h"
#include "Transform.h"

/// 시간차를 두고 텍스트를 출력하기 위한 참조
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

	// 마우스 클릭이 감지가 된다면
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

	// 델타 타임이 쌓였고 반복자의 위치가 문자열 끝이 아니라면
	if (stack > messageGap && itr != msg->GetMessageString().end())
	{
		// 반복자가 가리키는 문자를 하나씩 버퍼에 넣고 반복자를 이동시킨 뒤
		// 버퍼를 출력한다.
		buffer.push_back(*itr);
		itr++;
		stack = 0.f;
		render->SetText(buffer);
	}

	// 반복자가 문자열 끝을 가리키고 있고 루프가 비활성화 상태라면
	else if (itr == msg->GetMessageString().end() && isLoop == false)
	{

		// 버튼을 활성화 시키는 무언가를 넣어주자.
		// 버튼은 다음 메세지를 활성화 시키도록 하면 될듯 하다.
		if (!eventButtonList.empty())
		{
			for (auto& e : eventButtonList)
			{
				e->SetActive(true);
			}
		}

		// 버튼 리스트에 버튼이 없다면
		// 다음 출력할 메세지 이벤트
		else
		{
			if (Input::GetInstance().ReleasedKeyNow(VK_LBUTTON) && nextEvent != nullptr)
			{
				nextEvent->GetGameObject()->SetActive(true);
			}
		}
	}

	// 반복자가 문자열 끝을 가리키고 있고 루프가 활성화 상태라면
	else if (itr == msg->GetMessageString().end() && isLoop == true)
	{
		// 반복자가 문자열 끝이 다다르고 루프가 활성화 되어있다면
		// 버퍼를 초기화하고
		// 반복자를 문자열 처음으로 되돌린다
		buffer.clear();
		itr = msg->GetMessageString().begin();
	}

	// 아무 것도 아니라면 델타타임을 쌓는다.
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
