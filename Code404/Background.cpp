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
	// ��׶��带 ���� ������Ʈ�� ī�޶� �θ� ������Ʈ�� ���� ������.
	m_cameraPosition = GetGameObject()->GetParent()->GetTransform()->GetLocalPosition();
	m_imagePosition = GetGameObject()->GetTransform()->GetLocalPosition();
}

void Background::Update()
{
	// �ӵ��� ������Ʈ��.
	m_speed = ((m_depth - m_imageZpos) / m_depth);

	// ���� ī�޶� ��ġ�κ��� ���� �󸶳� �̵��ߴ°� �Ǵ���.
	Vector2D newLocalPosition = GetGameObject()->GetParent()->GetTransform()->GetLocalPosition() - m_cameraPosition;

	// �̵��� ��ŭ �׸��� ���� �������� �Ű�����ϴµ�
	// �̰� �׳� ������ ���� �ӷ��� ������ �̵��ϴ� ���� �Ǿ���� �̰� �����?
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
