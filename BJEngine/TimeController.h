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
		float timeScale; // deltaTime의 크기를 조정하여 게임에서 흐르는 시간을 조정할 수 있다.
		std::unordered_map<std::string, std::chrono::system_clock::time_point> timetable;
		std::unordered_map<std::string, float> deltaTime;
		std::unordered_map<std::string, float> eventDeltaTime;
		int framePerSecond; // 목표 초당 프레임 수
		float physicsLoopTime;
		unsigned long long frameNumber;

		std::string mainTime;
		std::string physicsTime;

		float GetDeltaTime(std::string timename);
		float GetEventDeltaTime(std::string timename);

		// 현재시간을 기록하고 이전시간과의 차이를 deltaTime에 저장해둔다.
		void Update(std::string timename);
		// 현재시간과 이전시간과의 시간차이(timeScale과 관계없음)를 반환받음
		float GetTimeDif(std::string timename);

		float GetFPS();
		float GetPhysicsLoopTime();

		void CountFrameNumber();

		friend CoreSystem;
	};
}

