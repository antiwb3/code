#include "stdafx.h"
#include "inifile.h"
#include <assert.h>
#include "lualoader.h"
#include "luainifile.h"

#define INI_META_LIB "_IniFile"

static int createLuaobj(lua_State* L, IniFile* ini)
{
    return newUserdata(L, ini, INI_META_LIB);
}

static int lua_loadIni(lua_State* L)
{
    int ret = false;
    int top = 0;
    const char* pszFile = NULL;
    const char* pszMode = NULL;
    IniFile* ini = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 1 || top == 2);

    pszFile = lua_tostring(L, 1);
    jn2Exit0(pszFile);

    if (top > 1)
    {
        pszMode = (char *)lua_tostring(L, 2);
        jn2Exit0(pszMode);
    }

    ini = new IniFile;
    jn2Exit0(ini);

    ret = ini->Load(pszFile, pszMode);
    jn2Exit0(ret);

    ret = createLuaobj(L, ini);
    jn2Exit0(ret);

    return 1;
Exit0:
    if (ini)
        delete ini;

    return 0;
}

static int lua_createIni(lua_State* L)
{
    int ret = false;
    int top = 0;
    IniFile* ini = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 0);

    ini = new IniFile;
    jn2Exit0(ini);

    ret = createLuaobj(L, ini);
    jn2Exit0(ret);

    return 1;
Exit0:
    if (ini)
        delete ini;

    return 0;
}
static int lua_closeIni(lua_State* L)
{
    int top = 0;
    IniFile* ini = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    ini = userdataToObj<IniFile>(L, 1);
    if (ini)
        delete ini;

Exit0:
    return 0;
}

static int luaIni_getString(lua_State* L)
{
    int ret = 0;
    int top = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtPtrs},
    };
    char szValue[MAX_PATH];

    top = lua_gettop(L);
    jn2Exit0(top == 3 || top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);
    
    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->GetString(Args[0].pszValue, Args[1].pszValue, Args[2].pszValue, szValue, _countof(szValue) - 1);
    jn2Exit0(ret);

    lua_pushstring(L, szValue);
    return 1;
Exit0:
    return 0;
}

static int luaIni_getInt(lua_State* L)
{
    int ret = 0;
    int top = 0;
    int nValue = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtInt},
    };

    top = lua_gettop(L);
    jn2Exit0(top == 3 || top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->GetInt(Args[0].pszValue, Args[1].pszValue, Args[2].nValue, nValue);
    jn2Exit0(ret);

    lua_pushinteger(L, nValue);
    return 1;
Exit0:
    return 0;
}

static int luaIni_getFloat(lua_State* L)
{
    int ret = 0;
    int top = 0;
    float value = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtFloat},
    };

    top = lua_gettop(L);
    jn2Exit0(top == 3 || top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->GetFloat(Args[0].pszValue, Args[1].pszValue, Args[2].fValue, value);
    jn2Exit0(ret);

    lua_pushnumber(L, value);
    return 1;
Exit0:
    return 0;
}

static int luaIni_getDouble(lua_State* L)
{
    int ret = 0;
    int top = 0;
    double value = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtDouble},
    };

    top = lua_gettop(L);
    jn2Exit0(top == 3 || top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->GetDouble(Args[0].pszValue, Args[1].pszValue, Args[2].lfValue, value);
    jn2Exit0(ret);

    lua_pushnumber(L, value);
    return 1;
Exit0:
    return 0;
}

static int luaIni_writeString(lua_State* L)
{
    int ret = 0;
    int top = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtPtrs},
    };

    top = lua_gettop(L);
    jn2Exit0(top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->WriteString(Args[0].pszValue, Args[1].pszValue, Args[2].pszValue);
    jn2Exit0(ret);
    
    lua_pushboolean(L, true);
    return 1;
Exit0:
    return 0;
}

static int luaIni_writeInt(lua_State* L)
{
    int ret = 0;
    int top = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtInt},
    };

    top = lua_gettop(L);
    jn2Exit0(top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->WriteInt(Args[0].pszValue, Args[1].pszValue, Args[2].nValue);
    jn2Exit0(ret);

    lua_pushboolean(L, true);
    return 1;
Exit0:
    return 0;
}

static int luaIni_writeFloat(lua_State* L)
{
    int ret = 0;
    int top = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtFloat},
    };

    top = lua_gettop(L);
    jn2Exit0(top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->WriteFloat(Args[0].pszValue, Args[1].pszValue, Args[2].fValue);
    jn2Exit0(ret);

    lua_pushboolean(L, true);
    return 1;
Exit0:
    return 0;
}

