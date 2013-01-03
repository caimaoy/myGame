#ifndef __playerH__
#define __playerH__

enum Direction {DOWN, LEFT, UP, RIGHT};
enum PlayState {ALIVE, DYING, DIE};

#include<windows.h>
#include "MyMap.h"
//#include <math.h>


class player
{

public:	
	
	Direction m_Direction;
	PlayState m_PlayState;
	float m_iVelocity;
	int m_iNumber;
	int m_iPower;
	float m_fX;
	float m_fY;
	int m_iPlayNumber;
	int m_iDyingTime;

	player();
	player(int i);
	void SetBomb(map<int, Bomb>& ma);
    void PickUpTool(int tool);
	void Walk(Direction direction);
	void PlayerUpdate();


	static const float player::m_ciInitialVelocity;
	static const float player::m_ciMaxVelocity;
	static const int player::m_ciInitialNumber;
	static const int player::m_ciMaxNumber;
	static const int player::m_ciInitialPower;
	static const int player::m_ciMaxPower;

	//³¯Ïò
/*	static const int DOWN;
	static const int LEFT;
	static const int UP;
	static const int RIGHT;
*/

/*	float GetV(void) {return m_iVelocity;};
	int	GetN(void) {return m_iNumber;};
	int GetP(void) {return m_iPower;};
	float GetX(void) {return m_fX;};
	float GetY(void) {return m_fY;};

	const  float player::m_ciInitialVelocity ;
	const  float player::m_ciMaxvelocity ;
	const  int player::m_ciInitialNumber  ;
	const  int player::m_ciMaxNumber ;
	const  int player::m_ciInitialPower ;
	const  int player::m_ciMaxPower;
*/
	
};
//	const int player::player::m_ciInitialVelocity=1;


#endif