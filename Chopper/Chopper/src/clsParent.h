#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
class clsParent
{
public:
	clsParent(void);
	~clsParent(void);

	clsParent* pNext;
	virtual void CreateObject(LPDIRECT3DDEVICE9);
	virtual void MoveBlock(int screenheight,int screenwidth,int speed);
	virtual void changelocation(float newxpos,float newypo);
	virtual void Draw(LPD3DXSPRITE d3dspt);
	virtual bool hasCollided(clsParent* pOtherObject);
	virtual void MoveU(int screenheight,int screenwidth);
	virtual void MoveG(int screenheight,int screenwidth);
	virtual int getHeight();
	virtual int getWidth();
	virtual float getX();
	virtual float getY();
	

	protected:
	LPDIRECT3DTEXTURE9 sprite;
	float yPos,xPos;
	int ScreenHeight,ScreenWidth;
	bool moveright, movedown;
	int width, height;
	float getrandomY;
	float rotation;
	int animationPosition;
	int numberOfFrames;
	int frameRate;
};

