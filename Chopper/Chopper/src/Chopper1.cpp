// Chopper1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Chopper1.h"
#include "clsChopper.h"
#include "clsblock.h"
#include "clsRotateBlock.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include<XAudio2.h>
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "clsParent.h"

// include the DirectInput Library files
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")


// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


// function prototypes
void initDInput(HINSTANCE hInstance, HWND hWnd);    // sets up and initializes DirectInput
void detect_keys(HWND);    // gets the current keys being pressed
void detect_mousepos(void);    // gets the mouse movement and updates the static variables
void cleanDInput(void);    // closes DirectInput and releases memory
void initD3D(HWND hWnd); // sets up and initializes Direct3D
void render_frame(); // renders a single frame
void cleanD3D(void); // closes Direct3D and releases memory
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);  // the WindowProc function prototype

// Global Variables:
LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8 dinkeyboard;    // the pointer to the keyboard device
LPDIRECTINPUTDEVICE8 dinmouse;    // the pointer to the mouse device
LPD3DXFONT dxfont;    // the pointer to the font object
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt; // the pointer to our Direct3D Sprite interface
LPD3DXFONT bigFont;
void playsound(LPCSTR);
clsParent* Objects[6];
int RandX[6] = { 1400.0f, 1800.0f, 2100.0f , 2500.0f,2800.0f,3100.0f };
int RandY[6] = { 300.0f, 500.0f, 130.0f , 525.0f, 80.0f, 350.0f };
bool lost = false;
bool won = false;
bool paused = true;
int score = 0;
int flightspeed = 20;
void addObjectToList(float, float, clsParent*, clsParent**);
void addChopper(float, float, clsParent**);
void addBlock(float, float, clsParent**);
void addBlock1(float, float, clsParent**);
void addBlock2(float, float, clsParent**);
void addBlock3(float, float, clsParent**);
void addBlock4(float, float, clsParent**);
void addBlock5(float, float, clsParent**);
void moveAllObjectsU(clsParent**);
void moveAllObjectsG(clsParent**);
void moveAllBlock(clsParent**);
void removeObjectFromList(clsParent**);
void createNewRotate(float, float, clsParent**);
clsParent* pHead;


HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}


	return S_OK;
}
HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = __T("WindowClass");
	RegisterClassEx(&wc);

	// Create the window
	hWnd = CreateWindowEx(NULL,
		"WindowClass",
		"Chopper",
		WS_OVERLAPPEDWINDOW,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	// initialise tranceperancies
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// set up and initialize DirectInput
	initDInput(hInstance, hWnd);

	MSG msg;


	// init

	addChopper(64, 64, &pHead);
	addBlock(700, 500, &pHead);
	addBlock1(800, 75, &pHead);
	addBlock2(-200, 450, &pHead);
	addBlock3(-750, 100, &pHead);
	addBlock4(1100, 375, &pHead);



	
	
	
	

	// main loop:
	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!paused)
			moveAllBlock(&pHead);
		
		render_frame();

		
		detect_keys(hWnd);
		detect_mousepos();

		while ((GetTickCount() - starting_point) < 25);
	}

	// Clean up DirectInput
	cleanDInput();
	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}

// this is the function that detects keystrokes and displays them in a message box
void detect_keys(HWND hWnd)
{
	static BYTE keystate[256];    // create a static storage for the key-states
	dinkeyboard->Acquire();    // get access if we don't have it already
	dinkeyboard->GetDeviceState(256, (LPVOID)keystate);    // fill keystate with values

	if (keystate[DIK_P] & 0x80)    // if the 'P' key was pressed...
	{
		if (paused) {
			paused = false;
		}
	}
	if (keystate[DIK_G] & 0x80)    // if the 'Q' key was pressed...
	{
		int i = 0;
		for (i; i < 6; i++)
		{
			addBlock5(RandX[i], RandY[i], &pHead);
		}
	}
	if (keystate[DIK_A] & 0x80)    // if the 'A' key was pressed...
	{
		removeObjectFromList(&pHead);
	}
	if (keystate[DIK_UP] & 0x80)    // if the 'up arrow' key was pressed...
	{
		if (!lost && !won)
		{
			moveAllObjectsU(&pHead);
		}
	}
	if (keystate[DIK_DOWN] & 0x80)    // if the 'down arrow' key was pressed...
	{
		if (!lost && !won)
		{
			moveAllObjectsG(&pHead);
		}
	}
	if (keystate[DIK_ESCAPE] & 0x80)   // if the 'escape' key was pressed...
		PostMessage(hWnd, WM_DESTROY, 0, 0);   //delete the main window

	return;
}

