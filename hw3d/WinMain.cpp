
#include "Window.h"
//#include "WindowsMessageMap.h"
#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try {
		Window wnd(800, 300, "window1");
		// Window wnd2(300, 600, "window2");
	
		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			// Keyboard test
			/*if (wnd.kbd.IsKeyPressed(VK_SPACE))
			{
				MessageBoxA(nullptr, "Smth happen!", "Space Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}*/
			
			static int i = 0;
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();
				switch(e.GetType())
				{
				case Mouse::Event::Type::Move:
				{
					std::ostringstream oss;
					oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
					wnd.SetTitle(oss.str());
					break;
				}
				case Mouse::Event::Type::Leave:
				{
					wnd.SetTitle("Left window region!");
					break;
				}
				// problem
				case Mouse::Event::Type::WheelUp:
					i++;
					{
						std::ostringstream oss;
						oss << "Wheel Up: " << i;
						wnd.SetTitle(oss.str());
					}
					break;
				case Mouse::Event::Type::WheelDown:
					i--;
					{
						std::ostringstream oss;
						oss << "Wheel Down: " << i;
						wnd.SetTitle(oss.str());
					}
					break;
				}
			}
		}

		if (gResult == -1)
		{
			return -1;
		}
		return msg.wParam;
	}
	catch (const ScaldException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}