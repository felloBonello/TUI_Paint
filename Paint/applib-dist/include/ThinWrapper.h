#pragma once
#include <windows.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#if defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt-gd.lib")
#elif defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-sgd.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-s.lib")
#endif

using namespace std;

namespace TUIPaint
{
	string ErrorDescription(DWORD dwMessageID);

	class XError {
	public:
		using id_type = decltype(GetLastError());
		using file_type = char const *;
		using string_type = std::string;
	private:
		id_type code_;
		int line_;
		file_type file_;
	public:
		XError(int line, file_type file);

		auto code() const->id_type;
		auto file() const->file_type;
		int line() const;

		string_type msg() const;
	};

	class Win32ThinWrapper
	{
	private:
		HANDLE hInput_;
		HANDLE hOutput_;

	public:
		Win32ThinWrapper();

		~Win32ThinWrapper();

		void AddAlias(
			_In_ LPWSTR Source,
			_In_ LPWSTR Target,
			_In_ LPWSTR ExeName);

		void AllocConsole();

		void AttachConsole(_In_ DWORD dwProcessId);

		HANDLE CreateScreenBuffer(
			_In_             DWORD               dwDesiredAccess,
			_In_             DWORD               dwShareMode,
			_In_opt_   const SECURITY_ATTRIBUTES *lpSecurityAttributes);

		DWORD FillOutputAttribute(
			_In_  WORD    wAttribute,
			_In_  DWORD   nLength,
			_In_  COORD   dwWriteCoord);

		DWORD FillOutputCharacter(
			_In_  TCHAR   cCharacter,
			_In_  DWORD   nLength,
			_In_  COORD   dwWriteCoord);

		void FlushInputBuffer();

		void FreeConsole();

		void GenerateCtrlEvent(
			_In_ DWORD dwCtrlEvent,
			_In_ DWORD dwProcessGroupId);

		LPTSTR GetAlias(
			_In_  LPTSTR lpSource,
			_In_  DWORD  TargetBufferLength,
			_In_  LPTSTR lpExeName);

		LPTSTR GetAliases(
			_In_  DWORD  AliasBufferLength,
			_In_  LPTSTR lpExeName);

		DWORD GetAliasesLength(_In_ LPTSTR lpExeName);

		LPTSTR GetAliasExes(_In_  DWORD  ExeNameBufferLength);

		DWORD GetAliasExesLength();

		UINT GetCP();

		CONSOLE_CURSOR_INFO GetCursorInfo();

		LPDWORD GetDisplayMode();

		COORD GetFontSize(_In_ DWORD  nFont);

		PCONSOLE_HISTORY_INFO GetHistoryInfo();

		LPDWORD GetMode(_In_ HANDLE hConsoleHandle);

		UINT GetOutputCP();

		LPDWORD GetProcessList(_In_  DWORD dwProcessCount);

		CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo();

		PCONSOLE_SCREEN_BUFFER_INFOEX GetScreenBufferInfoEx();

		PCONSOLE_SELECTION_INFO GetSelectionInfo();

		string GetTitle(DWORD nSize);

		HWND GetWindow();

		PCONSOLE_FONT_INFO GetCurrentFont(BOOL bMaximumWindow);

		PCONSOLE_FONT_INFOEX GetCurrentFontEx(BOOL bMaximumWindow);

		COORD GetLargestWindowSize();

		LPDWORD GetNumberOfInputEvents();

		LPDWORD GetNumberOfMouseButtons();

		HANDLE GetStdHandle(DWORD nStdHandle);

		void PeekInput(
			_Out_ PINPUT_RECORD lpBuffer,
			_In_  DWORD         nLength,
			_Out_ LPDWORD       lpNumberOfEventsRead);

		void Read(
			_Out_    LPVOID							 lpBuffer,
			_In_     DWORD							 nNumberOfCharsToRead,
			_Out_    LPDWORD						 lpNumberOfCharsRead,
			_In_opt_ PCONSOLE_READCONSOLE_CONTROL	 pInputControl);

		long ReadInput(
			vector<INPUT_RECORD>& buffer
		);

		void ReadOutput(
			_Out_   PCHAR_INFO  lpBuffer,
			_In_    COORD       dwBufferSize,
			_In_    COORD       dwBufferCoord,
			_Inout_ PSMALL_RECT lpReadRegion);

		long ReadOutputAttribute(
			_Out_ LPWORD  lpAttribute,
			_In_  DWORD   nLength,
			_In_  COORD   dwReadCoord);

		long ReadOutputCharacter(
			_Out_ LPTSTR  lpCharacter,
			_In_  DWORD   nLength,
			_In_  COORD   dwReadCoord);

		void ScrollScreenBuffer(
			_In_     const SMALL_RECT *lpScrollRectangle,
			_In_           COORD      dwDestinationOrigin,
			_In_     const CHAR_INFO  *lpFill);

		void SetActiveScreenBuffer();

		void SetCP(_In_ UINT wCodePageID);

		void SetCtrlHandler(
			_In_opt_ PHANDLER_ROUTINE HandlerRoutine,
			_In_     BOOL             Add);

		void SetCursorInfo(
			_In_ const CONSOLE_CURSOR_INFO *lpConsoleCursorInfo);

		void SetCursorPosition(
			_In_ COORD  dwCursorPosition);

		void SetDisplayMode(
			_In_      DWORD  dwFlags,
			_Out_opt_ PCOORD lpNewScreenBufferDimensions);

		void SetHistoryInfo(_In_ PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo);

		void SetInputMode(
			_In_ DWORD  dwMode);

		void SetOutputMode(
			_In_ DWORD  dwMode);

		void SetOutputCP(_In_ UINT wCodePageID);

		void SetScreenBufferInfoEx(
			_In_ PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx);

		void SetScreenBufferSize(
			_In_ COORD  dwSize);

		void SetTextAttribute(
			_In_ WORD wAttributes);

		void SetTitle(string title);

		void SetWindowInfo(
			_In_       BOOL       bAbsolute,
			_In_ const SMALL_RECT *lpConsoleWindow);

		void SetCurrentFontEx(
			_In_ BOOL                 bMaximumWindow,
			_In_ PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);

		void SetStdHandle(
			_In_ DWORD  nStdHandle,
			_In_ HANDLE hHandle);

		long WriteConsole(
			_In_       const VOID    *lpBuffer,
			_In_             DWORD   nNumberOfCharsToWrite);

		long WriteInput(
			_In_  const INPUT_RECORD *lpBuffer,
			_In_        DWORD        nLength);

		PSMALL_RECT WriteOutput(
			_In_    const CHAR_INFO   *lpBuffer,
			_In_          COORD       dwBufferSize,
			_In_          COORD       dwBufferCoord,
			_Inout_       PSMALL_RECT lpWriteRegion);

		long WriteOutputAttribute(
			_In_		WORD    *lpAttribute,
			_In_        DWORD   nLength,
			_In_        COORD   dwWriteCoord);

		long WriteOutputCharacter(
			_In_  LPCTSTR lpCharacter,
			_In_  DWORD   nLength,
			_In_  COORD   dwWriteCoord);
	};
}
