#pragma once

#include "Component.h"

#include "GameObject.h"
#include "Scene.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class Skill;

class SkillCoolTimeViewer : public Component
{
public:
	enum class SkillName
	{
		DaggerA,
		DaggerS,
		DaggerD
	};

	void SetSkill(SkillName skillname);

	void Start();

	void Update();

private:
	SkillName skillName;
	Skill* skill;

};

