// IniTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "xmlassistor.h"
#include <string>
#include "StructDesc_Make.h"

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

	node = doc.first_node("MakeStringPath");
	if (node)
	{
		param.MakeStringPath = node->value();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	MAKE_PARAM param;
	InitCfg(param);

	StartMakeCode(param);

	return 0;
}

