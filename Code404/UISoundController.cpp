#include "UISoundController.h"
#include "SoundManager.h"


UISoundController::UISoundController()
{
}

void UISoundController::Awake()
{
	originPosX = GetGameObject()->GetTransform()->GetLocalPosition().x;
	originPosY = GetGameObject()->GetTransform()->GetLocalPosition().y;

	isDrag = false;
	float volumeData = SoundManager::GetInstance().GetVolume();	//0~1부터
	volumeData *= moveRangeX;
	GetGameObject()->GetTransform()->SetLocalPosition({ originPosX + volumeData, originPosY });
}

void UISoundController::OnMouseLeftDown()
{
	isDrag = true;
	dragPosition = Input::GetInstance().GetMousePosition();
}

void UISoundController::OnMouseLeftUp()
{
	isDrag = false;

	float v = (GetGameObject()->GetTransform()->GetLocalPosition().x - originPosX) / moveRangeX;

	SoundManager::GetInstance().SetVolume(v);
}

void UISoundController::Update()
{
	if (isDrag)
	{
		if (Input::GetInstance().ReleasedKey(VK_LBUTTON))
		{
			isDrag = false;

			float v = (GetGameObject()->GetTransform()->GetLocalPosition().x - originPosX) / moveRangeX;

			SoundManager::GetInstance().SetVolume(v);

			return;
		}

		// 마우스 현재 위치
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

void UISoundController::SetRange(float rangeX)
{
	moveRangeX = rangeX;
}