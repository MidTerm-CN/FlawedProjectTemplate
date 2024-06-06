#pragma once

namespace dxgi
{
	inline IDXGISwapChain* swapChain = NULL;
	inline ID3D11Device* device = NULL;
	inline ID3D11DeviceContext* context = NULL;
	inline ID3D11RenderTargetView* mainRenderTargetView = NULL;

	HOOK(LRESULT WINAPI, WndProc, const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (unload)
			return oWndProc(hWnd, uMsg, wParam, lParam);
		if (ImGui::GetCurrentContext() != nullptr)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

			if (menu)
			{
				ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
				LPTSTR win32_cursor = IDC_ARROW;
				switch (imgui_cursor)
				{
				case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
				case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
				case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
				case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
				case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
				case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
				case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
				case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
				case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
				}
				::SetCursor(::LoadCursor(NULL, win32_cursor));
				{
					if (uMsg == WM_LBUTTONDOWN
						|| uMsg == WM_LBUTTONUP
						|| uMsg == WM_RBUTTONDOWN
						|| uMsg == WM_RBUTTONUP
						|| uMsg == WM_MBUTTONDOWN
						|| uMsg == WM_MBUTTONUP
						|| uMsg == WM_XBUTTONDOWN
						|| uMsg == WM_XBUTTONUP
						|| uMsg == WM_MOUSEWHEEL
						|| uMsg == WM_MOUSEHWHEEL
						|| uMsg == WM_KEYDOWN
						|| uMsg == WM_KEYUP
						|| uMsg == WM_SYSKEYDOWN
						|| uMsg == WM_SYSKEYUP)
					{
						return DefWindowProc(hWnd, uMsg, wParam, lParam);
					}
				}
			}
		}
		return oWndProc(hWnd, uMsg, wParam, lParam);
	}

	void DeviceObjectsInitialization()
	{
		if (FAILED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device)))
		{
			MessageBoxA(NULL, XOR("Failed to get device"), XOR("Error"), MB_OK);
			return;
		}
		device->GetImmediateContext(&context);
		DXGI_SWAP_CHAIN_DESC sd;
		swapChain->GetDesc(&sd);
		window = sd.OutputWindow;
		ID3D11Texture2D* pBackBuffer = NULL;
		if (FAILED((swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))))
		{
			MessageBoxA(NULL, XOR("Failed to get backbuffer"), XOR("Error"), MB_OK);
			return;
		}
		if (FAILED(device->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView)))
		{
			MessageBoxA(NULL, XOR("Failed to create render target view"), XOR("Error"), MB_OK);
			return;
		}
		pBackBuffer->Release();
		pBackBuffer = NULL;
		if (!Utils::Hook(WndProc, (void**)&oWndProc, (void*)GetWindowLongPtr(sd.OutputWindow, GWLP_WNDPROC)))
		{
			MessageBoxA(NULL, XOR("Failed to hook WndProc"), XOR("Error"), MB_OK);
			return;
		}

		ImGui::CreateContext();
		ImGui_ImplDX11_Init(device, context);
		ImGui_ImplWin32_Init(sd.OutputWindow);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		ImGui::StyleColorsDark();
		ImGui::GetIO().LogFilename = nullptr;
		ImGui::GetStyle().FrameRounding = 3.0f;
		ImGui::GetStyle().WindowRounding = 2.0f;
		ImGui::GetStyle().GrabRounding = 2.0f;
		ImGui::GetStyle().WindowRounding = 3.0f;
		ImGui::GetStyle().WindowTitleAlign = { 0.5f ,0.5f };
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.28f, 0.28f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.17f, 0.17f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.22f, 0.22f, 0.22f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.46f, 0.46f, 0.46f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.32f, 0.32f, 0.32f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.71f, 0.71f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.43f, 0.43f, 0.43f, 0.95f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.40f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.36f, 0.36f, 0.36f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	}

	HOOK(HRESULT, Present, IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (unload)
			return oPresent(pSwapChain, SyncInterval, Flags);
		if (swapChain == NULL) swapChain = pSwapChain;
		static std::once_flag onceFlags = {};
		std::call_once(onceFlags, DeviceObjectsInitialization);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		Menu();
		ImGui::Render();
		context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	HOOK(HRESULT, ResizeBuffers, IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		if (unload)
			return oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
		if (mainRenderTargetView)
		{
			context->OMSetRenderTargets(0, 0, 0);
			mainRenderTargetView->Release();
			mainRenderTargetView = NULL;
		}
		HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
		ID3D11Texture2D* pBuffer = NULL;
		if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer)))
		{
			return hr;
		}
		device->CreateRenderTargetView(pBuffer, nullptr, &mainRenderTargetView);
		pBuffer->Release();
		context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)Width;
		vp.Height = (FLOAT)Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1, &vp);
		return hr;
	}
}