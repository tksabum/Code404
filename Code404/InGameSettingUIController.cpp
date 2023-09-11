#include "InGameSettingUIController.h"

#include "Input.h"

#include "GameObject.h"
#include "Image.h"
#include "TimeController.h"

#include "CoreSystem.h"
#include "SceneManager.h"

using namespace BJEngine;

void InGameSettingUIController::SetTarget(BJEngine::GameObject* target)
{
	this->target = target;
}

void InGameSettingUIController::SetSound(BJEngine::GameObject* sound)
{
	this->sound = sound;
}

void InGameSettingUIController::SetScreen(BJEngine::GameObject* screen)
{
	this->screen = screen;
}

void InGameSettingUIController::SetBlackMask(BJEngine::GameObject* blackmask)
{
	blackMask = blackmask;
}

void InGameSettingUIController::GoToMainTitle()
{
	isMain = true;
	target->SetActive(false);
}

void InGameSettingUIController::Awake()
{
	blackMaskImage = blackMask->GetComponent<Image>();
}

void InGameSettingUIController::Update()
{
	if (Input::GetInstance().PressedKeyNow(VK_ESCAPE))
	{
		if (target->GetActive() || sound->GetActive() || screen->GetActive())
		{
			target->SetActive(false);
			sound->SetActive(false);
			screen->SetActive(false);
		}
		else
		{
			target->SetActive(true);
		}
	}
	if (isMain)
	{
		float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
		if (!blackMask->GetActive())
		{
			blackMask->SetActive(true);
		}
		opacity += opacitySpeed * deltaTime;
		blackMaskImage->SetOpacity(opacity);
		if (opacity >= 1.f)
		{
			isMain = false;
			opacity = 0.f;
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"MainScene");
		}
	}
}
