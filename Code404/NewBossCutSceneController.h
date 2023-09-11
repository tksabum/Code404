#pragma once
#include "Component.h"

using namespace BJEngine;

namespace BJEngine
{
	class SpriteRenderer;
}

class NewBossCutSceneController :
	public BJEngine::Component
{
private:
	// �� ������Ʈ
	GameObject* leftHand;
	GameObject* rightHand;

	// �� ����
	float waitTime = 3.f;;
	float totalWaitTime;
	bool isWait;
	SpriteRenderer* eyeRenderer;
	bool isEye;

	float eyeWaitTime = 2.f;
	float totalEyeWaitTime;
	bool isEyeWait;

	// �Ӹ� ����
	SpriteRenderer* headRenderer;
	float headOpacity;
	float headOpacitySpeed = 0.2f;
	bool isHead;

	// �� ����
	SpriteRenderer* leftRenderer;
	SpriteRenderer* rightRenderer;
	float handOpacity;
	float handOpacitySpeed = 0.2f;
	bool isHand;

	// ������ũ ����
	SpriteRenderer* blackMask;
	float blackMaskOpacity =1.f;
	float blackMaskOpacitySpeed = 0.2f;
	bool isBlackMask;

	// �� �ε����� ����
	bool isCrash;
	int crashCount;
	float crashSpeed = 450;
	int maxCrashCount = 3;
	bool isEffect;

public:
	void SetEye(SpriteRenderer* eye);
	void SetHead(SpriteRenderer* head);
	void SetLeft(SpriteRenderer* left);
	void SetRight(SpriteRenderer* right);
	void SetMask(SpriteRenderer* mask);


public:
	virtual void Awake() override;
	virtual void Update() override;

};

