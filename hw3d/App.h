#pragma once
#include "Window.h"
#include "ScaldTimer.h"

class App
{
public:
	App();
	// master frame / message loop
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
	ScaldTimer timer;
};