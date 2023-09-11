#include "BGMSoundController.h"
#include "SoundManager.h"


BGMSoundController::BGMSoundController()
{

}

void BGMSoundController::Awake()
{
	originPosX = GetGameObject()->GetTransform()->GetLocalPosition().x;
	originPosY = GetGameObject()->GetTransform()->GetLocalPosition().y;

	isDrag = false;
	float volumeData = SoundManager::GetInstance().GetBGMVolume();	//0~1����
	volumeData *= moveRangeX;
	GetGameObject()->GetTransform()->SetLocalPosition({ originPosX + volumeData, originPosY });
}

void BGMSoundController::OnMouseLeftDown()
{
	isDrag = true;
	dragPosition = Input::GetInstance().GetMousePosition();
}

void BGMSoundController::OnMouseLeftUp()
{
	isDrag = false;

	float v = (GetGameObject()->GetTransform()->GetLocalPosition().x - originPosX) / moveRangeX;

	SoundManager::GetInstance().SetBGM(v);
}

void BGMSoundController::Update()
{
	if (isDrag)
	{
		if (Input::GetInstance().ReleasedKey(VK_LBUTTON))
		{
			isDrag = false;

			float v = (GetGameObject()->GetTransform()->GetLocalPosition().x - originPosX) / moveRangeX;

			SoundManager::GetInstance().SetBGM(v);

			return;
		}

		// ���콺 ���� ��ġ
		Vector2D currentpos = Input::GetInstance().GetMousePosition();

		float nextX = dragPosition.x - GetGameObject()->GetParent()->GetTransform()->GetUIPosition().x + (currentpos - dragPosition).x;

		if (nextX < originPosX)
		{
			nextX = originPosX;
		}

		if (nextX > originPosX + moveRangeX)
		{
			nextX = originPosX + moveRangeX;
		}

		GetGameObject()->GetTransform()->SetLocalPosition(Vector2D(nextX, originPosY));
	}
}

void BGMSoundController::SetRange(float rangeX)
{
	moveRangeX = rangeX;
}