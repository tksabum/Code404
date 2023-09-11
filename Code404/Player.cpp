
#include "Player.h"

#include "ThrowDagger.h"
#include "ThrowUniqueDagger.h"
#include "UniqueDagger.h"
#include "WithdrawDagger.h"
#include "Dash.h"
#include "AttackTrigger.h"
#include "GameObjectFactory.h"
#include "EffectPlayer.h"
#include "ObjectPoolSystem.h"
#include "GameDataManager.h"
#include "Stage1_2.h"
#include "GameDataManager.h"
#include "SoundManager.h"

const float Player::RUN_SOUND_PERIOD = 0.3f;

const float Player::MOVE_SPEED = 600.0f;
const float Player::JUMP_SPEED = 1020.0f;

const float Player::MAX_HP_DEFAULT = 200.0f;
const float Player::MAX_MP_DEFAULT = 200.0f;

const int Player::SKILL_ID_THROWDAGGER = 1;
const int Player::SKILL_ID_THROWDAGGER_A = 2;
const int Player::SKILL_ID_THROWDAGGER_S = 3;
const int Player::SKILL_ID_THROWDAGGER_D = 4;
const int Player::SKILL_ID_WITHDRAWDAGGER_A = 5;
const int Player::SKILL_ID_WITHDRAWDAGGER_S = 6;
const int Player::SKILL_ID_WITHDRAWDAGGER_D = 7;
const int Player::SKILL_ID_DASH = 8;

const float Player::ATTACK_DELAY = 0.3f;

const float Player::NORMAL_ATTACK_COOLTIME = 0.3f;

const float Player::CONNECTION_TIME = 0.5f;

const float Player::NORMAL_ATTACK_DELAY = 0.15f;
const float Player::WAIT_COMBO_ATTACK_DELAY = CONNECTION_TIME - NORMAL_ATTACK_DELAY;
const float Player::COMBO_ATTACK_DELAY = 0.15f;
const float Player::WAIT_FINAL_ATTACK_DELAY = CONNECTION_TIME - COMBO_ATTACK_DELAY;
const float Player::FINAL_ATTACK_DELAY = 0.3f;

const float Player::DASH_SPEED = 1800.0f;
const float Player::DASH_TIME = 0.3f;

const float Player::HIT_STUN_TIME = 0.3f;
const float Player::HIT_INVINCIBILITY_TIME = 0.8f;

const int Player::STEALTH_COUNT_DEFAULT = 4;
const int Player::STEALTH_COUNT_MAX = 4;

const float Player::STEALTH_DURATION = 3.0f;

const Vector2D Player::ATTACK_DISTANCE = Vector2D(50.0f, 0.0f);
const Vector2D Player::ATTACK_RANGE = Vector2D(280.0f, 200.0f);

const float Player::NORMALATTACK_DAMAGE = 100.0f;
const float Player::COMBOATTACK_DAMAGE = 150.0f;
const float Player::FINALATTACK_DAMAGE = 200.0f;

const float Player::MP_RECOVERY_AMOUNT = (25.0f / 3.0f);

wstring stage1_1 = L"Stage1_1";
wstring stage1_2 = L"Stage1_2";
wstring stage1_3 = L"Stage1_3";
wstring stage1_4 = L"Stage1_4";
wstring stage1_5 = L"Stage1_5";
wstring stage2_1 = L"Stage2_1";
wstring stage2_2 = L"Stage2_2";
wstring stage2_3 = L"Stage2_3";
wstring stage2_4 = L"Stage2_4";
wstring stage2_5 = L"Stage2_5";
wstring stage2_6 = L"Stage2_6";
wstring stage2_7 = L"Stage2_7";
wstring stage2_8 = L"Stage2_8";
wstring stage2_9 = L"Stage2_9";
wstring BossStage = L"SampleScene4";

Player::~Player()
{
	for (int i = 0; i < skillList.size(); i++)
	{
		if (skillList[i] != nullptr)
		{
			delete skillList[i];
		}
	}
}

