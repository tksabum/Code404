#include "AnimationToScene.h"

#include "GameObject.h"
#include "Animator.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace BJEngine;

void AnimationToScene::Awake()
{
	anim = GetGameObject()->GetComponent<Animator>();
}

void AnimationToScene::Update()
{
	if (anim->IsWait())
	{
		CoreSystem::GetInstance().GetSceneManager()->LoadScene(L"MainScene");
	}
}
