#pragma once
#include "clsParent.h"
class clsRotateBlock :
	public clsParent
{
public:
	clsRotateBlock(void);
	~clsRotateBlock(void);
	clsRotateBlock* pNext;
	 void CreateObject(LPDIRECT3DDEVICE9);
	 void MoveBlock(int screenheight,int screenwidth,int speed);
	 void changelocation(float newxpos,float newypo);
	 void DrawBlock(LPD3DXSPRITE d3dspt);
	 void DrawChopper(LPD3DXSPRITE d3dspt);
	 bool hasCollided(clsParent* pOtherObject);
	 void MoveU(int screenheight,int screenwidth);
	 void MoveG(int screenheight,int screenwidth);
	 int getHeight();
	 int getWidth();
	 float getX();
	 float getY();
	 int type();

	 
	protected:
	LPDIRECT3DTEXTURE9 sprite;
	float yPos,xPos;
	int ScreenHeight,ScreenWidth;
	bool moveright, movedown;
	int width, height;
	float getrandomY;
	float rotation;

};

