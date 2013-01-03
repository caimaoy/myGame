#include"Class_GameEngine.h"
#include"d3dUtility.h"
#include "test.h"
#include <windows.h>
#include "camera.h"
#include "cube.h"
#include "player.h"
#include "pSystem.h"
#include <map>
using namespace std;
#include <cstdlib>
#include <ctime>
#include "d3dfont.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-----------------------------------------------------------
//静态变量初始化
//-----------------------------------------------------------
GameEngine *GameEngine::m_pGameEngine = NULL;

IDirect3DDevice9* Device = 0; 
D3DXMATRIX WorldMatrix;
IDirect3DVertexBuffer9* Triangle = 0;

HANDLE thread_handle[2];  // this is the handle to the thread
DWORD  thread_id[2];      // this is the id of the thread

map<int, Bomb> m;
map<int, Firework> f;

player player1(1);
player player2(2);

const int Width  = 800;
const int Height = 600;

CD3DFont* Font    = 0;

char Copyright[90];
/*Square g_square[15][13]=
{
	SAND,NONE,NONE,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,NONE,NONE,SAND,
	SAND,NONE,NONE,SAND,SAND,SAND,NONE,NONE,NONE,SAND,SAND,SAND,NONE,NONE,SAND,
	SAND,SAND,NONE,SAND,SAND,NONE,BOX,BOX,BOX,NONE,SAND,SAND,NONE,SAND,SAND,
	SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,
	SAND,SAND,SAND,NONE,BOX,SAND,SAND,SAND,SAND,SAND,BOX,NONE,SAND,SAND,SAND,
	SAND,SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,SAND,
	SAND,NONE,BOX,SAND,SAND,SAND,NONE,NONE,NONE,SAND,SAND,SAND,BOX,NONE,SAND,
	SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,
	SAND,NONE,BOX,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,BOX,NONE,SAND,
	SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,
	SAND,SAND,NONE,BOX,NONE,SAND,NONE,BOX,NONE,SAND,NONE,BOX,NONE,SAND,SAND,
	SAND,NONE,SAND,NONE,BOX,BOX,BOX,NONE,BOX,BOX,BOX,NONE,SAND,NONE,SAND,
	SAND,SAND,SAND,SAND,NONE,NONE,NONE,SAND,NONE,NONE,NONE,SAND,SAND,SAND,SAND
};*/

//const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

Camera TheCamera(Camera::AIRCRAFT);

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////

DWORD WINAPI Printer_Thread1(LPVOID data)
{
	sndPlaySound("bg.wav",SND_ASYNC|SND_NODEFAULT|SND_LOOP); //播放


return((DWORD)data);

}

DWORD WINAPI Printer_Thread2(LPVOID data)
{
	sndPlaySound("start.wav",SND_SYNC|SND_NODEFAULT); //播放


return((DWORD)data);

}

//
// Framework functions
//
bool Setup()
{
	//
	// Setup a basic scene.  The scene will be created the
	// first time this function is called.
	//
/*
	DrawBasicScene(Device, 0.0f); 
	Box = new Cube(Device);
	//
	// Create texture.
	//
	D3DXCreateTextureFromFile(
		Device,
		"boxtop.bmp",
		&Texboxtop);

	D3DXCreateTextureFromFile(
		Device,
		"boxsides.bmp",
		&Texboxsides);

	// 
	// Set Texture Filter States.
	//

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//
	*/
	// Set projection matrix.
	//
//	sndPlaySound("start.wav",SND_SYNC|SND_NODEFAULT); //播放

//	sndPlaySound("bg.wav",SND_SYNC|SND_NODEFAULT|SND_LOOP); //播放

//	Sleep(100000);
//	sndPlaySound(NULL,NULL); //停止
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.25f, // 45 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	thread_handle[0] = CreateThread(NULL,           // default security
		                     0,				 // default stack 
			   			     Printer_Thread1, // use this thread function
							 NULL,		 // user data sent to thread
							 0,				 // creation flags, 0=start now.
							 &thread_id[0]);	 // send id back in this var

	thread_handle[1] = CreateThread(NULL,           // default security
		                     0,				 // default stack 
			   			     Printer_Thread2, // use this thread function
							 NULL,		 // user data sent to thread
							 0,				 // creation flags, 0=start now.
							 &thread_id[1]);	 // send id back in this var

	Font = new CD3DFont("Times New Roman", 12, 100);
	Font->InitDeviceObjects( Device );
	Font->RestoreDeviceObjects();
	strcpy(Copyright , "Copyright (c) 2010 CHENYUE. All rights reserved");

	return true;
}

void Cleanup()
{
	// pass 0 for the first parameter to instruct cleanup.
	 DrawBasicScene(0, 0.0f);
	 if( Font )
	{
		Font->InvalidateDeviceObjects();
		Font->DeleteDeviceObjects();
		Delete<CD3DFont*>(Font);
	}
}

