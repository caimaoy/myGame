#include"player.h"
#include"MyMap.h"
#include"bomb.h"
#include"funcs.h"

//extern map<int,Bomb> m;
extern int tiles[2][13][15];
extern player player1;
extern player player2;

	const float player::m_ciInitialVelocity =0.06;
	const float player::m_ciMaxVelocity = 0.12;
	const int player::m_ciInitialNumber = 1 ;
	const int player::m_ciMaxNumber = 7;
	const int player::m_ciInitialPower = 2;
	const int player::m_ciMaxPower =6;

/*	const int MyMap::TOOL_BOMB  = 29;
	const int MyMap::TOOL_POWER = 30;
	const int MyMap::TOOL_SPEED = 31;
*/
//	extern BOMB 
//	player::m_ciInitialNumber 
player::player()
{
	this->m_Direction = UP;
	this->m_iNumber = m_ciInitialNumber ;
	this->m_iPower = m_ciInitialPower;
	this->m_iVelocity = m_ciInitialVelocity;
	this->m_fX = 2.0;
	this->m_fY = 2.0;
	this->m_PlayState = ALIVE;
	this->m_iPlayNumber = 0;
	this->m_iDyingTime = 0;

}

player::player(int i)
{
	this->m_Direction = UP;
	this->m_iNumber = m_ciInitialNumber ;
	this->m_iPower = m_ciInitialPower;
	this->m_iVelocity = m_ciInitialVelocity;
	if(i == 1)
	{
		this->m_fX = 12.0;
		this->m_fY = 2.0;
	}
	else if(i == 2) 
	{	
		this->m_fX = 2.0;
		this->m_fY = 2.0;
	}
		
//	this->m_fX = 2.0;
//	this->m_fY = 2.0;
//	this->m_fY = 12.0;
	this->m_PlayState = ALIVE;
	this->m_iPlayNumber = i;
	this->m_iDyingTime = 0;

}

void player::SetBomb(map<int, Bomb>& ma)
{
/*	if(myabs(round(m_fY) - m_fY) > 0.2 ||
		myabs(round(m_fX) - m_fX) > 0.2 )
		return;
*/
  if(!MyMap::HasFeature (round(m_fY), round(m_fX), MyMap::BOMB))
	{
		if( m_PlayState == ALIVE && m_iNumber > 0)
		{
			Bomb *p = new Bomb(round(m_fY), round(m_fX), m_iPower, m_iPlayNumber);
			ma.insert (pair<int , Bomb>(p->m_iPlace ,*p));
			delete p;
			p = NULL;
			m_iNumber--;
		}
	}
}

void player::PickUpTool(int tool)
{
    //	int a = MyMap::TOOL_BOMB;

	switch(tool)
    {
    case 29://MyMap::TOOL_BOMB:
        if(m_iNumber < m_ciMaxNumber)
			m_iNumber++;
        break;
    case 30://MyMap::TOOL_POWER:
		if(m_iPower < m_ciMaxPower)
			m_iPower++;
        break;
    case 31://MyMap::TOOL_SPEED:
		if(m_iVelocity < m_ciMaxVelocity)
			m_iVelocity += 0.01;
		break;
	case 32: //MyMap::TOOL_POWER_MAX
		m_iPower = m_ciMaxPower;
        break;
    default:
        break;
    }
}

