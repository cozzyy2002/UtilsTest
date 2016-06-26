#include "stdafx.h"

#include <memory>

using namespace ::testing;

#pragma region Mock for Win32 API

class Win32Mock {
public:
	MOCK_METHOD1(_CloseHandle, BOOL(HANDLE));
	MOCK_METHOD0(_GetLastError, DWORD());
};

static Win32Mock* g_pWin32Mock;

BOOL _CloseHandle(HANDLE h)
{
	return g_pWin32Mock->_CloseHandle(h);
}

DWORD _GetLastError()
{
	return g_pWin32Mock->_GetLastError();
}

#define CloseHandle _CloseHandle
#define GetLastError _GetLastError
#include "win32/Win32Utils.h"

#pragma endregion

class Win32UtilsTest : public Test {
public:
	Win32UtilsTest() : m_pWin32Mock(new Win32Mock()), win32Mock(*m_pWin32Mock) {
		g_pWin32Mock = m_pWin32Mock.get();
	}

	std::unique_ptr<Win32Mock> m_pWin32Mock;
	Win32Mock& win32Mock;
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
