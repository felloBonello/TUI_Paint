#include "Paint.h"


namespace TUIPaint
{
	Paint::Paint() { done = false; }

	int Paint::Execute()
	{
		Win32ThickWrapper console;
		prevConsoleState = console.SaveConsole();
		console.SetControlHandler(CtrlHandler, true);
		console.SetKeyHandler(KeyHandler);
		console.SetMouseHandler(MouseHandler);
		console.SetTitle("Paint");
		console.ResizeWindow(70, 50);
		console.SetBackgroundColour(B_WHITE);
		console.HideCursor(true);

		while (!done) {
			console.Update();
		}

		console.RestoreConsole(prevConsoleState);
		return EXIT_SUCCESS;
	}

	bool CtrlHandler(unsigned long ctrlType)
	{
		switch (ctrlType) {
		case CTRL_C_EVENT:
			done = true;
			return true;
		}
		return false;
	}

	void KeyHandler(KEY_EVENT_RECORD& key)
	{
		if (key.bKeyDown && (key.uChar.AsciiChar == 'c' || key.uChar.AsciiChar == 'C'))
			Win32ThickWrapper::Instance().SetBackgroundColour(B_WHITE);
	}

	void MouseHandler(MOUSE_EVENT_RECORD& mouse)
	{
		switch (mouse.dwEventFlags)
		{
		case MOUSE_MOVED:
			if (mouseDown)
				Win32ThickWrapper::Instance().SetColourAtCell(mouse.dwMousePosition.X, mouse.dwMousePosition.Y, colour);
			break;

		default:
			if (mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
			{
				switch (colour)
				{
				case B_RED:
					colour = B_GREEN;
					break;
				case B_GREEN:
					colour = B_BLUE;
					break;
				case B_BLUE:
					colour = B_CYAN;
					break;
				case B_CYAN:
					colour = B_MAGENTA;
					break;
				case B_MAGENTA:
					colour = B_YELLOW;
					break;
				case B_YELLOW:
					colour = B_RED;
					break;
				}
			}
			if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				Win32ThickWrapper::Instance().SetColourAtCell(mouse.dwMousePosition.X, mouse.dwMousePosition.Y, colour);
				mouseDown = true;
			}
			if (mouse.dwButtonState == 0)
				mouseDown = false;
			break;


		}
	}
}

