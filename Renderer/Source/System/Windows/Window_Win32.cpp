#include "System/Windows/Window_Win32.h"
#include "System/Logger.h"
#include "System/Assert.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Input.h"
#include "Engine/Application.h"
#include "Engine/Engine.h"
#include <Windows.h>

const wchar_t* className = L"SnowFall";
DWORD WS_WINDOWED = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
DWORD WS_BORDERLESS = WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

Window::Window()
{
}

Window::Window(WindowHandle handle)
{
	Create(handle);
}

Window::Window(Vector2 size, bool fullScreen, const std::string& title)
{
	Create(size, fullScreen, title);
}

Window::~Window()
{
	if (m_CallBack == 0)
	{
		if (m_Handle)
		{
			DestroyWindow((HWND)m_Handle);
			UnregisterClassW(className, GetModuleHandleW(NULL));
		}
	}
	else
	{
		// Remove our hook into the window.
		SetWindowLongPtrW((HWND)m_Handle, GWLP_WNDPROC, (LONG)m_CallBack);
	}
}

void Window::Create(WindowHandle handle)
{
	if (m_Handle)
	{
		// Steal access to this forigen window.!
		m_Handle = handle;
		// Pass our class as user data in the long ptr.
		SetWindowLongPtrW((HWND)m_Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		// Grab pointer too the call back from other window where stealing.
		m_CallBack = SetWindowLongPtrW((HWND)m_Handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::WndProc));

		assert(RegisterRawInput());

		m_Open = true;
		SetVisible(true);
	}
}

void Window::Create(Vector2 size, bool fullScreen, const std::string& title)
{
	LogInfo("Creating Window");
	// register the Window
	RegisterWindowClass();

	int width = (int)size.x;
	int height = (int)size.y;
	m_ClientSize = size;
	DWORD style = WS_BORDERLESS;

	// Get the window size adjusted too account for the Frame etc.
	if (fullScreen == false)
	{
		RECT rect = { 0,0, (LONG)size.x, (LONG)size.y };
		AdjustWindowRect(&rect, WS_WINDOWED, false);
		width = (int)(rect.right - rect.left);
		height = (int)(rect.bottom - rect.top);
		style = WS_WINDOWED;
	}

	HINSTANCE instance = GetModuleHandle(NULL);
	// Create the window, pass our window class as user data again as the long ptr.
	m_Handle = CreateWindowW(className, StringUtil::Widen(title.c_str()).c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, instance, this);
	HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
	assert(m_Handle);
	LogInfo("Window Registered");

	assert(RegisterRawInput());

	if (fullScreen)
	{
		SetFullScreen(true);
	}

	m_Open = true;
	SetVisible(true);
}

void Window::Close()
{
	if (m_Handle)
	{
		DestroyWindow((HWND)m_Handle);
		m_Handle = nullptr;
	}
	m_Open = false;
}


WindowHandle Window::GetHandle() const
{
	return m_Handle;
}

Vector2 Window::GetPosition() const
{
	RECT rect;
	GetWindowRect((HWND)m_Handle, &rect);

	return Vector2((float)rect.left, (float)rect.top);
}

