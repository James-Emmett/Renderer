#include "Input/ActionMap.h"
#include "Input/Input.h"

void ActionMap::Initialize()
{
    ResetActions();
}

void ActionMap::ResetActions()
{
    m_ActionMap.clear();
    m_ActionMap.resize((int)Action::TotalCount);

    m_ActionMap[(int)Action::Menu]          = ActionKey(Keys::Escape, GamePadButtons::Start);
    m_ActionMap[(int)Action::Ok]            = ActionKey(Keys::A, GamePadButtons::A);
    m_ActionMap[(int)Action::Back]          = ActionKey(Keys::B, GamePadButtons::B);
    m_ActionMap[(int)Action::Attack]        = ActionKey(Keys::X, GamePadButtons::X);
    m_ActionMap[(int)Action::Ability]       = ActionKey(Keys::Q, GamePadButtons::Y);
    m_ActionMap[(int)Action::MoveUp]        = ActionKey(Keys::W, GamePadButtons::LeftThumbstickUp);
    m_ActionMap[(int)Action::MoveDown]      = ActionKey(Keys::S, GamePadButtons::LeftThumbstickDown);
    m_ActionMap[(int)Action::MoveLeft]      = ActionKey(Keys::A, GamePadButtons::LeftThumbstickLeft);
    m_ActionMap[(int)Action::MoveRight]     = ActionKey(Keys::D, GamePadButtons::LeftThumbstickRight);
    m_ActionMap[(int)Action::Jump]          = ActionKey(Keys::Space, GamePadButtons::A);
}

bool ActionMap::IsAction(Action action, PlayerIndex index)
{
    ActionKey key = m_ActionMap[(int)action];
    return Input::GetKeyboard().IsKey(key.m_Key) || Input::GetGamePad(index).IsButton(key.m_GamePadButton);
}

bool ActionMap::IsActionDown(Action action, PlayerIndex index)
{
    ActionKey key = m_ActionMap[(int)action];
    return Input::GetKeyboard().IsKeyDown(key.m_Key) || Input::GetGamePad(index).IsButtonDown(key.m_GamePadButton);
}

bool ActionMap::IsActionUp(Action action, PlayerIndex index)
{
    ActionKey key = m_ActionMap[(int)action];
    return Input::GetKeyboard().IsKeyUp(key.m_Key) || Input::GetGamePad(index).IsButtonUp(key.m_GamePadButton);
}