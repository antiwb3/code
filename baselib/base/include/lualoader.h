#ifndef _LUA_LOADER_H_
#define _LUA_LOADER_H_

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

template <typename T>
int newUserdata(lua_State* L, T* pUserData, LPCSTR szMetatable)
{
    assert(szMetatable);

    T** ppUserData = reinterpret_cast<T**>(lua_newuserdata(L, sizeof(T*)));
    if (!ppUserData)
        return false;

    *ppUserData = pUserData;

    luaL_getmetatable(L, szMetatable);
    lua_setmetatable(L, -2);
    return true;
}

template <typename T>
static T* userdataToObj(lua_State* L, int stkId)
{
    T** ppObj = (T** )lua_touserdata(L, stkId);
    if (ppObj)
        return *ppObj;

    return NULL;
}

inline int lua_regFuns2Table(lua_State* L, luaL_Reg const* funcs, int nTable)
{
    if (!funcs)
        return false;

    for (; funcs->name && funcs->func; ++funcs)
    {
        lua_pushstring(L, funcs->name);
        lua_pushcfunction(L, funcs->func);
        lua_settable(L, nTable);
    }
    return true;
}


inline int lua_regFuns(lua_State* L, luaL_Reg const* funcs)
{
    if (!funcs)
        return false;

    for (; funcs->name && funcs->func; ++funcs)
    {
        lua_register(L, funcs->name, funcs->func);
    }
    return true;
}

inline int lua_regMetafuns(lua_State* L, const char* metaName, luaL_Reg const* funcs)
{
    if (!metaName || !funcs)
        return false;

    int ret = 0;
    ret = luaL_newmetatable(L, metaName);    /* create new metatable */
    if (ret == 0) // exist meta same name as metaName
        return false;

    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -2);				/* push metatable */
    lua_rawset(L, -3);					/* metatable.__index = metatable */

    lua_pushstring(L, "__metatable");
    lua_pushboolean(L, true);
    lua_rawset(L, -3); /* set value not allowed be modify the meta table */

    //luaL_openlib(L, NULL, funcs, 0);  // 5.1
    luaL_setfuncs(L, funcs, 0); // 5.2

    lua_pop(L, 1);

    return true;
}

inline int lua_regLibtable(lua_State* L, const char* libName, luaL_Reg const* funcs)
{
    if (!libName || !funcs)
        return false;

    // luaL_openlib(L, libName, funcs, 0); // 5.1
    
    luaL_newlib(L, funcs); // 5.2
    lua_setglobal(L, libName);

    lua_pop(L, 1);

    return true;
}

enum VAR_TYPE
{
    vtInvalid = 0,
    vtBool,
    vtInt,
    vtFloat,
    vtDouble,
    vtDword,
    vtPtrs,
    vtNewPtrs,
    vtUserData
};

struct CMArg
{
    union 
    {
        bool     bValue;
        int      nValue;
        float    fValue;
        double   lfValue;
        char*    pszValue;
        void*    pvUserData;
        unsigned int dwValue;
    };
    BYTE         byType;
};

inline int lua_getArgs(lua_State* L, CMArg* arg, int narg, int topStkId, int startStkId = 1)
{
    int num = (topStkId - startStkId + 1);

    if (narg <= 0 || num <= 0 || narg < num)
        return false;

    for (int i = 0; i < num; i++)
    {
        switch (arg[i].byType)
        {
        case vtInt:
            arg[i].nValue = (int)lua_tointeger(L, i + startStkId);
            break;
        case vtFloat:
            arg[i].fValue = (float)lua_tonumber(L, i + startStkId);
            break;
        case vtDouble:
            arg[i].lfValue = (double)lua_tonumber(L, i + startStkId);
            break;
        case vtDword:
            arg[i].dwValue = (DWORD)lua_tonumber(L, i + startStkId);
            break;
        case vtPtrs:
            {
                arg[i].pszValue = (char *)lua_tostring(L, i + startStkId);
                if (!arg[i].pszValue)
                    goto Exit0;
            }
            break;
        }
    }
    return true;
Exit0:
    return false;
}
#endif //_LUA_LOADER_H_