void Player::Awake()
{
	GetGameObject()->SetLayerName(L"Player");

	sprite = GetGameObject()->GetComponent<SpriteRenderer>();
	animator = GetGameObject()->GetComponent<Animator>();

	rigidbody = GetGameObject()->GetComponent<Rigidbody2D>();

	SetDirection(Vector2D::RIGHT);

	isReadyJump = false;

	maxHealthPoint = MAX_HP_DEFAULT;
	maxMagicPoint = MAX_MP_DEFAULT;

	// 단검 생성
	ThrowUniqueDagger* skillthrowdaggerA = new ThrowUniqueDagger(&direction, GetGameObject()->GetTransform(), &currentMagicPoint, &isReadyDaggerA);
	if (daggerA == nullptr)
	{
		daggerA = GetGameObject()->GetScene()->CreateGameObject(L"UniqueDagger");

		skillthrowdaggerA->SetUniqueDagger(daggerA, L"Animation_Dagger_Effect_A");

		daggerA->AddComponent<ConvexpolygonCollider>();
		daggerA->AddComponent<Rigidbody2D>();
		UniqueDagger* uniquedagger = daggerA->AddComponent<UniqueDagger>();
		SpriteRenderer* sprite = daggerA->AddComponent<SpriteRenderer>();
		sprite->SetLayer(Code404::Layer::Dagger);
		Animator* daggeranimator = daggerA->AddComponent<Animator>();

		daggerA->SetLayerName(L"Dagger");
		daggerA->GetComponent<ConvexpolygonCollider>()->SetPolygon({ Vector2D(-40.0f, 5.0f), Vector2D(-40.0f, -5.0f), Vector2D(40.0f, -5.0f), Vector2D(40.0f, 5.0f) });
		Rigidbody2D* rigidbody = daggerA->GetComponent<Rigidbody2D>();
		rigidbody->SetAffectedByGravity(false);
		rigidbody->SetPositionXLock(true);
		rigidbody->SetPositionYLock(true);
		rigidbody->SetRotationLock(true);
		rigidbody->SetVelocity(Vector2D(0.0f, 0.0f));
		uniquedagger->SetUniqueDagger(&direction, GetGameObject()->GetTransform(), &isReadyDaggerA, &isNeedDaggerA, skillthrowdaggerA);
		uniquedagger->SetAnimation(L"Animation_Dagger_Stuck_A", L"Animation_Dagger_Flying_A");
		daggeranimator->Play(L"Animation_Dagger_Flying_A");
		SoundManager::GetInstance().Play(SOUND_EFFECT_THROWING);

		daggerA->SetActive(false);
	}

	ThrowUniqueDagger* skillthrowdaggerS = new ThrowUniqueDagger(&direction, GetGameObject()->GetTransform(), &currentMagicPoint, &isReadyDaggerS);
	if (daggerS == nullptr)
	{
		daggerS = GetGameObject()->GetScene()->CreateGameObject(L"UniqueDagger");

		skillthrowdaggerS->SetUniqueDagger(daggerS, L"Animation_Dagger_Effect_S");

		daggerS->AddComponent<ConvexpolygonCollider>();
		daggerS->AddComponent<Rigidbody2D>();
		UniqueDagger* uniquedagger = daggerS->AddComponent<UniqueDagger>();
		SpriteRenderer* sprite = daggerS->AddComponent<SpriteRenderer>();
		sprite->SetLayer(Code404::Layer::Dagger);
		Animator* daggeranimator = daggerS->AddComponent<Animator>();

		daggerS->SetLayerName(L"Dagger");
		daggerS->GetComponent<ConvexpolygonCollider>()->SetPolygon({ Vector2D(-40.0f, 5.0f), Vector2D(-40.0f, -5.0f), Vector2D(40.0f, -5.0f), Vector2D(40.0f, 5.0f) });
		Rigidbody2D* rigidbody = daggerS->GetComponent<Rigidbody2D>();
		rigidbody->SetAffectedByGravity(false);
		rigidbody->SetPositionXLock(true);
		rigidbody->SetPositionYLock(true);
		rigidbody->SetRotationLock(true);
		rigidbody->SetVelocity(Vector2D(0.0f, 0.0f));
		uniquedagger->SetUniqueDagger(&direction, GetGameObject()->GetTransform(), &isReadyDaggerS, &isNeedDaggerS, skillthrowdaggerS);
		uniquedagger->SetAnimation(L"Animation_Dagger_Stuck_S", L"Animation_Dagger_Flying_S");
		daggeranimator->Play(L"Animation_Dagger_Flying_S");
		SoundManager::GetInstance().Play(SOUND_EFFECT_THROWING);

		daggerS->SetActive(false);
	}

	ThrowUniqueDagger* skillthrowdaggerD = new ThrowUniqueDagger(&direction, GetGameObject()->GetTransform(), &currentMagicPoint, &isReadyDaggerD);
	if (daggerD == nullptr)
	{
		daggerD = GetGameObject()->GetScene()->CreateGameObject(L"UniqueDagger");

		skillthrowdaggerD->SetUniqueDagger(daggerD, L"Animation_Dagger_Effect_D");

		daggerD->AddComponent<ConvexpolygonCollider>();
		daggerD->AddComponent<Rigidbody2D>();
		UniqueDagger* uniquedagger = daggerD->AddComponent<UniqueDagger>();
		SpriteRenderer* sprite = daggerD->AddComponent<SpriteRenderer>();
		sprite->SetLayer(Code404::Layer::Dagger);
		Animator* daggeranimator = daggerD->AddComponent<Animator>();

		daggerD->SetLayerName(L"Dagger");
		daggerD->GetComponent<ConvexpolygonCollider>()->SetPolygon({ Vector2D(-40.0f, 5.0f), Vector2D(-40.0f, -5.0f), Vector2D(40.0f, -5.0f), Vector2D(40.0f, 5.0f) });
		Rigidbody2D* rigidbody = daggerD->GetComponent<Rigidbody2D>();
		rigidbody->SetAffectedByGravity(false);
		rigidbody->SetPositionXLock(true);
		rigidbody->SetPositionYLock(true);
		rigidbody->SetRotationLock(true);
		rigidbody->SetVelocity(Vector2D(0.0f, 0.0f));
		uniquedagger->SetUniqueDagger(&direction, GetGameObject()->GetTransform(), &isReadyDaggerD, &isNeedDaggerD, skillthrowdaggerD);
		uniquedagger->SetAnimation(L"Animation_Dagger_Stuck_D", L"Animation_Dagger_Flying_D");
		daggeranimator->Play(L"Animation_Dagger_Flying_D");
		SoundManager::GetInstance().Play(SOUND_EFFECT_THROWING);

		daggerD->SetActive(false);
	}

	skillList.clear();
	skillList.insert({ SKILL_ID_THROWDAGGER, new ThrowDagger(&direction, GetGameObject()->GetTransform(), &currentMagicPoint) });
	skillList.insert({ SKILL_ID_THROWDAGGER_A, skillthrowdaggerA });
	skillList.insert({ SKILL_ID_THROWDAGGER_S, skillthrowdaggerS });
	skillList.insert({ SKILL_ID_THROWDAGGER_D, skillthrowdaggerD });

	WithdrawDagger* withdrawdaggerA = new WithdrawDagger(&direction, GetGameObject()->GetTransform(), &currentMagicPoint, &isNeedDaggerA);
	withdrawdaggerA->SetTargetDagger(daggerA);
	WithdrawDagger* withdrawdaggerS = new WithdrawDagger(&direction, GetGameObject()->GetTransform(), &currentMagicPoint, &isNeedDaggerS);
	withdrawdaggerS->SetTargetDagger(daggerS);
	WithdrawDagger* withdrawdaggerD = new WithdrawDagger(&direction, GetGameObject()->GetTransform(), &currentMagicPoint, &isNeedDaggerD);
	withdrawdaggerD->SetTargetDagger(daggerD);

	skillList.insert({ SKILL_ID_WITHDRAWDAGGER_A, withdrawdaggerA });
	skillList.insert({ SKILL_ID_WITHDRAWDAGGER_S, withdrawdaggerS });
	skillList.insert({ SKILL_ID_WITHDRAWDAGGER_D, withdrawdaggerD });

	Dash* dashskill = new Dash(&direction, GetGameObject()->GetTransform(), &currentMagicPoint);
	dashskill->SetPlayer(this);
	skillList.insert({ SKILL_ID_DASH, dashskill });

	state = State::STAND;
	attackState = AttackState::READY;
	animator->Play(L"Animation_Player_Stand");

	isReadyDaggerA = true;
	isReadyDaggerS = true;
	isReadyDaggerD = true;

	isNeedDaggerA = false;
	isNeedDaggerS = false;
	isNeedDaggerD = false;

	attackKeyBuffer = false;

	isStealth = false;

	elapsedLastAttackTime = ATTACK_DELAY;
	elapsedNomalAttackTime = NORMAL_ATTACK_COOLTIME;
	elapsedAttackedTime = max(HIT_STUN_TIME, HIT_INVINCIBILITY_TIME);

	maxStealthCount = STEALTH_COUNT_MAX;

	if (normalAttackTrigger == nullptr)
	{
		normalAttackTrigger = GameObjectFactory::GetInstance().CreatePlayerAttackTrigger(GetGameObject(), ATTACK_DISTANCE, ATTACK_RANGE, PLAYERATTACK_TYPE::COMBO1, NORMALATTACK_DAMAGE);
		normalAttackTrigger->SetActive(false);
	}

	if (comboAttackTrigger == nullptr)
	{
		comboAttackTrigger = GameObjectFactory::GetInstance().CreatePlayerAttackTrigger(GetGameObject(), ATTACK_DISTANCE, ATTACK_RANGE, PLAYERATTACK_TYPE::COMBO2, COMBOATTACK_DAMAGE);
		comboAttackTrigger->SetActive(false);
	}

	if (finalAttackTrigger == nullptr)
	{
		finalAttackTrigger = GameObjectFactory::GetInstance().CreatePlayerAttackTrigger(GetGameObject(), ATTACK_DISTANCE, ATTACK_RANGE, PLAYERATTACK_TYPE::COMBO3, FINALATTACK_DAMAGE);
		finalAttackTrigger->SetActive(false);
	}

	runSoundTimer = 0.0f;

	SetHP(GameDataManager::GetInstance().hp);
	SetMP(GameDataManager::GetInstance().mp);
	SetStealthCount(GameDataManager::GetInstance().stealthCount);
}

