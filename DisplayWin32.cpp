#include "pch.h"
#include "DisplayWin32.h"
#include "InputDevice.h"


LRESULT CALLBACK DisplayWin32::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		// If a key is pressed, send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

		unsigned int keyCode = static_cast<unsigned int>(wparam);

		switch (keyCode)
		{
		case 27:
			PostQuitMessage(0);
			return 0;
			break;
		case 87: //W
			PostMessage(hwnd, 87, wparam, lparam);
			break;
		case 83: //S
			PostMessage(hwnd, 88, wparam, lparam);
			break;
		case 38: //вверх
			PostMessage(hwnd, VK_UP, wparam, lparam);
			break;
		case 40: //вниз
			PostMessage(hwnd, VK_DOWN, wparam, lparam);
			break;
		default:
			break;
		}
	}
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		
		if (lpb == nullptr)
			return 0;

		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.Reserved,
				raw->data.keyboard.ExtraInformation,
				raw->data.keyboard.Message,
				raw->data.keyboard.VKey);

			InputDevice::inputInstance->OnKeyDown({
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.VKey,
				raw->data.keyboard.Message
				});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			InputDevice::inputInstance->OnMouseMove({
				raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}

		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

DisplayWin32::DisplayWin32(long clientWidth, long clientHeight, HINSTANCE hInstance, LPCWSTR appName)
{
	WNDCLASSEX wc;

	this->clientHeight = clientHeight;
	this->clientWidth = clientWidth;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);


	RECT windowRect = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - clientWidth) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - clientHeight) / 2;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
}
