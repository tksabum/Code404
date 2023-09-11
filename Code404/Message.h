#pragma once
#include <string>

#include "Color.h"

class Message
{
public:
	Message(std::wstring msg, std::wstring font, size_t fontSize, BJEngine::GraphicsEngine::Color fontColor = BJEngine::GraphicsEngine::Color::Black);
	Message(const Message& other);
	~Message();

public:
	std::wstring& GetMessageString();
	std::wstring GetMessageFontName();
	size_t GetMessageFontSize();
	BJEngine::GraphicsEngine::Color GetMessageColor();

private:
	std::wstring m_message;
	std::wstring m_fontName;
	size_t m_size;
	BJEngine::GraphicsEngine::Color m_color;
};