void player::Walk(Direction direction)
{
	m_Direction = direction;

	if(m_PlayState != ALIVE)
		return;

	switch(direction)
	{
	case DOWN:
		if(round(m_fX - m_iVelocity - 1.5) >= 0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity -0.5)][round(m_fX - 0.4)] & MyMap::CAN_PUSH) !=0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - 0.5)][round(m_fX + 0.4)] & MyMap::CAN_PUSH) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - 1.5)][round(m_fX + 0.4)] & MyMap::CAN_PASS) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - 1.5)][round(m_fX - 0.4)] & MyMap::CAN_PASS) != 0
			&& !(round(m_fX ) == round(player1.m_fX) && round(m_fY - m_iVelocity - 1.5) == round(player1.m_fY))
			&& !(round(m_fX ) == round(player2.m_fX) && round(m_fY - m_iVelocity - 1.5) == round(player2.m_fY)))

		{
			MyMap::RemoveFeature  (round(m_fY - m_iVelocity - 0.5), round(m_fX), MyMap::CAN_DESTROY );
			MyMap::RemoveFeature  (round(m_fY - m_iVelocity - 0.5), round(m_fX), MyMap::CAN_PUSH  );
			MyMap::AddFeature  (round(m_fY - m_iVelocity - 0.5), round(m_fX), MyMap::CAN_PASS );
			
			MyMap::RemoveFeature (round(m_fY - m_iVelocity - 1.5), round(m_fX),  MyMap::CAN_PASS  );
			MyMap::AddFeature  (round(m_fY - m_iVelocity - 1.5), round(m_fX),  MyMap::CAN_PUSH );
			MyMap::AddFeature  (round(m_fY - m_iVelocity - 1.5), round(m_fX),  MyMap::CAN_DESTROY  );
			tiles[MyMap::LAYER_BUILDING ][round(m_fY - m_iVelocity -0.5)][round(m_fX)] = 0;
			tiles[MyMap::LAYER_BUILDING ][round(m_fY - m_iVelocity -1.5)][round(m_fX)] = MyMap::BOX;
			break;
		}
		else if((m_fY - m_iVelocity ) >= 0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - 0.5)][round(m_fX - 0.49)] & MyMap::CAN_PASS) !=0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - 0.5)][round(m_fX + 0.49)] & MyMap::CAN_PASS) != 0)
		
		{
			m_fY -= m_iVelocity;
		}
		else
		{
			if((m_fY - m_iVelocity ) >= 0 
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - 0.5)][round(m_fX - MyMap::EPSILON)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - 0.5)][round(m_fX + MyMap::EPSILON)] & MyMap::CAN_PASS) != 0)
			{
				m_fY -=m_iVelocity;
				m_fX = round(m_fX);
			}

			else if((tiles[MyMap::LAYER_FEATURE ][round(m_fY) ][round(m_fX)] & MyMap::BOMB) != 0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - MyMap::BOMB_DT)][round(m_fX - MyMap::EPSILON)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - m_iVelocity - MyMap::BOMB_DT)][round(m_fX + MyMap::EPSILON)] & MyMap::CAN_PASS) != 0)
			{
				m_fY -=m_iVelocity;
				m_fX = round(m_fX);

			}

		}
		

		break;


	case UP:
		if(round(m_fY + m_iVelocity + 1.5) <= MyMap::MAX_Y  
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity +0.5)][round(m_fX + 0.4)] & MyMap::CAN_PUSH) !=0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + 0.5)][round(m_fX - 0.4)] & MyMap::CAN_PUSH) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + 1.5)][round(m_fX - 0.4)] & MyMap::CAN_PASS) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + 1.5)][round(m_fX + 0.4)] & MyMap::CAN_PASS) != 0
			&& !(round(m_fX ) == round(player1.m_fX) && round(m_fY + m_iVelocity + 1.5) == round(player1.m_fY))
			&& !(round(m_fX ) == round(player2.m_fX) && round(m_fY + m_iVelocity + 1.5) == round(player2.m_fY)))
		{
			MyMap::RemoveFeature  (round(m_fY + m_iVelocity + 0.5), round(m_fX), MyMap::CAN_DESTROY );
			MyMap::RemoveFeature  (round(m_fY + m_iVelocity + 0.5), round(m_fX), MyMap::CAN_PUSH  );
			MyMap::AddFeature  (round(m_fY + m_iVelocity + 0.5), round(m_fX), MyMap::CAN_PASS );
			
			MyMap::RemoveFeature (round(m_fY + m_iVelocity + 1.5), round(m_fX),  MyMap::CAN_PASS  );
			MyMap::AddFeature  (round(m_fY + m_iVelocity + 1.5), round(m_fX),  MyMap::CAN_PUSH );
			MyMap::AddFeature  (round(m_fY + m_iVelocity + 1.5), round(m_fX),  MyMap::CAN_DESTROY  );
			tiles[MyMap::LAYER_BUILDING ][round(m_fY + m_iVelocity +0.5)][round(m_fX)] = 0;
			tiles[MyMap::LAYER_BUILDING ][round(m_fY + m_iVelocity +1.5)][round(m_fX)] = MyMap::BOX;
			break;
		}

		else if((m_fY + m_iVelocity ) <= MyMap::MAX_Y 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + 0.5) ][round(m_fX - 0.49)] & MyMap::CAN_PASS) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + 0.5) ][round(m_fX + 0.49)] & MyMap::CAN_PASS) != 0)
