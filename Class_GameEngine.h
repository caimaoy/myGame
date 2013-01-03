#ifndef CLASS_GAMEENGINE_H
#define CLASS_GAMEENGINE_H

#include<windows.h>
#include <d3dx9.h>
#include <string>



//---------------------------------------------------------------------
//Windows 函数声明
//---------------------------------------------------------------------
int WINAPI		WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
						PSTR szCmdLine, int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//---------------------------------------------------------------------
//游戏引擎函数声明
//---------------------------------------------------------------------

BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWindow);
void GameEnd();
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC hDC);
void GameCycle();
void GameCycle(float timeDelta);

//---------------------------------------------------------------------
//GameEngine 类
//---------------------------------------------------------------------

class GameEngine
{
protected:
	//Menber Variables
	static GameEngine*	m_pGameEngine;
	HINSTANCE			m_hInstance;
	HWND				m_hWindow;
	TCHAR				m_szWindowClass[32];
	TCHAR				m_szTitle[32];
	WORD				m_wIcon, m_wSmallIcon;
	int					m_iWidth, m_iHeight;
	int					m_iFrameDelay;
	BOOL				m_bSleep;


public:

	GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass, LPTSTR szTitle,
		WORD wIcion, WORD wSmallIcon, int iWidth , int iHeight);
	virtual ~ GameEngine();

	//常规方法
	static GameEngine*	GetEngine(){return m_pGameEngine;}
	BOOL				Initialize(int iCmdShow);
	LRESULT				HandleEvent(HWND hWindow, UINT msg, WPARAM wParam,
							LPARAM lparam);
	
	//访问方法
	HINSTANCE		Getinstance() { return m_hInstance;};
	HWND			GetWindow() { return m_hWindow;};
	void			SetWindow ( HWND hWindow){m_hWindow = hWindow;};
	LPTSTR			GetTitle() { return m_szTitle;};
	WORD			GetIcon() { return m_wIcon;};
	WORD			GetSmallIcon() { return m_wSmallIcon;};
	int				GetWidth() { return m_iWidth;};
	int				GetHeight() { return m_iHeight;};
	int				GetFrameDelay() { return m_iFrameDelay;};
	void			SetFrameRate(int iFrameRate) { m_iFrameDelay =1000/iFrameRate;};
	BOOL			GetSleep() { return m_bSleep;};
	void			SetSleep(BOOL bSleep) { m_bSleep = bSleep;};

};




#endif