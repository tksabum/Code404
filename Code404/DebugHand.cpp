#include "DebugHand.h"

#include "Text.h"
#include "GameObject.h"
#include "BJMath.h"
#include "Vector2D.h"

#include "BossHand.h"
#include "Scene.h"

using namespace std;


void DebugHand::SetTemp(int temp)
{
	this->temp = temp;
}

void DebugHand::Awake()
{
	if (temp == 0)
	{
		hand = GetGameObject()->GetScene()->GetGameObject(L"bossLeftHand")->GetComponent<BossHand>();
	}
	else
	{
		hand = GetGameObject()->GetScene()->GetGameObject(L"bossRightHand")->GetComponent<BossHand>();
	}
	
	text = GetGameObject()->GetComponent<Text>();
	text->SetTextColor(GraphicsEngine::Color::Yellow);
	text->SetTextSize(100);
	text->SetTextAreaSize(Math::Vector2D(1000, 1000));
}

void DebugHand::Update()
{
	wstring str = L"MoveSpeed : ";
	str += to_wstring(hand->GetMoveSpeed());
	str += L"\Count";
	str += to_wstring(hand->GetTriggerCount());
	text->SetText(str);
}
