#pragma once
#undef min
#include <ThinWrapper.h>
#include <vector>
#include <algorithm>
#include <memory>

#if defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt-gd.lib")
#elif defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-sgd.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-s.lib")
#endif



namespace TUIPaint
{
	static WORD const F_BLACK = 0;
	static WORD const F_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	static WORD const F_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
	static WORD const F_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static WORD const F_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	static WORD const F_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static WORD const F_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static WORD const F_MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;

	static WORD const B_BLACK = 0;
	static WORD const B_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	static WORD const B_RED = BACKGROUND_RED | BACKGROUND_INTENSITY;
	static WORD const B_GREEN = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	static WORD const B_BLUE = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	static WORD const B_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	static WORD const B_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	static WORD const B_MAGENTA = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;


	struct ConsoleState {
		CONSOLE_SCREEN_BUFFER_INFO	CSBI;
		CONSOLE_CURSOR_INFO			CCI;
		vector<CHAR_INFO>			buffer;
		COORD						bufferCoord;
		DWORD						consoleMode;
		WORD						consoleWidth = 0;
		SMALL_RECT					bufferRect{ 0 };
		string						title;
	};

	class Win32ThickWrapper
	{
		typedef void(*KeyHandler)(KEY_EVENT_RECORD&);
		typedef bool(*CtrlHandler)(unsigned long);
		typedef void(*MouseHandler)(MOUSE_EVENT_RECORD&);

	private:
		Win32ThinWrapper Win32;	
		KeyHandler keyHandler;
		MouseHandler mouseHandler;
		vector<INPUT_RECORD> buffer;

	public:
		static Win32ThickWrapper& Instance();

		Win32ThickWrapper();

		~Win32ThickWrapper();

		ConsoleState SaveConsole();

		void RestoreConsole(ConsoleState state);

		void SetTitle(string title);

		void ResizeWindow(short width, short height);

		void SetControlHandler(CtrlHandler handler, bool add);

		void SetKeyHandler(KeyHandler handler);

		void SetMouseHandler(MouseHandler handler);

		void SetColourAtCell(unsigned short x, unsigned short y, unsigned short colour);

		void SetBackgroundColour(unsigned short colour);

		void HideCursor(bool hidden);

		void Update();
	};

	
}

