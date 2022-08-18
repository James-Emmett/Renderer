//Note::
/*
    At this point it may as well be a singleton?
*/
#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "ActionMap.h"

class Input
{
private:
	static KeyboardData Keyboard;
    static MouseData Mouse;
	static GamePadData GamePads[4];
    static ActionMap s_ActionMap; // Do we need 4 of these technically because of 4 player local co-op?
    static bool s_AnyKey;

public:
    static void Initialize();
    static void ShutDown();
    static KeyboardData GetKeyboard();
    static MouseData GetMouse();
    static GamePadData& GetGamePad(PlayerIndex index = PlayerIndex::One);
    static ActionMap GetActionMap();
    // called by window only
    static void KeyPressed();
    // Call this at the start of each loop, AFTER event processing.
    static void Update(float deltaTime);
    static void Reset();
};