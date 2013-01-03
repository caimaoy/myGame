#include"test.h"
#include"Bomb.h"
#include"player.h"
#include"pSystem.h"

GameEngine *g_pGame;

extern map<int, Bomb>m;
extern player player1;
extern player player2;
extern map<int, Firework> f;
extern IDirect3DDevice9* Device; 


BOOL GameInitialize(HINSTANCE hInstance)
{
	//������Ϸ����
	g_pGame = new GameEngine(hInstance, TEXT("My_Game"),
		TEXT("My_Game"), 0, 0, 800, 600);//IDI_BLIZZARD, IDI_BLIZZARD_SM);
	if(g_pGame == NULL)
		return FALSE;

	//����֡Ƶ
	g_pGame->SetFrameRate(60);


	return TRUE;
}

void GameStart(HWND hWindow)
{
	//�������������������
	srand(GetTickCount());
}

void GameEnd()
{
	//������Ϸ����
	delete g_pGame;

}

void GameActivate(HWND hWindow)
{
	HDC hDC;
	RECT rect;

	//����Ϸ��Ļ�ϻ����ı�
	GetClientRect(hWindow, &rect);
	hDC = GetDC(hWindow);
	DrawText(hDC,TEXT("Here comes the blizzard!"),-1, &rect,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	ReleaseDC(hWindow, hDC);
}

void GameDeactivate(HWND hWindow)
{
	HDC hDC;
	RECT rect;

	//����Ϸ��Ļ�ϻ���ͣ���ı�
	GetClientRect(hWindow, &rect);
	hDC = GetDC(hWindow);
	DrawText(hDC, TEXT("The blizzard has passed"), -1, &rect,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	ReleaseDC(hWindow, hDC);

}

void GamePaint(HDC hDC)
{
}

void GameCycle(float timeDelta)
{
/*	HDC hDC;
	HWND hWindow = g_pGame->GetWindow();
	
	//����Ϸ��Ļ�ϵ����λ�û���ѩƬͼ��
	hDC = GetDC(hWindow);
	DrawIcon(hDC, rand() % g_pGame->GetWidth(), rand() % g_pGame->GetHeight(),
		(HICON)(WORD)GetClassLong(hWindow, GCL_HICON));
	ReleaseDC(hWindow, hDC);*/
	//Display();
	player1.PlayerUpdate();
	player2.PlayerUpdate();
	map<int, Bomb>::iterator iter;
	for(iter = m.begin(); iter != m.end();)// iter++)
	{
		(iter++)->second.Update();
//		if((timeGetTime() -iter->second.m_iStartTime) > 3000)
//			m.erase(iter);
	}

	map<int, Firework>::iterator iter_f;
	if(!f.empty())
	for(iter_f = f.begin(); iter_f != f.end();)
	{
		Device->BeginScene();

		D3DXMATRIX T;
		D3DXMatrixIdentity(&T);

		D3DXMatrixTranslation(&T,   (iter_f->second.col) + 1  , 1.0f,    (iter_f->second.row) + 1 ); //ƫ������
//		D3DXMatrixTranslation(&T,  2 * (iter_f->second.row) + 1 , 2.0f,  2 * (iter_f->second.col) + 1); //ƫ������
		//		P =  Z * Y * R * T * S;
		Device->SetTransform(D3DTS_WORLD, &T);



		iter_f->second.render();

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		(iter_f++)->second.update(timeDelta, iter_f->second._origin);

	}

	for(iter_f = f.begin(); iter_f != f.end();)
	{
		if((iter_f)->second.isDead())
			f.erase(iter_f++);		
		else 
			iter_f++;
	}


}
