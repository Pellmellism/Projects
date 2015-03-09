#pragma once

#include <windows.h>
#include"D3DClass.h"

const int width = 800;
const int height = 600;

class FrameWork
{
public:
	FrameWork();
	~FrameWork();
	bool InitializeWindow(HINSTANCE , int , int, int, bool);
	int MessageLoop(D3DClass*);
	HWND GetHWND();

private:
	HWND hwnd;

	
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



