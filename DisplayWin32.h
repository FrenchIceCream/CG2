#pragma once
#include <windows.h>
#include <WinUser.h>
#include <iostream>

class DisplayWin32
{
public:
	long clientHeight = 800;
	long clientWidth = 800;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;

	DisplayWin32(long clientWidth, long clientHeight, HINSTANCE hInstance, LPCWSTR appName);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};