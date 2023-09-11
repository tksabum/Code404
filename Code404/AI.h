#pragma once
#include "Component.h"

#include <map>

class State;
namespace BJEngine
{
	class Collider;
}

enum class AI_STATE
{
	NONE,
	IDLE,
	PATROL,
	TRACETARGET,
	NORMALATTACK,
	HIT,
	DIE,
	END
};

enum class ENEMY_TYPE
{
	MeleeA_Enemy,
	MeleeB_Enemy,
	RangeA_Enemy,
	RangeB_Enemy
};

class ParryingSystem;

class AI :
	public BJEngine::Component
{
private:
	std::map<AI_STATE, State*> mapState;

	State* curState;
	AI_STATE eCurState;
	State* prevState;
	AI_STATE ePrevState;

	ENEMY_TYPE enemyType;

	ParryingSystem* parryingSystem;

public:
	void CreateState(AI_STATE aistate, State* paistate);
	void ChangeState(AI_STATE nextstate);
	State* GetState(AI_STATE aistate);

	void SetEnemyType(ENEMY_TYPE type);
	ENEMY_TYPE GetEnemyType();

	State* GetPrevState();
public:
	std::map<AI_STATE, State*>& GetMapState();

public:
	virtual void Awake() override;
	virtual void Update() override;

public:
	AI();
	~AI();
};

