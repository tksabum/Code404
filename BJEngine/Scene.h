#pragma once

#ifdef SCENE_EXPORTS
#define SCENE_API __declspec(dllexport)
#else
#define SCENE_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

namespace BJEngine
{
	class Camera;
	class SceneManager;
	class GameObject;
	class Component;
	class TimeController;

	class Scene
	{
	public:
		// 월드에 transform만을 가진 게임오브젝트 생성
		SCENE_API GameObject* CreateGameObject(std::wstring gameobjectname);
		//
		SCENE_API GameObject* CreateGameObject(GameObject* parentobject, std::wstring gameobjectname);

		// 프리펩을 이용하여 게임오브젝트 생성 (int는 나중에 적절한 타입으로 변경할 예정)
		SCENE_API void CreateGameObject(int);

		// 게임오브젝트 제거
		SCENE_API void DeleteGameObject(GameObject* gameobject);

		SCENE_API GameObject* GetGameObject(std::wstring gameobjectname);

		SCENE_API TimeController* GetTimeController();

		SCENE_API Camera* GetMainCamera();
		SCENE_API void SetMainCamera(Camera* camera);
		SCENE_API std::wstring GetName();
	
	// Serialization 기능을 구현하기 전까지 private -> protected로 사용
	protected:
		friend SceneManager;

		SCENE_API Scene(std::wstring scenename = L"");
		SCENE_API virtual ~Scene();

		void Initialize(TimeController* timecontroller);
		std::wstring name;
		void InitializeEvent();
		void DestroyEvent();
		void PhysicsEvent();
		void EarlyUpdateEvent();
		void UpdateEvent();
		void LateUpdateEvent();
		void RenderEvent();

		TimeController* timeController;
		
		std::vector<GameObject*> gameobjectList;
		Camera* mainCamera; // 현재 화면을 띄울 때 사용하는 카메라
		Camera* defaultCamera; // 씬이 시작될때 mainCamera로 설정되는 카메라



		// 임시로 Serialization 없이 여러 씬의 사용을 위한 코드
	public:
		// 씬 시작시 오브젝트 배치를 위한 함수, 씬의 생성자에서 카메라오브젝트를 하나 생성하는 것을 주의할 것, game
		SCENE_API virtual void Start() abstract;
		
		SCENE_API void DeleteAllObject();
	};
}