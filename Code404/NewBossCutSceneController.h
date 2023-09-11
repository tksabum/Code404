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
	// 손 오브젝트
	GameObject* leftHand;
	GameObject* rightHand;

	// 눈 변수
	float waitTime = 3.f;;
	float totalWaitTime;
	bool isWait;
	SpriteRenderer* eyeRenderer;
	bool isEye;

	float eyeWaitTime = 2.f;
	float totalEyeWaitTime;
	bool isEyeWait;

	// 머리 변수
	SpriteRenderer* headRenderer;
	float headOpacity;
	float headOpacitySpeed = 0.2f;
	bool isHead;

	// 손 변수
	SpriteRenderer* leftRenderer;
	SpriteRenderer* rightRenderer;
	float handOpacity;
	float handOpacitySpeed = 0.2f;
	bool isHand;

	// 블랙마스크 변수
	SpriteRenderer* blackMask;
	float blackMaskOpacity =1.f;
	float blackMaskOpacitySpeed = 0.2f;
	bool isBlackMask;

	// 손 부딪히기 변수
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

