#pragma once
#include "Keyboard.h"
#include "GamePad.h"
#include <vector>

// Think i may changes this too have profiles?
// Added the concept of an "axis"
class ActionMap
{
public:
    enum class Action
    {
        Menu,
        Ok,
        Back,

        // Player stuff:
        Attack,
        Ability,
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight,
        Jump,
        TotalCount // Always have this last its a count for how many Enum Members!
    };


    struct ActionKey
    {
    public:
        Keys m_Key;
        GamePadButtons m_GamePadButton; // Could make this a list? LeftStick, dPad could do same action for instance?

    public:
        ActionKey(Keys key = Keys::A, GamePadButtons gamePadButton = GamePadButtons::A)
        {
            m_Key = key;
            m_GamePadButton = gamePadButton;
        }
    };

private:
    std::vector<ActionKey> m_ActionMap;

public:
    void Initialize();
    void ResetActions();
    bool IsAction(Action action, PlayerIndex index = PlayerIndex::One);
    bool IsActionDown(Action action, PlayerIndex index = PlayerIndex::One);
    bool IsActionUp(Action action, PlayerIndex index = PlayerIndex::One);
};