bool Display(float timeDelta)
{
	if( Device )
	{
		//
		// Update: Update the camera.
		//
		if( ::GetAsyncKeyState('T') & 0x8000f )
			TheCamera.walk(4.0f * timeDelta);

		if( ::GetAsyncKeyState('G') & 0x8000f )
			TheCamera.walk(-4.0f * timeDelta);

		if( ::GetAsyncKeyState('F') & 0x8000f )
			TheCamera.strafe(-4.0f * timeDelta);

		if( ::GetAsyncKeyState('H') & 0x8000f )
			TheCamera.strafe(4.0f * timeDelta);

		if( ::GetAsyncKeyState('Y') & 0x8000f )
			TheCamera.fly(4.0f * timeDelta);

		if( ::GetAsyncKeyState('U') & 0x8000f )
			TheCamera.fly(-4.0f * timeDelta);

/*		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			TheCamera.pitch(-1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			TheCamera.pitch(1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			TheCamera.yaw(-1.0f * timeDelta);
			
		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			TheCamera.yaw(1.0f * timeDelta);
*/

		if( ::GetAsyncKeyState('I') & 0x8000f )
			TheCamera.pitch(-1.0f * timeDelta);

		if( ::GetAsyncKeyState('K') & 0x8000f )
			TheCamera.pitch(1.0f * timeDelta);

		if( ::GetAsyncKeyState('J') & 0x8000f )
			TheCamera.yaw(-1.0f * timeDelta);
			
		if( ::GetAsyncKeyState('L') & 0x8000f )
			TheCamera.yaw(1.0f * timeDelta);

		if( ::GetAsyncKeyState('N') & 0x8000f )
			TheCamera.roll(1.0f * timeDelta);

		if( ::GetAsyncKeyState('M') & 0x8000f )
			TheCamera.roll(-1.0f * timeDelta);

		/*		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			TheCamera.pitch(-1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			TheCamera.pitch(1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			TheCamera.yaw(-1.0f * timeDelta);
			
		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			TheCamera.yaw(1.0f * timeDelta);
*/		
		//----------------------------------
		//player1 keys
		//----------------------------------	
		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			player1.Walk (DOWN);

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			player1.Walk (LEFT);
		
		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			player1.Walk (UP);

		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			player1.Walk (RIGHT);

		if( ::GetAsyncKeyState(VK_SPACE) & 0x8000f )
			player1.SetBomb(m);

		//----------------------------------
		//player2 keys
		//----------------------------------	
		if( ::GetAsyncKeyState('S') & 0x8000f )
			player2.Walk (DOWN);

		if( ::GetAsyncKeyState('A') & 0x8000f )
			player2.Walk (LEFT);
		
		if( ::GetAsyncKeyState('W') & 0x8000f )
			player2.Walk (UP);

		if( ::GetAsyncKeyState('D') & 0x8000f )
			player2.Walk (RIGHT);

		if( ::GetAsyncKeyState(VK_SHIFT) & 0x8000f )
			player2.SetBomb(m);

		// Update the view matrix representing the cameras 
        // new position/orientation.
		D3DXMATRIX V;
		TheCamera.getViewMatrix(&V);
		Device->SetTransform(D3DTS_VIEW, &V);

		//
		// Render
		//


		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();
		if( Font )
			Font->DrawText(20, 580, 0xff000000, Copyright);	


		DrawBasicScene(Device, 1.0f);
//		DrawPaoPao(Device, 1.0f);
//		DrawSky(Device, 100.0f);
		DrawPlayer(Device, 0.01f, player1);
		DrawPlayer2(Device, 0.01f, player2);
		static float i = 0.9;
		static bool add =true;
		if (add)
		{
			if(i<1.05)
				i += 0.01;
			else
				add = false;
		}
		else	
		{
			if(i > 0.95)
				i -= 0.004;
			else
				add = true;
		}

		DrawPaopao(Device, i, m);
//		D3DMATERIAL9 a = WHITE_MTRL;
//		Box->draw(0, &a, Texboxtop,Texboxsides);
//"Copyright (c) 2010 aimao & 陈阅. All rights reserved"

		Device->EndScene();
		Device->Present(0, 0, 0, 0);


	}
	return true;
}

