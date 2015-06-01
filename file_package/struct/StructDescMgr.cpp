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

	//==== struct KItemNull info init! =======================================
	{
		StructDesc* info = new StructDesc();
		if (!info)
			return false;

		info->Resize(26);
		info->SetValue(0, "Left", 0, 4, vtInt);
		info->SetValue(1, "Top", 4, 4, vtInt);
		info->SetValue(2, "Width", 8, 4, vtInt);
		info->SetValue(3, "Height", 12, 4, vtInt);
		info->SetValue(4, "PosType", 16, 4, vtInt);
		info->SetValue(5, "EventID", 20, 4, vtInt);
		info->SetValue(6, "DisableScale", 24, 4, vtInt);
		info->SetValue(7, "LockShowAndHide", 28, 1, vtBool);
		info->SetValue(8, "ShowTipType", 29, 4, vtInt);
		info->SetValue(9, "TipRichText", 33, 1, vtBool);
		info->SetValue(10, "Alpha", 34, 4, vtInt);
		info->SetValue(11, "Script", 38, 128, vtBuffer);
		info->SetValue(12, "AniID", 166, 4, vtInt);
		info->SetValue(13, "AniSPercentX", 170, 4, vtFloat);
		info->SetValue(14, "AniSPercentY", 174, 4, vtFloat);
		info->SetValue(15, "AniDelayTime", 178, 4, vtInt);
		info->SetValue(16, "AniCalcByTwoPoint", 182, 1, vtBool);
		info->SetValue(17, "AniEPercentX", 183, 4, vtFloat);
		info->SetValue(18, "AniEPercentY", 187, 4, vtFloat);
		info->SetValue(19, "AniID", 191, 4, vtInt);
		info->SetValue(20, "AniSPercentX", 195, 4, vtFloat);
		info->SetValue(21, "AniSPercentY", 199, 4, vtFloat);
		info->SetValue(22, "AniDelayTime", 203, 4, vtInt);
		info->SetValue(23, "AniCalcByTwoPoint", 207, 1, vtBool);
		info->SetValue(24, "AniEPercentX", 208, 4, vtFloat);
		info->SetValue(25, "AniEPercentY", 212, 4, vtFloat);
		m_structs.push_back(info);
	}

	//==== struct Image info init! =======================================
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

	return true;
}
