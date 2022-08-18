#include "Input/GamePad.h"
#include "Math/Mathf.h"
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput9_1_0.lib")

// Avoid, causes cross
float ApplyLinearDeadZone(float value, float deadZone)
{
	return (Mathf::Abs(value) < deadZone) ? 0 : value;
}

Vector2 ApplyRadialDeadZone(Vector2 value, float deadZone)
{
	if (value.SqrMagnitude() < (deadZone * deadZone))
	{
		return Vector2::Zero;
	}
	else
	{
		return Vector2::Normalize(value) * ((value.Magnitude() - deadZone) / (1.0f - deadZone));
	}
}

const GamePadState GamePad::GetState(PlayerIndex index, GamePadDeadZone deadZone, Vector2 deadZoneAmount)
{
	if (deadZoneAmount == Vector2::Zero)
	{
		deadZoneAmount = Vector2(0.25f, 0.25f);
	}

	DWORD result;
	//--Get the current State--
	XINPUT_STATE xState;
	result = XInputGetState((u32)index, &xState);

	GamePadState gamePadState;

	if (result != 0)
	{
		//Log here.
		gamePadState.IsConnected = false;
		return GamePadState();
	}
	else
	{
		gamePadState.IsConnected = true;
	}

	//--Set all the settings--
	//--Buttons--
	gamePadState.Buttons[0] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)				 != 0);
	gamePadState.Buttons[1] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)				 != 0);
	gamePadState.Buttons[2] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)				 != 0);
	gamePadState.Buttons[3] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)				 != 0);
	gamePadState.Buttons[4] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0);
	gamePadState.Buttons[5] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
	gamePadState.Buttons[6] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)     != 0);
	gamePadState.Buttons[7] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)    != 0);
	gamePadState.Buttons[8] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_START)          != 0);
	gamePadState.Buttons[9] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)           != 0);

	//--DPad--
	gamePadState.Buttons[10] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    != 0);
	gamePadState.Buttons[11] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  != 0);
	gamePadState.Buttons[12] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  != 0);
	gamePadState.Buttons[13] = ((xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);

	//--Triggers--
	gamePadState.Triggers.Left  = (float)xState.Gamepad.bLeftTrigger / 255.0f;  // Normalized
	gamePadState.Triggers.Right = (float)xState.Gamepad.bRightTrigger / 255.0f; // Normalized

	//--ThumbSticks--
	gamePadState.ThumbSticks.Left  = Vector2((float)(xState.Gamepad.sThumbLX / 32767.0f), (float)(xState.Gamepad.sThumbLY / 32767.0f));
	gamePadState.ThumbSticks.Right = Vector2((float)(xState.Gamepad.sThumbRX / 32767.0f), (float)(xState.Gamepad.sThumbRY / 32767.0f));


	if (deadZone == GamePadDeadZone::Axes)
	{
		gamePadState.ThumbSticks.Left.x  = ApplyLinearDeadZone(gamePadState.ThumbSticks.Left.x, deadZoneAmount.x);
		gamePadState.ThumbSticks.Left.y  = ApplyLinearDeadZone(gamePadState.ThumbSticks.Left.y, deadZoneAmount.y);
		gamePadState.ThumbSticks.Right.x = ApplyLinearDeadZone(gamePadState.ThumbSticks.Right.x, deadZoneAmount.x);
		gamePadState.ThumbSticks.Right.y = ApplyLinearDeadZone(gamePadState.ThumbSticks.Right.y, deadZoneAmount.y);
	}
	else if (deadZone == GamePadDeadZone::Radial)
	{
		gamePadState.ThumbSticks.Left  = ApplyRadialDeadZone(gamePadState.ThumbSticks.Left,  deadZoneAmount.x);
		gamePadState.ThumbSticks.Right = ApplyRadialDeadZone(gamePadState.ThumbSticks.Right, deadZoneAmount.y);
	}

	return gamePadState;
}

const void GamePad::SetFeedback(PlayerIndex index, GamePadFeedback feedback)
{
	SetFeedback(index, feedback.LeftMotor, feedback.RightMotor);
}

const void GamePad::SetFeedback(PlayerIndex index, float leftMotor, float rightMotor, Color led)
{
	XINPUT_VIBRATION vibration{};
	vibration.wLeftMotorSpeed  = (WORD)Mathf::Clamp01(leftMotor) * 65535;
	vibration.wRightMotorSpeed = (WORD)Mathf::Clamp01(rightMotor) * 65535;

	XInputSetState((DWORD)index, &vibration);
}

GamePadData::GamePadData(PlayerIndex index)
{
	m_PlayerIndex = index;
}

GamePadState GamePadData::CurrentGamePadState() const
{
	return m_CurrentState;
}

GamePadState GamePadData::PreviousGamePadState() const
{
	return m_PreviousState;
}

void GamePadData::StartRumble(float strength, float time)
{
	// Basically only rumble if not rumbling or the new time or strength is greater
	if (m_RumbleTime <= 0.0f || m_RumbleStrength < strength || (strength == m_RumbleStrength && time > m_RumbleTime))
	{
		GamePad::SetFeedback(m_PlayerIndex, strength, strength);
		m_RumbleStrength = strength;
		m_RumbleTime = time;
	}
}

void GamePadData::StopRumble()
{
	GamePad::SetFeedback(m_PlayerIndex, 0, 0);
	m_RumbleTime = 0.0f;
}

void GamePadData::SetDeadZone(float deadZone)
{
	m_DeadZone = deadZone;
}

Vector2 GamePadData::GetLeftStick() const
{
	// Only return if the stick is greater than the deadzone (radial in this case)
	Vector2 product = m_CurrentState.ThumbSticks.Left;
	if (product.SqrMagnitude() < m_DeadZone * m_DeadZone)
	{
		product = Vector2::Zero;
	}
	return product;
}

Vector2 GamePadData::GetRightStick() const
{
	// Only return if the stick is greater than the deadzone (radial in this case)
	Vector2 product = m_CurrentState.ThumbSticks.Right;
	if (product.SqrMagnitude() < m_DeadZone * m_DeadZone)
	{
		product = Vector2::Zero;
	}
	return product;
}

bool GamePadData::IsButton(GamePadButtons button) const
{
	return m_CurrentState.IsPressed(button);
}

bool GamePadData::IsButtonDown(GamePadButtons button)const
{
	return  m_CurrentState.IsPressed(button) &&
			m_PreviousState.IsReleased(button);
}

bool GamePadData::IsButtonUp(GamePadButtons button)const
{
	return  m_CurrentState.IsReleased(button) &&
			m_PreviousState.IsPressed(button);
}

void GamePadData::Update(float deltaTime)
{
	m_PreviousState = m_CurrentState;
	m_CurrentState = GamePad::GetState(m_PlayerIndex, GamePadDeadZone::Radial, Vector2(m_DeadZone, m_DeadZone));

	if (m_RumbleTime > 0.0f)
	{
		m_RumbleTime -= deltaTime; // use engine delta time when you have it!!!
	}
	else
	{
		StopRumble();
	}
}