void Player::Update()
{
	float deltatime = GetGameObject()->GetTimeController()->GetDeltaTime();

	// 마나회복
	SetMP(currentMagicPoint + deltatime * MP_RECOVERY_AMOUNT);

	// 이전 상태기록
	lastState = state;
	lastAttackState = attackState;

	elapsedAttackedTime += deltatime;
	elapsedDashTime += deltatime;
	elapsedLastAttackTime += deltatime;
	elapsedNomalAttackTime += deltatime;
	elapsedStealthTime += deltatime;

	normalAttackTrigger->SetActive(false);
	comboAttackTrigger->SetActive(false);
	finalAttackTrigger->SetActive(false);

	if (state == State::DEATH)
	{
		elapsedDeathTime += deltatime;
		wstring currentScene = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetName();

		if (elapsedDeathTime >= 3 && gameoverpanel->GetActive() == false)
		{
			SoundManager::GetInstance().Play(SOUND_BGM_GAMEOVER);
			gameoverpanel->SetActive(true);
		}

		DeathUpdate();
	}
	else
	{
		SkillUpdate();
		MoveUpdate();
		AttackUpdate();
		AnimationUpdate();
	}

	// 매 프레임 GameDataManager에 정보저장
	SetHP(currentHealthPoint);
	SetMP(currentMagicPoint);
	SetStealthCount(currentStealthCount);

	CheatUpdate();
	DebugUpdate();
}

