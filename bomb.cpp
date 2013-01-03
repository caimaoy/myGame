#include"bomb.h"
#include"MyMap.h"
#include"player.h"
#include"pSystem.h"
#include<windows.h>

extern int tiles[2][13][15];
extern player player1;
extern player player2;
extern map<int, Bomb> m;
extern map<int, Firework> f;
extern IDirect3DDevice9* Device; 

const int Bomb::EXP_DELAY = 3000;


Bomb::Bomb()
{
	m_BombState = STATE_NORMAL;
	m_iStartTime = timeGetTime();
	m_iExplodeClock = 0;	
	m_iRow = -1;
	m_iCol = -1;
	m_iPower = 0;
	m_iURDL[0] = 1;
	m_iURDL[1] = 1;
	m_iURDL[2] = 1;
	m_iURDL[3] = 1;
	m_bURDL[0] = true;
	m_bURDL[1] = true;
	m_bURDL[2] = true;
	m_bURDL[3] = true;

}

Bomb::Bomb(int row, int col)
{
	m_iPlace = 100 * row + col;
	m_BombState = STATE_NORMAL;
	m_iStartTime = timeGetTime();
	m_iExplodeClock = 0;
	m_iRow = row;
	m_iCol = col;
	m_iPower = 2;
	m_iURDL[0] = 1;
	m_iURDL[1] = 1;
	m_iURDL[2] = 1;
	m_iURDL[3] = 1;
	m_bURDL[0] = true;
	m_bURDL[1] = true;
	m_bURDL[2] = true;
	m_bURDL[3] = true;


}

Bomb::Bomb(int row, int col, int power)
{
	m_iPlace = 100 * row + col;
	m_iPower = power;
	m_BombState = STATE_NORMAL;
	m_iStartTime = timeGetTime();
	MyMap::RemoveFeature(row, col , MyMap::CAN_PASS);
	MyMap::AddFeature (row, col, MyMap::BOMB );
	m_iExplodeClock = 0;
	m_iRow = row;
	m_iCol = col;
	m_iFrom = 0;
	m_iURDL[0] = 1;
	m_iURDL[1] = 1;
	m_iURDL[2] = 1;
	m_iURDL[3] = 1;
	m_bURDL[0] = true;
	m_bURDL[1] = true;
	m_bURDL[2] = true;
	m_bURDL[3] = true;


}


Bomb::Bomb(int row, int col, int power, int from)
{
	m_iPlace = 100 * row + col;
	m_iPower = power;
	m_BombState = STATE_NORMAL;
	m_iStartTime = timeGetTime();
	MyMap::RemoveFeature(row, col , MyMap::CAN_PASS);
	MyMap::AddFeature (row, col, MyMap::BOMB );
	m_iExplodeClock = 0;
	m_iRow = row;
	m_iCol = col;
	m_iFrom = from;
	m_iURDL[0] = 1;
	m_iURDL[1] = 1;
	m_iURDL[2] = 1;
	m_iURDL[3] = 1;
	m_bURDL[0] = true;
	m_bURDL[1] = true;
	m_bURDL[2] = true;
	m_bURDL[3] = true;


}

void Bomb::Update ()
{
	if((timeGetTime() - m_iStartTime) > Bomb::EXP_DELAY)
	{
		m_BombState = STATE_EXPLODE;
	}

    // 是否完成爆炸, 清空爆炸效果
    if (m_iExplodeClock >= 4)
    {
		ClearExplode();
        return;
    }
    
    if(m_BombState == STATE_EXPLODE)
    {
        // 爆炸效果
        Explode();
//		D3DXVECTOR3 origin(m_iRow, 2.0f, m_iCol);
		D3DXVECTOR3 origin(m_iCol, 2.0f, m_iRow);
		Firework* Exp = new Firework(&origin, 30, m_iURDL);
		Exp->init(Device, "flare.bmp");
		f.insert (pair<int, Firework>(Exp->m_iPplace , *Exp));
		delete Exp;
		Exp = NULL;

    }
}