// this is the function that detects mouse movements and mouse buttons
void detect_mousepos(void)
{
	static DIMOUSESTATE mousestate;    // create a static storage for the mouse states

	dinmouse->Acquire();    // get access if we don't have it already

	// fill the mouse state with values
	dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);

	//get absolute mouse cursor position
	POINT cursorPos;
	float mouseX;
	float mouseY;
	GetCursorPos(&cursorPos);
	mouseX = cursorPos.x;
	mouseY = cursorPos.y;

	if (mousestate.rgbButtons[0] & 0x80)    // if the left mouse button was clicked...
	{
	}

	if (mousestate.rgbButtons[1] & 0x80)    // if the right mouse button was clicked...
	{

	}
	if (mousestate.rgbButtons[2] & 0x80)    // if the wheel mouse button was clicked...
	{
	}
	if (mousestate.lZ < 0)    // if the wheel moved down
	{
	}
	if (mousestate.lZ > 0)    // if the wheel moved up
	{
	}

	return;
}

// this is the function used to render a single frame
void render_frame()
{
	// clear the window
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();    // begins the 3D scene
	d3dspt->Begin(NULL);    // begin sprite drawing

	

	if (!lost && !won && !paused)
	{
		static int time = 0;
		time++;

		// Start/Loop hack on chopper sound effect
		if (time % 100 == 0) {
			// sound
			// TODO: Fix the sloppiness - it will keep loading the file into the buffer every nth time
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			IXAudio2* pXAudio2 = NULL;
			XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
			IXAudio2MasteringVoice* pMasterVoice = NULL;
			pXAudio2->CreateMasteringVoice(&pMasterVoice, XAUDIO2_DEFAULT_CHANNELS,
				XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
			WAVEFORMATEXTENSIBLE wfx = { 0 };
			XAUDIO2_BUFFER buffer = { 0 };

			// Load chopper sound effect file
			HANDLE hFile = CreateFile("resource/inflight.wav", GENERIC_READ,
				FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			DWORD dwChunkSize;
			DWORD dwChunkPosition;
			//check the file type, should be fourccWAVE or 'XWMA'
			FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
			DWORD filetype;
			ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
			FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
			ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
			//fill out the audio data buffer with the contents of the fourccDATA chunk
			FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
			BYTE * pDataBuffer = new BYTE[dwChunkSize];
			ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
			buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
			buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
			buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer	
			IXAudio2SourceVoice* pSourceVoice;
			pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx,
				0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
			pSourceVoice->SubmitSourceBuffer(&buffer);
			pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
		}
		

		//place pTemp at the start of the list
		clsParent* pTemp;
		pTemp = *&pHead;
		//loop while not at the end of the list (NULL)
		while (pTemp != NULL)
		{

			pTemp->Draw(d3dspt);

			//move to the next object in list
			pTemp = pTemp->pNext;
		}


		wchar_t buffer2[20];	
		swprintf(buffer2, 20, L"Score: %i", time);
		static RECT textbox2; SetRect(&textbox2, 1100, -690, SCREEN_WIDTH, SCREEN_HEIGHT);
		dxfont->DrawTextW(NULL, buffer2, -1, &textbox2, DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 0, 255, 0));
		score = time;


		if (time >= 500)
		{
			flightspeed = +25;
		}
		if (time >= 1000)
		{
			flightspeed = +30;
		}
		if (time >= 2000)
		{
			flightspeed = +35;
		}
		if (time >= 2500)
		{
			flightspeed = +40;
		}
		if (time >= 2800)
		{
			won = true;
		}
	}


	if (won && !paused)
	{
		// playsound("resource/win.wav"); // encoding or speed issue, uncomment with caution, VERY LOUD!
		static RECT textbox3; SetRect(&textbox3, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		dxfont->DrawTextW(NULL, L"YOU MADE IT!", -1, &textbox3, DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 0, 255, 0));
	}


	if (lost && !paused)
	{


		static RECT textbox3; SetRect(&textbox3, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		dxfont->DrawTextW(NULL, L"YOU CRASHED!", -1, &textbox3, DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 255, 0, 0));

		wchar_t buffer2[20];

		swprintf(buffer2, 20, L"Score: %i", score);
		static RECT textbox2; SetRect(&textbox2, 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT);
		dxfont->DrawTextW(NULL, buffer2, -1, &textbox2, DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 0, 255, 0));

	}

	if (paused)
	{
		static RECT textbox3; SetRect(&textbox3, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		dxfont->DrawTextW(NULL, L"Press 'P' to play", -1, &textbox3, DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 0, 255, 0));
	}

	d3dspt->End();    // end sprite drawing
	d3ddev->EndScene();    // ends the 3D scene
	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	d3ddev->Release();
	d3d->Release();
	return;
}

