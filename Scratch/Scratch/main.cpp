#include "FrameWork.h"
//#include "D3DClass.h"

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevhInstance,LPSTR lpCmdline,int nShowcmd)
{
	FrameWork* System;
	D3DClass* d3d;
	System = new FrameWork;
	d3d = new D3DClass;
	if (!System->InitializeWindow(hInstance, nShowcmd, width, height, true))
	{
		MessageBox(0,L"Window Initialization - Failed", L"Error", MB_OK);
		return 0;
	}

	if (!d3d->InitializeD3DApp(hInstance,System->GetHWND()))
	{
		MessageBox(0, L"Direct3d Initialization Failed", L"Error", MB_OK);
		return 0;
	}

	if (!d3d->InitScene())
	{
		MessageBox(0, L"Scene Initialization Failed", L"Error", MB_OK);
		return 0;
	}

	System->MessageLoop(d3d);

	d3d->ReleaseObjects();

	delete d3d;
	d3d = 0;
	delete System;
	System = 0;

	return 0;
}