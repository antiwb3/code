// ParseUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "./struct/StructDescMgr.h"
#include "./struct/StructDef.h"
#include "StrAssistor.inl"
#include "inifile.h"



int ParseUI(IniSection* sec, StructDesc* sdesc, KItemNull* data)
{
	STRUCT_INFO info;
	IniKeyMap::iterator it;
	
	int valueInt = 0;
	float valueFloat = 0.0f;
	double valueDouble= 0.0;
	const char* value = 0;

	int count = sdesc->GetVarCount();
	for (int i = 0; i < count; i ++)
	{
		sdesc->GetInfo(i, &info);
		//if (info.bNotNeedParse)
		//	 continue;
		if (stricmp(info.Name, "AniID") == 0)
		{
			data->AniID = 0;
			valueInt = StrAssistor::ToInt(IniFile::GetValue(sec, "AniID"), 0);
			if ( valueInt != 0 )
			{
				data->AniID				= valueInt;
				data->AniSPercentX		= StrAssistor::ToInt(IniFile::GetValue(sec, "AniSPercentX"), 0);
				data->AniSPercentY		= StrAssistor::ToInt(IniFile::GetValue(sec, "AniSPercentY"), 0);
				data->AniDelayTime		= StrAssistor::ToInt(IniFile::GetValue(sec, "AniDelayTime"), 0);
				data->bEndPos			= StrAssistor::ToBool(IniFile::GetValue(sec, "AniCalcByTwoPoint"), 0);
				if (data->bEndPos)
				{
					data->AniEPercentX		= StrAssistor::ToInt(IniFile::GetValue(sec, "AniEPercentX"), 0);
					data->AniEPercentY		= StrAssistor::ToInt(IniFile::GetValue(sec, "AniEPercentY"), 0);
				}
				else
				{
					data->AniEPercentX		= StrAssistor::ToInt(IniFile::GetValue(sec, "AniScale"), 0);
					data->AniEPercentY		= StrAssistor::ToInt(IniFile::GetValue(sec, "AniRotate"), 0);
				}
			}
		}
		else if (stricmp(info.Name, "TipIndex") == 0)
		{
			value = IniFile::GetValue(sec, "$Tip");
			if (value && value[0])
			{
				valueInt = StrAssistor::ToBool( IniFile::GetValue(sec, "OrgTip"), false );
				if (valueInt)
				{

				}
				else
				{

				}
			}
		}
		else
		{
			value = IniFile::GetValue(sec, info.Name);
			if (!value)
				continue;

			switch (info.VType)
			{
			case vtInt:
				valueInt = StrAssistor::ToInt(value, 0);
				sdesc->SetInt((byte*)data, i, valueInt);
				break;
			case vtFloat:
				valueFloat = StrAssistor::ToFloat(value, 0);
				sdesc->SetFloat((byte*)data, i, valueFloat);
				break;
			case vtDouble:
				valueDouble = StrAssistor::ToFloat(value, 0);
				sdesc->SetDouble((byte*)data, i, valueDouble);
				break;
			case vtBuffer:
				sdesc->SetBuffer((byte*)data, i, value);
			default:
				break;
			}
			
		}
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int num = StrAssistor::ToInt("90921");

	return 0;
}
