#include "stdafx.h"
//#include "xmlassistor.h"
#include <assert.h>
#include "lualoader.h"
#include "luaxml.h"

#define LIB_NAME "xml"

static int lua_openXML(lua_State* L)
{

Exit0:
    return 0;
}


static int lua_createXML(lua_State* L)
{

Exit0:
    return 0;
}

static int luaXML_toBuffer(lua_State* L)
{
Exit0:
    return 0;
}

static int luaXML_delNode(lua_State* L)
{
Exit0:
    return 0;
}

static int luaXML_insertNode(lua_State* L)
{
Exit0:
    return 0;
}

static int luaXML_addSibling(lua_State* L)
{
Exit0:
    return 0;
}

static int luaXML_getAttrivalue(lua_State* L)
{
Exit0:
    return 0;
}

int lua_openxmllib(lua_State* L)
{
    static const struct luaL_Reg global_funs[] = {
        {"OpenXML",           lua_openXML}, 
        {"CreateXML",         lua_createXML}, 

        {NULL,              NULL},
    };
    
    static const struct luaL_Reg xmlnode_funs[] = {
        {"OpenXML",           lua_openXML}, 
        {"CreateXML",         lua_createXML}, 

        {NULL,              NULL},
    };

    static const struct luaL_Reg xml_libs[] = {
        {"toBuffer",        luaXML_toBuffer}, 
        {"delNode",         luaXML_delNode}, 
        {"insertNode",      luaXML_insertNode}, 
        {"addSibling",      luaXML_addSibling}, 
        {"attriValue",      luaXML_getAttrivalue}, 

        {NULL,              NULL},
    };
    lua_regFuns(L, global_funs);
    lua_regLibtable(L, LIB_NAME, xml_libs);
    return true;
}