#pragma once
#include <string>

#include "Color.h"

class MessageEvent
{
public:
	MessageEvent(std::wstring msg, std::wstring font, size_t fontSize, BJEngine::GraphicsEngine::Color fontColor = BJEngine::GraphicsEngine::Color::Black);
	MessageEvent(const MessageEvent& other);
	~MessageEvent();

public:
	std::wstring GetMessageString();
	std::wstring GetMessageFontName();
	size_t GetMessageFontSize();
	BJEngine::GraphicsEngine::Color GetMessageColor();

private:
	std::wstring m_message;
	std::wstring m_fontName;
	size_t m_size;
	BJEngine::GraphicsEngine::Color m_color;
};

