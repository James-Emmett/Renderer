#include "Input/Keyboard.h"
#include "..\..\Include\Input\Mouse.h"

KeyboardState Keyboard::s_KeyboardState;

bool KeyboardState::IsKeyDown(Keys key)const
{
    return keyState[(int)key] == true;
}

bool KeyboardState::IsKeyUp(Keys key)const
{
    return keyState[(int)key] == false;
}

KeyboardState Keyboard::GetState()
{
    return s_KeyboardState;
}

void Keyboard::HandleEvent(const WindowEvent& event)
{
    if (event.type == WindowEvent::EventType::Key)
    {
        s_KeyboardState.keyState[event.key.key] = event.key.pressed;
    }
}

const KeyboardState& KeyboardData::CurrentKeyboardState()const
{
    return m_CurrentKeyboardState;
}

const KeyboardState& KeyboardData::PreviousKeyboardState()const
{
    return m_PreviousKeyboardState;
}

bool KeyboardData::IsKey(Keys key)const
{
    return m_CurrentKeyboardState.IsKeyDown(key);
}

bool KeyboardData::IsKeyDown(Keys key)const
{
    return (m_CurrentKeyboardState.IsKeyDown(key) && (m_PreviousKeyboardState.IsKeyUp(key)));
}

bool KeyboardData::IsKeyUp(Keys key)const
{
    return (m_CurrentKeyboardState.IsKeyUp(key) && (m_PreviousKeyboardState.IsKeyDown(key)));
}

// Update the keyboard and previous, Call at start of update cycle!
void KeyboardData::Update()
{
    m_PreviousKeyboardState = m_CurrentKeyboardState;
    m_CurrentKeyboardState  = Keyboard::GetState();
}