static int luaIni_writeDouble(lua_State* L)
{
    int ret = 0;
    int top = 0;
    IniFile* ini = NULL;

    CMArg Args[3] = {
        {0, vtPtrs},
        {0, vtPtrs},
        {0, vtDouble},
    };

    top = lua_gettop(L);
    jn2Exit0(top == 4);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    ret = lua_getArgs(L, Args, _countof(Args), top, 2);
    jn2Exit0(ret);

    ret = ini->WriteDouble(Args[0].pszValue, Args[1].pszValue, Args[2].lfValue);
    jn2Exit0(ret);

    lua_pushboolean(L, true);
    return 1;
Exit0:
    return 0;
}

static int luaIni_removeSection(lua_State* L)
{
    int ret = 0;
    int top = 0;
    IniFile* ini = NULL;
    const char* section = NULL;
    
    top = lua_gettop(L);
    jn2Exit0(top == 2);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    section = lua_tostring(L, 2);
    jn2Exit0(section);

    ret = ini->RemoveSection(section);
    jn2Exit0(ret);

    lua_pushboolean(L, true);
    return 1;
Exit0:
    return 0;
}

static int luaIni_nextSection(lua_State* L)
{
    int ret = 0;
    int top = 0;
    IniFile* ini = NULL;
    const char* section = NULL;
    const char* nextSection = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 2);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    section = lua_tostring(L, 2);
    jn2Exit0(section);

    nextSection = ini->NextSection(section);
    jn2Exit0(nextSection);

    lua_pushstring(L, nextSection);
    return 1;
Exit0:
    return 0;
}

static int luaIni_sectionCount(lua_State* L)
{
    int top = 0;
    IniFile* ini = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 1);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    lua_pushinteger(L, ini->GetSectionCount());
    return 1;
Exit0:
    return 0;
}

static int luaIni_isSectionExist(lua_State* L)
{
    int top = 0;
    IniFile* ini = NULL;
    const char* section = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 2);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    section = lua_tostring(L, 2);
    jn2Exit0(section);

    lua_pushboolean(L, ini->IsSectionExist(section));
    return 1;
Exit0:
    return 0;
}

static int luaIni_save(lua_State* L)
{
    int res = false;
    int top = 0;
    IniFile* ini = NULL;
    const char* path = NULL;

    top = lua_gettop(L);
    jn2Exit0(top == 2);

    ini = userdataToObj<IniFile>(L, 1);
    jn2Exit0(ini);

    path = lua_tostring(L, 2);
    jn2Exit0(path);

    res = ini->Save(path);
    lua_pushboolean(L, res);
    return 1;
Exit0:
    return 0;
}

static const struct luaL_Reg global_funs[] = {
    {"OpenIni",         lua_loadIni}, 
    {"CreateIni",       lua_createIni}, 
    {"CloseIni",        lua_closeIni}, 

    {NULL,              NULL},
};

static const struct luaL_Reg ini_libs[] = {
    {"GetString",       luaIni_getString}, 
    {"GetInt",          luaIni_getInt}, 
    {"GetFloat",        luaIni_getFloat}, 
    {"GetDouble",       luaIni_getDouble}, 
    {"WriteString",     luaIni_writeString}, 
    {"WriteInt",        luaIni_writeInt}, 
    {"WriteFloat",      luaIni_writeFloat}, 
    {"WriteDouble",     luaIni_writeDouble}, 
    {"RemoveSection",   luaIni_removeSection}, 
    {"NextSection",     luaIni_nextSection}, 
    {"SectionCount",    luaIni_sectionCount}, 
    {"IsSectionExist",  luaIni_isSectionExist}, 
    {"Save",            luaIni_save}, 

    
    {NULL,              NULL},
};

int lua_displayInilib(lua_State* L)
{
    int nTable = 0;

    lua_newtable(L);
    nTable = lua_gettop(L);

    luaL_Reg* funcs = (luaL_Reg*)global_funs;
    for ( int i = 1 ;funcs && funcs->name && funcs->func; ++funcs)
    {
        lua_pushinteger(L, i++);
        lua_pushstring(L, funcs->name);
        lua_settable(L, -3);
    }
    
    lua_pushstring(L, INI_META_LIB);
    lua_newtable(L);
    funcs = (luaL_Reg*)ini_libs;
    for ( int i = 1; funcs && funcs->name && funcs->func; ++funcs)
    {
        lua_pushinteger(L, i++);
        lua_pushstring(L, funcs->name);
        lua_settable(L, -3);
    }
    lua_settable(L, nTable);
    return 1;
}

int lua_openInilib(lua_State* L, int libtable)
{
    int top = lua_gettop(L);

    lua_regMetafuns(L, INI_META_LIB, ini_libs);
    
    lua_regFuns2Table(L, global_funs, libtable);

    lua_settop(L, top);
    return true;
}