#pragma once
#include <Windows.h>
#include <thread>
#include <d3d11.h>
#include <thread>
#include <regex>
#include <unordered_set>
#include <map>
#include <xcall_once.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <Detours/detours.h>
#pragma comment(lib, "detours.lib")

#include <XorString/XorString.h>

#include <ImRender/ImRender.h>

#include <NaResolver/NaResolver.h>

#include <SDK/SDK.h>

#include "Core/Utils/String.h"
#include "Core/Utils/Hook.h"

HMODULE localModule = NULL;
bool unload = false;
bool menu = true;
HWND window = NULL;

inline extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

#define IsBadPtr(ptr) ((ptr) == nullptr ? true : (\
	(IsBadReadPtr((void*)(ptr), sizeof((ptr)))) \
	|| ((uintptr_t)(ptr) < 0x10000) \
	|| ((uintptr_t)(ptr) > 0x00007FFFFFFEFFFF) \
	|| (((uintptr_t)(ptr) & 0x1) != 0)))

#include "Core/Feature/Main.h"

#include "Core/Menu/Menu.h"
#include "Core/Menu/dxgi.h"