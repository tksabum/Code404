#include "EnemyDebug.h"

#include "Enemy.h"

#include <string.h>

using namespace std;

void EnemyDebug::Awake()
{
	text = GetGameObject()->GetComponent<Text>();
}

void EnemyDebug::Update()
{
	Enemy* enemy = nullptr;
	if (GetGameObject()->GetScene()->GetGameObject(L"enemy") != nullptr)
	{
		enemy = GetGameObject()->GetScene()->GetGameObject(L"enemy")->GetComponent<Enemy>();
	}

	wstring str;
	str += L"CurHP : " + to_wstring(enemy->GetCurHP());

	text->SetText(str);
}
