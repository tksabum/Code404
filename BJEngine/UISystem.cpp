#include "UISystem.h"

#include "UI.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

namespace BJEngine
{
	using namespace Math;

	void UISystem::AddUI(UI* ui)
	{
		uiList.push_back(ui);
	}
	
	void UISystem::RemoveUI(UI* ui)
	{
		uiList.erase(remove(uiList.begin(), uiList.end(), ui), uiList.end());
	}
	
	void UISystem::GUIEvent()
	{
		// currentMouseOver�� ������ lastMouseOver�� �̵�
		if (uiList.size() != currentMouseOver.size())
		{
			lastMouseOver.resize(uiList.size(), false);
		}
		else
		{
			lastMouseOver = currentMouseOver;
		}

		currentMouseOver.resize(uiList.size(), false);

		// ���콺�� ��ġ�� UI�� ��ġ�� ũ��� mouseOver���� �Ǵ�
		Vector2D mousePos = Input::GetInstance().GetMousePosition();

		for (int i = 0; i < uiList.size(); i++)
		{
			UI* ui = uiList[i];
			//Vector2D lefttop = ui->GetGameObject()->GetTransform()->GetWorldPosition();
			Vector2D lefttop = ui->GetGameObject()->GetTransform()->GetUIPosition();
			Vector2D rightbottom = lefttop + ui->GetSize();

			bool result = (lefttop.x <= mousePos.x) && (mousePos.x <= rightbottom.x) && (lefttop.y <= mousePos.y) && (mousePos.y <= rightbottom.y);

			if (!ui->GetGameObject()->GetRootActive())
			{
				result = false;
			}

			currentMouseOver[i] = result;
		}

		// OnMouseEnter
		for (int i = 0; i < uiList.size(); i++)
		{
			GameObject* gameobject = uiList[i]->GetGameObject();

			if (!lastMouseOver[i] && currentMouseOver[i])
			{
				gameobject->OnMouseEnterEvent();
			}
		}

		// OnMouseExit
		for (int i = 0; i < uiList.size(); i++)
		{
			GameObject* gameobject = uiList[i]->GetGameObject();

			if (lastMouseOver[i] && !currentMouseOver[i])
			{
				gameobject->OnMouseExitEvent();
			}
		}

		// OnMouseOver
		for (int i = 0; i < uiList.size(); i++)
		{
			GameObject* gameobject = uiList[i]->GetGameObject();

			if (lastMouseOver[i] && currentMouseOver[i])
			{
				gameobject->OnMouseOverEvent();
			}
		}

		if (Input::GetInstance().PressedKeyNow(VK_LBUTTON))
		{
			// OnMouseLeftDown
			for (int i = 0; i < uiList.size(); i++)
			{
				GameObject* gameobject = uiList[i]->GetGameObject();

				if (gameobject->GetRootActive() && currentMouseOver[i])
				{
					gameobject->OnMouseLeftDownEvent();
				}
			}
		}

		if (Input::GetInstance().PressedKeyNow(VK_RBUTTON))
		{
			// OnMouseRightDown
			for (int i = 0; i < uiList.size(); i++)
			{
				GameObject* gameobject = uiList[i]->GetGameObject();

				if (gameobject->GetRootActive() && currentMouseOver[i])
				{
					gameobject->OnMouseRightDownEvent();
				}
			}
		}

		if (Input::GetInstance().ReleasedKeyNow(VK_LBUTTON))
		{
			// OnMouseLeftUp
			for (int i = 0; i < uiList.size(); i++)
			{
				GameObject* gameobject = uiList[i]->GetGameObject();

				if (gameobject->GetRootActive() && currentMouseOver[i])
				{
					gameobject->OnMouseLeftUpEvent();
				}
			}
		}

		if (Input::GetInstance().ReleasedKeyNow(VK_RBUTTON))
		{
			// OnMouseRightUp
			for (int i = 0; i < uiList.size(); i++)
			{
				GameObject* gameobject = uiList[i]->GetGameObject();

				if (gameobject->GetRootActive() && currentMouseOver[i])
				{
					gameobject->OnMouseRightUpEvent();
				}
			}
		}
	}

	UISystem& UISystem::GetInstance()
	{
		static UISystem instance;
		return instance;
	}
	
	UISystem::UISystem()
	{

	}
	
	UISystem::~UISystem()
	{

	}
}