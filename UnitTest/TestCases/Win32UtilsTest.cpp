#include "stdafx.h"

#include "win32/Win32Utils.h"
#include "Win32Mock.h"

#include <memory>

using namespace ::testing;

DECRARE_WIN32_MOCK;

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
	EXPECT_CALL(win32Mock, CloseHandle(handle)).WillOnce(Return(TRUE));
	EXPECT_CALL(win32Mock, GetLastError()).Times(0);
	{
		Win32HookCloseHandle;
		Win32HookGetLastError;

		CSafeHandle h(handle);
	}
}

/**
	@test CloseHandle() failed in CSafeHandle destructor
*/
TEST_F(Win32UtilsTest, CSafeHandle_Error)
{
	const HANDLE handle = (HANDLE)10;
	EXPECT_CALL(win32Mock, CloseHandle(handle)).WillOnce(Return(FALSE));
	EXPECT_CALL(win32Mock, GetLastError()).WillOnce(Return(5));
	{
		Win32HookCloseHandle;
		Win32HookGetLastError;

		CSafeHandle h(handle);
	}
}

/**
	@test CloseHandle() default constructor(HANDLE = NULL)
*/
TEST_F(Win32UtilsTest, CSafeHandle_NULL)
{
	EXPECT_CALL(win32Mock, CloseHandle(_)).Times(0);
	{
		Win32HookCloseHandle;

		CSafeHandle h;
	}
}

/**
	@test CloseHandle() constructor assigned INVALID_HANDLE_VALUE
*/
TEST_F(Win32UtilsTest, CSafeHandle_INVALID)
{
	EXPECT_CALL(win32Mock, CloseHandle(_)).Times(0);
	{
		Win32HookCloseHandle;

		CSafeHandle h(INVALID_HANDLE_VALUE);
	}
}
