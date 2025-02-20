#include "App.h"
#include <sstream>

App::App()
    :
    wnd(800, 600, "window1")
{}

int App::Go()
{
	//MSG msg;
	//BOOL gResult;
	//while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	//{
	//	// TranslateMessage  will post auxilliary WM_CHAR messages from key msgs
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);

	//	DoFrame();
	//}
	//// check if GetMessage call itself borked
	//if (gResult == -1)
	//{
	//	throw SCALDWND_LAST_EXCEPT();
	//}
	//// wParam here is the value passed to PostQuitMessage
	//return msg.wParam;

	while (true)
	{
		// process all messages pending, but to not block
		if (const auto eCode = Window::ProcessMessages())
		{
			// if return optional has a value, means we're quitting
			return *eCode;
		}
		// otherwise
		DoFrame();
	}
}

void App::DoFrame()
{
	// test logic
	/*static int i = 0;
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		switch (e.GetType())
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
	}*/

	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.GetGfx().DrawTestTriangle();
	wnd.GetGfx().ClearBuffer(c, c, 1.0f);
	wnd.GetGfx().EndFrame();

	/*const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());*/
}