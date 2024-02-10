﻿#pragma once

#include "module.hpp"
#undef MessageBox

namespace win_api
{
	static int AllocConsole(lua_State* L) {
		::AllocConsole();
		return 0;
	}

	static int FreeConsole(lua_State* L) {
		::FreeConsole();
		return 0;
	}

	static int GetAsyncKeyState(lua_State* L) {
		ARG(1, integer, int32_t, vKey);
		RET(integer, ::GetAsyncKeyState(vKey));
	}

	static int GetKeyState(lua_State* L) {
		ARG(1, integer, int32_t, nVirtKey);
		RET(integer, ::GetKeyState(nVirtKey));
	}

	static int MessageBox(lua_State* L) {
		ARG(1, integer, HWND, hWnd);
		ARG(2, string, LPCSTR, lpText);
		ARG(3, string, LPCSTR, lpCaption);
		ARG(4, integer, UINT, uType);

		size_t len_t = utils::U8ToU16(lpText);
		vector<wchar_t> u16_t(len_t);
		utils::U8ToU16(lpText, u16_t.data(), len_t);

		size_t len_c = utils::U8ToU16(lpCaption);
		vector<wchar_t> u16_c(len_c);
		utils::U8ToU16(lpCaption, u16_c.data(), len_c);

		RET(integer, MessageBoxW(hWnd, u16_t.data(), u16_c.data(), uType));
	}

	static void Init() {
		DEFMOD(WinAPI);
		DEF(AllocConsole);
		DEF(FreeConsole);
		DEF(GetAsyncKeyState);
		DEF(GetKeyState);
		DEF(MessageBox);
		ENDMOD();
	}
}