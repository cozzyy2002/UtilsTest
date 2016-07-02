#include "stdafx.h"
#include "win32/Win32Utils.h"

/*static*/ bool CSafeHandle::isValid(HANDLE h)
{
	return (h != NULL) && (h != INVALID_HANDLE_VALUE);
}

void CSafeHandle::Deleter::operator()(HANDLE h)
{
	if (isValid(h)) {
		WIN32_EXPECT(::CloseHandle(h));
	}
}

CSafeHandleEx::CSafeHandleEx(HANDLE h /*= NULL*/)
	: CSafeHandle(h)
	, m_process(GetCurrentProcess())
{
}

CSafeHandleEx::CSafeHandleEx(const CSafeHandleEx & other)
	: m_process(GetCurrentProcess())
{
	*this = other;
}

CSafeHandleEx& CSafeHandleEx::operator=(const CSafeHandleEx& other)
{
	HANDLE handle;
	HRESULT hr = WIN32_EXPECT(
		DuplicateHandle(other.m_process, other, m_process, &handle, 0, FALSE, DUPLICATE_SAME_ACCESS)
	);
	if (SUCCEEDED(hr)) {
		m_handle.reset(handle);
	}

	return *this;
}
