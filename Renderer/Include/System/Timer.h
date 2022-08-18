#pragma once
#include <chrono>
#include "System/Types.h"
#include <string>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> High_Res_TimePoint;

// Simple timer to track elapsed time
class Time
{
private:
	High_Res_TimePoint m_GameStart;
	High_Res_TimePoint m_CurrentFrameTime;
	High_Res_TimePoint m_PreviousFrameTime;
	High_Res_TimePoint m_OneSecondTimer;

	float m_DeltaTime = 0;
	int   m_FPS = 0;
	int   m_MaxFPS = INT32_MIN;
	int   m_MinFPS = INT32_MAX;
	int   m_Frame = 0;      // Total frame count
	int   m_FrameCount = 0; // Track Frame count during 1 second period to create new FPS
	float m_FrameCap = 0;   // Good for laptops

	//--statics fixed time--
	static float g_FixedTimeStep;
	static float g_TimeScale;

public:
	Time();

public:
	void Reset();
	void Update();

public:
	void  SetFrameCap(float cap);
	float GetFrameCapStep();
	float TimeSinceStart()const;
	float FrameTime()const;
	float RawDeltaTime()const;
	float DeltaTime()const;
	int	  FPS()const;
	int	  MaxFPS()const;
	int	  MinFPS()const;
	int	  Frame()const;

public:
	//--Statics--
	static u64 CurrentTimeMilliseconds();
	static u64 CurrentTimeSeconds();
	static std::string CurrentTimeAndDate();
	static float FixedTimeStep();
	static float TimeScale();
	static void SetTimeScale(float value);
	static void SetFixedTimeStep(float value);
};

class ScopedTimer
{
private:
	High_Res_TimePoint m_Start;
	std::string m_Name;

public:
	ScopedTimer(const std::string& name);
	~ScopedTimer();
};