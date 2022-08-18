#include "Input/Input.h"

KeyboardData Input::Keyboard;
MouseData    Input::Mouse;
ActionMap    Input::s_ActionMap; // Do we need 4 of these technically because of 4 players?
GamePadData  Input::GamePads[4] = { GamePadData((PlayerIndex)0), GamePadData((PlayerIndex)1),
                                    GamePadData((PlayerIndex)2), GamePadData((PlayerIndex)3) };

bool Input::s_AnyKey = false;

void Input::Initialize()
{
    s_ActionMap.Initialize();
}

void Input::ShutDown()
{
    for (int i = 0; i < 4; i++)
    {
        GamePads[i].StopRumble();
    }
}

KeyboardData Input::GetKeyboard()
{
    return Keyboard;
}

MouseData Input::GetMouse()
{
    return Mouse;
}


GamePadData& Input::GetGamePad(PlayerIndex index)
{
    return GamePads[(u32)index];
}

ActionMap Input::GetActionMap()
{
    return s_ActionMap;
}

void Input::KeyPressed()
{
    s_AnyKey = true;
}

void Input::Update(float deltaTime)
{
    Keyboard.Update();
    Mouse.Update();
    for (int i = 0; i < 4; i++)
    {
        GamePads[i].Update(deltaTime);
    }
}

void Input::Reset()
{
    s_AnyKey = false;
}
