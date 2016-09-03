#include "stdafx.h"
#include "lualib.h"
#include "lua.h"
#include "lauxlib.h"
#include "Test.h"

static int RegisterMetaTable(lua_State* L, char const* szTableName, luaL_Reg const* pFunc) 
{
	int nResult = false;
	int nTableIndex = 0;

	luaL_newmetatable(L, szTableName);  /* create new metatable */
	nTableIndex = lua_gettop(L);

	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -2);				/* push metatable */
	lua_rawset(L, -3);					/* metatable.__index = metatable */

	lua_pushstring(L, "__metatable");
	lua_pushboolean(L, true);//可以是任何值
	lua_rawset(L, -3); //__metatable设置metatable 防止修改元表

	for (; pFunc->name && pFunc->func; ++pFunc)
	{
		lua_pushstring(L, pFunc->name);
		lua_pushcfunction(L, pFunc->func);
		lua_rawset(L, nTableIndex);
	}

	lua_pop(L, 1);

	nResult = true;
Exit0:
	return nResult;
}

int PushTest(lua_State* L, Test* pTest)
{
	int nIndex = 0;
	Test** ppTest = (Test**)lua_newuserdata(L, sizeof(Test*));
	*ppTest = pTest;

	nIndex = lua_gettop(L);

	luaL_getmetatable(L, "TestFunc");
	lua_setmetatable(L, -2);
}

static int SetVisible(lua_State* L)
{
	return 0;
}

void Test::RegisterFunc(lua_State* L)
{
	static luaL_Reg const s_aItemNullMetaTable[] =
	{
		{ "SetVisible",						SetVisible},
		{NULL, NULL},
	};

	RegisterMetaTable(L, "TestFunc", s_aItemNullMetaTable);
}