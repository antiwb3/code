#ifndef _STRUCT_DESC_MAKE_H
#define _STRUCT_DESC_MAKE_H

#include <string>
struct MAKE_PARAM
{
	std::string StructCfgPath;
	std::string MakeDstDir;
	std::string MakeStringPath;
};

int StartMakeCode(MAKE_PARAM const& param);

#endif // ! _STRUCT_DESC_MAKE_H