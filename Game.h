#pragma once
#include "DisplayWin32.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <vector>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class GameComponent;

class Game
{
	void CreateBackBuffer();
	bool isExitRequested;
public:
	
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView * rtv;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext * context;
	IDXGISwapChain * swapChain;
	DisplayWin32 * Display;
	ID3D11Texture2D* backTex;
	std::vector<GameComponent*> gameComponents;

	float totalTime;
	std::chrono::time_point<std::chrono::steady_clock> prevTime;
	float startTime;

	Game(LPCWSTR applicationName, long clientWidth, long clientHeight);

	void Initialize();
	void Run();
	void Draw();
	void MessageHandler();
	void DestroyResources();
	void Exit();
};

