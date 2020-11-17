#ifndef UNICODE
#define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <map>
#include <memory>

#include "ControllerKit.hxx"
#include "Utils.hxx"
#include "App.hxx"
#include "Interface.hxx"
#include "Discord.hxx"

using namespace BrokenBytes::DualSense4Windows;

namespace BrokenBytes::DualSense4Windows {
	std::shared_ptr<UI::App> APP;
	std::shared_ptr<Interface> INTERFACE;

	void Init() {
		InitInterface();
		InitUI();
	}

	void InitUI() {
		APP = UI::App::Create();
		APP->Run();
	}

	void InitInterface() {
		INTERFACE = Interface::Create();
		INTERFACE->Init();
	}
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

	auto wrapper = 
		BrokenBytes::DualSense4Windows::Discord::Wrapper::Create();
	auto mute = wrapper->Mute();
	Init();
	return 1;
	
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = WINDOW_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		WINDOW_NAME,                     // Window class
		WINDOW_NAME,    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);



		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
