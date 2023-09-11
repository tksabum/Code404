#include "Background.h"

#include "GameObject.h"
#include "Transform.h"

using namespace BJEngine;
using namespace BJEngine::Math;

Background::Background() : m_depth(1.f), m_imageZpos(1.f)
{

}

Background::~Background()
{

}

void Background::Awake()
{
	// 백그라운드를 가진 오브젝트는 카메라를 부모 오브젝트로 가질 예정임.
	m_cameraPosition = GetGameObject()->GetParent()->GetTransform()->GetLocalPosition();
	m_imagePosition = GetGameObject()->GetTransform()->GetLocalPosition();
}

void Background::Update()
{
	// 속도를 업데이트함.
	m_speed = ((m_depth - m_imageZpos) / m_depth);

	// 시작 카메라 위치로부터 지금 얼마나 이동했는가 판단함.
	Vector2D newLocalPosition = GetGameObject()->GetParent()->GetTransform()->GetLocalPosition() - m_cameraPosition;

	// 이동한 만큼 그림의 로컬 포지션을 옮겨줘야하는데
	// 이걸 그냥 깡으로 쓰면 속력을 가지고 이동하는 꼴이 되어버림 이걸 어떻게함?
	GetGameObject()->GetTransform()->SetLocalPosition(m_imagePosition - (newLocalPosition * m_speed));
}

void Background::SetCameraDepth(float depth)
{
	if (depth < 0.f)
	{
		depth = 0.f;
	}
	m_depth = depth;
}

void Background::SetImageZaxisPosition(float zPos)
{
	if (zPos > m_depth)
	{
		m_imageZpos = m_depth;
		return;
	}
	if (zPos < 0.f)
	{
		m_imageZpos = 0.f;
		return;
	}
	m_imageZpos = zPos;
}
