#include "NavigationSystem.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"
#include "Collider.h"
#include "Navigation.h"

using namespace std;

namespace BJEngine
{
	using namespace Math;

	void NavigationSystem::AddCollider(Collider* collider)
	{
		colliderList.push_back(collider);

		isOldTable = true;
	}
	
	void NavigationSystem::RemoveCollider(Collider* collider)
	{
		colliderList.erase(remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());

		isOldTable = true;
	}
	
	void NavigationSystem::AddNavigation(Navigation* navigation)
	{
		navigationList.push_back(navigation);

		isOldTable = true;
	}
	
	void NavigationSystem::RemoveNavigation(Navigation* navigation)
	{
		navigationList.erase(remove(navigationList.begin(), navigationList.end(), navigation), navigationList.end());

		isOldTable = true;
	}

	vector<Vector2D> NavigationSystem::FindShortestPath(Navigation* navigation, Vector2D destination)
	{
		if (isOldTable)
		{
			recreateTable();
			resetNodeEdge();
			resetCost();
		}
		else
		{
			unsigned long long currentframenumber = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetFrameNumber();

			if (currentframenumber == lastFrameNumber)
			{
				resetCost();
			}
			else
			{
				lastFrameNumber = currentframenumber;
				
				resetNodeEdge();
				resetCost();
			}
		}


		

		// not implement
		return vector<Vector2D>();
	}
	
	void NavigationSystem::resetNodeEdge()
	{
		// not implement
	}

	void NavigationSystem::resetCost()
	{
		// not implement
	}

	void NavigationSystem::recreateTable()
	{
		// colliderTable
		colliderTable.clear();

		for (int i = 0; i < colliderList.size(); i++)
		{
			for (int j = 0; j < navigationList.size(); j++)
			{
				if (colliderList[i]->GetGameObject() == navigationList[j]->GetGameObject())
				{
					colliderTable[navigationList[j]] = colliderList[i];
					break;
				}
			}
		}

		// navigationTable
		navigationTable.clear();

		for (int i = 0; i < colliderList.size(); i++)
		{
			for (int j = 0; j < navigationList.size(); j++)
			{
				if (colliderList[i]->GetGameObject() == navigationList[j]->GetGameObject())
				{
					navigationTable[colliderList[i]] = navigationList[j];
					break;
				}
			}
		}

		isOldTable = false;
	}
	
	NavigationSystem& NavigationSystem::GetInstance()
	{
		static NavigationSystem instance;
		return instance;
	}

	NavigationSystem::NavigationSystem() : isOldTable(true), lastFrameNumber(0ull)
	{

	}

	NavigationSystem::~NavigationSystem()
	{

	}
}