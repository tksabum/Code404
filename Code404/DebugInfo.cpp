#include "DebugInfo.h"

#include "Player.h"
#include "Skill.h"

void DebugInfo::Awake()
{
	text = GetGameObject()->GetComponent<Text>();
	text->SetTextColor(GraphicsEngine::Color::Yellow);
	text->SetTextSize(30);
}

void DebugInfo::Update()
{
	Player* player = nullptr;
	if (GetGameObject()->GetScene()->GetGameObject(L"PlayerObject") != nullptr)
	{
		player = GetGameObject()->GetScene()->GetGameObject(L"PlayerObject")->GetComponent<Player>();
	}

	wstring str;
	str += L"DeltaTime        : " + to_wstring(GetGameObject()->GetTimeController()->GetDeltaTime()) + L"\n";
	str += L"PhysicsDeltaTime : " + to_wstring(GetGameObject()->GetTimeController()->GetPhysicsDeltaTime()) + L"\n";
	str += L"FPS              : " + to_wstring(1.0f / GetGameObject()->GetTimeController()->GetDeltaTime()) + L"\n";
	str += L"\n";

	Transform* cameratransform = GetGameObject()->GetScene()->GetMainCamera()->GetGameObject()->GetTransform();

	str += L"Camera Position  : " + to_wstring(cameratransform->GetWorldPosition().x) + L", " + to_wstring(cameratransform->GetWorldPosition().y) + L"\n";
	str += L"\n";

	if (player != nullptr)
	{
		str += L"HP               : " + to_wstring(player->currentHealthPoint) + L" / " + to_wstring(player->maxHealthPoint) + L"\n";
		str += L"MP               : " + to_wstring(player->currentMagicPoint) + L" / " + to_wstring(player->maxMagicPoint) + L"\n";
		str += L"\n";

		str += L"Cost             : " + to_wstring(player->skillList[player->SKILL_ID_THROWDAGGER]->GetCost()) + L"\n";
		str += L"ElapsedTime      : " + to_wstring(max(player->skillList[player->SKILL_ID_THROWDAGGER]->repeatUseTime - player->skillList[player->SKILL_ID_THROWDAGGER]->elapsedTime, 0.0f)) + L"\n";
	
		str += L"Position		  :	" + to_wstring(player->GetGameObject()->GetTransform()->GetWorldPosition().x) + L" / " + to_wstring(player->GetGameObject()->GetTransform()->GetWorldPosition().y) + L"\n";
	}

	text->SetText(str);
}
