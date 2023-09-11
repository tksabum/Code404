#pragma once

#include <string>
#include <unordered_map>

#include "Color.h"

class Message;

class MessageManager
{
private:
	MessageManager() {};
	~MessageManager() {};

public:
	static MessageManager& GetInstance();

public:
	void AddMessage(std::wstring messagename, std::wstring message, std::wstring fontName,size_t fontSize, BJEngine::GraphicsEngine::Color fontColor = BJEngine::GraphicsEngine::Color::Black);
	Message* GetMessage(std::wstring messagename);

private:
	std::unordered_map<std::wstring, Message*> messageList;
};

