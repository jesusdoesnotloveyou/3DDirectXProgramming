#pragma once

#include "ScaldWin.h"
#include "ScaldException.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window 
{
public:
	class Exception : public ScaldException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();

		WindowClass(const WindowClass& w) = delete;
		WindowClass operator=(const WindowClass& w) = delete;
		static constexpr const char* wndClassName = "Scald Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;
	void SetTitle(const std::string& title);

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	// Input Components
	Keyboard kbd;
	Mouse mouse;
private:
	int width;
	int height;
	HWND hWnd;
	static int windowCount;
};

// error exception helper macro
#define SCALDWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define SCALDWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())