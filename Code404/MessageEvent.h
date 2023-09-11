#pragma once
#include <string>
#include <vector>
#include "Component.h"
#include "Color.h"
#include "Vector2D.h"

namespace BJEngine
{
	class Text;
	class GameObject;
}

class Message;

class MessageEvent : public BJEngine::Component
{
public:
	MessageEvent();
	~MessageEvent();
	
public:
	void SetRenderer(BJEngine::Text* renderer);
	void SetMessageEvent(Message* message, std::wstring fontname, float timegap, size_t width, size_t hight, BJEngine::Math::Vector2D area);
	void AddEventButton(BJEngine::GameObject* button);
	void SetNextMessage(MessageEvent* button);
	void SetPrevMessage(MessageEvent* button);

public:
	void Update();
	// void Render(BJEngine::GraphicsEngine::Graphics* graphics);

private:
	BJEngine::Text* render;
	std::vector<BJEngine::GameObject*> eventButtonList;
	MessageEvent* nextEvent;
	MessageEvent* prevEvent;

	BJEngine::Math::Vector2D margin;
	BJEngine::Math::Vector2D messageArea;
	Message* msg{};
	std::wstring buffer{};
	std::wstring font{};
	float messageGap{};

	bool isLoop = false;

	std::wstring::iterator itr;
};

/*
* �׷��� UI�� �θ�� �ϴµ� UI�� size�� ���� ����
* �����¿� ������ �޾� �ؽ�Ʈ ��� ������ ���� ����
* 
* 
*/