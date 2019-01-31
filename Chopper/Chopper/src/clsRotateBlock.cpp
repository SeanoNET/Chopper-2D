#include "StdAfx.h"
#include "clsRotateBlock.h"
#include "StdAfx.h"
#include "clsblock.h"
#include "StdAfx.h"
#include "clsParent.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

clsRotateBlock::clsRotateBlock(void)
{
	moveright = true;
	movedown = false;	
	width = 100;
	height = 100;
	rotation=3.14;
}


clsRotateBlock::~clsRotateBlock(void)
{
}

void clsRotateBlock::CreateObject(LPDIRECT3DDEVICE9 d3ddev)
{
D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
                                "Block1.png",    // the file name
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



void clsRotateBlock::changelocation(float newxpos,float newypos)
{
	xPos = newxpos;
	yPos = newypos;
	
}

void clsRotateBlock::MoveBlock(int screenheight,int screenwidth, int speed)
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
void clsRotateBlock::DrawBlock(LPD3DXSPRITE d3dspt)
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
	
	//create matrix for transforming/scaling/rotating sprite
	D3DXMatrixTransformation2D(&mat,NULL,0.0,NULL,NULL,NULL,NULL);

	// apply the matrix to the sprite
	d3dspt->SetTransform(&mat);

}
void clsRotateBlock::DrawChopper(LPD3DXSPRITE d3dspt)
{ 
	// draw the sprite
    D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
    D3DXVECTOR3 position( xPos, yPos, 0.0f);    // position at 50, 50 with no depth
    d3dspt->Draw(sprite, NULL, &center, &position, D3DCOLOR_ARGB(127, 255, 255, 255));
}
bool clsRotateBlock::hasCollided(clsParent* pOtherObject)
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

int clsRotateBlock::getHeight()
{
	return height;
}
int clsRotateBlock::getWidth()
{
	return width;
}
float clsRotateBlock::getX()
{
	return xPos;
}
float clsRotateBlock::getY()
{
	return yPos;
}
void clsRotateBlock::MoveU(int screenheight,int screenwidth)
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
void clsRotateBlock::MoveG(int screenheight,int screenwidth)
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
int clsRotateBlock::type()
{
	return 0;
}