#pragma once
#include "System/WindowEvent.h"
#include "Math/Vector2.h"

enum class MouseButton
{
	Left,
	Right,
	Middle,
};

struct MouseState
{
	Vector2 m_Position;
	Vector2 m_Raw;
	int		m_ScrollValue;
	bool	m_MoustButtons[3]; // No support for your fancy mouses sorry.
};

class Mouse
{
	static MouseState s_MouseState;

public:
	static MouseState GetState();
	static void HandleEvent(const WindowEvent& event);
};

// Helper class you can use for Keyboard Managmnet
class MouseData
{
private:
	MouseState m_CurrentMouseState;
	MouseState m_PreviousMouseState;

public:
	const MouseState& CurrentMouseState()const;
	const MouseState& PreviousMouseState()const;
	bool IsButton(MouseButton button)const;
	bool IsButtonDown(MouseButton button)const;
	bool IsButtonUp(MouseButton button)const;
	int GetMouseScroll()const;
	Vector2 GetMousePosition()const;
	Vector2 GetMouseDelta()const;

	// Update the keyboard and previous, Call at start of update cycle!
	void Update();

};