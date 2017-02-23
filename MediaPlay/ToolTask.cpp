#include "stdafx.h"
#include "ToolTask.h"
#include "MainWnd.h"


CToolTask::CToolTask(const std::string& name)
	: Task(name)
{
}


CToolTask::~CToolTask(void)
{
}


CPlayTask::CPlayTask() 
	: CToolTask("PlayTask")
{
	
}

CPlayTask::~CPlayTask()
{

}

void CPlayTask::runTask()
{
	while (!isCancelled())
	{
		if (g_pMainWndDlg->m_pPlayBtn->GetToolTip().CompareNoCase(STR_PAUSE.c_str()) == 0)
		{
			Sleep(50);
			g_pMainWndDlg->m_d3d.Render();
		}
		else
		{
			//::KillTimer(pMainDlg->m_hWnd, 1);
			//::ShowWindow(pMainDlg->m_pMediaWnd->GetHWND(), SW_HIDE);
			Sleep(1000);
		}
	}
}

CParseTask::CParseTask() 
	: CToolTask("ParseTask")
{

}

CParseTask::~CParseTask()
{

}

void CParseTask::runTask()
{
	/*	��ȡ��������Ϣ	*/
	H264_LIBINFO_S    lib_info;
	if ( 0 == Hi264DecGetInfo(&lib_info) )
	{
		LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("�汾: %s\n��Ȩ: %s\n������: 0x%x"), CStringUtil::AnsiToTStr(lib_info.sVersion).c_str(), CStringUtil::AnsiToTStr(lib_info.sCopyRight).c_str(), lib_info.uFunctionSet);
	}
	else
	{
		LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��ѯ�����汾��Ϣʧ��"));
	}

	g_pMainWndDlg->m_pAvi = AVI_open_input_file((CStringUtil::TStrToAnsi(g_pMainWndDlg->m_strPathName)).c_str(), 1);
	if (nullptr == g_pMainWndDlg->m_pAvi)
	{
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("�޷��� H264 �ļ� %s"), g_pMainWndDlg->m_strPathName.c_str());
		return;
	}
	else if (memcmp(AVI_video_compressor(g_pMainWndDlg->m_pAvi), "h264", 4) != 0)
	{
		LogMsg(WT_EVENTLOG_WARNING_TYPE, _T("%s ��AVI�ļ��� H264 �ļ�"), g_pMainWndDlg->m_strPathName.c_str());
		AVI_close(g_pMainWndDlg->m_pAvi);
		g_pMainWndDlg->m_pAvi = nullptr;

		return;
	}
	g_pMainWndDlg->m_d3d.InitD3D(g_pMainWndDlg->m_pMediaWnd->GetHWND(), g_pMainWndDlg->m_pAvi->width, g_pMainWndDlg->m_pAvi->height);
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("�����ļ�: %s"), g_pMainWndDlg->m_strPathName.c_str());
	g_pMainWndDlg->ClickPlayBtn(STR_PAUSE, true);
	if (g_pMainWndDlg->m_spPlayTask == nullptr)
	{
		g_pMainWndDlg->m_spPlayTask = new CPlayTask;
	}
	g_pMainWndDlg->m_taskManager.start(g_pMainWndDlg->m_spPlayTask);

	/*	���ý���������	*/
	H264_DEC_ATTR_S   dec_attrbute;
	memset(&dec_attrbute, 0x00, sizeof(dec_attrbute));
	dec_attrbute.uPictureFormat = 0x00;		/*	���ͼ���ʽ��0x00��ʾ4:2:0	*/
	dec_attrbute.uBufNum        = 16;		/*	�ο�֡�Ļ�������Ŀ			*/
	dec_attrbute.uPicWidthInMB  = 120;		/*	1080p	*/
	dec_attrbute.uPicHeightInMB = 68;		/*	1080p	*/
	dec_attrbute.uStreamInType  = 0x00;		/* ����������ʽ  "00 00 01" or "00 00 00 01"	*/
	dec_attrbute.pUserData  = nullptr;   // ���û�����
	
	dec_attrbute.uWorkMode = 0x10;		/* bit0 = 1: H.264 ���ģʽ; bit0 = 0: �������ģʽ */
	//dec_attrbute.uWorkMode |= 0x10;	/* bit4 = 1:  �������;    bit4 = 0: disable ������ */
	//dec_attrbute.uWorkMode |= 0x20;		/* �������߳�	*/
	

	HI_HDL hDecoder = Hi264DecCreate(&dec_attrbute);		/*	����������	*/
	if(nullptr ==  hDecoder)
	{
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("����������ʧ��"));
		AVI_close(g_pMainWndDlg->m_pAvi);
		g_pMainWndDlg->m_pAvi = nullptr;
		return;
	}
	
	LARGE_INTEGER lpFrequency;
	QueryPerformanceFrequency(&lpFrequency);
	LARGE_INTEGER timeStart;
	QueryPerformanceCounter(&timeStart);

	HI_S32 end = 0;
	HI_U8  buf[BUFF_LEN];	/*	����������	*/
	H264_DEC_FRAME_S  dec_frame;
	HI_U32 pic_cnt = 0;
	HI_U32 ImageEnhanceEnable = 1;
	HI_U32 StrenthCoeff = 40;	/*	ͼ����ǿϵ��	*/
	HI_U32 tmpW = 0;
	HI_U32 tmpH = 0;
	HI_U32 curFrm = 0;
	/* ���� h264 ���ļ� */
	while (!isCancelled())
	{
		end = 0;
		HI_U32	len = AVI_read_frame(g_pMainWndDlg->m_pAvi, (char*)buf);
		HI_U32  flags = (len > 0) ? 0 : 1;		/*	�����Ƿ������־	*/

		/*	�������һ���������н��벢��֡���ͼ��	*/
		HI_S32 result = Hi264DecFrame(hDecoder, buf, len, 0, &dec_frame, flags);
		while(HI_H264DEC_NEED_MORE_BITS  !=  result && g_pMainWndDlg->m_pAvi)
		{
			if(HI_H264DEC_NO_PICTURE ==  result)   //flush over and all the remain picture are output
			{
				end = 1;		/*	������������ͼ����������������	*/
				break;
			}

			if(HI_H264DEC_OK == result)   /*	��һ֡ͼ�����	*/
			{
				if (ImageEnhanceEnable)    //	ͼ����ǿ
				{
					Hi264DecImageEnhance(hDecoder, &dec_frame, StrenthCoeff);
				}

				const HI_U8 *pY = dec_frame.pY;
				const HI_U8 *pU = dec_frame.pU;
				const HI_U8 *pV = dec_frame.pV;
				HI_U32 width    = dec_frame.uWidth;
				HI_U32 height   = dec_frame.uHeight - dec_frame.uCroppingBottomOffset;
				HI_U32 yStride  = dec_frame.uYStride;
				HI_U32 uvStride = dec_frame.uUVStride;

				pic_cnt++;
				while (g_pMainWndDlg->m_d3d.Size() >= MAX_MEMORY_POOL)
				{
					Sleep(50);
				}
				USERDATA userData;
				userData.pszData = new char[height*yStride + height* uvStride/2 + height* uvStride/2];
				ASSERT(userData.pszData != nullptr);
				memcpy(userData.pszData, pY, height*yStride);
				memcpy(userData.pszData + height*yStride, pU, height* uvStride/2);
				memcpy(userData.pszData + height*yStride + height* uvStride/2, pV, height* uvStride/2);
				if (g_pMainWndDlg->m_pAvi)
				{
					userData.lPos = g_pMainWndDlg->m_pAvi->video_pos;
				}
				g_pMainWndDlg->m_d3d.Push(userData);

				if (tmpW != width || tmpH != height)
				{
					LogMsg(WT_EVENTLOG_WARNING_TYPE, _T("Size change-->width: %d, height: %d, stride: %d\n"), width, height, yStride);
					tmpW = width;
					tmpH = height;
				}

				/*	�洢һ��ͼ��	*/
				/*LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("PicBytes: %8d, I4:%4d, I8:%4d, 16:%4d, Pskip:%4d, P16:%4d, P16x8:%4d, P8x16:%4d, P8:%4d"), 
					dec_frame.pFrameInfo->uPicBytes,
					dec_frame.pFrameInfo->uI4MbNum,
					dec_frame.pFrameInfo->uI8MbNum,
					dec_frame.pFrameInfo->uI16MbNum,
					dec_frame.pFrameInfo->uPskipMbNum,
					dec_frame.pFrameInfo->uP16MbNum,
					dec_frame.pFrameInfo->uP16x8MbNum,
					dec_frame.pFrameInfo->uP8x16MbNum,
					dec_frame.pFrameInfo->uP8MbNum);*/

					
				#pragma region λͼ�洢
					/*	 λͼ�ļ�ͷ		*/
/*BITMAPFILEHEADER bmpFileHeader;
					ZeroMemory(&bmpFileHeader, sizeof(BITMAPFILEHEADER));
					bmpFileHeader.bfType = 0x4d42;
					bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 + width*height;
					bmpFileHeader.bfReserved1 = 0;
					bmpFileHeader.bfReserved2 = 0;
					bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;

					/*	λͼ��Ϣͷ		*/
					/*BITMAPINFOHEADER bmp_infoheader;
					ZeroMemory(&bmp_infoheader, sizeof(BITMAPINFOHEADER));
					bmp_infoheader.biSize = sizeof(BITMAPINFOHEADER);
					bmp_infoheader.biWidth = width;
					bmp_infoheader.biHeight = height;
					bmp_infoheader.biPlanes = 1;
					bmp_infoheader.biBitCount = 8;
					bmp_infoheader.biCompression = BI_RGB;
					bmp_infoheader.biSizeImage = 0;			//ͼ���С����ѹ�������ݣ��������Ϊ0
					bmp_infoheader.biXPelsPerMeter = 0;
					bmp_infoheader.biYPelsPerMeter = 0;
					bmp_infoheader.biClrUsed = 0;
					bmp_infoheader.biClrImportant = 0;

					//	����Ҷ�ͼ�ĵ�ɫ��
					RGBQUAD rgbquad[256];
					for(int i = 0; i < 256; i++)
					{
						rgbquad[i].rgbBlue = i;
						rgbquad[i].rgbGreen = i;
						rgbquad[i].rgbRed = i;
						rgbquad[i].rgbReserved = 0;
					}

					fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, yuv);
					fwrite(&bmp_infoheader, sizeof(BITMAPINFOHEADER), 1, yuv);
					fwrite(&rgbquad, sizeof(RGBQUAD)*256, 1, yuv);
					fwrite(pY, height*width, 1, yuv);*/
					#pragma endregion λͼ�洢

				/*fwrite(pY, 1, height* yStride, yuv);
				fwrite(pU, 1, height* uvStride/2, yuv);
				fwrite(pV, 1, height* uvStride/2, yuv);*/
			}
			/* ��������ʣ������� */
			result = Hi264DecFrame(hDecoder, NULL, 0, 0, &dec_frame, flags);
		}

		if (end)
		{
			Sleep(1000);
		}
	}

	LARGE_INTEGER timeEnd;
	QueryPerformanceCounter(&timeEnd);
	HI_U32 time = (HI_U32)((timeEnd.QuadPart - timeStart.QuadPart)*1000/lpFrequency.QuadPart);
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("time= %d ms \nframes = %d \nfps = %d"), time, pic_cnt, pic_cnt*1000/(time+1));

	/* ���ٽ��������ͷž�� */
	Hi264DecDestroy(hDecoder);
	hDecoder = nullptr;
}
