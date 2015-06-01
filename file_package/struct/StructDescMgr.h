#ifndef _STRUCT_DESC_MGR_H
#define _STRUCT_DESC_MGR_H

#include "StructDesc.h"
#include <vector>

class StructDescMgr
{
public:
	typedef std::vector<StructDesc *> StructDescArray;
public:
	StructDescMgr& Instance();

public:
	int Init();
	StructDesc const* Struct(uint index);

private:
	StructDescArray m_structs;
};

#endif // ! _STRUCT_DESC_MGR_H