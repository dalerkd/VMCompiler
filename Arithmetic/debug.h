//////debug.h
/*
更新日期:2018年7月9日
version:0.3
内容:增加Log宏
LogError会抛异常哟
*/
#include "EXCEP.h"

#ifndef _DEBUG_H
#define _DEBUG_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#define LogError(format, ...)\
		{\
			OutputDebug("Error:");\
			OutputDebug(format, __VA_ARGS__);\
			OutputDebug(" at %s:%s<line:%d>\r\n", __FILE__ ,__FUNCTION__, __LINE__);\
			char throwBuffer[256]={0};\
			sprintf_s(throwBuffer,256,format,__VA_ARGS__);\
			throw(EXCEP(throwBuffer));\
		}
#define LogTips(format, ...)\
		{\
			OutputDebug("Tips:");\
			OutputDebug(format, __VA_ARGS__);\
			OutputDebug(" at %s:%s<line:%d>\r\n", __FILE__ ,__FUNCTION__, __LINE__);\
		}
#define LogInfo(format, ...)\
		{\
			OutputDebug("Info:");\
			OutputDebug(format, __VA_ARGS__);\
			OutputDebug(" at %s:%s<line:%d>\r\n", __FILE__ ,__FUNCTION__, __LINE__);\
		}



VOID OutputDebug(const wchar_t* strFormat, ...);

VOID OutputDebug(const char* strFormat, ...);

VOID HexDump(WCHAR *desc, VOID *addr, UINT len);
VOID HexDumpPrintf(WCHAR *desc, VOID *addr, UINT len);

#endif // _DEBUG_H