// this is the function that initializes DirectInput
void initDInput(HINSTANCE hInstance, HWND hWnd)
{
	// create the DirectInput interface
	DirectInput8Create(hInstance,    // the handle to the application
		DIRECTINPUT_VERSION,    // the compatible version
		IID_IDirectInput8,    // the DirectInput interface version
		(void**)&din,    // the pointer to the interface
		NULL);    // COM stuff, so we'll set it to NULL

// create the keyboard device
	din->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
		&dinkeyboard,    // the pointer to the device interface
		NULL);    // COM stuff, so we'll set it to NULL

// create the mouse device
	din->CreateDevice(GUID_SysMouse,    // the default mouse ID being used
		&dinmouse,    // the pointer to the device interface
		NULL);    // COM stuff, so we'll set it to NULL

	dinkeyboard->SetDataFormat(&c_dfDIKeyboard); // set the data format to keyboard format
	dinmouse->SetDataFormat(&c_dfDIMouse);    // set the data format to mouse format

   // set the control you will have over the keyboard
	dinkeyboard->SetCooperativeLevel(hWnd,
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	// set the control you will have over the mouse
	dinmouse->SetCooperativeLevel(hWnd,
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	dinmouse->Acquire();    // Acquire the mouse only once

	return;    // return to WinMain()
}



// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite drawing object

	D3DXCreateFont(d3ddev,    // the D3D Device
		30,    // font height of 30
		0,    // default font width
		FW_NORMAL,    // font weight
		1,    // not using MipLevels
		true,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		DEFAULT_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		"Arial",    // use Facename Arial
		&dxfont);    // the font object
	D3DXCreateFont(d3ddev,    // the D3D Device
		60,    // font height of 30
		0,    // default font width
		FW_NORMAL,    // font weight
		1,    // not using MipLevels
		true,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		DEFAULT_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		"Arial",    // use Facename Arial
		&bigFont);    // the font object

	return;
}