//			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY) ][round(m_fX)] & MyMap::BOMB) == 0)
		
		{
			m_fY += m_iVelocity;
		}
		else
		{
			if((m_fY + m_iVelocity ) <= MyMap::MAX_Y  
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + 0.5)][round(m_fX - MyMap::EPSILON)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + 0.5)][round(m_fX + MyMap::EPSILON)] & MyMap::CAN_PASS) != 0)
//				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY) ][round(m_fX)] & MyMap::BOMB) == 0)
			{
				m_fY +=m_iVelocity;
				m_fX = round(m_fX);
			}
			else if((tiles[MyMap::LAYER_FEATURE ][round(m_fY) ][round(m_fX)] & MyMap::BOMB) != 0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + MyMap::BOMB_DT)][round(m_fX - MyMap::EPSILON)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + m_iVelocity + MyMap::BOMB_DT)][round(m_fX + MyMap::EPSILON)] & MyMap::CAN_PASS) != 0)
			{
				m_fY +=m_iVelocity;
				m_fX = round(m_fX);

			}

/*
			if((m_fY + min( fabs((m_fY + 0.5) - int(m_fY + 0.5)) , m_iVelocity)) <= MyMap::MAX_Y )
				m_fY += min( fabs((m_fY + 0.5) - int(m_fY + 0.5)) , m_iVelocity);
			if((m_fY + min( (m_fY + 0.5) - int(m_fY + 0.5) , m_iVelocity)) <= MyMap::MAX_Y )
				m_fY += min( 1 + int(m_fY + 0.5) - (m_fY + 0.5) , m_iVelocity);
			
			if(m_fX - int(m_fX) <= MyMap::EPSILON  )
				m_fX -= min(m_iVelocity, m_fX - int(m_fX));
			else if(int(m_fX) + 1 - m_fX <= MyMap::EPSILON)
				m_fX += min(m_iVelocity, int(m_fX) + 1 - m_fX);
*/		}
		break;

	case LEFT:
		if(round(m_fX - m_iVelocity - 1.5) >= 0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - 0.4)][round(m_fX - m_iVelocity -0.5)] & MyMap::CAN_PUSH) !=0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + 0.4)][round(m_fX - m_iVelocity - 0.5)] & MyMap::CAN_PUSH) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + 0.4)][round(m_fX - m_iVelocity - 1.5)] & MyMap::CAN_PASS) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - 0.4)][round(m_fX - m_iVelocity - 1.5)] & MyMap::CAN_PASS) != 0
			&& !(round(m_fY) == round(player1.m_fY) && round(m_fX - m_iVelocity - 1.5) == round(player1.m_fX))
			&& !(round(m_fY) == round(player2.m_fY) && round(m_fX - m_iVelocity - 1.5) == round(player2.m_fX)))
			
		{
			MyMap::RemoveFeature (round(m_fY), round(m_fX - m_iVelocity - 0.5), MyMap::CAN_DESTROY );
			MyMap::RemoveFeature (round(m_fY), round(m_fX - m_iVelocity - 0.5), MyMap::CAN_PUSH  );
			MyMap::AddFeature  (round(m_fY), round(m_fX - m_iVelocity - 0.5), MyMap::CAN_PASS );
			
			MyMap::RemoveFeature (round(m_fY), round(m_fX - m_iVelocity - 1.5), MyMap::CAN_PASS  );
			MyMap::AddFeature  (round(m_fY), round(m_fX - m_iVelocity - 1.5), MyMap::CAN_PUSH );
			MyMap::AddFeature  (round(m_fY), round(m_fX - m_iVelocity - 1.5), MyMap::CAN_DESTROY  );
			tiles[MyMap::LAYER_BUILDING ][round(m_fY)][round(m_fX - m_iVelocity -0.5)] = 0;
			tiles[MyMap::LAYER_BUILDING ][round(m_fY)][round(m_fX - m_iVelocity -1.5)] = MyMap::BOX;
				break;
		}

		else if((m_fX - m_iVelocity ) >= 0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - 0.49)][round(m_fX - m_iVelocity -0.5)] & MyMap::CAN_PASS) !=0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + 0.49)][round(m_fX - m_iVelocity - 0.5)] & MyMap::CAN_PASS) != 0)
		
		{
			m_fX -= m_iVelocity;
			break;
		}
		else
		{
			if((m_fX - m_iVelocity ) >= 0 
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - MyMap::EPSILON)][round(m_fX - m_iVelocity - 0.5)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + MyMap::EPSILON)][round(m_fX - m_iVelocity - 0.5)] & MyMap::CAN_PASS) != 0)
			{
				m_fX -=m_iVelocity;
				m_fY = round(m_fY);
				break;
			}
			else if((tiles[MyMap::LAYER_FEATURE ][round(m_fY) ][round(m_fX)] & MyMap::BOMB) != 0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - MyMap::EPSILON)][round(m_fX - m_iVelocity - MyMap::BOMB_DT)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + MyMap::EPSILON)][round(m_fX - m_iVelocity - MyMap::BOMB_DT)] & MyMap::CAN_PASS) != 0)
			{
				m_fX -=m_iVelocity;
				m_fY = round(m_fY);
			}

		}
		break;

	case RIGHT:
		if(round(m_fX + m_iVelocity + 1.5) <= MyMap::MAX_X 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + 0.4)][round(m_fX + m_iVelocity +0.5)] & MyMap::CAN_PUSH) !=0 
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - 0.4)][round(m_fX + m_iVelocity + 0.5)] & MyMap::CAN_PUSH) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - 0.4)][round(m_fX + m_iVelocity + 1.5)] & MyMap::CAN_PASS) != 0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + 0.4)][round(m_fX + m_iVelocity + 1.5)] & MyMap::CAN_PASS) != 0
			&& !(round(m_fY) == round(player1.m_fY) && round(m_fX + m_iVelocity + 1.5) == round(player1.m_fX))
			&& !(round(m_fY) == round(player2.m_fY) && round(m_fX + m_iVelocity + 1.5) == round(player2.m_fX)))

		{
			MyMap::RemoveFeature (round(m_fY), round(m_fX + m_iVelocity + 0.5), MyMap::CAN_DESTROY );
			MyMap::RemoveFeature (round(m_fY), round(m_fX + m_iVelocity + 0.5), MyMap::CAN_PUSH  );
			MyMap::AddFeature  (round(m_fY), round(m_fX + m_iVelocity + 0.5), MyMap::CAN_PASS );
			
			MyMap::RemoveFeature (round(m_fY), round(m_fX + m_iVelocity + 1.5), MyMap::CAN_PASS  );
			MyMap::AddFeature  (round(m_fY), round(m_fX + m_iVelocity + 1.5), MyMap::CAN_PUSH );
			MyMap::AddFeature  (round(m_fY), round(m_fX + m_iVelocity + 1.5), MyMap::CAN_DESTROY  );
			tiles[MyMap::LAYER_BUILDING ][round(m_fY)][round(m_fX + m_iVelocity +0.5)] = 0;
			tiles[MyMap::LAYER_BUILDING ][round(m_fY)][round(m_fX + m_iVelocity +1.5)] = MyMap::BOX;
			break;
		}
		else if((m_fX + m_iVelocity ) <= MyMap::MAX_X  
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - 0.49)][round(m_fX + m_iVelocity + 0.5)] & MyMap::CAN_PASS) !=0
			&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + 0.49)][round(m_fX + m_iVelocity + 0.5)] & MyMap::CAN_PASS) != 0)
		
		{
			m_fX += m_iVelocity;
		}
		else
		{
			if((m_fX + m_iVelocity ) <= MyMap::MAX_X 
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - MyMap::EPSILON)][round(m_fX + m_iVelocity + 0.5)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + MyMap::EPSILON)][round(m_fX + m_iVelocity + 0.5)] & MyMap::CAN_PASS) != 0)
			{
				m_fX +=m_iVelocity;
				m_fY = round(m_fY);
			}
			else if((tiles[MyMap::LAYER_FEATURE ][round(m_fY) ][round(m_fX)] & MyMap::BOMB) != 0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY - MyMap::EPSILON)][round(m_fX + m_iVelocity + MyMap::BOMB_DT)] & MyMap::CAN_PASS) !=0
				&& (tiles[MyMap::LAYER_FEATURE ][round(m_fY + MyMap::EPSILON)][round(m_fX + m_iVelocity + MyMap::BOMB_DT)] & MyMap::CAN_PASS) != 0)
			{
				m_fX +=m_iVelocity;
				m_fY = round(m_fY);

			}

		}
		break;

	default:
		break;
	}


}




void player::PlayerUpdate ()
{
	if(m_PlayState == ALIVE)
	{
		int x = round(m_fX);
		int y = round(m_fY);
		int tool = tiles[MyMap::LAYER_BUILDING][y][x];
		if (tool >= MyMap::TOOL_BOMB ) //29 30 31 32
		{
			PickUpTool(tool);
			tiles[MyMap::LAYER_BUILDING][y][x] = 0;
		}
		if (MyMap::HasFeature(y, x, MyMap::DEADLY))
		{
			m_PlayState = DYING;
		}

	}
	else if(m_PlayState == DYING)
	{
		if (m_iDyingTime++ > 100)
			m_PlayState = DIE;
	}



}

