#pragma once
#include "Component.h"

#include "BJMath.h"
#include "Vector2D.h"
#include "Vector3D.h"

class Background : public BJEngine::Component
{
public:
	Background();
	~Background();

	void Awake();
	void Update();

	void SetCameraDepth(float depth);
	void SetImageZaxisPosition(float zPos);

private:
	BJEngine::Math::Vector3D m_cameraPosition;
	BJEngine::Math::Vector3D m_imagePosition;
	float m_speed; // 0.f ~ 1.f;
	float m_depth;
	float m_imageZpos;
};


/*
* ���̸� ������
* ��� �׸��� �����̴� �ӵ��� �����ϰ� ������
*/