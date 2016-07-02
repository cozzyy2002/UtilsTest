#pragma once

#include "ComUtils.h"

#include <memory>

#if !defined(TYPEDEF_CSafeHandle)

class CNoneCopyable {
protected:
	CNoneCopyable() {}

private:
	CNoneCopyable(const CNoneCopyable&);
	CNoneCopyable& operator=(CNoneCopyable&);
};

class CSafeHandle : CNoneCopyable {
public:
	CSafeHandle(HANDLE h = NULL) : m_handle(h) {}

	operator HANDLE() const { return m_handle.get(); }

	bool isValid() const { return isValid(m_handle.get()); }
	static bool isValid(HANDLE h);

	struct Deleter {
		typedef HANDLE pointer;
		void operator()(HANDLE h);
	};

protected:
	std::unique_ptr<HANDLE, Deleter> m_handle;
};

/**
	Safe handle class that duplicat another handle.

	Copy constructor and = operator can accept CSafeHandleEx object created in the other process.
	This object can be used in the process that created the object,
	except used as source of copy constructor or = operator.
*/
class CSafeHandleEx : public CSafeHandle {
public:
	CSafeHandleEx(HANDLE h = NULL);

	CSafeHandleEx(const CSafeHandleEx& other);
	CSafeHandleEx& operator=(const CSafeHandleEx& other);

protected:
	// Process handle in which this object is created.
	CSafeHandle m_process;
};

#else
struct HandleDeleter {
	typedef HANDLE pointer;
	void operator()(HANDLE h) {
		if ((h != NULL) && (h != INVALID_HANDLE_VALUE)) {
			HR_EXPECT(::CloseHandle(h), HRESULT_FROM_WIN32(::GetLastError()));
		}
	}
};

typedef std::unique_ptr<HANDLE, HandleDeleter> CSafeHandle;

#endif