void Bomb::Explode ()
{
    m_BombState = STATE_EXPLODE;
    m_iExplodeClock++;
 //   itemsLayer.setCell(col, row, 4 + explodeClock);
	MyMap::AddFeature(m_iRow, m_iCol, MyMap::DEADLY);
    int i;
    // 右方冲击波
    for (i = 1; i <= m_iPower; i++)
    {
        // 是否超出边界，超出则不检测
        if (m_iCol + i <= MyMap::MAX_X && m_bURDL[1])
        {
            // 引爆该单元格
            Detonate(m_iRow, m_iCol + i);

            // 单元格是否可通过
            if (MyMap::HasFeature(m_iRow, m_iCol + i, MyMap::CAN_PASS))
            {
                // 设置单元格为致命
				MyMap::AddFeature(m_iRow, m_iCol + i, MyMap::DEADLY);
				m_iURDL[1] = i;
            }
            else
            {
				MyMap::Destroy(m_iRow, m_iCol + i);
				m_bURDL[1] = false;
				m_iURDL[1] = i;
                break;
            }
        }
    }

	  // 上方冲击波
    for (i = 1; i <= m_iPower; i++)
    {
        // 是否超出边界，超出则不检测
        if (m_iRow + i <= MyMap::MAX_Y && m_bURDL[0])
        {
            // 引爆该单元格
            Detonate(m_iRow + i, m_iCol);

            // 单元格是否可通过
            if (MyMap::HasFeature(m_iRow + i, m_iCol, MyMap::CAN_PASS))
            {
                // 设置单元格为致命
				MyMap::AddFeature(m_iRow + i, m_iCol, MyMap::DEADLY);
 				m_iURDL[0] = i;
           }
            else
            {
				MyMap::Destroy(m_iRow + i, m_iCol);
			    m_bURDL[0] = false;
				m_iURDL[0] = i;     
				break;

            }
        }
    }

	    // 左方冲击波
    for (i = 1; i <= m_iPower; i++ )
    {
        // 是否超出边界，超出则不检测
        if (m_iCol - i >= 0 && m_bURDL[3])
        {
            // 引爆该单元格
            Detonate(m_iRow, m_iCol - i);

            // 单元格是否可通过
            if (MyMap::HasFeature(m_iRow, m_iCol - i, MyMap::CAN_PASS))
            {
                // 设置单元格为致命
				MyMap::AddFeature(m_iRow, m_iCol - i, MyMap::DEADLY);
				m_iURDL[3] = i;
           }
            else
            {
				MyMap::Destroy(m_iRow, m_iCol - i);
			    m_bURDL[3] = false;
				m_iURDL[3] = i;
			
				break;
            }
        }
    }

	  // 下6方冲击波
    for (i = 1; i <= m_iPower; i++)
    {
        // 是否超出边界，超出则不检测
        if (m_iRow - i >= 0 && m_bURDL[2])
        {
            // 引爆该单元格
            Detonate(m_iRow - i, m_iCol);

            // 单元格是否可通过
            if (MyMap::HasFeature(m_iRow - i, m_iCol, MyMap::CAN_PASS))
            {
                // 设置单元格为致命
				MyMap::AddFeature(m_iRow - i, m_iCol, MyMap::DEADLY);
  				m_iURDL[2] = i;
          }
            else
            {
				MyMap::Destroy(m_iRow - i, m_iCol);
				m_bURDL[2] = false;
			    m_iURDL[2] = i;
			
			 break;
            }
        }
    }
}

void Bomb::ClearExplode()
{
	if(m_iFrom == 1)
		player1.m_iNumber ++;
	if(m_iFrom == 2)
		player2.m_iNumber ++;
	
	MyMap::RemoveFeature (m_iRow, m_iCol, MyMap::BOMB);
	MyMap::RemoveFeature (m_iRow, m_iCol, MyMap::DEADLY);
	MyMap::AddFeature(m_iRow, m_iCol, MyMap::CAN_PASS);
	
	int i;
    for (i = 1; i <= m_iURDL[1]; i++)
    {
        // 检测右方
        if (m_iCol + i <= MyMap::MAX_X )
        {
			if(MyMap::HasFeature (m_iRow, m_iCol + i, MyMap::DEADLY))
				MyMap::RemoveFeature(m_iRow, m_iCol + i, MyMap::DEADLY);
        }
    }
     
    for (i = 1; i <= m_iURDL[3]; i++)
    {
        // 检测左方
        if (m_iCol - i >= 0)
        {
            if(MyMap::HasFeature (m_iRow, m_iCol - i, MyMap::DEADLY))
				MyMap::RemoveFeature (m_iRow, m_iCol - i, MyMap::DEADLY);
        }
    }
        
    for (i = 1; i <= m_iURDL[2]; i++)
    {
        // 检测下方
        if (m_iRow - i >= 0)
        {
            if(MyMap::HasFeature (m_iRow - i, m_iCol, MyMap::DEADLY))
				MyMap::RemoveFeature(m_iRow - i, m_iCol, MyMap::DEADLY);
        }
    }
        
    for (i = 1; i <= m_iURDL[0]; i++)
    {
        // 检测上方
        if (m_iRow + i <=  MyMap::MAX_Y)
        {
            if(MyMap::HasFeature (m_iRow + i, m_iCol, MyMap::DEADLY))
			  MyMap::RemoveFeature(m_iRow + i, m_iCol, MyMap::DEADLY);
        }
    }
	map<int, Bomb>::iterator iter;
	iter = m.find(100 * m_iRow + m_iCol);
	if(iter != m.end())
	{
		m.erase(iter);		
	}


}

bool Bomb::Detonate(int row, int col)
{
	map<int, Bomb>::iterator iter;
	iter = m.find(100 * row + col);
	if(iter != m.end())
	{
		iter->second.m_BombState = STATE_EXPLODE;
		return true;
	}
	else 
	{
		tiles[MyMap::LAYER_BUILDING ][row][col] = 0;
		return false;
	}
}