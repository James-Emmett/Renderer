#include "Input/Mouse.h"
#include "System/Types.h"

MouseState Mouse::s_MouseState;

MouseState Mouse::GetState()
{
	return s_MouseState;
}

void Mouse::HandleEvent(const WindowEvent& event)
{
	switch (event.type)
	{
	case WindowEvent::EventType::MouseAbsolute:
	{
		s_MouseState.m_Position = Vector2((float)event.mouseAbsolute.x, (float)event.mouseAbsolute.y);
		break;
	}

	case WindowEvent::EventType::MouseRaw:
	{
		s_MouseState.m_Raw += Vector2((float)event.mouseRaw.x, (float)event.mouseRaw.y);
		break;
	}

	case WindowEvent::EventType::MouseButton:
	{
		if (event.mouseButton.button == 0)
		{
			s_MouseState.m_MoustButtons[(u32)MouseButton::Left] = event.mouseButton.m_Value;
		}
		else if (event.mouseButton.button == 1)
		{
			s_MouseState.m_MoustButtons[(u32)MouseButton::Right] = event.mouseButton.m_Value;
		}
		else
		{
			s_MouseState.m_MoustButtons[(u32)MouseButton::Middle] = event.mouseButton.m_Value;
		}
		break;
	}

	case WindowEvent::EventType::MouseWheel:
	{
		s_MouseState.m_ScrollValue = event.mouseWheel.delta;
		break;
	}
	}
}

const MouseState& MouseData::CurrentMouseState() const
{
	return m_CurrentMouseState;
}

const MouseState& MouseData::PreviousMouseState() const
{
	return m_PreviousMouseState;
}

bool MouseData::IsButton(MouseButton button) const
{
	return m_CurrentMouseState.m_MoustButtons[(u32)button];
}

bool MouseData::IsButtonDown(MouseButton button)const
{
	return  m_CurrentMouseState.m_MoustButtons[(u32)button] == true &&
			m_PreviousMouseState.m_MoustButtons[(u32)button] == false;
}

bool MouseData::IsButtonUp(MouseButton button)const
{
	return	m_CurrentMouseState.m_MoustButtons[(u32)button]  == false &&
			m_PreviousMouseState.m_MoustButtons[(u32)button] == true;
}

int MouseData::GetMouseScroll()const
{
	return m_CurrentMouseState.m_ScrollValue - m_PreviousMouseState.m_ScrollValue;
}

Vector2 MouseData::GetMousePosition()const
{
	return m_CurrentMouseState.m_Position;
}

Vector2 MouseData::GetMouseDelta()const
{
	return m_CurrentMouseState.m_Raw - m_PreviousMouseState.m_Raw;
}

void MouseData::Update()
{
	m_PreviousMouseState = m_CurrentMouseState;
	m_CurrentMouseState = Mouse::GetState();
}