//-----------------------------------------------------------
//Windows函数
//-----------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG msg;
	static int	iTickTrigger = 0;
	int			iTickCount;
	if(GameInitialize(hInstance))
	{
		//初始化游戏引擎
		if(!GameEngine::GetEngine ()->Initialize (iCmdShow))
			return FALSE;
			if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

		//进入住消息循环
		while(TRUE)
		{
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				//处理消息
				if(msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
		/*		//确保游戏引擎没有休眠
				if(!GameEngine::GetEngine ()->GetSleep())
				{*/
					//检测滴答计数，查看是否过了一个游戏周期
					iTickCount = GetTickCount();
					if(iTickCount > iTickTrigger)
					{
						iTickTrigger = iTickCount + GameEngine::GetEngine()->GetFrameDelay ();
						static float lastTime = (float)timeGetTime(); 
						
						float currTime  = (float)timeGetTime();
						float timeDelta = (currTime - lastTime)*0.001f;
					
						GameCycle(timeDelta);
						Display(timeDelta);
						lastTime = currTime;

					}
			//	}
			}
		}

		return (int)msg.wParam;
	}
	//结束游戏
	GameEnd();
	Cleanup();
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//将所有Windows消息都传递给游戏引擎
	return GameEngine::GetEngine()->HandleEvent(hWindow,msg,wParam,lParam);
}


//-----------------------------------------------------------
//GameEngine的构造函数/析构函数
//-----------------------------------------------------------

GameEngine::GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass,
					   LPTSTR szTitle, WORD wIcon,WORD wSmallIcon,int iWidth,int iHeight)
{
	//设置游戏引擎的成员变量
	m_pGameEngine = this;
	m_hInstance = hInstance;
	m_hWindow = NULL;
	if(lstrlen(szWindowClass) > 0)
		lstrcpy(m_szWindowClass, szWindowClass);
	if(lstrlen(szTitle) > 0)
		lstrcpy(m_szTitle, szTitle);
	m_wIcon = wIcon;
	m_wSmallIcon = wSmallIcon;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iFrameDelay =50;				//默认为20帧/秒
	m_bSleep = TRUE;
//	m_windowed = windowed;
//	m_deviceType = deviceType;
//	p_device  = device;
}

GameEngine::~GameEngine()
{
}

//-----------------------------------------------------------
//游戏引擎常规方法
//-----------------------------------------------------------


BOOL GameEngine::Initialize(int iCmdShow)
{
/*	WNDCLASSEX wndclass;

	//创建主窗口的窗口类
	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= CS_HREDRAW  |  CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= m_hInstance;
	wndclass.hIcon			= LoadIcon(m_hInstance,MAKEINTRESOURCE(GetIcon()));
	wndclass.hIconSm		= LoadIcon(m_hInstance,	MAKEINTRESOURCE(GetSmallIcon()));
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);//(HBRUSH)(COLOR_WINDOW +1);
	wndclass.lpszMenuName   = NULL;
	wndclass.lpszClassName	= m_szWindowClass;


	
	//注册窗口类
	if(!RegisterClassEx(&wndclass))
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return FALSE;
	}	

	//根据游戏大小计算窗口大小和位置
	int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME)*2,
		iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME)*2 +
		GetSystemMetrics(SM_CYCAPTION);
	if(wndclass.lpszMenuName !=NULL)
		iWindowHeight +=GetSystemMetrics(SM_CYMENU);
	int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth)/2,
		iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight)/2;

	//创建窗口
	m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
		WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
		iWindowHeight, NULL, NULL, m_hInstance, NULL);
	if(!m_hWindow)
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return FALSE;
	}	


	//显示和更改新窗口
	ShowWindow(m_hWindow, iCmdShow);
	UpdateWindow(m_hWindow);*/
	BOOL b_Windowed;
		if (MessageBox(NULL,"Would You Like To Run In Windowed Mode?", 
			"Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		b_Windowed=FALSE;							// Fullscreen Mode
	}

	if(! InitD3D(m_hInstance,
		this->GetWidth(), this->GetHeight(), b_Windowed, m_hWindow, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if(!m_hWindow)
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return FALSE;
	}	

	return TRUE;
}


LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//将 Windows 消息传递给游戏引擎成员函数
	switch (msg)
	{
	case WM_CREATE:
		//设置游戏窗口并开始游戏
		SetWindow(hWindow);
		GameStart(hWindow);
		return 0;

/*	case WM_SETFOCUS:
		//激活游戏并更新休眠状态
		GameActivate(hWindow);
		SetSleep(FALSE);
		return 0;

	case WM_KILLFOCUS:
		//停用游戏并更新休眠状态
		GameDeactivate(hWindow);
		SetSleep(TRUE);
		return 0;*/
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hWindow);
		break;

/*	case WM_PAINT:
		HDC hDC;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWindow, &ps);
		//绘制游戏
		GamePaint(hDC);

		EndPaint(hWindow, &ps);
		return 0;
*/
	case WM_DESTROY:
		//结束游戏并退出应用程序
		GameEnd();
		WaitForSingleObject(thread_handle[0], INFINITE);
		CloseHandle(thread_handle[0]);
		WaitForSingleObject(thread_handle[1], INFINITE);
		CloseHandle(thread_handle[1]);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWindow, msg, wParam, lParam);
}


