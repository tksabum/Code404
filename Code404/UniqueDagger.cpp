#include "UniqueDagger.h"

#include "Skill.h"

#include "Code404.h"
#include "Enemy.h"

#include "ObjectPoolSystem.h"
#include "EffectPlayer.h"
#include "SoundManager.h"

const float UniqueDagger::PROJECTILE_SPEED = 2500.0f;
const float UniqueDagger::WITHDRAWING_SPEED = 2500.0f;
const float UniqueDagger::MAX_DISTANCE_FROM_USER = 2000.0f;
const float UniqueDagger::KNIFE_SIZE = 0.7f;
const float UniqueDagger::THROW_DAMAGE = 10.0f;
const float UniqueDagger::WITHDRAW_DAMAGE = 50.0f;

void UniqueDagger::Awake()
{
	if (owncollider == nullptr)
	{
		owncollider = GetGameObject()->GetComponent<ConvexpolygonCollider>();
	}

	if (rigidbody == nullptr)
	{
		rigidbody = GetGameObject()->GetComponent<Rigidbody2D>();
	}

	if (animator == nullptr)
	{
		animator = GetGameObject()->GetComponent<Animator>();
	}

	isWithdrawing = false;
}

void UniqueDagger::OnEnable()
{
	GetGameObject()->GetTransform()->SetLocalPosition(userTransform->GetWorldPosition());

	owncollider->SetTrigger(true);
	state = State::THROWING;

	float playerscale = userTransform->GetLocalScale().x;

	if (userDirection->x < 0.0f)
	{
		GetGameObject()->GetTransform()->SetLocalScale(Vector3D(-KNIFE_SIZE * playerscale, KNIFE_SIZE * playerscale, 1.0f));
	}
	else
	{
		GetGameObject()->GetTransform()->SetLocalScale(Vector3D(KNIFE_SIZE * playerscale, KNIFE_SIZE * playerscale, 1.0f));
	}

	rigidbody->SetVelocity((*userDirection) * PROJECTILE_SPEED);
	animator->Play(stuckAnimation);
}

void UniqueDagger::SetUniqueDagger(Vector2D* userdirection, Transform* usertransform, bool* ready, bool* need, Skill* throwskill)
{
	userDirection = userdirection;
	userTransform = usertransform;
	isReady = ready;
	isNeed = need;
	throwSkill = throwskill;
}

void UniqueDagger::SetAnimation(wstring stuckanimation, wstring flyinganimation)
{
	stuckAnimation = stuckanimation;
	flyingAnimation = flyinganimation;
}

void UniqueDagger::Update()
{
	if (state == State::THROWING)
	{
		Vector2D daggerpos = GetGameObject()->GetTransform()->GetWorldPosition();
		Vector2D userpos = userTransform->GetWorldPosition();

		if ((daggerpos - userpos).Size() > MAX_DISTANCE_FROM_USER)
		{
			*isReady = true;
			state = State::READY;
			GetGameObject()->SetActive(false);
		}
	}
	else if (state == State::HOLDING)
	{
		Vector2D daggerpos = GetGameObject()->GetTransform()->GetWorldPosition();
		Vector2D userpos = userTransform->GetWorldPosition();

		if ((daggerpos - userpos).Size() > MAX_DISTANCE_FROM_USER)
		{
			*isReady = true;
			state = State::READY;
			throwSkill->ResetCooltime();
			GetGameObject()->SetActive(false);
		}
	}
	else if (state == State::WITHDRAWING)
	{
		rigidbody->SetVelocity((userTransform->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).GetUnitVector() * WITHDRAWING_SPEED);
	}
}

void UniqueDagger::LateUpdate()
{
	if (isWithdrawing == true)
	{
		WithdrawingEvent();
	}
}

void UniqueDagger::OnTriggerEnter(Collider* collider)
{
	TriggerEvent(collider);
}

void UniqueDagger::OnTriggerStay(Collider* collider)
{
	TriggerEvent(collider);
}

void UniqueDagger::Withdraw()
{
	if (state == State::HOLDING)
	{
		attackedColliders.clear();

		state = State::WITHDRAWING;
		owncollider->SetTrigger(true);

		animator->Play(flyingAnimation);
	}
}

