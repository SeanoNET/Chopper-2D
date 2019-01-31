#pragma once
#include "clsParent.h"
class clsChopper:
	public clsParent
{
public:
	clsChopper(void);
	~clsChopper(void);

	clsChopper* pNext;
	void CreateObject(LPDIRECT3DDEVICE9);
	void MoveU(int screenheight,int screenwidth);
	void MoveG(int screenheight,int screenwidth);
	void changelocation(float newxpos,float newypos);
	void Draw(LPD3DXSPRITE d3dspt);
	
	
	
protected:
	int animationPosition;
	int numberOfFrames;
	int frameRate;
};