void Player::ReadyJump()
{
	if (state == State::DEATH || state == State::ATTACKED || state == State::ATTACKED_JUMP)
	{
		return;
	}



	if (isReadyJump == false && state != State::DASH && state != State::JUMPDASH && rigidbody->GetVelocity().y >= 0.0f)
	{
		isReadyJump = true;
		state = State::STAND;
		animator->Play(L"Animation_Player_Stand");

		// 착지 이펙트
		GameObject* jumpeffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
		jumpeffect->SetActive(true);

		EffectPlayer* effectanimator = jumpeffect->GetComponent<EffectPlayer>();
		effectanimator->Reset(L"Animation_Player_Land_Effect");
		SoundManager::GetInstance().Play(SOUND_EFFECT_JUMP);


		if (GetGameObject()->GetTransform()->GetLocalScale().x == 1.0f)
		{

			jumpeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(0.0f, 70.0f));
			jumpeffect->GetTransform()->SetLocalScale(GetGameObject()->GetTransform()->GetLocalScale());
		}
		else
		{
			jumpeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(0.0f, 55.0f));
			jumpeffect->GetTransform()->SetLocalScale(GetGameObject()->GetTransform()->GetLocalScale());
		}
	}
}

void Player::DashMove()
{
	if (state == State::JUMP)
	{
		state = State::JUMPDASH;
	}
	else
	{
		state = State::DASH;

		// 대쉬 이펙트
		GameObject* daggereffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
		daggereffect->SetActive(true);

		EffectPlayer* effectanimator = daggereffect->GetComponent<EffectPlayer>();
		effectanimator->Reset(L"Animation_Player_Dash_Effect");
		SoundManager::GetInstance().Play(SOUND_EFFECT_DASH);

		float playerscale = GetGameObject()->GetTransform()->GetLocalScale().x;
		Vector3D effectscale = Vector3D(playerscale * direction.x, playerscale, 1.0f);

		daggereffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition());
		daggereffect->GetTransform()->SetLocalScale(effectscale);
	}

	// 대쉬중에는 중력을 무시한다.
	rigidbody->SetAffectedByGravity(false);

	rigidbody->SetVelocity(direction * DASH_SPEED);

	animator->Play(L"Animation_Player_Dash");

	elapsedDashTime = 0.0f;
}

void Player::Attacked(ENEMYATTACK_TYPE type, float damage)
{
	// 이미 죽어있다면 공격받지 않음
	if (state == State::DEATH)
	{
		return;
	}

	// 대쉬중에는 공격받지 않음
	if (state == State::DASH || state == State::JUMPDASH)
	{
		return;
	}

	// 피격후 무적시간 동안은 공격받지 않음
	if (elapsedAttackedTime <= HIT_INVINCIBILITY_TIME)
	{
		return;
	}

	float fixeddamage = damage;

	// type에 따라 데미지 조정
	switch (type)
	{
	case ENEMYATTACK_TYPE::MELEE_A:
		// 은신한 경우 일반 몬스터에게 공격받지 않음
		if (isStealth == true)
		{
			fixeddamage = 0.0f;
		}
		break;

	case ENEMYATTACK_TYPE::MELEE_B:
		// 은신한 경우 일반 몬스터에게 공격받지 않음
		if (isStealth == true)
		{
			fixeddamage = 0.0f;
		}
		break;

	case ENEMYATTACK_TYPE::RANGE_A:
		// 은신한 경우 일반 몬스터에게 공격받지 않음
		if (isStealth == true)
		{
			fixeddamage = 0.0f;
		}
		break;

	case ENEMYATTACK_TYPE::RANGE_B:
		// 은신한 경우 일반 몬스터에게 공격받지 않음
		if (isStealth == true)
		{
			fixeddamage = 0.0f;
		}
		break;

	case ENEMYATTACK_TYPE::BOSSHEAD:
		break;

	case ENEMYATTACK_TYPE::BOSSHAND:
		break;

	case ENEMYATTACK_TYPE::BOSSCROSSLASER:
	{
		
	}
		break;

	case ENEMYATTACK_TYPE::BOSSSCENELASER:
	{
		
	}
		break;

	case ENEMYATTACK_TYPE::BOSSBULLET:
		break;

	default:
		break;
	}


	// 데미지가 0보다 작거나 같다면 무시한다.
	if (fixeddamage <= 0.0f)
	{
		return;
	}

	// 데미지 적용
	SetHP(currentHealthPoint - fixeddamage);

	// 공격받은 시간 초기화
	elapsedAttackedTime = 0.0f;

	// 플레이어가 사망한 경우
	if (currentHealthPoint <= 0.0f)
	{
		state = State::DEATH;
		elapsedDeathTime = 0.0f;
		rigidbody->SetVelocity(Vector2D(0.0f, rigidbody->GetVelocity().y));
		animator->Play(L"Animation_Player_Death");
	}
	// 플레이어가 사망하지 않은 경우
	else
	{
		if (state == State::JUMP)
		{
			state = State::ATTACKED_JUMP;
		}
		else
		{
			state = State::ATTACKED;
		}
		animator->Play(L"Animation_Player_Attacked");
	}
}

void Player::OnTriggerEnter(Collider* collider)
{
	if (collider->GetGameObject()->GetLayerName() == L"Item")
	{
		collider->GetGameObject()->SetActive(false);
		SetHP(maxHealthPoint);
		SetStealthCount(currentStealthCount + 1);
	}
}

