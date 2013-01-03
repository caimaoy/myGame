#ifndef __FUNCS_H__
#define __FUNCS_H__

/*inline float min(float a, float b) 
{
	return a > b ? b : a ;
}

inline float max(float a, float b) 
{
	return a > b ? a : b ;
}
*/
inline float myabs(float a)
{
	return a > 0 ? a : -a;
}
#endif