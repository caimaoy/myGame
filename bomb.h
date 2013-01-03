#ifndef __BOMB_H__
#define __BOMB_H__

#include<windows.h>
#include"MyMap.h"
#pragma comment(lib,"winmm.lib")

enum BombState{ STATE_NORMAL,STATE_EXPLODE};

class Bomb
{
public:
	BombState m_BombState;
	int m_iPlace;
	int m_iRow;
	int m_iCol;
	int m_iStartTime;
	int m_iPower;
	int m_iExplodeClock;
	int m_iFrom;
	int m_iURDL[4];
	bool  m_bURDL[4];

	Bomb();
	Bomb(int row, int col);
	Bomb(int row, int col, int power);
	Bomb::Bomb(int row, int col, int power, int from);

	void Update();
	void Explode();
	void ClearExplode();
	bool Detonate(int row, int col);

	static const int EXP_DELAY;

};
/*
class sort
{
public:
		bool operator() (Bomb const &_A, Bomb const &_B) const
	{
		if(_A.m_iStartTime < _B.m_iStartTime)
			return true;
		else 
			return false;
	};
};
*/

#endif