// this is the function that closes DirectInput
void cleanDInput(void)
{
	dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
	dinmouse->Unacquire();    // make sure the mouse is unacquired
	din->Release();    // close DirectInput before exiting

	return;
}
void playsound(LPCSTR FileName)
{
	//sound
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	IXAudio2* pXAudio2 = NULL;
	XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	IXAudio2MasteringVoice* pMasterVoice = NULL;
	pXAudio2->CreateMasteringVoice(&pMasterVoice, XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	// Load the file
	HANDLE hFile = CreateFile(FileName, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
	buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
	buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	IXAudio2SourceVoice* pSourceVoice;
	pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx,
		0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
	pSourceVoice->SubmitSourceBuffer(&buffer);
	pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
	//sound
}
void addBlock(float x, float y, clsParent** ppList)
{
	clsblock* pTemp;
	pTemp = new clsblock;
	addObjectToList(x, y, pTemp, ppList);
}
void addBlock1(float x, float y, clsParent** ppList)
{
	clsblock* pTemp;
	pTemp = new clsblock;
	addObjectToList(x, y, pTemp, ppList);
}
void addBlock2(float x, float y, clsParent** ppList)
{
	clsblock* pTemp;
	pTemp = new clsblock;
	addObjectToList(x, y, pTemp, ppList);
}
void addBlock3(float x, float y, clsParent** ppList)
{
	clsblock* pTemp;
	pTemp = new clsblock;
	addObjectToList(x, y, pTemp, ppList);
}
void addBlock4(float x, float y, clsParent** ppList)
{
	clsblock* pTemp;
	pTemp = new clsblock;
	addObjectToList(x, y, pTemp, ppList);
}
void addBlock5(float x, float y, clsParent** ppList)
{
	clsblock* pTemp;
	pTemp = new clsblock;
	addObjectToList(x, y, pTemp, ppList);
}
void addChopper(float x, float y, clsParent** ppList)
{
	clsChopper* pTemp;
	pTemp = new clsChopper;
	addObjectToList(x, y, pTemp, ppList);
}
void addObjectToList(float x, float y, clsParent* pTemp, clsParent** ppList)
{
	pTemp->CreateObject(d3ddev);
	pTemp->changelocation(x, y);
	//add new object to start of linked list
	pTemp->pNext = *ppList;
	*ppList = pTemp;
}
void moveAllObjectsU(clsParent** ppList)
{
	//if there is atleast one object in list
	if (*ppList != NULL)
	{
		clsParent* pCurrent;
		//point to first object in list
		pCurrent = *ppList;
		//while not at the end of the list
		while (pCurrent != NULL)
		{
			clsChopper* tempchopper = NULL;

			tempchopper = dynamic_cast<clsChopper*>(pCurrent);


			if (tempchopper != NULL)
			{
				pCurrent->MoveU(SCREEN_HEIGHT, SCREEN_WIDTH);
			}
			//move to next object in list
			pCurrent = pCurrent->pNext;
		}
	}
}
void moveAllObjectsG(clsParent** ppList)
{
	//if there is atleast one object in list
	if (*ppList != NULL)
	{
		clsParent* pCurrent;
		//point to first object in list
		pCurrent = *ppList;
		//while not at the end of the list
		while (pCurrent != NULL)
		{

			clsChopper* tempchopper = NULL;

			tempchopper = dynamic_cast<clsChopper*>(pCurrent);


			if (tempchopper != NULL)
			{
				tempchopper->MoveG(SCREEN_HEIGHT, SCREEN_WIDTH);
			}
			//move to next object in list
			pCurrent = pCurrent->pNext;

		}
	}
}
void moveAllBlock(clsParent** ppList)
{
	//if there is atleast one object in list
	if (*ppList != NULL)
	{
		clsParent* pCurrent;
		//point to first object in list
		pCurrent = *ppList;
		//while not at the end of the list
		while (pCurrent != NULL)
		{
			clsblock* tempBlock = NULL;

			tempBlock = dynamic_cast<clsblock*>(pCurrent);


			if (tempBlock != NULL)
			{
				tempBlock->MoveBlock(SCREEN_HEIGHT, SCREEN_WIDTH, flightspeed);



				clsParent* pnextCurrent;
				//point to first object in list
				pnextCurrent = *ppList;
				//while not at the end of the list
				while (pnextCurrent != NULL)
				{

					clsChopper* tempchopper = NULL;

					tempchopper = dynamic_cast<clsChopper*>(pnextCurrent);


					if (tempchopper != NULL)
					{
						if (tempBlock->hasCollided(tempchopper))
						{
							lost = true;
						}

					}
					//move to next object in list
					pnextCurrent = pnextCurrent->pNext;
				}
			}

			//move to next object in list
			pCurrent = pCurrent->pNext;

		}
	}
}
void removeObjectFromList(clsParent** ppList)
{
	//if there is atleast one item in the list
	if (*ppList != NULL)
	{
		clsParent* pCurrent;
		clsParent* pPrevious;
		//point to first object in list
		pPrevious = *ppList;
		//point to second object in list
		pCurrent = pPrevious->pNext;

		clsblock* tempBlock = NULL;


		tempBlock = dynamic_cast<clsblock*>(pCurrent);


		if (tempBlock != NULL)
		{
			//remove this object from list and RAM
			*ppList = pCurrent;
			delete pPrevious;
			return;
		}
		//while not at the end of the list
		while (pCurrent != NULL)
		{

			if (tempBlock != NULL)
			{
				//remove this object from list and RAM
				pPrevious->pNext = pCurrent->pNext;
				delete pCurrent;
				return;
			}
			//point to next objects in list
			pPrevious = pCurrent;
			pCurrent = pCurrent->pNext;
		}
	}
}
