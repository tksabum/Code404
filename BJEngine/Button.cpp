#include "Button.h"

#include "SoundManager.h"
#include "GameObject.h"
#include "Image.h"

namespace BJEngine
{
	Button::Button() : isMouseLeftDown(false), effectSoundID(-1), image(nullptr), idleImage(L""), enterImage(L""), clickImage(L"")
	{

	}

	Button::~Button()
	{

	}

	void Button::AddButtonEvent(std::function<void()> eventfunction)
	{
		buttonEventList.push_back(eventfunction);
	}

	void Button::Awake()
	{
		if (image == nullptr)
		{
			image = GetGameObject()->GetComponent<Image>();
		}
	}

	void Button::OnMouseLeftDown()
	{
		if (image != nullptr && clickImage != L"")
		{
			image->SetSpriteName(clickImage);
		}

		isMouseLeftDown = true;

		if (effectSoundID != -1)
		{
			SoundManager::GetInstance().Play(effectSoundID);
		}
	}

	void Button::OnMouseLeftUp()
	{
		if (image != nullptr && enterImage != L"")
		{
			image->SetSpriteName(enterImage);
		}

		if (isMouseLeftDown == true)
		{
			for (int i = 0; i < buttonEventList.size(); i++)
			{
				buttonEventList[i]();
			}
		}

		isMouseLeftDown = false;
	}

	void Button::OnMouseEnter()
	{
		if (image != nullptr && enterImage != L"")
		{
			image->SetSpriteName(enterImage);
		}
	}

	void Button::OnMouseExit()
	{
		if (image != nullptr && idleImage != L"")
		{
			image->SetSpriteName(idleImage);
		}

		isMouseLeftDown = false;
	}

	void Button::SetSound(int effectsoundID)
	{
		effectSoundID = effectsoundID;
	}

	void Button::SetImage(std::wstring idleimage, std::wstring enterimage, std::wstring clickimage)
	{
		idleImage = idleimage;
		enterImage = enterimage;
		clickImage = clickimage;
	}
}