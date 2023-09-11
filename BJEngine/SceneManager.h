#pragma once

#ifdef SCENEMANAGER_EXPORTS
#define SCENEMANAGER_API __declspec(dllexport)
#else
#define SCENEMANAGER_API __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

namespace BJEngine
{
	class Scene;
	class CoreSystem;
	class TimeController;

	class SceneManager
	{
	public:
		SCENEMANAGER_API SceneManager();
		SCENEMANAGER_API ~SceneManager();

		// Scene�� �����ϰ� SceneID ��ȯ
		SCENEMANAGER_API int CreateScene(std::wstring name) = delete; // Serialization ��� ���������� ������ ����
		SCENEMANAGER_API void DeleteScene(int sceneID);
		SCENEMANAGER_API void DeleteScene(Scene* scene);

		SCENEMANAGER_API Scene* GetScene(int sceneID);

		SCENEMANAGER_API Scene* GetCurrentScene();
		SCENEMANAGER_API int GetCurrentSceneID();
		

	private:
		void Initialize(TimeController* timecontroller);

		void InitializeEvent();
		void DestroyEvent();
		void PhysicsEvent();
		void EarlyUpdateEvent();
		void UpdateEvent();
		void LateUpdateEvent();
		void RenderEvent();

		TimeController* timeController;
		std::vector<Scene*> sceneList;
		std::vector<std::wstring> sceneNameList;
		
		int currentScene;
		int defaultScene;

		static const int DEFAULTSCENE_DEFAULT;

		friend CoreSystem;



		// �ӽ÷� Serialization ���� ���� ���� ����� ���� �ڵ�
	public:
		// scene�� ����Ʈ�� �߰��ϰ� sceneID�� �����Ѵ�. �ߺ��� �̸��� Scene�� �߰��� ��� ����Ʈ�� �߰����� ������ -1�� ����
		SCENEMANAGER_API int AddScene(Scene* scene);

		SCENEMANAGER_API void LoadScene(std::wstring scenename);
		SCENEMANAGER_API void LoadScene(int sceneID);

	private:
		// unordered_map<scenename, sceneID>
		std::unordered_map<std::wstring, int> sceneTable;
		std::queue<int> nextSceneQueue;

		void ChangeScene(int sceneID);

	};
}