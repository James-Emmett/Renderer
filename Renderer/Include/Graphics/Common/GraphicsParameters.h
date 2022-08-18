#pragma once
#include "SurfaceFormat.h"

struct GraphicsParameters
{
	RenderFormat	m_RenderFormat = RenderFormat::R8G8B8A8_Unorm;
	DepthFormat		m_DepthFormat = DepthFormat::Depth32;
	u32				m_Width = 1280;
	u32				m_Height = 720;
	WindowHandle	m_Handle = nullptr;
	u8				m_VsyncInterval = 0; // 0 == None, 1 Max HZ, 2 Half and so on
	bool			m_Stero = false; // Support this in future
};