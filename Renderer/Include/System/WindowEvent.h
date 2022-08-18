#pragma once
#include "Types.h"

class WindowEvent
{
public:
	struct KeyEvent
	{
		unsigned int key = 0;
		bool pressed = false;
	};

	struct MouseAbsoluteEvent
	{
		u32 x = 0;
		u32 y = 0;
	};

	struct MouseRawEvent
	{
		float x = 0.0f;
		float y = 0.0f;
	};

	struct MouseButtonEvent
	{
		unsigned int button = 0;  // 0 = left, 1 = mid, 2 = right
		bool m_Value = false;
	};

	struct MouseWheelEvent
	{
		int delta = 0; // How much the mouse has scrolled since last position
	};

	struct ResizedEvent
	{
		u32 width = 0;
		u32 height = 0;
	};

	enum class EventType
	{
		Closed,
		Resize,
		LostFocus,
		GainedFocus,
		Key,
		MouseAbsolute,
		MouseRaw,
		MouseButton,
		MouseWheel,
		Idle,
		Awake,
		Count
	};

public:
	WindowEvent() {}
	~WindowEvent() {}

public:

	union
	{
		KeyEvent			key;
		MouseAbsoluteEvent	mouseAbsolute;
		MouseRawEvent		mouseRaw;
		MouseButtonEvent	mouseButton;
		MouseWheelEvent		mouseWheel;
		ResizedEvent		resized;
		bool				isIdle = true;
	};

	EventType type = EventType::Count;
};