// luaassistengine.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luapath.h"
#include "luainifile.h"
//#include "luaxml.h"
#include "luafileassistor.h"
#include "lualoader.h"

static int lua_libInfo(lua_State* L)
{
    int top = lua_gettop(L);
    jn2Exit0(top == 0);

    lua_newtable(L);

    lua_pushstring(L, "ini_libs");
    lua_displayInilib(L);
    lua_settable(L, -3);

    lua_pushstring(L, "path_libs");
    lua_displayPathlib(L);
    lua_settable(L, -3);
    
    return 1;
Exit0:
    return 0;
}

static const struct luaL_Reg global_funs[] = {
    {"libinfo",     lua_libInfo}, 

    {NULL,                  NULL},
};

#ifdef __cplusplus
extern "C"
{
#endif

__declspec(dllexport) int luaopen_lib(lua_State *L) {

    lua_newtable(L);
    int table = lua_gettop(L);

    lua_openInilib(L, table);
    lua_openpathlib(L, table);

    //lua_openxmllib(L);
    //lua_openFAssistorlib(L, table);
    lua_regFuns2Table(L, global_funs, table);
    return 1;
}

#ifdef __cplusplus
}
#endif
