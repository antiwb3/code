#pragma once

class Test
{
public:
	static void RegisterFunc(lua_State* L);
private:
	int a;
};