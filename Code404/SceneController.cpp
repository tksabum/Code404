#include "SceneController.h"
#include "EngineSetting.h"

void SceneController::Update()
{
	if (Input::GetInstance().PressedKeyNow(VK_BACK))
	{
		CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_1");
		EngineSetting::GetInstance().SetShowDebugInfo(false);
	}
}
