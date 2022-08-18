#pragma once
#include "System/WindowEvent.h"

// Same as XNA/Mono, VKey/ascii, Just map other platforms with
// a look up table in the window class...
enum class Keys
{
    None            = 0,
    Back            = 8,
    Tab             = 9,
    Enter           = 13,
    CapsLock        = 20,
    Escape          = 27,
    Space           = 32,
    PageUp          = 33,
    PageDown        = 34,
    End             = 35,
    Home            = 36,
    Left            = 37,
    Up              = 38,
    Right           = 39,
    Down            = 40,
    Select          = 41,
    Print           = 42,
    Execute         = 43,
    PrintScreen     = 44,
    Insert          = 45,
    Delete          = 46,
    Help            = 47,
    Alpha0          = 48,
    Alpha1          = 49,
    Alpha2          = 50,
    Alpha3          = 51,
    Alpha4          = 52,
    Alpha5          = 53,
    Alpha6          = 54,
    Alpha7          = 55,
    Alpha8          = 56,
    Alpha9          = 57,
    A               = 65,
    B               = 66,
    C               = 67,
    D               = 68,
    E               = 69,
    F               = 70,
    G               = 71,
    H               = 72,
    I               = 73,
    J               = 74,
    K               = 75,
    L               = 76,
    M               = 77,
    N               = 78,
    O               = 79,
    P               = 80,
    Q               = 81,
    R               = 82,
    S               = 83,
    T               = 84,
    U               = 85,
    V               = 86,
    W               = 87,
    X               = 88,
    Y               = 89,
    Z               = 90,
    LeftWindows     = 91,
    RightWindows    = 92,
    Apps            = 93,
    Sleep           = 95,
    NumPad0         = 96,
    NumPad1         = 97,
    NumPad2         = 98,
    NumPad3         = 99,
    NumPad4         = 100,
    NumPad5         = 101,
    NumPad6         = 102,
    NumPad7         = 103,
    NumPad8         = 104,
    NumPad9         = 105,
    Multiply        = 106,
    Add             = 107,
    Separator       = 108,
    Subtract        = 109,
    Decimal         = 110,
    Divide          = 111,
    F1              = 112,
    F2              = 113,
    F3              = 114,
    F4              = 115,
    F5              = 116,
    F6              = 117,
    F7              = 118,
    F8              = 119,
    F9              = 120,
    F10             = 121,
    F11             = 122,
    F12             = 123,
    F13             = 124,
    F14             = 125,
    F15             = 126,
    F16             = 127,
    F17             = 128,
    F18             = 129,
    F19             = 130,
    F20             = 131,
    F21             = 132,
    F22             = 133,
    F23             = 134,
    F24             = 135,
    NumLock         = 144,
    Scroll          = 145,
    LeftShift       = 160,
    RightShift      = 161,
    LeftControl     = 162,
    RightControl    = 163,
    LeftAlt         = 164,
    RightAlt        = 165,

    Exclaim         = 166,
    DoubleQuote     = 167,
    Pound           = 168, // UK �, usa keyboard no idea
    Dollar          = 169,
    Percent         = 170,
    Caret           = 171,
    Ampersand       = 172,
    Astrix          = 173,
    Quote           = 174,
    LeftParen       = 175,
    RightParen      = 176,
    Minus           = 177,
    Period          = 178,
    OpenCurly       = 179,
    CloseCurly      = 180,
    OpenSquare      = 181,
    CloseSquare     = 182,
    BackSlash       = 183,
    Question        = 184,
    Colon           = 185,
    SemiColon       = 186,
    SingleQuote     = 187,
    Ampersat        = 188,
    Hash            = 189,
    Tidle           = 190,
    Comma           = 191,
    LessThan        = 192,
    FullStop        = 193,
    GreaterThan     = 194,
    ForwardSlash    = 195,
    Pipe            = 198,
};

struct KeyboardState
{
    bool keyState[256] = { false };

    bool IsKeyDown(Keys key)const;
    bool IsKeyUp(Keys key)const;
};

// The main keyboard class used to interact with events and update the static keyboard data!
class Keyboard
{
    static KeyboardState s_KeyboardState;

public:
    static KeyboardState GetState();
    static void HandleEvent(const WindowEvent& event);
};

// Helper class you can use for Keyboard Managmnet
class KeyboardData
{
private:
    KeyboardState m_CurrentKeyboardState;
    KeyboardState m_PreviousKeyboardState;

public:
    const KeyboardState& CurrentKeyboardState()const;
    const KeyboardState& PreviousKeyboardState()const;
    bool IsKey(Keys key)const;
    bool IsKeyDown(Keys key)const;
    bool IsKeyUp(Keys key)const;
    // Update the keyboard and previous, Call at start of update cycle!
    void Update();
 
};