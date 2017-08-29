#include <ThinWrapper.h>

namespace TUIPaint
{
	string ErrorDescription(DWORD dwMessageID) {
		char* msg;
		auto c = FormatMessageA(
			/* flags */			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_MAX_WIDTH_MASK,
			/* source */		NULL,
			/* message ID */	dwMessageID,
			/* language */		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			/* buffer */		(LPSTR)&msg,
			/* size */			0,
			/* args */			NULL
		);
		string strMsg = (c == 0) ? "unknown" : msg;
		LocalFree(msg);
		return strMsg;
	}

	auto XError::code() const -> id_type { return code_; }
	auto XError::line() const -> int { return line_; }
	auto XError::file() const -> file_type { return file_; }

	XError::XError(int line, file_type file) :code_(GetLastError()), line_(line), file_(file) {}

	XError::string_type XError::msg() const {
		ostringstream oss;
		oss << "Error: " << code() << "\n";
		oss << ErrorDescription(code()) << "\n";
		oss << "In: " << file() << "\n";
		oss << "Line: " << line() << "\n";
		return oss.str();
	}

#define THROW_CONSOLE_ERROR() throw XError(__LINE__,__FILE__)
#define THROW_IF_CONSOLE_ERROR(res) if(!res) throw XError(__LINE__,__FILE__)


	Win32ThinWrapper::Win32ThinWrapper()
	{
		hInput_ = GetStdHandle(STD_INPUT_HANDLE);
		hOutput_ = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	Win32ThinWrapper::~Win32ThinWrapper()
	{
		hInput_ = nullptr;
		hOutput_ = nullptr;
	}

	void Win32ThinWrapper::AddAlias(
		_In_ LPWSTR Source,
		_In_ LPWSTR Target,
		_In_ LPWSTR ExeName)
	{
		THROW_IF_CONSOLE_ERROR(::AddConsoleAlias(Source, Target, ExeName));
	}

	void Win32ThinWrapper::AllocConsole()
	{
		THROW_IF_CONSOLE_ERROR(::AllocConsole());		
	}

	void Win32ThinWrapper::AttachConsole(_In_ DWORD dwProcessId)
	{
		THROW_IF_CONSOLE_ERROR(::AttachConsole(dwProcessId));
	}

	//
	HANDLE Win32ThinWrapper::CreateScreenBuffer(
		_In_             DWORD               dwDesiredAccess,
		_In_             DWORD               dwShareMode,
		_In_opt_   const SECURITY_ATTRIBUTES *lpSecurityAttributes)
	{
		HANDLE result;
		result = ::CreateConsoleScreenBuffer(dwDesiredAccess, dwShareMode, lpSecurityAttributes, CONSOLE_TEXTMODE_BUFFER, NULL);
		if (result == INVALID_HANDLE_VALUE)
			THROW_CONSOLE_ERROR();
		return result;
	}

	DWORD Win32ThinWrapper::FillOutputAttribute(
		_In_  WORD    wAttribute,
		_In_  DWORD   nLength,
		_In_  COORD   dwWriteCoord)
	{
		DWORD lpNumberOfAttrsWritten;
		THROW_IF_CONSOLE_ERROR(::FillConsoleOutputAttribute(hOutput_, wAttribute, nLength, dwWriteCoord, &lpNumberOfAttrsWritten));
		return lpNumberOfAttrsWritten;
	}

	DWORD Win32ThinWrapper::FillOutputCharacter(
		_In_  TCHAR   cCharacter,
		_In_  DWORD   nLength,
		_In_  COORD   dwWriteCoord)
	{
		DWORD lpNumberOfCharsWritten;
		THROW_IF_CONSOLE_ERROR(::FillConsoleOutputCharacter(hOutput_, cCharacter, nLength, dwWriteCoord, &lpNumberOfCharsWritten));
		return lpNumberOfCharsWritten;
	}

	void Win32ThinWrapper::FlushInputBuffer()
	{
		THROW_IF_CONSOLE_ERROR(::FlushConsoleInputBuffer(hInput_));
	}

	void Win32ThinWrapper::FreeConsole()
	{
		THROW_IF_CONSOLE_ERROR(::FreeConsole());
	}

	void Win32ThinWrapper::GenerateCtrlEvent(
		_In_ DWORD dwCtrlEvent,
		_In_ DWORD dwProcessGroupId)
	{
		THROW_IF_CONSOLE_ERROR(::GenerateConsoleCtrlEvent(dwCtrlEvent, dwProcessGroupId));
	}

	LPTSTR Win32ThinWrapper::GetAlias(
		_In_  LPTSTR lpSource,
		_In_  DWORD  TargetBufferLength,
		_In_  LPTSTR lpExeName)
	{
		LPTSTR lpTargetBuffer = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleAlias(lpSource, lpTargetBuffer, TargetBufferLength, lpExeName));
		return lpTargetBuffer;
	}

