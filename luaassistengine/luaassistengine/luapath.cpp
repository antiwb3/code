#include "stdafx.h"
#include "path.h"
#include <assert.h>
#include "lualoader.h"
#include "luapath.h"

#define LIB_NAME "path"

static int lua_isDir(lua_State* L)
{
    int top = 0;
    const char* pszPath = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 1);
    
    pszPath = lua_tostring(L, 1);
    jn2Exit0(pszPath);

    lua_pushboolean(L, IsDir(pszPath));
    return 1;
Exit0:
    return 0;
}

static int lua_isPathExist(lua_State* L)
{
    int top = 0;
    const char* pszPath = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    pszPath = lua_tostring(L, 1);
    jn2Exit0(pszPath);

    lua_pushboolean(L, IsPathExist(pszPath));
    return 1;
Exit0:
    return 0;
}

static int lua_removePathSpec(lua_State* L)
{
    int top = 0;
    const char* pszPath = NULL;
    char szPath[MAX_PATH];

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    pszPath = lua_tostring(L, 1);
    jn2Exit0(pszPath);
    
    strncpy_s( szPath,  _countof(szPath) - 1, pszPath, strlen(szPath) );
    szPath[ _countof(szPath) - 1 ] = '\0';
    RemovePathSpec(szPath);

    lua_pushstring(L, szPath);
    return 1;
Exit0:
    return 0;
}

static int lua_makeDir(lua_State* L)
{
    int res = false;
    int top = 0;
    const char* pszPath = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    pszPath = lua_tostring(L, 1);
    jn2Exit0(pszPath);

    res = MakeDir(pszPath);
    lua_pushboolean(L, res);
    return 1;
Exit0:
    return 0;
}

static int lua_pathCat(lua_State* L)
{
    int top = 0;
    const char* pszDst = NULL;
    const char* pszSrc = NULL;
    char szPath[MAX_PATH];

    top = lua_gettop(L);
    jn2Exit0(top == 2);

    pszDst = lua_tostring(L, 1);
    jn2Exit0(pszDst);

    pszSrc = lua_tostring(L, 2);
    jn2Exit0(pszSrc);

    strncpy_s( szPath,  _countof(szPath) - 1, pszDst, strlen(szPath) );
    szPath[ _countof(szPath) - 1 ] = '\0';

    PathCat(szPath, _countof(szPath) - 1, pszSrc);

    lua_pushstring(L, szPath);
    return 1;
Exit0:
    return 0;
}

static int lua_walkDir(lua_State* L)
{
    struct LuaTask
    {
        int Run(const char* pszPath, WIN32_FIND_DATAA& FindData)
        {
            int res = true;
            int top = lua_gettop(L);

            lua_pushvalue(L, nFunStkId);

            lua_pushstring(L, pszPath);
            lua_pushvalue(L, nTableStkId);
            
            lua_pushstring(L, "bDir");
            lua_pushboolean(L, (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
            lua_settable(L, -3);

            lua_pushstring(L, "FileName");
            lua_pushstring(L, FindData.cFileName);
            lua_settable(L, -3);

            lua_pushstring(L, "dwCreateTimeLow");
            lua_pushnumber(L, FindData.ftCreationTime.dwLowDateTime);
            lua_settable(L, -3);

            lua_pushstring(L, "dwCreateTimeHigh");
            lua_pushnumber(L, FindData.ftCreationTime.dwHighDateTime);
            lua_settable(L, -3);

            lua_pushstring(L, "dwLastAccessTimeLow");
            lua_pushnumber(L, FindData.ftLastAccessTime.dwLowDateTime);
            lua_settable(L, -3);

            lua_pushstring(L, "dwLastAccessTimeHigh");
            lua_pushnumber(L, FindData.ftLastAccessTime.dwHighDateTime);
            lua_settable(L, -3);

            lua_pushstring(L, "dwLastWriteTimeLow");
            lua_pushnumber(L, FindData.ftLastWriteTime.dwLowDateTime);
            lua_settable(L, -3);

            lua_pushstring(L, "dwLastWriteTimeHigh");
            lua_pushnumber(L, FindData.ftLastWriteTime.dwHighDateTime);
            lua_settable(L, -3);

            int ret = lua_pcall(L, 2, 1, 0);
            if (ret != 0)
                printf("lua call failed [%s]\n%s\n", lua_tostring(L, 1), lua_tostring(L, -1));

            res = (int)lua_toboolean(L, -1);
            lua_settop(L, top);
            return res;
        }

        int nFunStkId;
        int nTableStkId;
        lua_State* L;
    } task;

    int res = false;
    int top = 0;
    const char* pszPath = NULL;
    const char* pszFliter = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 2 || top == 3);

    pszPath = lua_tostring(L, 1);
    jn2Exit0(pszPath);

    res = lua_isfunction(L, 2);
    jn2Exit0(res);

    if (top == 3)
    {
        pszFliter = lua_tostring(L, 3);
        jn2Exit0(pszFliter);
    }

    task.L = L;
    task.nFunStkId = 2;
    lua_newtable(L);
    task.nTableStkId = lua_gettop(L);

    WalkDir(pszPath, &task, pszFliter);
Exit0:
    lua_settop(L, top);
    return 0;
}

static const struct luaL_Reg path_libs[] = {
    {"isdir",           lua_isDir}, 
    {"isexist",         lua_isPathExist}, 
    {"removespec",      lua_removePathSpec}, 
    {"makedir",         lua_makeDir}, 
    {"cat",             lua_pathCat}, 
    {"walkdir",         lua_walkDir}, 

    {NULL,              NULL},
};

int lua_displayPathlib(lua_State* L)
{
    lua_newtable(L);

    lua_pushstring(L, LIB_NAME);
    lua_newtable(L);
    luaL_Reg* funcs = (luaL_Reg*)path_libs;
    for ( int i = 1; funcs && funcs->name && funcs->func; ++funcs)
    {
        lua_pushinteger(L, i++);
        lua_pushstring(L, funcs->name);
        lua_settable(L, -3);
    }
    
    lua_settable(L, -3);
    return 1;
}

int lua_openpathlib(lua_State* L, int libtable)
{
    int top = lua_gettop(L);

    lua_pushstring(L, LIB_NAME);
    lua_newtable(L);

    int table = lua_gettop(L);
    lua_regFuns2Table(L, path_libs, table);

    lua_settable(L, libtable);

    lua_settop(L, top);
    return true;
}