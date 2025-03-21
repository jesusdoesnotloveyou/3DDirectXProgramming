#pragma once

#include "ScaldWin.h"
#include "ScaldException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class Window 
{
public:
	class Exception : public ScaldException
	{
		using ScaldException::ScaldException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};

	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};

	class NoGfxException : Exception
	{
	public:
		using Exception::Exception;
		virtual const char* GetType() const noexcept override;
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
	// almost the same what was in while loop with GetMessage. Check GetMessage and PeekMessage in MSDN to rewind.
	static std::optional<int> ProcessMessages() noexcept;
	Graphics& GetGfx();

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
	// Graphics
	// doesn't need to manually delete it cool yeah
	std::unique_ptr<Graphics> pGfx;
};

// error exception helper macro
#define SCALDWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define SCALDWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())