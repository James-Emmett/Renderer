#include "Engine/Engine.h"
#include "Engine/Application.h"
#include "Graphics/Common/GraphicsParameters.h"
#include "Input/Input.h"

void Engine::Run()
{
	Application::GEngine = this;

	m_Time.Reset();
	m_LogHandler.Subscribe(&m_FileLogger);
	LogInfo("Begin Engine Initialise");

	// Load the config file
	m_Settings.Open("Settings.ini");

	u32 width = m_Settings.GetInt("Display", "Width", 1280);
	u32 height = m_Settings.GetInt("Display", "Height", 800);
	bool fullscreen = m_Settings.GetBool("Display", "FullScreen", false);

	m_Window.Create(Vector2((float)width, (float)height), fullscreen, m_Title);

	LogInfo("Begin Graphics Initialise");

	GraphicsParameters parameters;
	parameters.m_Width = 1280;
	parameters.m_Height = 720;
	parameters.m_Handle = m_Window.GetHandle();
	parameters.m_VsyncInterval = 1;
	parameters.m_RenderFormat = RenderFormat::R8G8B8A8_Unorm; // Basic LDR Buffer

	m_GraphicsDevice = new GraphicsDevice();
	m_GraphicsDevice->Initialize(parameters);

	LogInfo("End Graphics Initalise");

	Input::Initialize();

	// Initialize users game stuff
	Initialize();
	LogInfo("Game Data Initialised");

	LogInfo("Engine Initialised Succesfully");
	while (m_Window.IsOpen())
	{
		WindowEvent event;
		while (m_Window.PollEvent(event))
		{
			if (event.type == WindowEvent::EventType::Closed)
			{
				m_Window.Close();
				m_SuspendLoop = true;
			}

			if (event.type == WindowEvent::EventType::Resize)
			{
				// TO DO: Re add support for this
				/*
				m_GraphicsManager.SetFullScreen(m_Window.IsFullScreen());
				m_GraphicsManager.SetWidth(event.resized.width);
				m_GraphicsManager.SetHeight(event.resized.height);
				m_GraphicsManager.Apply(); // May do nothing if nothing actually changed.
				*/
				OnResize();
			}
		}

		if (m_SuspendLoop == false)
		{
			Tick();
		}
	}
	Exit();
}

void Engine::Tick()
{
	m_Time.Update();
	m_Window.SetTitle(m_Title + " FPS: " + std::to_string(m_Time.FPS()));
	Input::Update(m_Time.DeltaTime());
	m_Accumulator += (double)m_Time.RawDeltaTime();

	while (m_Accumulator >= Time::FixedTimeStep())
	{
		m_Accumulator -= Time::FixedTimeStep();
		FixedUpdate();
	}

	Update();
	Draw();
	OnGui();
	m_GraphicsDevice->Present();
}

void Engine::Exit()
{
	LogInfo("Engine Shutting Down");
	ShutDown();

	m_GraphicsDevice->Dispose();
	if (m_Window.IsOpen())
	{
		m_Window.Close();
	}

	m_SuspendLoop = true;
	m_LogHandler.FlushAll();
	m_Settings.Close();
}