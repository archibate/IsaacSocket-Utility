﻿#pragma once
#include "pch.h"
#include "isaac.hpp"

using isaac::lua_State;

//lua类型枚举
#define LUA_TNONE			(-1)
#define LUA_TNIL			0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER			3
#define LUA_TSTRING			4
#define LUA_TTABLE			5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD			8
#define LUA_NUMTAGS			9

//lua类型
#define LUA_INTEGER		long long
#define LUA_NUMBER		double
#define LUA_KCONTEXT	ptrdiff_t

//lua状态码
#define LUA_OK			0
#define LUA_YIELD		1
#define LUA_ERRRUN		2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM		4
#define LUA_ERRGCMM		5
#define LUA_ERRERR		6

//lua类型
typedef LUA_INTEGER lua_Integer;
typedef LUA_NUMBER lua_Number;
typedef int (*lua_CFunction) (lua_State* L);
typedef int ptrdiff_t;
typedef int intptr_t;
typedef LUA_KCONTEXT lua_KContext;
typedef int (*lua_KFunction) (lua_State* L, int status, lua_KContext ctx);
typedef void* (*lua_Alloc) (void* ud, void* ptr, size_t osize, size_t nsize);

namespace lua {

#define _(ret,name,...) ret(*name)(__VA_ARGS__)=(decltype(name))GetProcAddress(hLuaModule, #name)

	struct Lua
	{
		HMODULE hLuaModule;

		_(const char*, lua_pushstring, lua_State* L, const char* s);

		_(const char*, lua_pushlstring, lua_State* L, const char* s, size_t len);

		_(void, lua_pushcclosure, lua_State* L, lua_CFunction fn, int n);

		_(void, lua_pushnumber, lua_State* L, lua_Number n);

		_(void, lua_pushinteger, lua_State* L, lua_Integer n);

		_(void, lua_settable, lua_State* L, int idx);

		_(int, lua_gettable, lua_State* L, int idx);

		_(lua_Number, lua_tonumberx, lua_State* L, int idx, int* pisnum);

		_(void, lua_setglobal, lua_State*, const char* name);

		_(void, lua_createtable, lua_State* L, int narray, int nrec);

		_(int, lua_getglobal, lua_State* L, const char* name);

		_(void, lua_settop, lua_State* L, int idx);

		_(int, lua_gettop, lua_State* L);

		_(int, lua_toboolean, lua_State* L, int idx);

		_(void, lua_pushboolean, lua_State* L, int b);

		_(lua_Integer, lua_tointegerx, lua_State* L, int idx, int* pisnum);

		_(int, lua_type, lua_State* L, int idx);

		_(int, lua_isnumber, lua_State* L, int idx);

		_(int, lua_isinteger, lua_State* L, int idx);

		_(int, lua_isstring, lua_State* L, int idx);

		_(const char*, lua_tolstring, lua_State* L, int idx, size_t* len);

		_(int, luaL_error, lua_State* L, const char* fmt, ...);

		_(void, lua_pushnil, lua_State* L);

		_(int, lua_pcallk, lua_State* L, int nargs, int nresults, int errfunc, lua_KContext ctx, lua_KFunction k);

		_(int, luaL_getsubtable, lua_State* L, int idx, const char* fname);

		_(lua_Integer, luaL_len, lua_State* L, int idx);

		_(int, lua_next, lua_State* L, int idx);

		const char* lua_tostring(lua_State* L, int i) const
		{
			return lua_tolstring(L, i, NULL);
		}

		int lua_istable(lua_State* L, int n) const
		{
			return lua_type(L, n) == LUA_TTABLE;
		}

		int lua_isboolean(lua_State* L, int n) const
		{
			return lua_type(L, n) == LUA_TBOOLEAN;
		}

		void lua_newtable(lua_State* L) const
		{
			lua_createtable(L, 0, 0);
		}

		void lua_pushcfunction(lua_State* L, lua_CFunction f) const
		{
			lua_pushcclosure(L, f, 0);
		}

		lua_Number lua_tonumber(lua_State* L, int i) const
		{
			return	lua_tonumberx(L, i, NULL);
		}

		void lua_pop(lua_State* L, int n) const
		{
			lua_settop(L, -n - 1);
		}

		lua_Integer lua_tointeger(lua_State* L, int i) const
		{
			return lua_tointegerx(L, i, NULL);
		}

		int lua_pcall(lua_State* L, int n, int r, int f) const
		{
			return lua_pcallk(L, n, r, f, 0, NULL);
		}

		int lua_isnone(lua_State* L, int n) const
		{
			return lua_type(L, (n)) == LUA_TNONE;
		}

		int lua_isnoneornil(lua_State* L, int n) const
		{
			return lua_type(L, (n)) <= 0;
		}

		int lua_isnil(lua_State* L, int idx) const {
			return lua_type(L, idx) == LUA_TNIL;
		}
	};
#undef _
}

#define _CHECK_ARG(index,luaType,type,name) if(local.lua->lua_is##luaType(L,index)){name = (type)local.lua->lua_to##luaType(L,index);}else{return local.lua->luaL_error(L, "bad argument #"#index" :"#name" should be "#luaType);}
#define ARG(index,luaType,type,name) type name; _CHECK_ARG(index,luaType,type,name)
#define ARG_DEF(index,luaType,type,name,def) type name;if(local.lua->lua_isnoneornil(L,index)){name=def;}else _CHECK_ARG(index,luaType,type,name)
#define ARG_RANGE(name,range) if (name >= range){std::ostringstream oss;oss<<"invalid "#name" : "<<std::to_string(name); return local.lua->luaL_error(L, oss.str().c_str());}
#define RET(type,value) local.lua->lua_push##type(L,value);return 1;