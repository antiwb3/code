#ifndef _MAKE_DYNAMIC_STRUCT_H
#define _MAKE_DYNAMIC_STRUCT_H

#include <string>
struct MAKE_PARAM
{
	std::string StructCfgPath;
	std::string MakeDstDir;
	std::string DefFileName;
	std::string StructMgrFileName;
};

int StartMakeCode(MAKE_PARAM const& param);

#endif // ! _MAKE_DYNAMIC_STRUCT_H