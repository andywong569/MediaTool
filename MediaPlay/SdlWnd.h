#pragma once

//ģ����Ƶ��  
typedef struct _tagFrame
{  
	Uint32  pixformat;	//���ظ�ʽ  
	int     w, h;		//���  
	Uint8   *data;		//��������  
}FRAME, *LPFRAME;  


class CSdlWnd
{
public:
	enum E_STATUS
	{
		E_INIT	= 0,
		E_REQ	= 1,
		E_STOP	= 2
	};

public:
	CSdlWnd(void);
	~CSdlWnd(void);

	int Init(void);
	void Quit(void);

	SDL_Window*		CreateWnd(HWND hWnd);	//	��������
	SDL_Renderer*	CreateRender();			//	������Ⱦ��
	SDL_Texture*	CreateTexture();		//	��������

	//ģ����������  
	void FillColorRgba8888();

	//��Ƶ��ʾ�߳�  
	//int SDLCALL video_process(void * data);

public:
	E_STATUS		m_emStatus;
	int				m_nScreenWidth;
	int				m_nScreenHeight;
	FRAME			m_frame;
	SDL_Window*		m_pWnd;
	SDL_Texture*	m_pTexture;
	SDL_Renderer*	m_pRender;
};

