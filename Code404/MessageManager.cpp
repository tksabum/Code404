#include "MessageManager.h"

#include "Message.h"

MessageManager& MessageManager::GetInstance()
{
	static MessageManager instance;
	return instance;
}

void MessageManager::AddMessage(std::wstring messagename, std::wstring message, std::wstring fontName, size_t fontSize, BJEngine::GraphicsEngine::Color fontColor /*= BJEngine::GraphicsEngine::Color::Black*/)
{
	Message* newMessage = new Message(message, fontName, fontSize, fontColor);
	messageList[messagename] = newMessage;
}

Message* MessageManager::GetMessage(std::wstring messagename)
{
	return messageList[messagename];
}
