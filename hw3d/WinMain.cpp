
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
			
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::Move)
				{
					std::ostringstream oss;
					oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
					wnd.SetTitle(oss.str());
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