void UniqueDagger::ThrowAttack(Collider* collider)
{
	collider->GetGameObject()->GetComponent<Enemy>()->Attacked(PLAYERATTACK_TYPE::DAGGER, THROW_DAMAGE);
	SoundManager::GetInstance().Play(SOUND_EFFECT_OBJECTHIT);
}

void UniqueDagger::WithdrawAttack(Collider* collider)
{
	collider->GetGameObject()->GetComponent<Enemy>()->Attacked(PLAYERATTACK_TYPE::DAGGER, WITHDRAW_DAMAGE);
	SoundManager::GetInstance().Play(SOUND_EFFECT_OBJECTHIT);
}

void UniqueDagger::TriggerEvent(Collider* collider)
{
	if (state == State::THROWING && collider->GetGameObject()->GetLayerName() == L"Platform")
	{
		rigidbody->SetVelocity(Vector2D(0.0f, 0.0f));

		state = State::HOLDING;
		owncollider->SetTrigger(false);
		Vector2D collisionvector = owncollider->GetLastCollisionVector();
		Vector2D xvector = Vector2D(collisionvector.x, 0.0f);
		Vector2D movevector = Vector2D(0.0f, 0.0f);

		SoundManager::GetInstance().Play(SOUND_EFFECT_OBJECTHIT);

		if (xvector.Size() != 0.0f)
		{
			movevector = xvector.GetUnitVector() * collisionvector.Size() * (collisionvector.Size() / xvector.Size());
		}

		if (DotProduct(GetGameObject()->GetTransform()->GetWorldPosition() - userTransform->GetWorldPosition(), movevector) > 0.0f)
		{
			movevector = movevector + (-movevector.GetUnitVector() * (64.0f + 80.0f * KNIFE_SIZE));
		}

		GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movevector);
		*isNeed = true;

		animator->Play(stuckAnimation);

		PlayCollisionEffect();
	}

	if (state == State::WITHDRAWING && (collider->GetGameObject()->GetLayerName() == L"Player" || collider->GetGameObject()->GetLayerName() == L"StealthPlayer"))
	{
		// 회수
		isWithdrawing = true;
	}

	// Throw Attack
	if (state == State::THROWING && (collider->GetGameObject()->GetLayerName() == L"Enemy" || collider->GetGameObject()->GetLayerName() == L"Boss"))
	{
		ThrowAttack(collider);

		*isReady = true;
		state = State::READY;
		GetGameObject()->SetActive(false);

		PlayCollisionEffect();
	}

	// Withdraw Attack
	if (state == State::WITHDRAWING && (collider->GetGameObject()->GetLayerName() == L"Enemy" || collider->GetGameObject()->GetLayerName() == L"Boss"))
	{
		if (attackedColliders.count(collider) == 0)
		{
			attackedColliders.insert(collider);

			WithdrawAttack(collider);

			PlayCollisionEffect();
		}
	}
}

void UniqueDagger::WithdrawingEvent()
{
	*isReady = true;
	state = State::READY;
	throwSkill->ResetCooltime();
	GetGameObject()->SetActive(false);

	isWithdrawing = false;
}

void UniqueDagger::PlayCollisionEffect()
{
	// 충돌 이펙트
	GameObject* jumpeffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
	jumpeffect->SetActive(true);

	EffectPlayer* effectanimator = jumpeffect->GetComponent<EffectPlayer>();
	effectanimator->Reset(L"Animation_Dagger_Collision_Effect");

	float effectdistance = 40.0f;
	float effectfixedY = 6.0f;
	float effectsize = 2.0f;

	if (GetGameObject()->GetTransform()->GetLocalScale().x > 0.0f)
	{
		jumpeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(effectdistance, effectfixedY * effectsize));
	}
	else
	{
		jumpeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(-effectdistance, effectfixedY * effectsize));
	}
	
	jumpeffect->GetTransform()->SetLocalScale(GetGameObject()->GetTransform()->GetLocalScale() * effectsize);
}