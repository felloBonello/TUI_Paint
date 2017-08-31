#pragma once
#include <App.h>

namespace TUIPaint
{
	static bool done = true;
	static bool mouseDown = false;
	static short colour = B_BLUE;

	class Paint : public App
	{
	private:
		ConsoleState prevConsoleState;
		Win32ThickWrapper console;
		
	public:
		Paint();
		int Execute() override;

	} paintApp;

	bool CtrlHandler(unsigned long ctrlType);

	void KeyHandler(KEY_EVENT_RECORD& key);

	void MouseHandler(MOUSE_EVENT_RECORD& mouse);
}

