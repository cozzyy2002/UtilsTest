#pragma once

class Win32Mock {
public:
	Win32Mock() { instance = this; }
	~Win32Mock() { instance = NULL; }

	MOCK_METHOD1(CloseHandle, BOOL(HANDLE));
	MOCK_METHOD0(GetLastError, DWORD());

	// Used by global hook functions.
	static Win32Mock* instance;;
};

// Specify this macro in any .cpp file.
#define DECRARE_WIN32_MOCK Win32Mock* Win32Mock::instance = NULL

#define Win32HookCloseHandle \
	SEXYHOOK_API_HOOK_1_BEGIN("Kernel32.dll",BOOL,CloseHandle,HANDLE h) \
	{ \
		return Win32Mock::instance->CloseHandle(h); \
	} \
	SEXYHOOK_API_END()

#define Win32HookGetLastError \
	SEXYHOOK_API_HOOK_0_BEGIN("Kernel32.dll",DWORD,GetLastError) \
	{ \
		return Win32Mock::instance->GetLastError(); \
	} \
	SEXYHOOK_API_END()
