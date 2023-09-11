#include "SkillCoolTimeViewer.h"

#include "Player.h"
#include "Skill.h"

void SkillCoolTimeViewer::SetSkill(SkillName skillname)
{
	skillName = skillname;
}

void SkillCoolTimeViewer::Start()
{
	GameObject* playerobject = GetGameObject()->GetScene()->GetGameObject(L"PlayerObject");
	Player* player = playerobject->GetComponent<Player>();

	switch (skillName)
	{
		case SkillName::DaggerA:
			skill = player->skillList[player->SKILL_ID_THROWDAGGER_A];
			break;
		
		case SkillName::DaggerS:
			skill = player->skillList[player->SKILL_ID_THROWDAGGER_S];
			break;
		
		case SkillName::DaggerD:
			skill = player->skillList[player->SKILL_ID_THROWDAGGER_D];
			break;
		
		default:
			break;
	}
}

void SkillCoolTimeViewer::Update()
{
	float cool = -skill->GetRemainingCooltime() / skill->GetCooltime();

	if ((((skill->uniqueCost1 == nullptr) || (*skill->uniqueCost1 == true)) && ((skill->uniqueCost2 == nullptr) || (*skill->uniqueCost2 == true))) == false)
	{
		cool = -1.0f;
	}

	GetGameObject()->GetTransform()->SetLocalScale(Vector3D(1.0f, cool, 1.0f));
}
