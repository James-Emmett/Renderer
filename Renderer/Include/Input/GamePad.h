#pragma once
#include "Math/Vector2.h"
#include "Math/Color.h"

enum class PlayerIndex
{
	One,
	Two,
	Three,
	Four
};

enum class GamePadDeadZone
{
	None,
	Axes,
	Radial
};

struct GamePadFeedback
{
	float LeftMotor  = 0.0f;
	float RightMotor = 0.0f;
	Color LedColor; // Playstation
};

enum class GamePadButtons
{
	A,
	B,
	X,
	Y,
	LB,
	RB,
	L3,
	R3,
	Start,
	Back,
	Up,
	Down,
	Left,
	Right,
	LeftThumbstickUp,
	LeftThumbstickDown,
	LeftThumbstickLeft,
	LeftThumbstickRight,
	RightThumbstickUp,
	RightThumbstickDown,
	RightThumbstickLeft,
	RightThumbstickRight,
};

struct GamePadThumbSticks
{
	Vector2 Left;
	Vector2 Right;
};

struct GamePadTriggers
{
	float Left = 0.0f;
	float Right = 0.0f;
};

struct GamePadState
{
	bool IsConnected = false;
	bool Buttons[14] = {};
	GamePadThumbSticks ThumbSticks;
	GamePadTriggers Triggers;

	bool IsPressed(GamePadButtons button)const
	{
		return Buttons[(int)button];
	}

	bool IsReleased(GamePadButtons button)const
	{
		return !Buttons[(int)button];
	}
};

class GamePad
{
public:
	static const GamePadState GetState(PlayerIndex index, GamePadDeadZone deadZone = GamePadDeadZone::Radial, Vector2 deadZoneAmount = Vector2::Zero);
	static const void SetFeedback(PlayerIndex index, GamePadFeedback feedback);
	static const void SetFeedback(PlayerIndex index, float leftMotor, float rightMotor, Color led = Color::Black);
};

class GamePadData
{
private:
	PlayerIndex  m_PlayerIndex;
	GamePadState m_CurrentState;
	GamePadState m_PreviousState;
	const float  m_TriggerThreshold = 0.5f; // How far analog moves to count as "pressed"
	float		 m_DeadZone = 0.25f; // How far analog moves to count as "pressed"
	float        m_RumbleTime = 0.0f;
	float        m_RumbleStrength = 0.0f;

public:
	GamePadData(PlayerIndex index);

public:
	GamePadState CurrentGamePadState()const;
	GamePadState PreviousGamePadState()const;
	void StartRumble(float strength, float time);
	void StopRumble();
	void SetDeadZone(float deadzone = 0.25f);
	//--ThumStick Stuff--
	// Basic non radial deadzone yuk
	Vector2 GetLeftStick()const;
	Vector2 GetRightStick()const;

	bool IsButton(GamePadButtons button)const;
	// True if down during frame Only (think one shot vs full auto)
	bool IsButtonDown(GamePadButtons button)const;
	// True if up during frame Only (think one shot vs full auto)
	bool IsButtonUp(GamePadButtons button)const;

	// Update the GamePad
	void Update(float deltaTime);
};