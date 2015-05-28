// IniTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "xmlassistor.h"
#include <string>
#include "MakeDynamicStruct.h"


void InitCfg(MAKE_PARAM& param)
{
	xml_doc doc;
	xml_node* node = 0;

	xml_createdoc("config.xml", &doc);
	node = doc.first_node("StructCfgPath");
	if (node)
	{
		param.StructCfgPath = node->value();
	}

	node = doc.first_node("MakeDstDir");
	if (node)
	{
		param.MakeDstDir = node->value();
	}

	node = doc.first_node("DefFileName");
	if (node)
	{
		param.DefFileName = node->value();
	}

	node = doc.first_node("StructMgrFileName");
	if (node)
	{
		param.StructMgrFileName = node->value();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	MAKE_PARAM param;
	InitCfg(param);

	StartMakeCode(param);

	return 0;
}

