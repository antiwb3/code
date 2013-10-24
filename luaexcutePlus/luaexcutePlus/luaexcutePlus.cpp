// luaexcutePlus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lualib.h"
#include "lua.h"
#include "lauxlib.h"

#ifdef _WIN32
#include <direct.h>
#include <Windows.h>
#endif

typedef int (*luaopen)(lua_State* L);

struct DllHolder
{
    HMODULE     hModule;
    DllHolder*  pNext;
};

static DllHolder* s_pDllHeader = NULL;


struct Dir
{
    char dir[MAX_PATH];

    Dir*  pNext;
};

static Dir* s_pDirHeader = NULL;

HMODULE LoadLib(const char* pszlib)
{
    char workdir[MAX_PATH];
    getcwd(workdir, _countof(workdir) - 1);
    workdir[_countof(workdir) - 1] = '\0';

    HMODULE module = LoadLibraryA(pszlib);
    if (module)
        return module;

    Dir* pNext = s_pDirHeader;
    while (pNext)
    {
        chdir(pNext->dir);
        module = LoadLibraryA(pszlib);
        if (module)
           break;
        pNext = pNext->pNext;
    }
    chdir(workdir);
    return module;
}

static int import_lualibdir(lua_State* L)
{
    int top = lua_gettop(L);
    if (top != 1)
        return 0;

    const char* pszdir = lua_tostring(L, 1);
    if (!pszdir)
        return 0;

    Dir *pDir = new Dir;
    strncpy(pDir->dir, pszdir, _countof(pDir->dir));
    pDir->dir[_countof(pDir->dir) - 1] = '\0';


    pDir->pNext = NULL;

    if (!s_pDirHeader)
        s_pDirHeader = pDir;
    else
    {
        pDir->pNext = s_pDirHeader;
        s_pDirHeader = pDir;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int import_lualib(lua_State* L)
{
    int top = lua_gettop(L);
    if (top != 1)
        return 0;

    const char* pszlib = lua_tostring(L, 1);
    if (!pszlib)
        return 0;

    HMODULE module = LoadLib(pszlib);
    if (module == 0)
    {
        printf("load lib failed !\n");
        return 0;
    }

    PVOID address = GetProcAddress(module, "luaopen_lib");
    if (!address)
    {
        printf("can not find luaopen_lib !\n");
        return 0;
    }

    int ret = (luaopen(address))(L);
    //FreeLibrary(module);
    if (ret == 0)
    {
        printf("luaopen_lib faild !\n");
        return 0;
    }

    DllHolder* pDll = new DllHolder;
    pDll->hModule = module;
    pDll->pNext = NULL;
    if (!s_pDllHeader)
        s_pDllHeader = pDll;
    else
    {
        pDll->pNext = s_pDllHeader;
        s_pDllHeader = pDll;
    }
    ret = lua_istable(L, -1);

    return 1;
}

static const struct luaL_Reg global_fun[] = {
    {"importdll", import_lualib},
    {"importdir", import_lualibdir},
    
    {0, 0},
};

void register_globalfuns(lua_State* L, luaL_Reg const* pFunc)
{
    for (; pFunc->name && pFunc->func; ++pFunc)
    {
        lua_register(L, pFunc->name, pFunc->func);
    }
} 

int msgerror(lua_State* L)
{
    int top = lua_gettop(L);
    lua_getglobal(L, "debug");

    lua_pushstring(L, "traceback");
    lua_gettable(L, -2);

    lua_pcall(L, 0, 1, 0);
    printf("lua call failed [%s]\n%s\n", lua_tostring(L, 1), lua_tostring(L, -1));

    lua_settop(L, top);
    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    register_globalfuns(L, global_fun);

    if (argc == 2)
    {
        char* pszlua = argv[1];
        if (pszlua)
        {
            int ret = luaL_dofile(L, pszlua);
            if (ret != 0)
            {
                msgerror(L);
            }
        }
    }
    DllHolder* pNext = s_pDllHeader;
    while  (pNext)
    {
        FreeLibrary(pNext->hModule);
        pNext = pNext->pNext;
    }
	return 0;
}

