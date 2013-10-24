#include "stdafx.h"
#include "fileassistor.h"
#include <assert.h>
#include "lualoader.h"
#include "luafileassistor.h"

static int lua_fileSize(lua_State* L)
{


Exit0:
    return 0;
}

static int lua_fileContent(lua_State* L)
{

Exit0:
    return 0;
}

int lua_openFAssistorlib(lua_State* L)
{
    static const struct luaL_Reg fassistor_libs[] = {
        {"FileSize",            lua_fileSize}, 
        {"FileContent",         lua_fileContent}, 

        {NULL,              NULL},
    };

    lua_regFuns(L, fassistor_libs);
    return true;
}