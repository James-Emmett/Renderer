#include "System/Timer.h"

float Time::g_FixedTimeStep = 1.0f / 60.0f;
float Time::g_TimeScale = 1.0f;

Time::Time()
{
	Reset();
}

void Time::Reset()
{
	m_DeltaTime = 0;
	m_FPS = 0;
	m_MaxFPS = 0;
	m_FrameCount = 0;
	m_FrameCap = 60;

	// Get the current system time in Miliseconds
	m_CurrentFrameTime = std::chrono::high_resolution_clock::now();
	m_GameStart = m_CurrentFrameTime;
	m_OneSecondTimer = m_CurrentFrameTime;
}

void Time::Update()
{
	++m_Frame;
	m_PreviousFrameTime = m_CurrentFrameTime;
	m_CurrentFrameTime = std::chrono::high_resolution_clock::now();

	m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_CurrentFrameTime - m_PreviousFrameTime).count();

	if (std::chrono::duration_cast<std::chrono::duration<float>>(m_CurrentFrameTime - m_OneSecondTimer).count() < 1.0f)
	{
		++m_FrameCount;
	}
	else
	{
		m_FPS = m_FrameCount;
		m_MinFPS = std::min(m_MinFPS, m_FPS);
		m_MaxFPS = std::max(m_MaxFPS, m_FPS);
		m_FrameCount = 0;
		m_OneSecondTimer = m_CurrentFrameTime;
	}

}

void Time::SetFrameCap(float cap)
{
	m_FrameCap = cap;
}

float Time::GetFrameCapStep()
{
	return 1.0f / m_FrameCap;
}

float Time::TimeSinceStart() const
{
	return std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::seconds>(m_CurrentFrameTime - m_GameStart)).count();
}

float Time::FrameTime()const
{
	return 1.0f / m_FPS;
}

float Time::RawDeltaTime() const
{
	return m_DeltaTime;
}

float Time::DeltaTime()const
{
	return m_DeltaTime * g_TimeScale;
}

int Time::FPS()const
{
	return m_FPS;
}

int Time::MaxFPS()const
{
	return m_MaxFPS;
}

int Time::MinFPS()const
{
	return m_MinFPS;
}

int Time::Frame() const
{
	return m_Frame;
}

u64 Time::CurrentTimeMilliseconds()
{
	return (u64)std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

u64 Time::CurrentTimeSeconds()
{
	return (u64)std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

std::string Time::CurrentTimeAndDate()
{
	std::time_t today = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm localTime;
	localtime_s(&localTime, &today);

	char s[128];
	strftime(s, 128, "%c", &localTime);
	return std::string(s);
}

float Time::FixedTimeStep()
{
	return g_FixedTimeStep;
}

float Time::TimeScale()
{
	return g_TimeScale;
}

void Time::SetTimeScale(float value)
{
	g_TimeScale = value;
}

void Time::SetFixedTimeStep(float value)
{
	g_FixedTimeStep = value;
}

ScopedTimer::ScopedTimer(const std::string& name) : m_Name(name)
{
	m_Start = std::chrono::high_resolution_clock::now();
}

ScopedTimer::~ScopedTimer()
{
	// Long Line
	float elapsed = std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - m_Start)).count();
	// TRACE OUT TO LOGGERS HERE
}
