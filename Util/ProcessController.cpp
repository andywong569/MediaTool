#include "stdafx.h"
#include "ProcessController.h"


// ���ҽ��������ڵĻص�����   
BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)   
{   
	CProcessController* pProcessController = (CProcessController*)lParam;
	if (pProcessController != NULL)
	{
		DWORD dwProcessId;   
		GetWindowThreadProcessId(hWnd, &dwProcessId);   

		// �ж��Ƿ���ָ�����̵�������   
		if (pProcessController->m_pi.dwProcessId == dwProcessId 
			&& GetParent(hWnd) == NULL)
		{
			tstring strCaption;
			int nTextLen = GetWindowTextLength(hWnd);
			TCHAR* pszText = new TCHAR[nTextLen + 1];
			memset(pszText, 0, nTextLen + 1);
			GetWindowText(hWnd, pszText, nTextLen + 1);
			strCaption = pszText;
			delete [] pszText;
			
			if (pProcessController->m_strCaption == strCaption)
			{
				pProcessController->m_hMainWnd = hWnd;   
				return FALSE;  
			} 
		}
		return TRUE; 
	}
	return FALSE;
} 

CProcessController::CProcessController()
{
	m_pi.hThread = INVALID_HANDLE_VALUE;
	m_pi.hProcess = INVALID_HANDLE_VALUE;
	m_hMainWnd = NULL;
	m_bAutoExit = true;
}

CProcessController::~CProcessController()
{
	if (m_pi.hThread != INVALID_HANDLE_VALUE && m_pi.hProcess != INVALID_HANDLE_VALUE && m_bAutoExit)
	{
		// �ر��ӽ��̵����߳̾��
		CloseHandle(m_pi.hThread);
		// ǿ���˳��ӽ���
		TerminateProcess(m_pi.hProcess, 0);
		// �ȴ������˳�
		WaitForSingleObject(m_pi.hProcess, INFINITE);
		// �ر��ӽ��̾��
		CloseHandle(m_pi.hProcess);
	}
}

bool CProcessController::Start(const TCHAR* pszAppliactionName, TCHAR* pszCommandLine)
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	BOOL bRet = CreateProcess(pszAppliactionName,
		pszCommandLine, // ��Unicode�汾�д˲�������Ϊ�����ַ�������Ϊ�˲����ᱻ�޸�
		NULL, NULL, FALSE, 0, NULL, NULL, &si, &m_pi);
	if (!bRet)
	{
		int nError = GetLastError();
		return false;
	}

	return true;
}

bool CProcessController::Start(const TCHAR* pszAppliactionName, TCHAR* pszCommandLine, const TCHAR* pszCaption)
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	BOOL bRet = CreateProcess(pszAppliactionName,
		pszCommandLine, // ��Unicode�汾�д˲�������Ϊ�����ַ�������Ϊ�˲����ᱻ�޸�
		NULL, NULL, FALSE, 0, NULL, NULL, &si, &m_pi);
	if (!bRet)
	{
		int nError = GetLastError();
		return false;
	}

	// �ȴ��½��̳�ʼ�����
	WaitForInputIdle(m_pi.hProcess, INFINITE);   

	
	// ����������
	m_strCaption = pszCaption;
	EnumWindows(EnumWindowCallBack, (LPARAM)this);

	if (m_hMainWnd == NULL)
	{
		return false;
	}  

	return true;
}

bool CProcessController::IsRunning()
{
	if (m_pi.hProcess != INVALID_HANDLE_VALUE)
	{
		return WaitForSingleObject(m_pi.hProcess, 0) != WAIT_OBJECT_0;
	}
	return false;
}

void CProcessController::SetAutoExit(bool bAutoExit)
{
	m_bAutoExit = bAutoExit;
}

DWORD CProcessController::WaitForExit(DWORD dwMilliseconds)
{
	if (m_pi.hProcess != INVALID_HANDLE_VALUE)
	{
		return WaitForSingleObject(m_pi.hProcess, dwMilliseconds);
	}
	return WAIT_FAILED;
}
	
DWORD CProcessController::GetExitCode()
{
	if (m_pi.hProcess != INVALID_HANDLE_VALUE)
	{
		DWORD dwExitCode;
		GetExitCodeProcess(m_pi.hProcess, &dwExitCode);
		return dwExitCode;
	}
	return 0;
}

void CProcessController::Terminate()
{
	if (m_pi.hThread != INVALID_HANDLE_VALUE && m_pi.hProcess != INVALID_HANDLE_VALUE)
	{
		// �ر��ӽ��̵����߳̾��
		CloseHandle(m_pi.hThread);
		// ǿ���˳��ӽ���
		TerminateProcess(m_pi.hProcess, 0);
		// �ȴ������˳�
		WaitForSingleObject(m_pi.hProcess, INFINITE);
		// �ر��ӽ��̾��
		CloseHandle(m_pi.hProcess);
	}

	m_pi.hThread = INVALID_HANDLE_VALUE;
	m_pi.hProcess = INVALID_HANDLE_VALUE;
	m_hMainWnd = NULL;
}