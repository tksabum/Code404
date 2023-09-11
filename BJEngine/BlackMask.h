#pragma once
#include "Component.h"

#include "Vector2D.h"
#include "Vector3D.h"
#include <string>

using namespace BJEngine;

class BlackMask : public BJEngine::Component
{
public:
	enum class eMode
	{
		SCENE_IN, // 새로운 씬에서 시작할 때 쓰는 모드
		SCENE_OUT, // 새로운 씬으로 이동할 때 쓰는 모드
	};

	virtual void Awake() override;
	virtual void Update() override;

	void SetScene(std::wstring scene);
	void SetScene(int scene);

	void SetMode(eMode mode);

	void SetStartPoint(BJEngine::Math::Vector2D start);
	void SetEndPoint(BJEngine::Math::Vector2D end);

	void SetSpeed(float speed);
	void SetAccelerator(float accel);

private:
	eMode sceneMode;
	bool isEnd;
	std::wstring nextScene;
	int nextSceneID;
	float moveSpeed = 2000.f;
	float accelerator = 0.f;

private:
	Math::Vector2D startPoint;
	Math::Vector2D endPoint;
};

