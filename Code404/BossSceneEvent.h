#pragma once
#include "Component.h"

using namespace BJEngine;

namespace BJEngine
{
	class GameObject;
	class Image;
}

class Enemy;

class BossSceneEvent :
	public Component
{
private:
	BJEngine::GameObject* bossObject;
	Enemy* bossEnemyComponent;

	Image* blackMask;
	Image* eyeRenderer;
	Image* headRenderer;
	Image* leftHandRenderer;
	Image* rightHandRenderer;
	Image* cutSceneRenderer;

	bool isWait;
	float waitTime = 2.f;
	float totalWaitTime;

	bool isEyeOpen;
	float openEyeTime = 4.f;
	float totalEyeTime;

	bool isOpacity;
	float opacity;
	float opacitySpeed = 0.2f;

	bool isHadnOpacity;

	bool isBossDie = false;
	float maskSpeed = 0.2f;
	float maskOpacity = 0.f;
public:
	void SetBlackMask(Image* renderer);
	void SetEyeRenderer(Image* renderer);
	void SetHeadRenderer(Image* renderer);
	void SetLeftHandRenderer(Image* renderer);
	void SetRightHandRenderer(Image* renderer);
	void SetBossObject(BJEngine::GameObject* boss);
	void SetCutSceneRenderer(Image* renderer);

public:
	virtual void Awake() override;
	virtual void Update() override;
};

