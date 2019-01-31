#include "StdAfx.h"
#include "clsParent.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


clsParent::clsParent(void)
{
	moveright = true;
	movedown = false;	
	width = 100;
	height = 100;
	rotation=3.14;
	animationPosition=0;
	numberOfFrames=4;
	frameRate=0;
}


clsParent::~clsParent(void)
{
}

void clsParent::CreateObject(LPDIRECT3DDEVICE9 d3ddev)
{
D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
                                "resource/Block1.png",    // the file name
                                D3DX_DEFAULT,    // default width
                                D3DX_DEFAULT,    // default height
                                D3DX_DEFAULT,    // no mip mapping
                                NULL,    // regular usage
                                D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
                                D3DPOOL_MANAGED,    // typical memory handling
                                D3DX_DEFAULT,    // no filtering
                                D3DX_DEFAULT,    // no mip filtering
                                D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
                                NULL,    // no image info struct
                                NULL,    // not using 256 colors
                                &sprite);    // load to sprite
		     
}



void clsParent::changelocation(float newxpos,float newypos)
{
	xPos = newxpos;
	yPos = newypos;
	
}

void clsParent::MoveBlock(int screenheight,int screenwidth, int speed)
{
	if(moveright)
	{
		xPos -= speed;
	}


	 if(xPos < -200)
	 {
		 xPos = 2000;
	 }
	
	 

}
void clsParent::Draw(LPD3DXSPRITE d3dspt)
{ 
		// 2d coordinates of rotation point on sprite
	D3DXVECTOR2 spriteCentre=D3DXVECTOR2(32.0f,32.0f);

	// Screen coordinates of the sprite
	D3DXVECTOR2 position=D3DXVECTOR2(xPos,yPos);

	//scalling factor for sprite
	D3DXVECTOR2 scaling(1.0f,1.0f);

	//matrix object set in function underneath
	D3DXMATRIX mat;

	rotation+=0.1;

	//create matrix for transforming/scaling/rotating sprite
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&position);

	// apply the matrix to the sprite
	d3dspt->SetTransform(&mat);

	d3dspt->Draw(sprite, NULL, NULL, NULL, D3DCOLOR_ARGB(50, 255, 255, 255));
	
}
bool clsParent::hasCollided(clsParent* pOtherObject)
{
	int w, h;
	float x ,y;
	w = pOtherObject->getWidth();
	h = pOtherObject->getHeight();
	x = pOtherObject->getX();
	y = pOtherObject->getY();

	if(((xPos <= x+w) && (xPos >= x)) || ((xPos + width <= x+w) && (xPos + width >= x))) //left hand side inside other shape
	{
		if(((yPos <= y+h) && (yPos >= y)) || ((yPos + height <= y+h) && (yPos + height >= y))) //top is inside
		{
			return true;
		}
	}
	return false;
}

int clsParent::getHeight()
{
	return height;
}
int clsParent::getWidth()
{
	return width;
}
float clsParent::getX()
{
	return xPos;
}
float clsParent::getY()
{
	return yPos;
}
void clsParent::MoveU(int screenheight,int screenwidth)
{


	 if(yPos < 0)
	 {
		 yPos -= 0;
	 }
	 else
	 {
	yPos -= 15;
	 }
	
}
void clsParent::MoveG(int screenheight,int screenwidth)
{

	 if (yPos > screenheight - 120)
	 {
		yPos += 0;
	 }
	 else
	 {
		 yPos += 15;
	 }
}