void Player::CheatUpdate()
{
	// 치트
	if (GameDataManager::GetInstance().runningCheat == true)
	{
		// 치트 비활성화를 위한 키입력 확인
		const static string cheatstring = "RKATKGKQSLEKTJSTODSLA"; // 21글자
		static list<char> cheatinputbuffer;
		if (cheatinputbuffer.empty())
		{
			for (int i = 0; i < 21; i++)
			{
				cheatinputbuffer.push_back(' ');
			}
		}

		for (int i = 0; i < 26; i++)
		{
			if (Input::GetInstance().PressedKeyNow('A' + i))
			{
				cheatinputbuffer.pop_front();
				cheatinputbuffer.push_back('A' + i);
			}
		}

		bool cheatOff = true;
		int cnt = 0;
		for (auto c : cheatinputbuffer)
		{
			if (c != cheatstring[cnt])
			{
				cheatOff = false;
				break;
			}
			cnt++;
		}

		if (cheatOff == true)
		{
			GameDataManager::GetInstance().ClearCheat();
			if (cheatinputbuffer.empty())
			{
				for (int i = 0; i < 26; i++)
				{
					cheatinputbuffer.pop_front();
					cheatinputbuffer.push_back(' ');
				}
			}
		}

		// 치트 동작
		SetHP(maxHealthPoint);
		SetMP(maxMagicPoint);
	}
	else
	{
		// 치트 활성화를 위한 키입력 확인
		const static string cheatstring = "EHDHKWNTPDYQJAWNSTJSTODSLA"; // 26글자
		static list<char> cheatinputbuffer;
		if (cheatinputbuffer.empty())
		{
			for (int i = 0; i < 26; i++)
			{
				cheatinputbuffer.push_back(' ');
			}
		}

		for (int i = 0; i < 26; i++)
		{
			if (Input::GetInstance().PressedKeyNow('A' + i))
			{
				cheatinputbuffer.pop_front();
				cheatinputbuffer.push_back('A' + i);
			}
		}

		bool cheatOn = true;
		int cnt = 0;
		for (auto c : cheatinputbuffer)
		{
			if (c != cheatstring[cnt])
			{
				cheatOn = false;
				break;
			}
			cnt++;
		}

		if (cheatOn == true)
		{
			GameDataManager::GetInstance().CheatOn();
			if (cheatinputbuffer.empty())
			{
				for (int i = 0; i < 26; i++)
				{
					cheatinputbuffer.pop_front();
					cheatinputbuffer.push_back(' ');
				}
			}
		}
	}
}

void Player::DebugUpdate()
{
	if (GameDataManager::GetInstance().debugmode == true)
	{
		// 디버그모드 비활성화를 위한 키입력 확인
		const static string debugstring = "ELQJRMWHDFY"; // 11글자
		static list<char> debuginputbuffer;
		if (debuginputbuffer.empty())
		{
			for (int i = 0; i < 11; i++)
			{
				debuginputbuffer.push_back(' ');
			}
		}

		for (int i = 0; i < 26; i++)
		{
			if (Input::GetInstance().PressedKeyNow('A' + i))
			{
				debuginputbuffer.pop_front();
				debuginputbuffer.push_back('A' + i);
			}
		}

		bool debugOff = true;
		int cnt = 0;
		for (auto c : debuginputbuffer)
		{
			if (c != debugstring[cnt])
			{
				debugOff = false;
				break;
			}
			cnt++;
		}

		if (debugOff == true)
		{
			GameDataManager::GetInstance().debugmode = false;
			if (debuginputbuffer.empty())
			{
				for (int i = 0; i < 11; i++)
				{
					debuginputbuffer.pop_front();
					debuginputbuffer.push_back(' ');
				}
			}
		}

		if (Input::GetInstance().PressedKeyNow(VK_F1))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_1");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F2))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_2");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F3))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_3");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F4))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_4");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F5))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage1_5");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F6))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_1");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F7))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_2");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F8))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_3");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F9))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_4");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F10))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_5");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F11))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_6");
		}
		else if (Input::GetInstance().PressedKeyNow(VK_F12))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_7");
		}
		// 0
		else if (Input::GetInstance().PressedKeyNow(0x30))
		{
			
		}
		// 1
		else if (Input::GetInstance().PressedKeyNow(0x31))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_8");
		}
		// 2
		else if (Input::GetInstance().PressedKeyNow(0x32))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"Stage2_9");
		}
		// 3
		else if (Input::GetInstance().PressedKeyNow(0x33))
		{
			CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"SampleScene4");
		}
		// 4
		else if (Input::GetInstance().PressedKeyNow(0x34))
		{

		}
		// 5
		else if (Input::GetInstance().PressedKeyNow(0x35))
		{

		}
		// 6
		else if (Input::GetInstance().PressedKeyNow(0x36))
		{

		}
		// 7
		else if (Input::GetInstance().PressedKeyNow(0x37))
		{

		}
		// 8
		else if (Input::GetInstance().PressedKeyNow(0x38))
		{

		}
		// 9
		else if (Input::GetInstance().PressedKeyNow(0x39))
		{

		}
	}
	else
	{
		// 디버그모드 활성화를 위한 키입력 확인
		const static string debugstring = "ELQJRMTLWKR"; // 11글자
		static list<char> debuginputbuffer;
		if (debuginputbuffer.empty())
		{
			for (int i = 0; i < 11; i++)
			{
				debuginputbuffer.push_back(' ');
			}
		}

		for (int i = 0; i < 26; i++)
		{
			if (Input::GetInstance().PressedKeyNow('A' + i))
			{
				debuginputbuffer.pop_front();
				debuginputbuffer.push_back('A' + i);
			}
		}

		bool debugOn = true;
		int cnt = 0;
		for (auto c : debuginputbuffer)
		{
			if (c != debugstring[cnt])
			{
				debugOn = false;
				break;
			}
			cnt++;
		}

		if (debugOn == true)
		{
			GameDataManager::GetInstance().debugmode = true;
			if (debuginputbuffer.empty())
			{
				for (int i = 0; i < 11; i++)
				{
					debuginputbuffer.pop_front();
					debuginputbuffer.push_back(' ');
				}
			}
		}
	}
}

