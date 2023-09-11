#include "Tracer.h"

void Tracer::SetTarget(Transform* target, Vector2D tracearea, Vector2D sceneLT, Vector2D sceneRB, bool usescenetrace)
{
	targetTransform = target;
	traceArea = tracearea;
	sceneLeftTop = sceneLT;
	sceneRightBottom = sceneRB;
	usingSceneTrace = usescenetrace;
}

void Tracer::Awake()
{
	ownTransform = GetGameObject()->GetTransform();
}

void Tracer::LateUpdate()
{
	Vector2D ownposition = ownTransform->GetLocalPosition();
	Vector2D targetposition = targetTransform->GetWorldPosition();

	if (targetposition.x < ownposition.x - (traceArea.x / 2.0f))
	{
		ownposition.x = targetposition.x + (traceArea.x / 2.0f);
	}
	if (ownposition.x + (traceArea.x / 2.0f) < targetposition.x)
	{
		ownposition.x = targetposition.x - (traceArea.x / 2.0f);
	}
	if (targetposition.y < ownposition.y - (traceArea.y / 2.0f))
	{
		ownposition.y = targetposition.y + (traceArea.y / 2.0f);
	}
	if (ownposition.y + (traceArea.y / 2.0f) < targetposition.y)
	{
		ownposition.y = targetposition.y - (traceArea.y / 2.0f);
	}

	if (usingSceneTrace == true)
	{
		if (ownposition.x < sceneLeftTop.x)
		{
			ownposition.x = sceneLeftTop.x;
		}
		if (sceneRightBottom.x < ownposition.x)
		{
			ownposition.x = sceneRightBottom.x;
		}
		if (ownposition.y < sceneLeftTop.y)
		{
			ownposition.y = sceneLeftTop.y;
		}
		if (sceneRightBottom.y < ownposition.y)
		{
			ownposition.y = sceneRightBottom.y;
		}
	}

	ownTransform->SetLocalPosition(ownposition);
}

void Tracer::SetUsingSceneTrace(bool value)
{
	usingSceneTrace = value;
}

bool Tracer::GetUsingSceneTrace()
{
	return usingSceneTrace;
}
