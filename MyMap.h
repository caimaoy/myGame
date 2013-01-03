#ifndef  __MYMAP_H__
#define  __MYMAP_H__

#include<iostream>
#include<map>
using namespace std;
#include"Bomb.h"

//map<int,Bomb> m;
inline int round (float f)
{
	return int(f+0.5);
}

class MyMap
{
public:
	static bool HasFeature(int row, int col,  int feature);
	static void AddFeature(int row, int col, int feature);
	static void RemoveFeature(int row, int col, int feature);
	static bool Destroy(int row, int col);

	static const int CAN_PASS;
	static const int CAN_DESTROY;
	static const int CAN_PUSH;
	static const int DEADLY;
	static const int BOMB;
    /**
     * ը����Ŀ
     */
	static const int TOOL_BOMB;

    /**
     * ����ҩˮ
     */
	static const int TOOL_POWER;
    /**
     * �ٶ�Ь
     */
	static const int TOOL_SPEED;
    /**
     * ����MAX
     */
	static const int TOOL_POWER_MAX;
	 /**
     * ������
     */
    static const int LAYER_BUILDING;

    /**
     * ���Բ�
     */
    static const int LAYER_FEATURE;

	static const int MAX_X;

	static const int MAX_Y;

	static const int EPSILON;

	static const int BOMB_DT;

	static const int BOX;
	static const int SAND;

};


#endif