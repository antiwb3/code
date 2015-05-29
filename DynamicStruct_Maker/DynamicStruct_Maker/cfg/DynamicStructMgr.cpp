#include "stdafx.h"
#include "DynamicStructMgr.h"

DynamicStructMgr& DynamicStructMgr::Instance()
{
	static DynamicStructMgr s_ins;
	return s_ins;
}

DynamicStruct const* DynamicStructMgr::Struct(uint index)
{
	if (index < m_struct.size())
	{
		return m_structs[index];	
	}
	return NULL;
}

int DynamicStructMgr::Init()
{

	//==== struct Item info init! =======================================
	{
		DynamicStruct dynStruct = new DynamicStruct();
		if (!dynStruct)
			return false;

		dynStruct.Resize(6);
		dynStruct.SetValue(0, "MultiLine", 0, 1, vtBool);
		dynStruct.SetValue(1, "RowSpacing", 1, 4, vtInt);
		dynStruct.SetValue(2, "FontSpacing", 5, 4, vtInt);
		dynStruct.SetValue(3, "HAlign", 9, 1, vtBool);
		dynStruct.SetValue(4, "VAlign", 10, 1, vtBool);
		dynStruct.SetValue(5, "Script", 11, 128, vtBuffer);
		m_structs.push_back(dynStruct);
	}

	//==== struct Image info init! =======================================
	{
		DynamicStruct dynStruct = new DynamicStruct();
		if (!dynStruct)
			return false;

		dynStruct.Resize(6);
		dynStruct.SetValue(0, "MultiLine", 139, 1, vtBool);
		dynStruct.SetValue(1, "RowSpacing", 140, 4, vtInt);
		dynStruct.SetValue(2, "FontSpacing", 144, 4, vtInt);
		dynStruct.SetValue(3, "HAlign", 148, 1, vtBool);
		dynStruct.SetValue(4, "VAlign", 149, 1, vtBool);
		dynStruct.SetValue(5, "Script", 150, 128, vtBuffer);
		m_structs.push_back(dynStruct);
	}

	return true;
}
