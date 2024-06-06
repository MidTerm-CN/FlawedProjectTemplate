#pragma once

#define RAW_HOOK(_returnType, _funcName, ...) \
	_returnType _funcName(__VA_ARGS__);             \
	using _funcName##Fn = decltype(&_funcName);     \
	inline _funcName##Fn o##_funcName = nullptr;    \

#define HOOK(_returnType, _funcName, ...) \
		RAW_HOOK(_returnType, _funcName, __VA_ARGS__) \
		inline _returnType _funcName(__VA_ARGS__)

namespace Utils
{
	bool Hook(void* detour, void** original, void* target)
	{
		*original = target;
		if (DetourTransactionBegin() != NO_ERROR)
		{
			return false;
		}
		if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
		{
			return false;
		}
		if (DetourAttach(original, detour) != NO_ERROR)
		{
			return false;
		}
		if (DetourTransactionCommit() != NO_ERROR)
		{
			return false;
		}
		return true;
	}

	bool Unhook(void* detour, void** original)
	{
		if (DetourTransactionBegin() != NO_ERROR)
		{
			return false;
		}
		if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
		{
			return false;
		}
		if (DetourDetach(original, detour) != NO_ERROR)
		{
			return false;
		}
		if (DetourTransactionCommit() != NO_ERROR)
		{
			return false;
		}
		return true;
	}
}