void Window::SetPosition(const Vector2& position)
{
	SetWindowPos((HWND)m_Handle, NULL, (int)position.x, (int)position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

Vector2 Window::GetSize() const
{
	RECT rect;
	GetClientRect((HWND)m_Handle, &rect);

	return Vector2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
}

void Window::SetSize(const Vector2& size)
{
 	if (size != GetSize())
	{
		// Adjust client size to include size for title bars etc.
		m_ClientSize = size;
		RECT rectangle = { 0, 0, static_cast<long>(size.x), static_cast<long>(size.y) };
		AdjustWindowRect(&rectangle, GetWindowLong((HWND)m_Handle, GWL_STYLE), false);
		int width = rectangle.right - rectangle.left;
		int height = rectangle.bottom - rectangle.top;

		SetWindowPos((HWND)m_Handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	}
}

void Window::SetTitle(const std::string& title)
{
	SetWindowTextW((HWND)m_Handle, StringUtil::Widen(title).c_str());
}

void Window::SetVisible(bool visible)
{
	ShowWindow((HWND)m_Handle, visible ? SW_SHOW : SW_HIDE);
}

void Window::SetMouseVisible(bool visible)
{
	if (visible)
	{
		ShowCursor(visible);
	}
	else
	{
		// why the heck do they increment this bloody thing???
		while (ShowCursor(false) >= 0);
	}
}

void Window::RequestFocus()
{
	DWORD pid	= GetWindowThreadProcessId((HWND)m_Handle, NULL); // My Window
	DWORD fgPid = GetWindowThreadProcessId(GetForegroundWindow(), NULL); // Current active window

	// Steal if same process else flash
	if (pid == fgPid)
	{
		SetForegroundWindow((HWND)m_Handle);
	}
	else
	{
		FLASHWINFO info = {};
		info.cbSize = sizeof(FLASHWINFO);
		info.hwnd = (HWND)m_Handle;
		info.dwFlags = FLASHW_TRAY;
		info.dwTimeout = 0;
		info.uCount = 3;

		FlashWindowEx(&info);
	}
}

bool Window::HasFocus() const
{
	return (HWND)m_Handle == GetForegroundWindow();
}

bool Window::IsOpen() const
{
	return m_Open;
}

bool Window::IsFullScreen() const
{
	return m_WindowState == WindowState::Fullscreen;
}

void Window::SetFullScreen(bool value)
{
	// Only update if required.
	if (IsFullScreen() != value)
	{
		m_WindowState = (value) ? WindowState::Fullscreen : WindowState::Windowed;

		if (value)
		{
			// Cache size and pos, Should this be client rect???
			RECT rect;
			GetWindowRect((HWND)m_Handle, &rect);
			m_lastSize.x = (float)(rect.right - rect.left);
			m_lastSize.y = (float)(rect.bottom - rect.top);
			m_lastPos = GetPosition();

			// Get display mode of primary monitor
			DEVMODEW mode = {};
			mode.dmSize = sizeof(mode);
			EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &mode);

			// Set window to be Borderless Fullscreen.
			SetWindowLongPtrW((HWND)m_Handle, GWL_STYLE, WS_BORDERLESS);
			SetWindowPos((HWND)m_Handle, HWND_TOP, CW_DEFAULT, CW_DEFAULT, (int)mode.dmPosition.x + mode.dmPelsWidth, (int)mode.dmPosition.y + mode.dmPelsHeight, SWP_FRAMECHANGED | SWP_NOACTIVATE);
			ShowWindow((HWND)m_Handle, SW_MAXIMIZE);
		}
		else
		{
			// Switch To Windowed.
			SetWindowLongPtrW((HWND)m_Handle, GWL_STYLE, WS_WINDOWED);
			SetWindowPos((HWND)m_Handle, NULL, (int)m_lastPos.x, (int)m_lastPos.y, (int)m_lastSize.x, (int)m_lastSize.y,
				SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

			ShowWindow((HWND)m_Handle, SW_SHOWNORMAL);
		}
	}
}


bool Window::PollEvent(WindowEvent& event)
{
	// Process only if owner, usally the case anyways...
	if (!m_CallBack)
	{
		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	if (m_Events.empty() == false)
	{
		event = m_Events.front();
		m_Events.pop();
		return true;
	}

	return false;
}

void Window::InvalidateWindow()
{
	InvalidateRect((HWND)m_Handle, NULL, false);
	ShowWindow((HWND)m_Handle, SW_SHOW);
}

void Window::ShowMessageBox(const std::string& message)
{
	MessageBoxA(NULL, message.c_str(), "Alert: ", MB_OK);
}


void Window::PushEvent(const WindowEvent& event)
{
	m_Events.push(event);
}

void Window::RegisterWindowClass()
{
	WNDCLASSW windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = (WNDPROC)Window::WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleW(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className;

	ATOM value = RegisterClassW(&windowClass);
	if (value == 0)
	{
		HRESULT error = HRESULT_FROM_WIN32(GetLastError());
		assert(0);
	}
}

bool Window::RegisterRawInput()
{
	//Register keyboard and mouse
	RAWINPUTDEVICE rid[2] = {};

	//Keyboard
	rid[0].usUsagePage = 1;
	rid[0].usUsage = 6;
	rid[0].dwFlags = NULL;
	rid[0].hwndTarget = NULL;

	//Mouse
	rid[1].usUsagePage = 1;
	rid[1].usUsage = 2;
	rid[1].dwFlags = 0; // Left legacy here for absolute mouse.
	rid[1].hwndTarget = NULL;

	if (RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		LogError("Failed to Register RawInput Win32");
		return false;
	}
	else
	{
		LogInfo("Raw Input Registered");
	}
	return true;
}

void Window::ProcessEvent(u32 message, unsigned __int64 wParam, s64 lParam)
{
	if (m_Handle == nullptr)
	{
		return;
	}

	switch (message)
	{
	case WM_PAINT:
	{
		if (m_Resizing)
		{
			// Keep Ticking during a resize event
			Application::GEngine->Tick();
		}
		else
		{
			//hdc = BeginPaint(hWnd, &ps);
			//EndPaint(hWnd, &ps);
		}
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		Close();
		break;
	}

	case WM_CLOSE:
	{
		WindowEvent event;
		event.type = WindowEvent::EventType::Closed;
		PushEvent(event);
		break;
	}

	case WM_SETFOCUS:
	{
		WindowEvent event;
		event.type = WindowEvent::EventType::GainedFocus;
		PushEvent(event);
		break;
	}

	case WM_KILLFOCUS:
	{
		WindowEvent event;
		event.type = WindowEvent::EventType::LostFocus;
		PushEvent(event);
		break;
	}

	case WM_MOVE:
	{
		m_Interacted = true;
		break;
	}

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			if ((m_WindowState != WindowState::Minimised))
			{
				m_WindowState = WindowState::Minimised;

				WindowEvent event;
				event.type = WindowEvent::EventType::Idle;
				event.isIdle = false;
				PushEvent(event);
			}
		}
		else if (m_WindowState == WindowState::Minimised)
		{
			m_WindowState = WindowState::Windowed;
			WindowEvent event;
			event.type = WindowEvent::EventType::Awake;
			event.isIdle = false;
			PushEvent(event);
		}
		else if (!m_Resizing)
		{
			ResizeEvent();
		}
		break;

	case WM_ENTERSIZEMOVE:
	{
		m_Resizing = true;
		break;
	}

	case WM_EXITSIZEMOVE:
	{
		m_Resizing = false;
		ResizeEvent();
		break;
	}

	case WM_INPUT:
	{
		UINT size = 512;
		BYTE buffer[512] = {};
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, &size, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)buffer;

		// Keyboard input
		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			bool keyUp = raw->data.keyboard.Flags & RI_KEY_BREAK;
			USHORT keyCode = raw->data.keyboard.VKey;

			// Set the correct version of the key!
			if (keyCode == VK_SHIFT)
			{
				if (raw->data.keyboard.MakeCode == 0x2A)
				{
					keyCode = 0xA0;
				}
				else
				{
					keyCode = 0xA1;
				}

				m_IsShift = (!keyUp) ? true : false;
			}
			else if (keyCode == VK_CONTROL)
			{
				if ((raw->data.keyboard.Flags & RI_KEY_E0) != 0)
				{
					keyCode = 0xA3;
				}
				else
				{
					keyCode = 0xA2;
				}
			}

			WindowEvent event;
			event.type = WindowEvent::EventType::Key;
			event.key.key = RawKeyToRuneKey(keyCode);
			event.key.pressed = (!keyUp) ? true : false;
			PushEvent(event);
			Keyboard::HandleEvent(event);
			Input::KeyPressed();
		}
		// End Keyboard

		// Mouse Input
		WindowEvent event;
		event.type = WindowEvent::EventType::MouseButton;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			//LEFT BUTTON
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			{
				event.mouseButton.button = 0;
				event.mouseButton.m_Value = true;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
			{
				event.mouseButton.button = 0;
				event.mouseButton.m_Value = false;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}

			//MIDDLE BUTTON
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
			{
				event.mouseButton.button = 2;
				event.mouseButton.m_Value = true;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
			{
				event.mouseButton.button = 2;
				event.mouseButton.m_Value = false;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}

			//RIGHT BUTTON
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
			{
				event.mouseButton.button = 1;
				event.mouseButton.m_Value = true;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
			{
				event.mouseButton.button = 1;
				event.mouseButton.m_Value = false;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}

			//MOUSE WHEEL
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
			{
				event.type = WindowEvent::EventType::MouseWheel;
				// Normalizes the scroll wheel, windows does 120 click intervals???
				event.mouseWheel.delta = ((short)raw->data.mouse.usButtonData) / 120;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}
			
			// Raw mouse position
			if (!(raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
			{
				//Raw mouse value, good for getting deltas of mouse positioning etc.
				event.type = WindowEvent::EventType::MouseRaw;
				event.mouseRaw.x = (float)raw->data.mouse.lLastX;
				event.mouseRaw.y = (float)raw->data.mouse.lLastY;
				Mouse::HandleEvent(event); // Convience push
				PushEvent(event);
			}
		}
		break;
	}

	// Position of mouse relative too my windows cliant space.
	case WM_MOUSEMOVE:
	{
		POINT point;
		GetCursorPos(&point);
		if (ScreenToClient((HWND)m_Handle, &point))
		{
			WindowEvent event;
			event.type = WindowEvent::EventType::MouseAbsolute;
			event.mouseAbsolute.x = point.x;
			event.mouseAbsolute.y = point.y;
			Mouse::HandleEvent(event); // Convience push
			PushEvent(event);
		}
		break;
	}

	default:
		break;
	}
}

void Window::ResizeEvent()
{
	Vector2 size = GetSize();

	WindowEvent event;
	event.type = WindowEvent::EventType::Resize;
	event.resized.width = (u32)size.x;
	event.resized.height = (u32)size.y;
	PushEvent(event);
}

u32 Window::RawKeyToRuneKey(u32 key)
{
	// My keys are basically just Vkeys, so we can return
	// the raw input for 90% but need to ensure "Shift" keys
	// are translate!

	// If shift key is pressed, we need to do special stuff
	switch (key)
	{
	case 0x30:
	{
		if (m_IsShift)
		{
			return (u32)Keys::RightParen;
		}
		else
		{
			return 0x30;
		}
		break;
	}
	case 0x31:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Exclaim;
		}
		else
		{
			return 0x31;
		}
		break;
	}
	case 0x32:
	{
		if (m_IsShift)
		{
			return (u32)Keys::DoubleQuote;
		}
		else
		{
			return 0x32;
		}
		break;
	}
	case 0x33:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Pound;
		}
		else
		{
			return 0x33;
		}
		break;
	}
	case 0x34:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Dollar;
		}
		else
		{
			return 0x34;
		}
		break;
	}
	case 0x35:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Percent;
		}
		else
		{
			return 0x35;
		}
		break;
	}
	case 0x36:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Caret;
		}
		else
		{
			return 0x35;
		}
		break;
	}
	case 0x37:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Ampersand;
		}
		else
		{
			return 0x37;
		}
		break;
	}
	case 0x38:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Astrix;
		}
		else
		{
			return 0x38;
		}
		break;
	}
	case 0x39:
	{
		if (m_IsShift)
		{
			return (u32)Keys::LeftParen;
		}
		else
		{
			return 0x39;
		}
		break;
	}
	case VK_OEM_3:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Ampersat;
		}
		else
		{
			return (u32)Keys::SingleQuote;
		}
		break;
	}
	case VK_OEM_1:
	{
		if (m_IsShift)
		{
			return (u32)Keys::SemiColon;
		}
		else
		{
			return (u32)Keys::Colon;
		}
		break;
	}
	case VK_OEM_2:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Question;
		}
		else
		{
			return (u32)Keys::BackSlash;
		}
		break;
	}
	case VK_OEM_COMMA:
	{
		if (m_IsShift)
		{
			return (u32)Keys::LessThan;
		}
		else
		{
			return (u32)Keys::Comma;
		}
		break;
	}
	case VK_OEM_PERIOD:
	{
		if (m_IsShift)
		{
			return (u32)Keys::GreaterThan;
		}
		else
		{
			return (u32)Keys::FullStop;
		}
		break;
	}
	case VK_OEM_7:
	{
		if (m_IsShift)
		{
			return (u32)Keys::Tidle;
		}
		else
		{
			return (u32)Keys::Hash;
		}
		break;
	}
	case VK_OEM_5:
	{
		if (m_IsShift)
		{
			return (u32)Keys::ForwardSlash;
		}
		else
		{
			return (u32)Keys::Pipe;
		}
		break;
	}

	default:
		return key;
		break;
	}
}

long long Window::WndProc(WindowHandle handle, u32 message, unsigned __int64 wParam, s64 lParam)
{
	if (message == WM_CREATE)
	{
		LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams;
		SetWindowLongPtrW((HWND)handle, GWLP_USERDATA, window);
	}

	// Cast window from user data to a useable window!
	Window* window = handle ? reinterpret_cast<Window*>(GetWindowLongPtr((HWND)handle, GWLP_USERDATA)) : nullptr;

	// Now steal the events muahahaha
	if (window)
	{
		window->ProcessEvent(message, wParam, lParam);

		//--Feed messages back to origional window--
		if (window->m_CallBack)
		{
			return CallWindowProcW(reinterpret_cast<WNDPROC>(window->m_CallBack), (HWND)handle, message, (WPARAM)wParam, (LPARAM)lParam);
		}
	}

	if (message == WM_CLOSE)
	{
		return 0;
	}

	return DefWindowProcW((HWND)handle, message, (WPARAM)wParam, (LPARAM)lParam);
}
