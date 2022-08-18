#pragma once
#include "System/Window.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "System/ConfigFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsDevice.h"
#include <string>

// Made inheritable for now, but should have scenemanager and be data driven!
class Engine
{
protected:
	std::string m_Title = "Renderer";
	Window		m_Window;
	LogHandler	m_LogHandler;
	FileLogger	m_FileLogger;
	Time		m_Time;
	ConfigFile	m_Settings;

	GraphicsDevice* m_GraphicsDevice;

private:
	double	m_Accumulator = 0.0f;
	bool	m_Resizing = false; // Think of a better way
	bool	m_SuspendLoop = false;

public:
	void Run();
	void Tick();
	void Exit();

	const Time& GetTime() { return m_Time; }
	const Window& GetWindow() { return m_Window; }
	GraphicsDevice* Device() { return m_GraphicsDevice; }

public:
	virtual void Initialize() {};
	virtual void FixedUpdate() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void Draw() {};
	virtual void OnGui() {};
	virtual void OnResize() {};
	virtual void ShutDown(){};
};