	LPTSTR Win32ThinWrapper::GetAliases(
		_In_  DWORD  AliasBufferLength,
		_In_  LPTSTR lpExeName)
	{
		LPTSTR lpAliasBuffer = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleAliases(lpAliasBuffer, AliasBufferLength, lpExeName));
		return lpAliasBuffer;
	}

	DWORD Win32ThinWrapper::GetAliasesLength(_In_ LPTSTR lpExeName)
	{
		return ::GetConsoleAliasesLength(lpExeName);
	}

	LPTSTR Win32ThinWrapper::GetAliasExes(_In_  DWORD  ExeNameBufferLength)
	{
		LPTSTR lpExeNameBuffer = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleAliasExes(lpExeNameBuffer, ExeNameBufferLength));
		return lpExeNameBuffer;
	}

	DWORD Win32ThinWrapper::GetAliasExesLength()
	{
		return ::GetConsoleAliasExesLength();
	}

	UINT Win32ThinWrapper::GetCP()
	{
		return ::GetConsoleCP();
	}

	CONSOLE_CURSOR_INFO Win32ThinWrapper::GetCursorInfo()
	{
		CONSOLE_CURSOR_INFO lpConsoleCursorInfo;
		THROW_IF_CONSOLE_ERROR(::GetConsoleCursorInfo(hOutput_, &lpConsoleCursorInfo));
		return lpConsoleCursorInfo;
	}

	LPDWORD Win32ThinWrapper::GetDisplayMode()
	{
		LPDWORD lpModeFlags = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleDisplayMode(lpModeFlags));
		return lpModeFlags;
	}

	COORD Win32ThinWrapper::GetFontSize(_In_ DWORD  nFont)
	{
		COORD var;
		var = ::GetConsoleFontSize(hOutput_, nFont);
		if (var.X == 0 && var.Y == 0)
			THROW_CONSOLE_ERROR();
		return var;
	}

	PCONSOLE_HISTORY_INFO Win32ThinWrapper::GetHistoryInfo()
	{
		PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleHistoryInfo(lpConsoleHistoryInfo));
		return lpConsoleHistoryInfo;
	}

	LPDWORD Win32ThinWrapper::GetMode(_In_ HANDLE hConsoleHandle)
	{
		LPDWORD lpMode = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleMode(hConsoleHandle, lpMode));
		return lpMode;
	}

	UINT Win32ThinWrapper::GetOutputCP()
	{
		return ::GetConsoleOutputCP();
	}

	LPDWORD Win32ThinWrapper::GetProcessList(_In_  DWORD dwProcessCount)
	{
		LPDWORD lpdwProcessList = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleProcessList(lpdwProcessList, dwProcessCount));
		return lpdwProcessList;
	}

	CONSOLE_SCREEN_BUFFER_INFO Win32ThinWrapper::GetScreenBufferInfo()
	{
		CONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo;
		THROW_IF_CONSOLE_ERROR(::GetConsoleScreenBufferInfo(hOutput_, &lpConsoleScreenBufferInfo));
		return lpConsoleScreenBufferInfo;
	}

	PCONSOLE_SCREEN_BUFFER_INFOEX Win32ThinWrapper::GetScreenBufferInfoEx()
	{
		PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleScreenBufferInfoEx(hOutput_, lpConsoleScreenBufferInfoEx));
		return lpConsoleScreenBufferInfoEx;
	}

	PCONSOLE_SELECTION_INFO Win32ThinWrapper::GetSelectionInfo()
	{
		PCONSOLE_SELECTION_INFO lpConsoleSelectionInfo = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetConsoleSelectionInfo(lpConsoleSelectionInfo));
		return lpConsoleSelectionInfo;
	}

	string Win32ThinWrapper::GetTitle(DWORD nSize)
	{
		LPSTR lpConsoleTitle = new CHAR[nSize];;
		THROW_IF_CONSOLE_ERROR(::GetConsoleTitleA(lpConsoleTitle, nSize));
		return lpConsoleTitle;
	}

	HWND Win32ThinWrapper::GetWindow()
	{
		HWND var;
		var = ::GetConsoleWindow();
		if (var == NULL)
			THROW_CONSOLE_ERROR();
		return var;
	}

	PCONSOLE_FONT_INFO Win32ThinWrapper::GetCurrentFont(BOOL bMaximumWindow)
	{
		PCONSOLE_FONT_INFO lpConsoleCurrentFont = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetCurrentConsoleFont(hOutput_, bMaximumWindow, lpConsoleCurrentFont));
		return lpConsoleCurrentFont;
	}

	PCONSOLE_FONT_INFOEX Win32ThinWrapper::GetCurrentFontEx(BOOL bMaximumWindow)
	{
		PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetCurrentConsoleFontEx(hOutput_, bMaximumWindow, lpConsoleCurrentFontEx));
		return lpConsoleCurrentFontEx;
	}

	COORD Win32ThinWrapper::GetLargestWindowSize()
	{
		COORD var;
		var = ::GetLargestConsoleWindowSize(hOutput_);
		if (var.X == 0 && var.Y == 0)
			THROW_CONSOLE_ERROR();
		return var;
	}

	LPDWORD Win32ThinWrapper::GetNumberOfInputEvents()
	{
		LPDWORD lpcNumberOfEvents = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetNumberOfConsoleInputEvents(hInput_, lpcNumberOfEvents));
		return lpcNumberOfEvents;
	}

	LPDWORD Win32ThinWrapper::GetNumberOfMouseButtons()
	{
		LPDWORD lpNumberOfMouseButtons = nullptr;
		THROW_IF_CONSOLE_ERROR(::GetNumberOfConsoleMouseButtons(lpNumberOfMouseButtons));
		return lpNumberOfMouseButtons;
	}

	HANDLE Win32ThinWrapper::GetStdHandle(DWORD nStdHandle)
	{
		HANDLE result;
		result = ::GetStdHandle(nStdHandle);
		if (result == INVALID_HANDLE_VALUE)
			THROW_CONSOLE_ERROR();
		return result;
	}

	void Win32ThinWrapper::PeekInput(
		_Out_ PINPUT_RECORD lpBuffer,
		_In_  DWORD         nLength,
		_Out_ LPDWORD       lpNumberOfEventsRead)
	{
		THROW_IF_CONSOLE_ERROR(::PeekConsoleInput(hInput_, lpBuffer, nLength, lpNumberOfEventsRead));
	}

	void Win32ThinWrapper::Read(
		_Out_    LPVOID							 lpBuffer,
		_In_     DWORD							 nNumberOfCharsToRead,
		_Out_    LPDWORD						 lpNumberOfCharsRead,
		_In_opt_ PCONSOLE_READCONSOLE_CONTROL	 pInputControl)
	{
		THROW_IF_CONSOLE_ERROR(::ReadConsole(hInput_, lpBuffer, nNumberOfCharsToRead, lpNumberOfCharsRead, pInputControl));
	}

	long Win32ThinWrapper::ReadInput(
		vector<INPUT_RECORD>& buffer
	)
	{
		DWORD lpNumberOfEventsRead;
		THROW_IF_CONSOLE_ERROR(::ReadConsoleInput(hInput_, buffer.data(), (DWORD)buffer.size(), &lpNumberOfEventsRead));
		return lpNumberOfEventsRead;
	}

	void Win32ThinWrapper::ReadOutput(
		_Out_   PCHAR_INFO  lpBuffer,
		_In_    COORD       dwBufferSize,
		_In_    COORD       dwBufferCoord,
		_Inout_ PSMALL_RECT lpReadRegion)
	{
		THROW_IF_CONSOLE_ERROR(::ReadConsoleOutput(hOutput_, lpBuffer, dwBufferSize, dwBufferCoord, lpReadRegion));
	}

	long Win32ThinWrapper::ReadOutputAttribute(
		_Out_ LPWORD  lpAttribute,
		_In_  DWORD   nLength,
		_In_  COORD   dwReadCoord)
	{
		DWORD lpNumberOfAttrsRead;
		THROW_IF_CONSOLE_ERROR(::ReadConsoleOutputAttribute(hOutput_, lpAttribute, nLength, dwReadCoord, &lpNumberOfAttrsRead));
		return lpNumberOfAttrsRead;
	}

	//
	long Win32ThinWrapper::ReadOutputCharacter(
		_Out_ LPTSTR  lpCharacter,
		_In_  DWORD   nLength,
		_In_  COORD   dwReadCoord)
	{
		DWORD lpNumberOfCharsRead;
		THROW_IF_CONSOLE_ERROR(::ReadConsoleOutputCharacter(hOutput_, lpCharacter, nLength, dwReadCoord, &lpNumberOfCharsRead));
		return lpNumberOfCharsRead;
	}

	//
	void Win32ThinWrapper::ScrollScreenBuffer(
		_In_     const SMALL_RECT *lpScrollRectangle,
		_In_           COORD      dwDestinationOrigin,
		_In_     const CHAR_INFO  *lpFill)
	{
		THROW_IF_CONSOLE_ERROR(::ScrollConsoleScreenBuffer(hOutput_, lpScrollRectangle, NULL, dwDestinationOrigin, lpFill));
	}

	void Win32ThinWrapper::SetActiveScreenBuffer()
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleActiveScreenBuffer(hOutput_));
	}

	void Win32ThinWrapper::SetCP(_In_ UINT wCodePageID)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleCP(wCodePageID));
	}

	void Win32ThinWrapper::SetCtrlHandler(
		_In_opt_ PHANDLER_ROUTINE HandlerRoutine,
		_In_     BOOL             Add)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleCtrlHandler(HandlerRoutine, Add));
	}

	void Win32ThinWrapper::SetCursorInfo(
		_In_ const CONSOLE_CURSOR_INFO *lpConsoleCursorInfo)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleCursorInfo(hOutput_, lpConsoleCursorInfo));
	}

	void Win32ThinWrapper::SetCursorPosition(
		_In_ COORD  dwCursorPosition)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleCursorPosition(hOutput_, dwCursorPosition));
	}

	void Win32ThinWrapper::SetDisplayMode(
		_In_      DWORD  dwFlags,
		_Out_opt_ PCOORD lpNewScreenBufferDimensions)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleDisplayMode(hOutput_, dwFlags, lpNewScreenBufferDimensions));
	}

	void Win32ThinWrapper::SetHistoryInfo(_In_ PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleHistoryInfo(lpConsoleHistoryInfo));
	}

	void Win32ThinWrapper::SetInputMode(
		_In_ DWORD  dwMode)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleMode(hInput_, dwMode));
	}

	void Win32ThinWrapper::SetOutputMode(
		_In_ DWORD  dwMode)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleMode(hOutput_, dwMode));
	}

	void Win32ThinWrapper::SetOutputCP(_In_ UINT wCodePageID)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleOutputCP(wCodePageID));
	}

	void Win32ThinWrapper::SetScreenBufferInfoEx(
		_In_ PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleScreenBufferInfoEx(hOutput_, lpConsoleScreenBufferInfoEx));
	}

	void Win32ThinWrapper::SetScreenBufferSize(
		_In_ COORD  dwSize)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleScreenBufferSize(hOutput_, dwSize));
	}

	void Win32ThinWrapper::SetTextAttribute(
		_In_ WORD wAttributes)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleTextAttribute(hOutput_, wAttributes));
	}

	void Win32ThinWrapper::SetTitle(string title)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleTitleA((LPCSTR)title.c_str()));
	}

	void Win32ThinWrapper::SetWindowInfo(
		_In_       BOOL       bAbsolute,
		_In_ const SMALL_RECT *lpConsoleWindow)
	{
		THROW_IF_CONSOLE_ERROR(::SetConsoleWindowInfo(hOutput_, bAbsolute, lpConsoleWindow));
	}

	void Win32ThinWrapper::SetCurrentFontEx(
		_In_ BOOL                 bMaximumWindow,
		_In_ PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx)
	{
		THROW_IF_CONSOLE_ERROR(::SetCurrentConsoleFontEx(hOutput_, bMaximumWindow, lpConsoleCurrentFontEx));
	}

	void Win32ThinWrapper::SetStdHandle(
		_In_ DWORD  nStdHandle,
		_In_ HANDLE hHandle)
	{
		THROW_IF_CONSOLE_ERROR(::SetStdHandle(nStdHandle, hHandle));
	}

	long Win32ThinWrapper::WriteConsole(
		_In_       const VOID    *lpBuffer,
		_In_             DWORD   nNumberOfCharsToWrite)
	{
		DWORD lpNumberOfCharsWritten;
		THROW_IF_CONSOLE_ERROR(::WriteConsole(hOutput_, lpBuffer, nNumberOfCharsToWrite, &lpNumberOfCharsWritten, NULL));
		return lpNumberOfCharsWritten;
	}

	long Win32ThinWrapper::WriteInput(
		_In_  const INPUT_RECORD *lpBuffer,
		_In_        DWORD        nLength)
	{
		DWORD lpNumberOfEventsWritten;
		THROW_IF_CONSOLE_ERROR(::WriteConsoleInput(hInput_, lpBuffer, nLength, &lpNumberOfEventsWritten));
		return lpNumberOfEventsWritten;
	}

	PSMALL_RECT Win32ThinWrapper::WriteOutput(
		_In_    const CHAR_INFO   *lpBuffer,
		_In_          COORD       dwBufferSize,
		_In_          COORD       dwBufferCoord,
		_Inout_       PSMALL_RECT lpWriteRegion)
	{
		THROW_IF_CONSOLE_ERROR(::WriteConsoleOutput(hOutput_, lpBuffer, dwBufferSize, dwBufferCoord, lpWriteRegion));
		return lpWriteRegion;
	}

	long Win32ThinWrapper::WriteOutputAttribute(
		_In_		WORD    *lpAttribute,
		_In_        DWORD   nLength,
		_In_        COORD   dwWriteCoord)
	{
		DWORD lpNumberOfAttrsWritten;
		THROW_IF_CONSOLE_ERROR(::WriteConsoleOutputAttribute(hOutput_, lpAttribute, nLength, dwWriteCoord, &lpNumberOfAttrsWritten));
		return lpNumberOfAttrsWritten;
	}

	long Win32ThinWrapper::WriteOutputCharacter(
		_In_  LPCTSTR lpCharacter,
		_In_  DWORD   nLength,
		_In_  COORD   dwWriteCoord)
	{
		DWORD lpNumberOfCharsWritten;
		THROW_IF_CONSOLE_ERROR(::WriteConsoleOutputCharacter(hOutput_, lpCharacter, nLength, dwWriteCoord, &lpNumberOfCharsWritten));
		return lpNumberOfCharsWritten;
	}
}
