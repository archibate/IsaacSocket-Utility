#pragma once

#include "lua.hpp"
#include "state.hpp"

#define DEFMOD(name) lua_State* L = local.isaac->luaVM->L; int top = local.lua.lua_gettop(L); local.lua.lua_getglobal(L, "_ISAAC_SOCKET"); local.lua.lua_pushstring(L, "IsaacSocket"); local.lua.lua_gettable(L, -2); local.lua.lua_pushstring(L, #name); local.lua.lua_newtable(L);
#define DEF(name) local.lua.lua_pushstring(L, #name);local.lua.lua_pushcfunction(L, name); local.lua.lua_settable(L, -3)
#define ENDMOD() local.lua.lua_settable(L, -3); local.lua.lua_settop(L, top);

#define _CHECK_ARG(index,luaType,type,name) if(local.lua.lua_is##luaType(L,index)){name = (type)local.lua.lua_to##luaType(L,index);}else{return local.lua.luaL_error(L, "bad argument #"#index": "#name" should be "#luaType);}
#define ARG(index,luaType,type,name) type name; _CHECK_ARG(index,luaType,type,name)
#define ARG_DEF(index,luaType,type,name,def) type name;if(local.lua.lua_isnoneornil(L,index)){name=def;}else _CHECK_ARG(index,luaType,type,name)
#define ARG_RANGE(name,range) if (name >= range){std::ostringstream oss;oss<<"invalid "#name": "<<std::to_string(name); return local.lua.luaL_error(L, oss.str().c_str());}
#define ARG_UDATA(index,udataType,type,name)type name = (type)local.lua.luaL_checkudata(L, index, #udataType);

#define _LUA_PCALL(paramNum,resultNum)if(local.lua.lua_pcall(L, paramNum, resultNum, 0)!=LUA_OK){ARG_DEF(-1,string,string,_err,"unknow error!");local.lua.lua_pop(L, 1);for(int i=0;i<resultNum;i++){local.lua.lua_pushnil(L);}_err.append("\n");function::ConsoleOutput(_err, 0xFFF08080);}

#define RET(type,value) local.lua.lua_push##type(L,value);return 1

#define RET_TABLE() local.lua.lua_newtable(L)
#define RET_TABLE_KEY(keyType,key,valueType,value) local.lua.lua_push##keyType(L,key);local.lua.lua_push##valueType(L,value);local.lua.lua_settable(L,-3)
#define RET_TABLE_END() return 1

#define MOD_CALLBACK_BEGIN(name){lua_State* L = local.isaac->luaVM->L;size_t top = local.lua.lua_gettop(L);local.lua.lua_getglobal(L, "Isaac");local.lua.lua_pushstring(L, "GetCallbacks");local.lua.lua_gettable(L, -2);local.lua.lua_pushstring(L, #name);_LUA_PCALL(1, 1);bool terminate = false;local.lua.lua_pushnil(L);while(local.lua.lua_next(L, -2) != 0){local.lua.lua_pushstring(L, "Function");local.lua.lua_gettable(L, -2);local.lua.lua_pushstring(L, "Mod");local.lua.lua_gettable(L, -3);size_t paramNum = 1
#define MOD_CALLBACK_ARG(paramType,...)local.lua.lua_push##paramType(L, __VA_ARGS__);paramNum++
#define MOD_CALLBACK_CALL()_LUA_PCALL(paramNum, 1)
#define MOD_CALLBACK_END()if(!local.lua.lua_isnil(L, -1)){terminate = true;break;}local.lua.lua_pop(L, 2);}local.lua.lua_settop(L, top);if(terminate){return 1;}}

#define FAST_MOD_CALLBACK_BEGIN(name){lua_State* L = local.isaac->luaVM->L;size_t top = local.lua.lua_gettop(L);local.lua.lua_getglobal(L, "Isaac");local.lua.lua_pushstring(L, "RunCallback");local.lua.lua_gettable(L, -2);local.lua.lua_pushstring(L, #name);size_t paramNum = 1
#define FAST_MOD_CALLBACK_END()_LUA_PCALL(paramNum,0);local.lua.lua_settop(L, top);}
