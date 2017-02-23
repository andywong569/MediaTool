#pragma once

#include "StringUtil.h"
#include <map>

#define  WM_ALTDOWN		(WM_USER+1000)
#define  WM_TRAY		(WM_USER+100) 

#define  APP_NAME    TEXT("����ע����޸Ĺ���")  

#define  ID_SHOW	4001
#define  ID_EXIT	4002

//////////////////////////////////////////////////////////////////////////
/*		CWConfig.ini�ļ��е�CONST	*/
const tstring cstrConfName = _T("CWConfig.ini");
const tstring cstrAutoRun = _T("AutoRun");
const tstring cstrShowWind = _T("ShowWind");
const tstring cstrBootDir = _T("BootDir");
const tstring cstrSilentLoad = _T("SilentLoad");
const tstring cstrExtendedKey = _T("ExtendedKey");

//////////////////////////////////////////////////////////////////////////


typedef std::map<tstring, tstring> mapString;