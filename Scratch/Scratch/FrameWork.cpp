#include "FrameWork.h"


FrameWork::FrameWork()
{
}


FrameWork::~FrameWork()
{
}

bool FrameWork::InitializeWindow(HINSTANCE hInstance, int nShowWnd, int width, int height, bool windowed)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"FirstWindow";
	wc.hIconSm = wc.hIcon;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, L"Error Registering Class", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"FirstWindow",
		L"Window",
		WS_OVERLAPPED,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hwnd)
	{
		MessageBox(0, L"Error Creating Window", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hwnd, nShowWnd);
	UpdateWindow(hwnd);
	SetFocus(hwnd);

	return true;
}


int FrameWork::MessageLoop(D3DClass* d3d)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//run game
			d3d->UpdateScene();
			d3d->DrawScene();
		}
	}

	return msg.wParam;
	
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_KEYDOWN:
		{
			if (wparam == VK_ESCAPE)
			{
				if (MessageBox(0, L"Are you sure you want to quit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					DestroyWindow(hwnd);
					hwnd = NULL;
				}
			}
			return 0;
		}
		
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

HWND FrameWork::GetHWND()
{
	return hwnd;	
}