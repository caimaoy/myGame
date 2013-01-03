#include "MyMap.h"

	const int MyMap::CAN_PASS = 1;
	const int MyMap::CAN_DESTROY = 2;
	const int MyMap::CAN_PUSH = 4;
	const int MyMap::DEADLY = 8;
	const int MyMap::BOMB = 16;
	const int MyMap::TOOL_BOMB  = 29;
	const int MyMap::TOOL_POWER = 30;
	const int MyMap::TOOL_SPEED = 31;
	const int MyMap::TOOL_POWER_MAX = 32;


	const int MyMap::LAYER_BUILDING = 0;
	const int MyMap::LAYER_FEATURE = 1;

	const int MyMap::BOX = 1;
	const int MyMap::SAND = 2;


	const int MyMap::MAX_X = 14;
	const int MyMap::MAX_Y = 12;
	const int MyMap::EPSILON = 0.3;
	const int MyMap::BOMB_DT = 1;
/*int tiles[3][7][8] = 
{
	{
		{ 1, 1, 1, 4, 4, 1, 1, 1 },
		{ 1, 1, 1, 2, 2, 1, 1, 1 },
		{ 1, 1, 1, 2, 2, 1, 1, 1 },
		{ 1, 1, 1, 2, 2, 1, 1, 1 },
		{ 1, 1, 1, 2, 2, 1, 1, 1 },
		{ 1, 1, 1, 2, 2, 1, 1, 1 },
		{ 1, 1, 1, 4, 4, 1, 1, 1 }
	},

	{
		{ 0, 0, 6, 11, 0, 6, 16, 21 },
		{ 0, 21, 5, 0, 11, 5, 7, 11 },
		{ 6, 16, 6, 11, 0, 0, 21, 16 },
		{ 5, 6, 5, 0, 11, 5, 6, 5 },
		{ 16, 21, 0, 11, 0, 6, 21, 6 },
		{ 11, 8, 5, 0, 11, 5, 16, 0 },
		{ 21, 16, 6, 11, 0, 6, 0, 0 }
	}, 

	{
		{ 1, 1, 0, 6, 1, 0, 2, 2 },
		{ 1, 2, 3, 1, 6, 3, 0, 6 },
		{ 0, 2, 0, 6, 1, 1, 2, 2 },
		{ 3, 0, 3, 1, 6, 3, 0, 3 },
		{ 2, 2, 1, 6, 1, 0, 2, 0 },
		{ 6, 0, 3, 1, 6, 3, 2, 1 },
		{ 2, 2, 0, 6, 1, 0, 1, 1 }
	} ,

};*/
int tiles[2][13][15] = 
{
	{
		{2,0,0,2,2,2,2,2,2,2,2,2,0,0,2,},
		{2,0,0,2,2,2,0,0,0,2,2,2,0,0,2,},
		{2,2,0,2,2,0,1,1,1,0,2,2,0,2,2,},
		{2,2,2,2,0,1,0,2,0,1,0,2,2,2,2,},
		{2,2,2,0,1,2,2,2,2,2,1,0,2,2,2,},
		{2,2,0,1,0,2,2,2,2,2,0,1,0,2,2,},
		{2,0,1,2,2,2,0,0,0,2,2,2,1,0,2,},	
		{2,0,1,0,2,2,2,2,2,2,2,0,1,0,2,},
		{2,0,1,2,2,2,2,2,2,2,2,2,1,0,2,},
		{2,0,1,0,2,2,2,2,2,2,2,0,1,0,2,},
		{2,2,0,1,0,2,0,1,0,2,0,1,0,2,2,},
		{2,0,2,0,1,1,1,0,1,1,1,0,2,0,2,},
		{2,2,2,2,0,0,0,2,0,0,0,2,2,2,2}
	},

	{
		{2,1,1,2,2,2,2,2,2,2,2,2,1,1,2,},
		{2,0,1,2,2,2,1,1,1,2,2,2,1,0,2,},
		{2,2,1,2,2,1,6,6,6,1,2,2,1,2,2,},
		{2,2,2,2,1,6,1,2,1,6,1,2,2,2,2,},
		{2,2,2,1,6,2,2,2,2,2,6,1,2,2,2,},
		{2,2,1,6,1,2,2,2,2,2,1,6,1,2,2,},
		{2,1,6,2,2,2,0,1,0,2,2,2,6,1,2,},
		{2,1,6,1,2,2,2,2,2,2,2,1,6,1,2,},
		{2,1,6,2,2,2,2,2,2,2,2,2,6,1,2,},
		{2,1,6,1,2,2,2,2,2,2,2,1,6,1,2,},
		{2,2,1,6,1,2,1,6,1,2,1,6,1,2,2,},
		{2,0,2,1,6,6,6,1,6,6,6,1,2,0,2,},
		{2,2,2,2,1,1,1,2,1,1,1,2,2,2,2,}
	}

};

bool MyMap::HasFeature(int row, int col, int feature)
{
	return((tiles[LAYER_FEATURE][row][col] & feature) != 0);
}

void MyMap::AddFeature(int row, int col, int feature)
{
	tiles[LAYER_FEATURE][row][col] |= feature;
}

void MyMap::RemoveFeature(int row, int col, int feature)
{
	tiles[LAYER_FEATURE][row][col] = (tiles[LAYER_FEATURE][row][col] | feature) - feature;
}

bool MyMap::Destroy(int row, int col)//29 30 31 32
{
	if(HasFeature(row,col,CAN_DESTROY))
	{
		RemoveFeature(row, col, CAN_DESTROY);
		RemoveFeature(row, col, CAN_PUSH);
		AddFeature(row, col, CAN_PASS);
		int temp_rand = rand();
		if( temp_rand & 1 ) 
		{
			temp_rand = rand() & 7;
			if(temp_rand == 0 || temp_rand == 4)
				tiles[LAYER_BUILDING][row][col] = 29;
			if(temp_rand == 1 )
				tiles[LAYER_BUILDING][row][col] = 30;
			if(temp_rand == 2 )// temp_rand == 6 && temp_rand == 7))
				tiles[LAYER_BUILDING][row][col] = 31;
			if(temp_rand == 3 )
			{
				temp_rand = rand() & 3;
				if(temp_rand == 1)
					tiles[LAYER_BUILDING][row][col] = 32;
			}
		}
			
		else
		{
			tiles[LAYER_BUILDING][row][col] = 0;
		}
	}
	else
	{
		return false;
	}

	    /*    if (tiles[Map.LAYER_BUILDING][row][col] > Map.ANIMATION_FRAMS
                && tiles[Map.LAYER_BUILDING][row][col] % Map.ANIMATION_FRAMS == 1)
        {
            tiles[Map.LAYER_BUILDING][row][col]++;
        }*/

	return true;
}
