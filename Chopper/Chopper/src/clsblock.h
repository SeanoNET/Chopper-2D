#pragma once
#include "clsParent.h"

class clsblock :
	public clsParent
{
public:
	clsblock(void);
	~clsblock(void);
	clsblock* pNext;
	 void CreateObject(LPDIRECT3DDEVICE9);
	 void MoveBlock(int screenheight,int screenwidth,int speed);
	 void changelocation(float newxpos,float newypo);
	 void Draw(LPD3DXSPRITE d3dspt);
	 bool hasCollided(clsParent* pOtherObject);
	 int getHeight();
	 int getWidth();
	 float getX();
	 float getY();
	

	protected:
	LPDIRECT3DTEXTURE9 sprite;
	float yPos,xPos;
	int ScreenHeight,ScreenWidth;
	bool moveright, movedown;
	int width, height;
	float getrandomY;
	float rotation;
	
};

