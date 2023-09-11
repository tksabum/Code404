#include "EventMessage.h"

using namespace std;

MessageEvent::MessageEvent(wstring msg, wstring font, size_t fontSize, BJEngine::GraphicsEngine::Color fontColor) : 
	m_message(msg), m_fontName(font), m_size(fontSize), m_color(fontColor)
{
}

MessageEvent::MessageEvent(const MessageEvent& other) :
	m_message(other.m_message), m_fontName(other.m_fontName), m_size(other.m_size), m_color(other.m_color)
{

}

MessageEvent::~MessageEvent()
{
}

wstring MessageEvent::GetMessageString()
{
	return m_message;
}

wstring MessageEvent::GetMessageFontName()
{
	return m_fontName;
}

size_t MessageEvent::GetMessageFontSize()
{
	return m_size;
}

BJEngine::GraphicsEngine::Color MessageEvent::GetMessageColor()
{
	return m_color;
}
