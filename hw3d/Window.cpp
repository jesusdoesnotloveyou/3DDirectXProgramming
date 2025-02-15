#include "Window.h"
#include <sstream>

int Window::windowCount = 0;

// Window class stuff
Window::WindowClass Window::WindowClass::wndClass;

const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::WindowClass::WindowClass() noexcept
    :
    hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEXA wc = { 0 };

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	RegisterClassExA(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClassA(wndClassName, GetInstance());
}

// Window Exception stuff
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	ScaldException(line, file),
	hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "\n"
		<< "[Error Code] " << GetErrorCode() << "\n"
		<< "[Description] " << GetErrorString() << "\n"
		<< GetOriginString();

	whatBuffer = oss.str();
	// to prevent pointing to the dead memory after oss dying, and for saving exception info
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Scald Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return std::string("Unidentified error code");
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
 	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

// Window stuff
Window::Window(int width, int height, const char* name)
	:
	width(width),
	height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = wr.left + width;
	wr.top = 100;
	wr.bottom = wr.top + height;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw SCALDWND_LAST_EXCEPT();
	}

	//throw SCALDWND_EXCEPT(ERROR_ARENA_TRASHED);
	//throw std::runtime_error{"my butt hurts"};
	//throw 6969;

	// create window & get hWnd
	hWnd = CreateWindowA(
		WindowClass::GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// check for error
	if (hWnd == nullptr)
	{
		throw SCALDWND_LAST_EXCEPT();
	}
	windowCount++;
	// show the damn window
	ShowWindow(hWnd, SW_SHOW);
}

Window::~Window()
{
	DestroyWindow(hWnd);
	windowCount--;
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw SCALDWND_LAST_EXCEPT();
	}
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		return 0;
		// clear keystate when window loses focus to prevent input getting zombie
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
		/****************** KEYBOARD MESSAGES *******************/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.IsAutorepeatEnabled()) // filter autorepeat key event
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		/****************** END KEYBOARD MESSAGES *******************/

		/****************** MOUSE MESSAGES *******************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// or
		// xPos = GET_X_LPARAM(lParam); 
		// yPos = GET_Y_LPARAM(lParam);

		// in client region -> log move, log enter + capture mouse
		if (pt.x > 0 && pt.x < width && pt.y > 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client region -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);

			}
			// button up -> release capture / log event for leaving
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}
	/****************** END MOUSE MESSAGES *******************/
	return DefWindowProc(hWnd, msg, wParam, lParam);
}