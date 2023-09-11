#pragma once

#include "Component.h"
#include "Code404.h"

#include <map>

#include "Rigidbody2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"
#include "Input.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace std;

using namespace BJEngine;
using namespace BJEngine::Math;

class Skill;
class DebugInfo;
class SkillCoolTimeViewer;
class PlayerStealthCounter;
class GameDataManager;

/// <summary>
/// �������� �۵��� ���ؼ��� ���ӿ�����Ʈ�� Rigidbody2D�� ������ �־���Ѵ�.
/// </summary>
class Player : public Component
{
public:
	~Player();

	void Awake();
	void Update();

	void ReadyJump();

	void DashMove();

	void Attacked(ENEMYATTACK_TYPE type, float damage);

	void OnTriggerEnter(Collider* collider);

private:
	enum class State
	{
		STAND,
		RUN,
		JUMP,
		DASH,
		JUMPDASH,
		ATTACKED,
		ATTACKED_JUMP,
		DEATH
	};

	enum class AttackState
	{
		READY,
		NORMALATTACK_JUMP,
		NORMALATTACK,
		WAIT_COMBOATTACK,
		COMBOATTACK,
		WAIT_FINALATTACK,
		FINALATTACK,
		THOROWDAGGER,
		WITHDRAWDAGGER
	};

	State lastState;
	State state;

	AttackState lastAttackState;
	AttackState attackState;

	GameObject* normalAttackTrigger;
	GameObject* comboAttackTrigger;
	GameObject* finalAttackTrigger;

	bool isStealth;

	bool attackKeyBuffer;

	// ������ ����� �ð�
	float elapsedStealthTime;
	// ��� �������� ����� �ð�
	float elapsedLastAttackTime;
	// �⺻�������� ����� �ð�
	float elapsedNomalAttackTime;
	// �뽬�� ������ ���� ����� �ð�
	float elapsedDashTime;
	// �ǰ��� ����� �ð�
	float elapsedAttackedTime;
	// ����� ����� �ð�
	float elapsedDeathTime;

	void CheatUpdate();
	void DebugUpdate();

	void SetHP(float hp);
	void SetMP(float mp);
	void SetStealthCount(int count);

	void NormalAttack();
	void ComboAttack();
	void FinalAttack();

	void WaitComboAttack();
	void WaitFinalAttack();

	void StealthOn();
	void StealthOff();

	// ��Ÿ�� ���
	void SkillUpdate();

	void DeathUpdate();
	void MoveUpdate();
	void AttackUpdate();
	void AnimationUpdate();

	// �÷��̾��� ����
	void SetDirection(Vector2D dir);
	Vector2D direction;

	// ����
	float currentHealthPoint;
	float currentMagicPoint;
	float maxHealthPoint;
	float maxMagicPoint;

	int currentStealthCount;
	int maxStealthCount;

	// ��ų
	map<int, Skill*> skillList;

	SpriteRenderer* sprite;
	Animator* animator;

	Rigidbody2D* rigidbody;
	bool isReadyJump;

	GameObject* daggerA;
	GameObject* daggerS;
	GameObject* daggerD;

	bool isReadyDaggerA;
	bool isReadyDaggerS;
	bool isReadyDaggerD;

	bool isNeedDaggerA;
	bool isNeedDaggerS;
	bool isNeedDaggerD;

	float runSoundTimer;

	const static float RUN_SOUND_PERIOD;

	const static float MOVE_SPEED;
	const static float JUMP_SPEED;

	const static float MAX_HP_DEFAULT;
	const static float MAX_MP_DEFAULT;

	const static int SKILL_ID_THROWDAGGER;
	const static int SKILL_ID_THROWDAGGER_A;
	const static int SKILL_ID_THROWDAGGER_S;
	const static int SKILL_ID_THROWDAGGER_D;
	const static int SKILL_ID_WITHDRAWDAGGER_A;
	const static int SKILL_ID_WITHDRAWDAGGER_S;
	const static int SKILL_ID_WITHDRAWDAGGER_D;
	const static int SKILL_ID_DASH;

	const static float ATTACK_DELAY;

	const static float NORMAL_ATTACK_COOLTIME;

	const static float CONNECTION_TIME;

	const static float NORMAL_ATTACK_DELAY;
	const static float WAIT_COMBO_ATTACK_DELAY;
	const static float COMBO_ATTACK_DELAY;
	const static float WAIT_FINAL_ATTACK_DELAY;
	const static float FINAL_ATTACK_DELAY;

	const static float DASH_SPEED;
	const static float DASH_TIME;

	// �ǰ� ���� �ð�
	const static float HIT_STUN_TIME;
	// �ǰ� ���� �ð�
	const static float HIT_INVINCIBILITY_TIME;

	// ����
	const static int STEALTH_COUNT_DEFAULT;
	const static int STEALTH_COUNT_MAX;
	const static float STEALTH_DURATION;

	const static Vector2D ATTACK_DISTANCE;
	const static Vector2D ATTACK_RANGE;

	const static float NORMALATTACK_DAMAGE;
	const static float COMBOATTACK_DAMAGE;
	const static float FINALATTACK_DAMAGE;

	const static float MP_RECOVERY_AMOUNT;

	GameObject* gameoverpanel;

public:
	float* GetMaxHp();
	float* GetCurHp();
	float* GetMaxMp();
	float* GetCurMp();

	void SetGameOverPanel(GameObject* panel);

	friend DebugInfo;
	friend SkillCoolTimeViewer;
	friend PlayerStealthCounter;
	friend GameDataManager;
};

