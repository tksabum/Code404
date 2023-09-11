#pragma once
#include "Component.h"

namespace BJEngine
{
    class GameObject;
    class SpriteRenderer;
}


class EndCutSceneController :
    public BJEngine::Component
{
    // �ƾ�1�� ���� ������
    BJEngine::GameObject* cutScene1;
    BJEngine::SpriteRenderer* cutScene1Renderer;
    bool isCutScene1 = true;
    float cutScene1Opacity;
    float cutScene1OpacitySpeed = 0.3f;

    // �ƾ�2�� ���� ������
	BJEngine::GameObject* cutScene2;
	BJEngine::SpriteRenderer* cutScene2Renderer;
	bool isCutScene2 = false;
	float cutScene2Opacity;
	float cutScene2OpacitySpeed = 0.2f;

	// �ƾ�3�� ���� ������
	BJEngine::GameObject* cutScene3;
	BJEngine::SpriteRenderer* cutScene3Renderer;
	bool isCutScene3 = false;
	float cutScene3Opacity;
	float cutScene3OpacitySpeed = 0.4f;

	// �� ����ũ�� ���� ������
	BJEngine::GameObject* blackMask;
	BJEngine::SpriteRenderer* blackMaskRenderer;
	bool isblackMask = false;
	float blackMaskOpacity;
	float blackMaskOpacitySpeed = 0.3f;

    // �ƾ�4�� ���� ������
	BJEngine::GameObject* cutScene4;
	BJEngine::SpriteRenderer* cutScene4Renderer;
	bool isCutScene4 = false;
	float cutScene4Opacity;
	float cutScene4OpacitySpeed = 0.3f;

	// �ƹ� Ű ������ ���ð�
	bool isAnyWait;
	float anyWaitTime = 2.f;
	float totalAnyWaitTime;
	

	// ���� ȭ������ ���� ���� ����
	BJEngine::GameObject* changeMask;
	BJEngine::SpriteRenderer* changeMaskRenderer;
	bool ischangeMask = false;
	float changeMaskOpacity;
	float changeMaskOpacitySpeed = 0.2f;

	BJEngine::GameObject* anyKey;
	BJEngine::SpriteRenderer* anyKeyRenderer;
	bool isAnyKeyDraw = false;
	float anyKeyOpacity;
	float anyKeyOpacitySpeed = 0.7f;
	bool isPingPong;

	// �ƹ�Ű�� ��������
	bool isAnykey;
	bool isChangeMainTitle;

public:
    void SetCutScene1(BJEngine::GameObject* cutscene1);
    void SetCutScene2(BJEngine::GameObject* cutscene2);
    void SetCutScene3(BJEngine::GameObject* cutscene3);
    void SetCutScene4(BJEngine::GameObject* cutscene4);
    void SetBlackMask(BJEngine::GameObject* blackmask);
    void SetChangekMask(BJEngine::GameObject* changemask);
	void SetAnyKek(BJEngine::GameObject* anykey);

public:
    virtual void Awake() override;
    virtual void Update() override;
};

