#include "StdAfx.h"
#include "clsChopper.h"


clsChopper::clsChopper(void)
{

	width = 64;
	height = 64;
	animationPosition=0;
	numberOfFrames=2;
	frameRate=0;
}


clsChopper::~clsChopper(void)
{
}

void clsChopper::CreateObject(LPDIRECT3DDEVICE9 d3ddev)
{
D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
                                "resource/Chopper.png",    // the file name
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



void clsChopper::changelocation(float newxpos,float newypos)
{
	xPos = newxpos;
	yPos = newypos;
	
}

void clsChopper::MoveU(int screenheight,int screenwidth)
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
void clsChopper::MoveG(int screenheight,int screenwidth)
{

	 if (yPos > screenheight - 64)
	 {
		yPos += 0;
	 }
	 else
	 {
		 yPos += 15;
	 }

}
void clsChopper::Draw(LPD3DXSPRITE d3dspt)
{
	// 2d coordinates of rotation point on sprite
	D3DXVECTOR2 spriteCentre=D3DXVECTOR2(32.0f,32.0f);
	// Screen coordinates of the sprite
	D3DXVECTOR2 position=D3DXVECTOR2(xPos,yPos);
	//scalling factor for sprite
	D3DXVECTOR2 scaling(1.0f,1.0f);
	//matrix object set in function underneath
	D3DXMATRIX mat;
	//create matrix for transforming/scaling/rotating sprite
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,0,&position);
	// apply the matrix to the sprite
	d3dspt->SetTransform(&mat);

	//create rectangle inside sprite at correct portion
    RECT frame;
    SetRect(&frame, animationPosition*64, 0, (animationPosition * 64)+64, 64);

	d3dspt->Draw(sprite, &frame, NULL, NULL, D3DCOLOR_ARGB(50, 255, 255, 255));


	//increment the animation timer
	frameRate++;
	if(frameRate>5)
	{
		frameRate=0;
		//move to the next frame
		animationPosition++;
		if (animationPosition == numberOfFrames)
		{
			//move back to the first frame
			animationPosition = 0;
		}
	}


}
