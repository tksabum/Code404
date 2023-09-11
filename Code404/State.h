#pragma once

class AI;

namespace BJEngine
{
	class Animator;
}

class Enemy;

class State
{
private:
	// ���� ���� AI ������Ʈ
	AI* aiComponent;

	BJEngine::Animator* animator;

protected:
	Enemy* enemyComponent;

public:
	AI* GetAI();
	BJEngine::Animator* GetAnimator();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

	virtual void AlwaysUpdate();

	virtual bool IsParryingTime();

public:
	State(AI* ai);
	virtual ~State();

	friend class AI;
};

