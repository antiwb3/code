#include "stdafx.h"
#include "StructDesc.h"
#include "StructDescMgr.h"

StructDescMgr& StructDescMgr::Instance()
{
	static StructDescMgr s_ins;
	return s_ins;
}

StructDesc const* StructDescMgr::Struct(uint index)
{
	if (index < m_structs.size())
	{
		return m_structs[index];	
	}
	return NULL;
}

int StructDescMgr::Init()
{

	//==== struct Item info init! =======================================
	{
		StructDesc* info = new StructDesc();
		if (!info)
			return false;

		info->Resize(6);
		info->SetValue(0, "MultiLine", 0, 1, vtBool);
		info->SetValue(1, "RowSpacing", 1, 4, vtInt);
		info->SetValue(2, "FontSpacing", 5, 4, vtInt);
		info->SetValue(3, "HAlign", 9, 1, vtBool);
		info->SetValue(4, "VAlign", 10, 1, vtBool);
		info->SetValue(5, "Script", 11, 128, vtBuffer);
		m_structs.push_back(info);
	}

	//==== struct Image info init! =======================================
	{
		StructDesc* info = new StructDesc();
		if (!info)
			return false;

		info->Resize(6);
		info->SetValue(0, "MultiLine", 139, 1, vtBool);
		info->SetValue(1, "RowSpacing", 140, 4, vtInt);
		info->SetValue(2, "FontSpacing", 144, 4, vtInt);
		info->SetValue(3, "HAlign", 148, 1, vtBool);
		info->SetValue(4, "VAlign", 149, 1, vtBool);
		info->SetValue(5, "Script", 150, 128, vtBuffer);
		m_structs.push_back(info);
	}

	return true;
}