void Player::SetHP(float hp)
{
	currentHealthPoint = hp;

	if (hp < 0.0f)
	{
		currentHealthPoint = 0.0f;
	}
	if (hp > maxHealthPoint)
	{
		currentHealthPoint = maxHealthPoint;
	}

	GameDataManager::GetInstance().hp = currentHealthPoint;
}

void Player::SetMP(float mp)
{
	currentMagicPoint = mp;

	if (mp < 0.0f)
	{
		currentMagicPoint = 0.0f;
	}
	if (mp > maxHealthPoint)
	{
		currentMagicPoint = maxMagicPoint;
	}

	GameDataManager::GetInstance().mp = currentMagicPoint;
}

void Player::SetStealthCount(int count)
{
	currentStealthCount = count;
	if (currentStealthCount < 0)
	{
		currentStealthCount = 0;
	}
	if (currentStealthCount > maxStealthCount)
	{
		currentStealthCount = maxStealthCount;
	}

	GameDataManager::GetInstance().stealthCount = currentStealthCount;
}

void Player::NormalAttack()
{
	if (state == State::JUMP || state == State::JUMPDASH)
	{
		attackState = AttackState::NORMALATTACK_JUMP;
		elapsedLastAttackTime = 0.0f;
		elapsedNomalAttackTime = 0.0f;
		animator->Play(L"Animation_Player_NormalAttack_Jump");
	}
	else
	{
		attackState = AttackState::NORMALATTACK;
		elapsedLastAttackTime = 0.0f;
		elapsedNomalAttackTime = 0.0f;
		animator->Play(L"Animation_Player_NormalAttack");

		attackKeyBuffer = false;
	}

	normalAttackTrigger->SetActive(true);
}

void Player::ComboAttack()
{
	attackState = AttackState::COMBOATTACK;
	elapsedLastAttackTime = 0.0f;
	elapsedNomalAttackTime = 0.0f;
	animator->Play(L"Animation_Player_ComboAttack");

	attackKeyBuffer = false;

	comboAttackTrigger->SetActive(true);
}

void Player::FinalAttack()
{
	attackState = AttackState::FINALATTACK;
	elapsedLastAttackTime = 0.0f;
	elapsedNomalAttackTime = 0.0f;
	animator->Play(L"Animation_Player_FinalAttack");

	finalAttackTrigger->SetActive(true);
}

void Player::WaitComboAttack()
{
	attackState = AttackState::WAIT_COMBOATTACK;
	elapsedLastAttackTime = 0.0f;
	animator->Play(L"Animation_Player_Wait_ComboAttack");
}

void Player::WaitFinalAttack()
{
	attackState = AttackState::WAIT_FINALATTACK;
	elapsedLastAttackTime = 0.0f;
	animator->Play(L"Animation_Player_Wait_FinalAttack");
}

void Player::StealthOn()
{
	if (currentStealthCount != 0)
	{
		SetStealthCount(currentStealthCount - 1);

		if (isStealth == true)
		{
			elapsedStealthTime -= STEALTH_DURATION;
		}
		else
		{
			isStealth = true;
			GetGameObject()->SetLayerName(L"StealthPlayer");
			elapsedStealthTime = 0.0f;
			sprite->SetOpacity(0.3f);
		}
	}
}

void Player::StealthOff()
{
	isStealth = false;
	GetGameObject()->SetLayerName(L"Player");
	sprite->SetOpacity(1.0f);
}

void Player::SkillUpdate()
{
	for (auto iter = skillList.begin(); iter != skillList.end(); iter++)
	{
		iter->second->TimeUpdate();
	}
}

void Player::DeathUpdate()
{

}

