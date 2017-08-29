#include <ThickWrapper.h>

namespace TUIPaint
{

	Win32ThickWrapper* g_Win32 = nullptr;

	//bool SetCtrlHandler_Wrapper(unsigned long ctrlEvent)
	//{
	//	if (g_Win32 != nullptr)
	//		return g_Win32->CtrlHandler->Execute(ctrlEvent);
	//	else
	//		return false;
	//}


	Win32ThickWrapper& Win32ThickWrapper::Instance()
	{
		static std::unique_ptr<Win32ThickWrapper> _instance(new Win32ThickWrapper);
		return *_instance;
	}

	Win32ThickWrapper::Win32ThickWrapper()
	{
		Win32ThinWrapper Win32;
		keyHandler = NULL;
		mouseHandler = NULL;
		Win32.SetInputMode(ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

		CONSOLE_SCREEN_BUFFER_INFO sbi = Win32.GetScreenBufferInfo();
		buffer.resize(sbi.dwSize.X * sbi.dwSize.Y);
		g_Win32 = this;
	}

	Win32ThickWrapper::~Win32ThickWrapper()
	{
		g_Win32 = nullptr;
	}

	ConsoleState Win32ThickWrapper::SaveConsole()
	{
		ConsoleState save_state;
		// Get the title
		save_state.title = Win32.GetTitle(64 * 1024);

		// Get the old window/buffer size
		save_state.CSBI = Win32.GetScreenBufferInfo();

		// Save the desktop
		save_state.buffer.resize(save_state.CSBI.dwSize.X*save_state.CSBI.dwSize.Y);
		save_state.bufferCoord = COORD{ 0 };
		save_state.bufferRect.Right = save_state.CSBI.dwSize.X - 1;
		save_state.bufferRect.Bottom = save_state.CSBI.dwSize.Y - 1;
		Win32.ReadOutput(save_state.buffer.data(), save_state.CSBI.dwSize, save_state.bufferCoord, &save_state.bufferRect);

		// Save the cursor
		save_state.CCI = Win32.GetCursorInfo();
		return save_state;
	}

	void Win32ThickWrapper::RestoreConsole(ConsoleState state)
	{
		// Restore the original settings/size
		SMALL_RECT sr{ 0 };
		Win32.SetWindowInfo(TRUE, &sr);
		Win32.SetScreenBufferSize(state.CSBI.dwSize);
		Win32.SetWindowInfo(TRUE, &state.CSBI.srWindow);

		// Restore the desktop contents
		state.bufferRect = SMALL_RECT{ 0 };
		state.bufferRect.Right = state.CSBI.dwSize.X - 1;
		state.bufferRect.Bottom = state.CSBI.dwSize.Y - 1;
		Win32.WriteOutput(state.buffer.data(), state.CSBI.dwSize, state.bufferCoord, &state.bufferRect);
		Win32.SetTitle(state.title);

		// Restore the cursor
		Win32.SetCursorInfo(&state.CCI);
		Win32.SetCursorPosition(state.CSBI.dwCursorPosition);
	}

	void Win32ThickWrapper::SetTitle(string title)
	{
		Win32.SetTitle(title);
	}

	void Win32ThickWrapper::ResizeWindow(short width, short height)
	{
		SMALL_RECT sr{ 0 };
		Win32.SetWindowInfo(TRUE, &sr);

		COORD bufferSize;
		bufferSize.X = width;
		bufferSize.Y = height;
		Win32.SetScreenBufferSize(bufferSize);

		CONSOLE_SCREEN_BUFFER_INFO sbi = Win32.GetScreenBufferInfo();

		sr.Top = sr.Left = 0;
		width = min(width, sbi.dwMaximumWindowSize.X);
		height = min(height, sbi.dwMaximumWindowSize.Y);
		sr.Right = width - 1;
		sr.Bottom = height - 1;

		Win32.SetWindowInfo(TRUE, &sr);
		//currentConsoleWidth = sr.Right - sr.Left + 1;
	}

	void Win32ThickWrapper::SetControlHandler(CtrlHandler handler, bool add)
	{
		//Install a control handler to trap ^C
		Win32.SetCtrlHandler((PHANDLER_ROUTINE)handler, add);
	}

	void Win32ThickWrapper::SetKeyHandler(KeyHandler handler)
	{
		//add a key handler
		keyHandler = handler;
	}

	void Win32ThickWrapper::SetMouseHandler(MouseHandler handler)
	{
		//add a key handler
		mouseHandler = handler;
	}

	void Win32ThickWrapper::SetColourAtCell(unsigned short x, unsigned short y, unsigned short colour)
	{
		CONSOLE_SCREEN_BUFFER_INFO sbi = Win32.GetScreenBufferInfo();
		COORD loc{ 0 };
		loc.X = min((short)x, sbi.dwMaximumWindowSize.X);
		loc.Y = min((short)y, sbi.dwMaximumWindowSize.Y);
		Win32.WriteOutputAttribute(&(WORD)colour, 1, loc);
	}

	void Win32ThickWrapper::SetBackgroundColour(unsigned short colour)
	{
		DWORD charsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi = Win32.GetScreenBufferInfo();
		DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
		COORD cursorHomeCoord{ 0, 0 };
		charsWritten = Win32.FillOutputCharacter(' ', consoleSize, cursorHomeCoord);
		charsWritten = Win32.FillOutputAttribute((WORD)colour, consoleSize, cursorHomeCoord);
	}

	void Win32ThickWrapper::HideCursor(bool hidden)
	{
		auto newCCI = Win32.GetCursorInfo();
		newCCI.bVisible = !hidden;
		Win32.SetCursorInfo(&newCCI);
	}

	void Win32ThickWrapper::Update()
	{
		unsigned long numEvents = Win32.ReadInput(buffer);
		for (unsigned long iEv = 0; iEv < numEvents; ++iEv)
		{
			switch (buffer[iEv].EventType)
			{
			case KEY_EVENT:
				if (keyHandler != nullptr)
					keyHandler(buffer[iEv].Event.KeyEvent);
				break;

			case MOUSE_EVENT:
				if (mouseHandler != nullptr)
					mouseHandler(buffer[iEv].Event.MouseEvent);
				break;
			}
		}
	}
};