#include "TwinklingTest.h"

#include <random>
#include <cmath>
using namespace std;

void TwinklingTest::Awake()
{
	transform = GetGameObject()->GetTransform();
	originPosition = transform->GetLocalPosition().y;
	originScale = transform->GetLocalScale().y;

	counter = 0.0f;
}

void TwinklingTest::Update()
{
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();

	counter += deltaTime;

	if (counter > period)
	{
		counter -= period;

		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<float> range(minPeriod, maxPeriod);
		float randomValue = range(mt);
		period = randomValue;
	}

	float pi = acos(-1);

	2 * pi / period;

	float sfunc = pow(sin(2 * pi * counter / period), 16.0f);

	float currentScale = transform->GetLocalScale().y;
	float targetScale = minScale + sfunc * (maxScale - minScale) / 2.0f;

	transform->SetLocalPosition(Vector2D(transform->GetLocalPosition().x, originPosition + (targetScale - originScale) * imageSize / 2.0f));
	transform->SetLocalScale(transform->GetLocalScale() + Vector2D(0.0f, targetScale - currentScale));
}