void Player::MoveUpdate()
{
	if (state == State::ATTACKED || state == State::ATTACKED_JUMP)
	{
		// 피격후 경직시간동안 움직일 수 없다.
		if (elapsedAttackedTime <= HIT_STUN_TIME)
		{
			rigidbody->SetVelocity(Vector2D(0.0f, rigidbody->GetVelocity().y));
			return;
		}
		else
		{
			if (state == State::ATTACKED)
			{
				state = State::STAND;
			}
			else
			{
				state = State::JUMP;
			}
		}
	}

	// 대쉬중에는 다른동작을 무시한다.
	if (state == State::DASH || state == State::JUMPDASH)
	{
		if (elapsedDashTime > DASH_TIME)
		{
			if (state == State::DASH)
			{
				state = State::STAND;
			}
			else if (state == State::JUMPDASH)
			{
				state = State::JUMP;
			}

			rigidbody->SetAffectedByGravity(true);

			rigidbody->SetVelocity(Vector2D(0.0f, 0.0f));
		}

		return;
	}

	// 기본공격중에는 좌우로 이동할 수 없다.
	if (attackState == AttackState::NORMALATTACK || attackState == AttackState::COMBOATTACK || attackState == AttackState::FINALATTACK)
	{
		rigidbody->SetVelocity(Vector2D(0.0f, rigidbody->GetVelocity().y));
		return;
	}

	Vector2D movevector = rigidbody->GetVelocity();

	bool leftKey = Input::GetInstance().PressedKey(VK_LEFT);
	bool rightKey = Input::GetInstance().PressedKey(VK_RIGHT);
	bool jumpKey = Input::GetInstance().PressedKeyNow('C');

	// 연계공격 대기중 이동할 경우 연계공격이 취소된다.
	if ((attackState == AttackState::WAIT_COMBOATTACK || attackState == AttackState::WAIT_FINALATTACK) && (leftKey || rightKey || jumpKey))
	{
		attackState = AttackState::READY;
	}

	if (!leftKey && !rightKey)
	{
		movevector.x = 0.0f;
		if (state == State::RUN)
		{
			state = State::STAND;
		}
	}

	if (leftKey)
	{
		movevector.x = -MOVE_SPEED;
		if (state == State::STAND)
		{
			state = State::RUN;
		}
	}
	if (rightKey)
	{
		movevector.x = MOVE_SPEED;
		if (state == State::STAND)
		{
			state = State::RUN;
		}
	}
	if (jumpKey && (isReadyJump || GameDataManager::GetInstance().runningCheat))
	{
		movevector.y = -JUMP_SPEED;
		isReadyJump = false;

		state = State::JUMP;
	}

	if (movevector.x < 0.0f)
	{
		SetDirection(Vector2D::LEFT);
		sprite->SetFlipX(true);
	}
	if (movevector.x > 0.0f)
	{
		SetDirection(Vector2D::RIGHT);
		sprite->SetFlipX(false);
	}

	rigidbody->SetVelocity(movevector);
}

void Player::AttackUpdate()
{
	// 은신 지속시간 체크
	if (isStealth == true && elapsedStealthTime > STEALTH_DURATION)
	{
		StealthOff();
	}

	// 은신
	if (Input::GetInstance().PressedKeyNow('F'))
	{
		StealthOn();
	}

	attackKeyBuffer = attackKeyBuffer || Input::GetInstance().PressedKeyNow('Z');

	if (elapsedNomalAttackTime >= NORMAL_ATTACK_COOLTIME && attackState == AttackState::READY && Input::GetInstance().PressedKeyNow('Z'))
	{
		NormalAttack();
		StealthOff();
	}

	if (attackState == AttackState::NORMALATTACK)
	{
		if (elapsedLastAttackTime >= NORMAL_ATTACK_DELAY)
		{
			WaitComboAttack();
		}
	}
	else if (attackState == AttackState::WAIT_COMBOATTACK)
	{
		if (elapsedLastAttackTime >= WAIT_COMBO_ATTACK_DELAY)
		{
			// Normal Attack 이후 공격키를 눌렀을 경우
			if (attackKeyBuffer == true)
			{
				ComboAttack();
				StealthOff();
			}
			else
			{
				attackState = AttackState::READY;
			}
		}
	}
	else if (attackState == AttackState::COMBOATTACK)
	{
		if (elapsedLastAttackTime >= COMBO_ATTACK_DELAY)
		{
			WaitFinalAttack();
		}
	}
	else if (attackState == AttackState::WAIT_FINALATTACK)
	{
		if (elapsedLastAttackTime >= WAIT_FINAL_ATTACK_DELAY)
		{
			// Normal Attack 이후 공격키를 눌렀을 경우
			if (attackKeyBuffer == true)
			{
				FinalAttack();
				StealthOff();
			}
			else
			{
				attackState = AttackState::READY;
			}
		}
	}
	else if (attackState == AttackState::FINALATTACK)
	{
		if (elapsedLastAttackTime >= FINAL_ATTACK_DELAY)
		{
			attackState = AttackState::READY;
		}
	}
	else if (elapsedLastAttackTime >= ATTACK_DELAY)
	{
		attackState = AttackState::READY;
	}

	// 사용하지 않는 스킬 ThrowDagger
	//if (Input::GetInstance().PressedKeyNow('Q'))
	//{
	//	if (skillList[SKILL_ID_THROWDAGGER]->Usable())
	//	{
	//		skillList[SKILL_ID_THROWDAGGER]->Use();
	//		
	//		attackState = AttackState::THOROWDAGGER;
	//		elapsedLastAttackTime = 0.0f;
	//	}
	//}

	bool waitingcombo = (attackState == AttackState::WAIT_COMBOATTACK || attackState == AttackState::WAIT_FINALATTACK);

	if ((attackState == AttackState::READY || waitingcombo) && Input::GetInstance().PressedKeyNow('X'))
	{
		if (skillList[SKILL_ID_DASH]->Usable())
		{
			skillList[SKILL_ID_DASH]->Use();
			StealthOff();
		}
	}

	if ((attackState == AttackState::READY || waitingcombo) && Input::GetInstance().PressedKeyNow('A'))
	{
		if (skillList[SKILL_ID_THROWDAGGER_A]->Usable())
		{
			skillList[SKILL_ID_THROWDAGGER_A]->Use();
			StealthOff();
			isReadyDaggerA = false;

			attackState = AttackState::THOROWDAGGER;
			elapsedLastAttackTime = 0.0f;
		}
		else if (skillList[SKILL_ID_WITHDRAWDAGGER_A]->Usable())
		{
			skillList[SKILL_ID_WITHDRAWDAGGER_A]->Use();
			StealthOff();
			isNeedDaggerA = false;

			attackState = AttackState::WITHDRAWDAGGER;
			elapsedLastAttackTime = 0.0f;
		}
	}

	if ((attackState == AttackState::READY || waitingcombo) && Input::GetInstance().PressedKeyNow('S'))
	{
		if (skillList[SKILL_ID_THROWDAGGER_S]->Usable())
		{
			skillList[SKILL_ID_THROWDAGGER_S]->Use();
			StealthOff();
			isReadyDaggerS = false;

			attackState = AttackState::THOROWDAGGER;
			elapsedLastAttackTime = 0.0f;
		}
		else if (skillList[SKILL_ID_WITHDRAWDAGGER_S]->Usable())
		{
			skillList[SKILL_ID_WITHDRAWDAGGER_S]->Use();
			StealthOff();
			isNeedDaggerS = false;

			attackState = AttackState::WITHDRAWDAGGER;
			elapsedLastAttackTime = 0.0f;
		}
	}

	if (attackState == AttackState::READY && Input::GetInstance().PressedKeyNow('D'))
	{
		if (skillList[SKILL_ID_THROWDAGGER_D]->Usable())
		{
			skillList[SKILL_ID_THROWDAGGER_D]->Use();
			StealthOff();
			isReadyDaggerD = false;

			attackState = AttackState::THOROWDAGGER;
			elapsedLastAttackTime = 0.0f;
		}
		else if (skillList[SKILL_ID_WITHDRAWDAGGER_D]->Usable())
		{
			skillList[SKILL_ID_WITHDRAWDAGGER_D]->Use();
			StealthOff();
			isNeedDaggerD = false;

			attackState = AttackState::WITHDRAWDAGGER;
			elapsedLastAttackTime = 0.0f;
		}
	}
}

