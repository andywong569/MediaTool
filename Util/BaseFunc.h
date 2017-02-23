#pragma once


//////////////////////////////////���ú���//////////////////////////////////

enum WT_EVENTLOG_TYPE
{
	WT_EVENTLOG_SUCCESS_TYPE = 0x0000, // �ɹ�
	WT_EVENTLOG_ERROR_TYPE = 0x0001, // ����
	WT_EVENTLOG_WARNING_TYPE = 0x0002,  // ����
	WT_EVENTLOG_INFORMATION_TYPE = 0x0004  // ��Ϣ
};

typedef void (CALLBACK* LogCallBack)(WT_EVENTLOG_TYPE emType, SYSTEMTIME stTime, LPCTSTR lpszMsg);

UTIL_API extern HMODULE g_hModule;
UTIL_API extern LogCallBack g_logCallBack;

UTIL_API void LogMsgString(WT_EVENTLOG_TYPE emType, const tstring& pszMsg);
UTIL_API void LogMsg(WT_EVENTLOG_TYPE emType, LPCTSTR lpszFmt, ...);
UTIL_API void OutDebugString(LPCTSTR lpszFmt, ...);
UTIL_API tstring GetGuid();
UTIL_API void WaitWithMessageLoop(unsigned long ulMilliseconds); // ����������Ϣѭ��