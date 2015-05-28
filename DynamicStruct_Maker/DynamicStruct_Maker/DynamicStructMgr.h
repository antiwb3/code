#ifndef _DYNAMIC_STRUCT_MGR_H
#define _DYNAMIC_STRUCT_MGR_H

#include "DynamicStruct.h"
#include <vector>

class DynamicStructMgr
{
public:
	typedef std::vector<DynamicStruct> DynamicStructArray;
public:
	DynamicStructMgr& Instance();

public:
	int Init();
	DynamicStruct const* Struct(uint index);

private:
	DynamicStructArray m_structs;
};

#endif // ! _DYNAMIC_STRUCT_MGR_H