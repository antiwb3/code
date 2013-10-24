#include "stdafx.h"
#include "path.h"
#include <assert.h>
#include "lualoader.h"
#include "luapath.h"

#define LIB_NAME "path"

static int lua_isDir(lua_State* L)
{
    int top = 0;
    const char* path = 0;

    top = lua_gettop(L);
    jn2Exit0(top == 1);
    
    path = lua_tostring(L, 1);
    jn2Exit0(path);

    lua_pushboolean(L, IsDir(path));
    return 1;
Exit0:
    return 0;
}

static int lua_isPathExist(lua_State* L)
{
    int top = 0;
    const char* path = 0;

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    path = lua_tostring(L, 1);
    jn2Exit0(path);

    lua_pushboolean(L, IsPathExist(path));
    return 1;
Exit0:
    return 0;
}

static int lua_removePathSpec(lua_State* L)
{
    int top = 0;
    const char* path = 0;
    char buff[MAX_PATH];

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    path = lua_tostring(L, 1);
    jn2Exit0(path);
    
    strncpy_s( buff,  _countof(buff) - 1, path, strlen(buff) );
    buff[ _countof(buff) - 1 ] = '\0';
    RemovePathSpec(buff);

    lua_pushstring(L, buff);
    return 1;
Exit0:
    return 0;
}

static int lua_makeDir(lua_State* L)
{
    int res = false;
    int top = 0;
    const char* path = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    path = lua_tostring(L, 1);
    jn2Exit0(path);

    res = MakeDir(path);
    lua_pushboolean(L, res);
    return 1;
Exit0:
    return 0;
}

static int lua_pathCat(lua_State* L)
{
    int top = 0;
    const char* dstPath = NULL;
    const char* srcPath = NULL;
    char buff[MAX_PATH];

    top = lua_gettop(L);
    jn2Exit0(top == 2);

    dstPath = lua_tostring(L, 1);
    jn2Exit0(dstPath);

    srcPath = lua_tostring(L, 2);
    jn2Exit0(srcPath);

    strncpy_s( buff,  _countof(buff) - 1, dstPath, strlen(buff) );
    buff[ _countof(buff) - 1 ] = '\0';

    PathCat(buff, _countof(buff) - 1, srcPath);

    lua_pushstring(L, buff);
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

            lua_pushvalue(L, funStkId);

            lua_pushstring(L, pszPath);
            lua_pushvalue(L, tabStkId);
            
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

        int funStkId;
        int tabStkId;
        lua_State* L;
    } task;

    int ret = false;
    int top = 0;
    const char* path = NULL;
    const char* fliter = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 2 || top == 3);

    path = lua_tostring(L, 1);
    jn2Exit0(path);

    ret = lua_isfunction(L, 2);
    jn2Exit0(ret);

    if (top == 3)
    {
        fliter = lua_tostring(L, 3);
        jn2Exit0(fliter);
    }

    task.L = L;
    task.funStkId = 2;
    lua_newtable(L);
    task.tabStkId = lua_gettop(L);

    WalkDir(path, &task, fliter);
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