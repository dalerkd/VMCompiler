///////debug.cpp

#include "pch.h"

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "Debug.h"

#define MAX_DEBUG_STRING 8192 * 2
#define MAX_REGISTRY_NAME 255
#define MAX_FILE_PATH 32767

extern bool g_OPEN_LOG;

VOID OutputDebug(const wchar_t* strFormat, ...)
{
	va_list vArgs;

	va_start(vArgs, strFormat);
	LPWSTR strDebug = (LPWSTR)malloc((MAX_DEBUG_STRING + 2) * sizeof(WCHAR)); // do not use zalloc here
	SecureZeroMemory(strDebug, (MAX_DEBUG_STRING + 2) * sizeof(WCHAR));
	vswprintf_s(strDebug, MAX_DEBUG_STRING, strFormat, vArgs);
	if(g_OPEN_LOG)
	{
		OutputDebugStringW(strDebug);
	}
	
	free(strDebug);
}



VOID OutputDebug(const char* strFormat, ...)
{
	va_list vArgs;

	va_start(vArgs, strFormat);
	LPSTR strDebug = (LPSTR)malloc((MAX_DEBUG_STRING + 2) * sizeof(CHAR)); // do not use zalloc here
	SecureZeroMemory(strDebug, (MAX_DEBUG_STRING + 2) * sizeof(CHAR));
	vsprintf_s(strDebug, MAX_DEBUG_STRING, strFormat, vArgs);
	if(g_OPEN_LOG)
	{
		OutputDebugStringA(strDebug);
	}
	free(strDebug);
}

/* simple hexdump, taken from stackoverflow :) */
VOID HexDump(WCHAR *desc, VOID *addr, UINT len) {
	DWORD i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		OutputDebug(L"%s:\n", desc);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				OutputDebug(L"  %S\n", buff);

			// Output the offset.
			OutputDebug(L"  %04x ", i);
		}

		// Now the hex code for the specific character.
		OutputDebug(L" %02x", pc[i]);

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		OutputDebug(L"   ");
		i++;
	}

	// And print the final ASCII bit.
	OutputDebug(L"  %S\n", buff);
}


VOID HexDumpPrintf(WCHAR *desc, VOID *addr, UINT len) {
	DWORD i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		wprintf(L"%s:\n", desc);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				wprintf(L"  %S\n", buff);

			// Output the offset.
			wprintf(L"  %04x ", i);
		}

		// Now the hex code for the specific character.
		wprintf(L" %02x", pc[i]);

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		wprintf(L"   ");
		i++;
	}

	// And print the final ASCII bit.
	wprintf(L"  %S\n", buff);
}