void Player::AnimationUpdate()
{
	switch (state)
	{
	case State::STAND:
		if (lastState != State::STAND)
		{
			animator->Play(L"Animation_Player_Stand");
		}

		break;

	case State::RUN:
		if (lastState != State::RUN)
		{
			animator->Play(L"Animation_Player_Run");
		}

		runSoundTimer += GetGameObject()->GetTimeController()->GetDeltaTime();

		if (runSoundTimer > RUN_SOUND_PERIOD)
		{
			runSoundTimer -= RUN_SOUND_PERIOD;

			SoundManager::GetInstance().Play(SOUND_EFFECT_WALK);
		}

		break;

	case State::JUMP:
		if (lastState != State::JUMP)
		{
			animator->Play(L"Animation_Player_Jump");
			//SoundManager::GetInstance().Play(SOUND_EFFECT_LANDING);

			// 점프 이펙트
			GameObject* jumpeffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
			jumpeffect->SetActive(true);

			EffectPlayer* effectanimator = jumpeffect->GetComponent<EffectPlayer>();
			effectanimator->Reset(L"Animation_Player_Jump_Effect");

			if (GetGameObject()->GetTransform()->GetLocalScale().x == 1.0f)
			{
				jumpeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(0.0f, 45.0f));
				jumpeffect->GetTransform()->SetLocalScale(GetGameObject()->GetTransform()->GetLocalScale());
			}
			else
			{
				jumpeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(0.0f, 35.0f));
				jumpeffect->GetTransform()->SetLocalScale(GetGameObject()->GetTransform()->GetLocalScale());
			}
		}

		break;

	default:

		break;
	}

	switch (attackState)
	{
	case Player::AttackState::READY:
		if (lastAttackState != AttackState::READY)
		{
			if (state == State::STAND)
			{
				animator->Play(L"Animation_Player_Stand");
			}
			else if (state == State::RUN)
			{
				animator->Play(L"Animation_Player_Run");
			}
		}
		break;

	case Player::AttackState::THOROWDAGGER:
		if (lastAttackState != AttackState::THOROWDAGGER)
		{
			if (state == State::STAND)
			{
				animator->Play(L"Animation_Player_ThrowDagger_Stand");
			}
			else if (state == State::RUN)
			{
				animator->Play(L"Animation_Player_ThrowDagger_Run");
			}
			else if (state == State::JUMP)
			{
				animator->Play(L"Animation_Player_ThrowDagger_Jump");
			}
		}
		break;

	case Player::AttackState::WITHDRAWDAGGER:

		break;

	default:
		break;
	}
}

void Player::SetDirection(Vector2D dir)
{
	if (normalAttackTrigger != nullptr && direction.x != dir.x)
	{
		Vector2D curpos = normalAttackTrigger->GetTransform()->GetLocalPosition();
		normalAttackTrigger->GetTransform()->SetLocalPosition(Vector2D(-curpos.x, curpos.y));
	}

	if (comboAttackTrigger != nullptr && direction.x != dir.x)
	{
		Vector2D curpos = comboAttackTrigger->GetTransform()->GetLocalPosition();
		comboAttackTrigger->GetTransform()->SetLocalPosition(Vector2D(-curpos.x, curpos.y));
	}

	if (finalAttackTrigger != nullptr && direction.x != dir.x)
	{
		Vector2D curpos = finalAttackTrigger->GetTransform()->GetLocalPosition();
		finalAttackTrigger->GetTransform()->SetLocalPosition(Vector2D(-curpos.x, curpos.y));
	}

	direction = dir;
}

float* Player::GetMaxHp()
{
	return &maxHealthPoint;
}

float* Player::GetCurHp()
{
	return &currentHealthPoint;
}

float* Player::GetMaxMp()
{
	return &maxMagicPoint;
}

float* Player::GetCurMp()
{
	return &currentMagicPoint;
}

void Player::SetGameOverPanel(GameObject* panel)
{
	gameoverpanel = panel;
}
