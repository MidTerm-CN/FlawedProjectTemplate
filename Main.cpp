#include "Main.h"

#pragma comment(lib, "d3d11.lib")

bool ScanDXGIFunction(void**& vTable)
{
	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		DefWindowProc,
		0,
		0,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		XOR(L"DirectX 11"),
		NULL,
	};

	RegisterClassEx(&windowClass);

	HWND window = CreateWindow(windowClass.lpszClassName, XOR(L"DirectX 11 Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	DXGI_MODE_DESC bufferDesc =
	{
		100,
		100,
		{60, 1},
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		DXGI_MODE_SCALING_UNSPECIFIED,
	};

	DXGI_SWAP_CHAIN_DESC swapChainDesc =
	{
		bufferDesc,
		{1, 0},
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		1,
		window,
		1,
		DXGI_SWAP_EFFECT_DISCARD,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
	};

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context);

	if (FAILED(result))
	{
		DestroyWindow(window);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		return false;
	}

	vTable = *(void***)swapChain;

	if (swapChain)
	{
		swapChain->Release();
		swapChain = NULL;
	}

	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (context)
	{
		context->Release();
		context = NULL;
	}

	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	return true;
}

bool DllInitialize()
{
	void** dxgiFunctionVTable = nullptr;
	if (!ScanDXGIFunction(dxgiFunctionVTable))
	{
		return false;
	}

	if (!Utils::Hook(dxgi::Present, (void**)&dxgi::oPresent, (void*)dxgiFunctionVTable[8])
		|| !Utils::Hook(dxgi::ResizeBuffers, (void**)&dxgi::oResizeBuffers, (void*)dxgiFunctionVTable[13]))
	{
		return false;
	}

	return true;
}

void Main()
{
	unload = !DllInitialize();

	while (!unload)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	Utils::Unhook(dxgi::Present, (void**)&dxgi::oPresent);
	Utils::Unhook(dxgi::ResizeBuffers, (void**)&dxgi::oResizeBuffers);
	Utils::Unhook(dxgi::WndProc, (void**)&dxgi::oWndProc);
	FreeLibraryAndExitThread(localModule, 0);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(module);
		localModule = module;
		CreateThread(LPSECURITY_ATTRIBUTES{}, SIZE_T(NULL), (LPTHREAD_START_ROUTINE)Main, LPVOID(NULL), DWORD(NULL), LPDWORD(NULL));
		break;
	}
	return TRUE;
}