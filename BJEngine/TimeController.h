#pragma once

#ifdef TIMECONTROLLER_EXPORTS
#define TIMECONTROLLER_API __declspec(dllexport)
#else
#define TIMECONTROLLER_API __declspec(dllimport)
#endif

#include <chrono>
#include <string>
#include <unordered_map>

namespace BJEngine
{
	class CoreSystem;

	class TimeController
	{
	public:
		static const float TIMESCALE_DEFAULT;
		static const float TIMESCALE_STOP;
		static const float PHYSICS_LOOP_TIME_DEFAULT;
		static const int FPS_DEFAULT;

		TIMECONTROLLER_API TimeController(std::string maintime, std::string physicstime);
		TIMECONTROLLER_API ~TimeController();

		TIMECONTROLLER_API float GetDeltaTime();
		TIMECONTROLLER_API float GetPhysicsDeltaTime();

		TIMECONTROLLER_API float GetEventDeltaTime();

		TIMECONTROLLER_API void SetTimeScale(float timescale);
		TIMECONTROLLER_API float GetTimeScale();

		TIMECONTROLLER_API unsigned long long GetFrameNumber();


	private:
		float timeScale; // deltaTime�� ũ�⸦ �����Ͽ� ���ӿ��� �帣�� �ð��� ������ �� �ִ�.
		std::unordered_map<std::string, std::chrono::system_clock::time_point> timetable;
		std::unordered_map<std::string, float> deltaTime;
		std::unordered_map<std::string, float> eventDeltaTime;
		int framePerSecond; // ��ǥ �ʴ� ������ ��
		float physicsLoopTime;
		unsigned long long frameNumber;

		std::string mainTime;
		std::string physicsTime;

		float GetDeltaTime(std::string timename);
		float GetEventDeltaTime(std::string timename);

		// ����ð��� ����ϰ� �����ð����� ���̸� deltaTime�� �����صд�.
		void Update(std::string timename);
		// ����ð��� �����ð����� �ð�����(timeScale�� �������)�� ��ȯ����
		float GetTimeDif(std::string timename);

		float GetFPS();
		float GetPhysicsLoopTime();

		void CountFrameNumber();

		friend CoreSystem;
	};
}

