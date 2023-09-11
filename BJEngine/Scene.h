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
		// ���忡 transform���� ���� ���ӿ�����Ʈ ����
		SCENE_API GameObject* CreateGameObject(std::wstring gameobjectname);
		//
		SCENE_API GameObject* CreateGameObject(GameObject* parentobject, std::wstring gameobjectname);

		// �������� �̿��Ͽ� ���ӿ�����Ʈ ���� (int�� ���߿� ������ Ÿ������ ������ ����)
		SCENE_API void CreateGameObject(int);

		// ���ӿ�����Ʈ ����
		SCENE_API void DeleteGameObject(GameObject* gameobject);

		SCENE_API GameObject* GetGameObject(std::wstring gameobjectname);

		SCENE_API TimeController* GetTimeController();

		SCENE_API Camera* GetMainCamera();
		SCENE_API void SetMainCamera(Camera* camera);
		SCENE_API std::wstring GetName();
	
	// Serialization ����� �����ϱ� ������ private -> protected�� ���
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
		Camera* mainCamera; // ���� ȭ���� ��� �� ����ϴ� ī�޶�
		Camera* defaultCamera; // ���� ���۵ɶ� mainCamera�� �����Ǵ� ī�޶�



		// �ӽ÷� Serialization ���� ���� ���� ����� ���� �ڵ�
	public:
		// �� ���۽� ������Ʈ ��ġ�� ���� �Լ�, ���� �����ڿ��� ī�޶������Ʈ�� �ϳ� �����ϴ� ���� ������ ��, game
		SCENE_API virtual void Start() abstract;
		
		SCENE_API void DeleteAllObject();
	};
}