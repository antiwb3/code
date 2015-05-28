#include "stdafx.h"
#include "xmlassistor.h"
#include "DataToFile.h"
#include "DynamicStruct.h"
#include <string>
#include "MakeDynamicStruct.h"

static std::string s_StructInitBegin;
static std::string s_StructInitEnd;
static std::string s_StructNewBegin;
static std::string s_StructNewEnd;
static std::string s_StructDefBegin;
static std::string s_StructDefEnd;

#define PUSH_STRUCT(buffer, str)			sprintf_s(buffer, "struct %s\n{\n", str)
#define PUSH_VAR(buffer, varType, str)		sprintf_s(buffer, "	%s %s;\n", varType, str)
#define PUSH_VAR_BUFFER(buffer, str, size)	sprintf_s(buffer, "	char %s[%d];\n",  str, size)

#define MAKE_INIT(buffer, size)					sprintf_s(buffer, "		dynStruct.Resize(%d);\n", size)
#define MAKE_SETVALUE(buffer, index, str, offset, bytes, eType)	sprintf_s(buffer, "		dynStruct.SetValue(%d, \"%s\", %d, %d, %s);\n", index, str, offset, bytes, #eType)

static int DoneMakeCode(xml_node* structNode, DataToFile& classDefDTF, DataToFile& initDataDTF);

int StartMakeCode(MAKE_PARAM const& param)
{
	char pathClassDef[MAX_PATH];
	char pathInitData[MAX_PATH];

	uint len = 0;
	xml_node* node = 0;
	xml_node* structNode = 0;

	DataToFile classDefDTF;
	DataToFile initDataDTF;

	xml_doc doc;

	strncpy_s(pathClassDef, _countof(pathClassDef) - 1, param.MakeDstDir.c_str(), param.MakeDstDir.size());
	strncpy_s(pathInitData, _countof(pathInitData) - 1, param.MakeDstDir.c_str(), param.MakeDstDir.size());

	len = strlen(pathClassDef);
	if (len > 2 && pathClassDef[ len - 2 ] != '\\' && pathClassDef[ len - 2 ] != '/')
	{
		strcat_s(pathClassDef, _countof(pathClassDef) - 1, "\\");
	}

	if (len > 2 && pathInitData[ len - 2 ] != '\\' && pathInitData[ len - 2 ] != '/')
	{
		strcat_s(pathInitData, _countof(pathInitData) - 1, "\\");
	}
	
	strcat_s(pathClassDef, _countof(pathClassDef) - 1, param.StructMgrFileName.c_str());
	strcat_s(pathInitData, _countof(pathInitData) - 1, param.DefFileName.c_str());

	classDefDTF.Start(pathClassDef);
	initDataDTF.Start(pathInitData);

	
	xml_createdoc(param.StructCfgPath.c_str(), &doc);
	node = doc.first_node("StructInitBegin");
	if (node)
		s_StructInitBegin = node->value();

	node = doc.first_node("StructNewBegin");
	if (node)
		s_StructNewBegin = node->value();

	node = doc.first_node("StructNewEnd");
	if (node)
		s_StructNewEnd = node->value();

	node = doc.first_node("StructDefBegin");
	if (node)
		s_StructDefBegin = node->value();

	node = doc.first_node("StructDefEnd");
	if (node)
		s_StructDefEnd = node->value();

	initDataDTF.AppendStr(s_StructDefBegin.c_str());
	classDefDTF.AppendStr(s_StructInitBegin.c_str());

	node = doc.first_node("structs");
	if (node)
	{
		structNode = node->first_node();
		while (structNode)
		{
			DoneMakeCode(structNode, classDefDTF, initDataDTF);
			structNode = structNode->next_sibling();
		}
	}

	node = doc.first_node("StructInitEnd");
	if (node)
		s_StructInitEnd= node->value();

	initDataDTF.AppendStr(s_StructDefEnd.c_str());
	classDefDTF.AppendStr(s_StructInitEnd.c_str());

	initDataDTF.Finish();
	classDefDTF.Finish();

	return true;
}

static int DoneMakeCode(xml_node* structNode, DataToFile& classDefDTF, DataToFile& initDataDTF)
{
	xml_node* child = 0;
	char buffer[MAX_PATH];
	char* value = 0;

	PUSH_STRUCT(buffer, structNode->name());
	initDataDTF.AppendStr(buffer);

	sprintf_s(buffer, "\n	//==== struct %s info init! =======================================\n", structNode->name());
	classDefDTF.AppendStr(buffer);
	classDefDTF.AppendStr(s_StructNewBegin.c_str());

	int offset = 0;
	int count = 0;

	child = structNode->first_node();
	while (child)
	{
		count++;
		child = child->next_sibling();
	}
	MAKE_INIT(buffer, count);
	classDefDTF.AppendStr(buffer);

	child = structNode->first_node();
	int index = 0;
	while (child)
	{
		value = xml_getattrivalue(child, "type");
		if ( _stricmp(value, "bool") == 0)
		{
			MAKE_SETVALUE(buffer, index, child->name(), offset, sizeof(unsigned char), vtBool);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(unsigned char);

			PUSH_VAR(buffer, "unsigned char", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "int") == 0)
		{
			MAKE_SETVALUE(buffer, index, child->name(), offset, sizeof(int), vtInt);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(int);

			PUSH_VAR(buffer, "int", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "float") == 0)
		{
			MAKE_SETVALUE(buffer, index, child->name(), offset, sizeof(float), vtFloat);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(float);

			PUSH_VAR(buffer, "float", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "double") == 0)
		{
			MAKE_SETVALUE(buffer, index, child->name(), offset, sizeof(double), vtDouble);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(double);

			PUSH_VAR(buffer, "double", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "string") == 0)
		{
			value = xml_getattrivalue(child, "size");
			int size = strtol(value, 0, 10);

			MAKE_SETVALUE(buffer, index, child->name(), offset, size, vtBuffer);
			classDefDTF.AppendStr(buffer);
			offset += size;

			PUSH_VAR_BUFFER(buffer, child->name(), size);
			initDataDTF.AppendStr(buffer);
		}
		index++;
		child = child->next_sibling();
	}

	initDataDTF.AppendStr("};\n\n");

	classDefDTF.AppendStr("		m_structs.push_back(dynStruct);\n");
	classDefDTF.AppendStr(s_StructNewEnd.c_str());
	return true;
}