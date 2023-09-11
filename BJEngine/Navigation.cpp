#include "Navigation.h"

#include "NavigationSystem.h"

using namespace std;

namespace BJEngine
{
	using namespace Math;

	Navigation::Navigation()
	{
		NavigationSystem::GetInstance().AddNavigation(this);
	}

	Navigation::~Navigation()
	{
		NavigationSystem::GetInstance().RemoveNavigation(this);
	}
	
	vector<Vector2D> Navigation::FindShortestPath(Vector2D destination)
	{
		return NavigationSystem::GetInstance().FindShortestPath(this, destination);
	}

	void Navigation::SetIsMoving(bool value)
	{
		isMoving = value;
	}
	
	bool Navigation::GetIsMoving()
	{
		return isMoving;
	}
}