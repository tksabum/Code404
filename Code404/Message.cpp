#include "Message.h"

using namespace std;

Message::Message(wstring msg, wstring font, size_t fontSize, BJEngine::GraphicsEngine::Color fontColor) : 
	m_message(msg), m_fontName(font), m_size(fontSize), m_color(fontColor)
{
}

Message::Message(const Message& other) :
	m_message(other.m_message), m_fontName(other.m_fontName), m_size(other.m_size), m_color(other.m_color)
{

}

Message::~Message()
{
}

wstring& Message::GetMessageString()
{
	return m_message;
}

wstring Message::GetMessageFontName()
{
	return m_fontName;
}

size_t Message::GetMessageFontSize()
{
	return m_size;
}

BJEngine::GraphicsEngine::Color Message::GetMessageColor()
{
	return m_color;
}
