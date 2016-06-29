#include "stdafx.h"

#include <memory>

using namespace ::testing;

#pragma region Mock for Win32 API

class Win32Mock {
public:
	Win32Mock() { instance = this; }
	~Win32Mock() { instance = NULL; }

	MOCK_METHOD1(_CloseHandle, BOOL(HANDLE));
	MOCK_METHOD0(_GetLastError, DWORD());

	/// Used by replaced global API.
	static Win32Mock* instance;;
};

Win32Mock* Win32Mock::instance = NULL;

BOOL _CloseHandle(HANDLE h)
{
	return Win32Mock::instance ? Win32Mock::instance->_CloseHandle(h) : FALSE;
}

DWORD _GetLastError()
{
	return Win32Mock::instance ? Win32Mock::instance->_GetLastError() : 0;
}

// Replace global Win23 API
#define CloseHandle _CloseHandle
#define GetLastError _GetLastError
#include "win32/Win32Utils.h"

#pragma endregion

class Win32UtilsTest : public Test {
public:
	Win32Mock win32Mock;
};

/**
	@test CloseHandle() succeeded in CSafeHandle destructor
*/
TEST_F(Win32UtilsTest, CSafeHandle_Normal)
{
	const HANDLE handle = (HANDLE)10;
	EXPECT_CALL(win32Mock, _CloseHandle(handle)).WillOnce(Return(TRUE));
	EXPECT_CALL(win32Mock, _GetLastError()).Times(0);
	{
		CSafeHandle h(handle);
	}
}

/**
	@test CloseHandle() failed in CSafeHandle destructor
*/
TEST_F(Win32UtilsTest, CSafeHandle_Error)
{
	const HANDLE handle = (HANDLE)10;
	EXPECT_CALL(win32Mock, _CloseHandle(handle)).WillOnce(Return(FALSE));
	EXPECT_CALL(win32Mock, _GetLastError()).WillOnce(Return(5));
	{
		CSafeHandle h(handle);
	}
}

/**
	@test CloseHandle() default constructor(HANDLE = NULL)
*/
TEST_F(Win32UtilsTest, CSafeHandle_NULL)
{
	EXPECT_CALL(win32Mock, _CloseHandle(_)).Times(0);
	{
		CSafeHandle h;
	}
}

/**
	@test CloseHandle() constructor assigned INVALID_HANDLE_VALUE
*/
TEST_F(Win32UtilsTest, CSafeHandle_INVALID)
{
	EXPECT_CALL(win32Mock, _CloseHandle(_)).Times(0);
	{
		CSafeHandle h(INVALID_HANDLE_VALUE);
	}
}
