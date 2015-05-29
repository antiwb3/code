#include "stdafx.h"
#include "xmlassistor.h"
#include "DataToFile.h"
#include <string>
#include <map>
#include "StructDesc_Make.h"

typedef std::map<std::string, std::string> StructStringMap;
static StructStringMap s_MakeStrings;

typedef std::map<std::string, int> StructOffsetMap;
static StructOffsetMap s_StructOffsetMap;

#define PUSH_STRUCT(buffer, type, str)			sprintf_s(buffer, "%s %s", type, str)
#define PUSH_INHERIT(buffer,accessType, type)	sprintf_s(buffer, " : %s%s", accessType, type)
#define PUSH_VAR(buffer, varType, str)			sprintf_s(buffer, "	%s %s;\n", varType, str)
#define PUSH_VAR_BUFFER(buffer, str, size)		sprintf_s(buffer, "	char %s[%d];\n",  str, size)
#define MAKE_INIT(buffer, fmt, size)			sprintf_s(buffer, fmt, size)
#define MAKE_SETVALUE(buffer, fmt, index, str, offset, bytes, eType)	sprintf_s(buffer, fmt, index, str, offset, bytes, #eType)

static int DoneMakeCode(xml_node* structNode, DataToFile& classDefDTF, DataToFile& initDataDTF);

static int InitMakerString(const char* path)
{
	xml_doc doc;
	
	xml_createdoc(path, &doc);

	xml_node* child = doc.first_node();
	while (child)
	{
		s_MakeStrings.insert(std::make_pair(child->name(), child->value()));
		child = child->next_sibling();
	}
	return true;
}

static int InitMakeFilePath(char pathClassDef[], uint pcdMaxCount, char pathInitData[], uint pidMaxCount, const char* dir)
{
	uint len = 0;
	strncpy_s(pathClassDef, pcdMaxCount - 1, dir, strlen(dir));
	strncpy_s(pathInitData, pidMaxCount - 1, dir, strlen(dir));

	len = strlen(pathClassDef);
	if (len >= 1 && pathClassDef[ len - 1 ] != '\\' && pathClassDef[ len - 1 ] != '/')
		strcat_s(pathClassDef, pcdMaxCount - 1, "\\");

	if (len >= 1 && pathInitData[ len - 1 ] != '\\' && pathInitData[ len - 1 ] != '/')
		strcat_s(pathInitData, pidMaxCount - 1, "\\");

	strcat_s(pathClassDef, pcdMaxCount - 1, s_MakeStrings["StructMgrFileName"].c_str());
	strcat_s(pathInitData, pidMaxCount - 1, s_MakeStrings["DefFileName"].c_str());

	return true;
}

int StartMakeCode(MAKE_PARAM const& param)
{
	char pathClassDef[MAX_PATH];
	char pathInitData[MAX_PATH];

	
	xml_node* node = 0;
	xml_node* structNode = 0;

	DataToFile classDefDTF;
	DataToFile initDataDTF;

	xml_doc doc;

	if (!InitMakerString(param.MakeStringPath.c_str()))
		return false;

	if (!InitMakeFilePath(pathClassDef, _countof(pathClassDef), pathInitData, _countof(pathInitData), param.MakeDstDir.c_str()))
		return false;

	classDefDTF.Start(pathClassDef);
	initDataDTF.Start(pathInitData);

	initDataDTF.AppendStr( s_MakeStrings["StructDefBegin"].c_str() );
	classDefDTF.AppendStr( s_MakeStrings["StructDescMgrBegin"].c_str() );

	xml_createdoc(param.StructCfgPath.c_str(), &doc);
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

	initDataDTF.AppendStr( s_MakeStrings["StructDefEnd"].c_str() );
	classDefDTF.AppendStr( s_MakeStrings["StructDescMgrEnd"].c_str() );

	initDataDTF.Finish();
	classDefDTF.Finish();
	s_MakeStrings.clear();
	s_StructOffsetMap.clear();
	return true;
}

static int DoneMakeCode(xml_node* structNode, DataToFile& classDefDTF, DataToFile& initDataDTF)
{
	xml_node* child = 0;
	char buffer[MAX_PATH];
	char* value = 0;
	char* sType = 0;
	char* sInherit = 0;
	int offset = 0;
	int count = 0;
	const char* init_fmt = s_MakeStrings["StructInitSize"].c_str();
	const char* set_fmt = s_MakeStrings["StructSetValue"].c_str();

	sType = xml_getattrivalue(structNode, "type", "struct");
	PUSH_STRUCT(buffer, sType, structNode->name());
	initDataDTF.AppendStr(buffer);

	sInherit = xml_getattrivalue(structNode, "inherit");
	if (sInherit)
	{
		if (_stricmp(sType, "class") == 0)
		{
			PUSH_INHERIT(buffer, "public ", sInherit);
			initDataDTF.AppendStr(buffer);
		}
		else
		{
			PUSH_INHERIT(buffer, "", sInherit);
			initDataDTF.AppendStr(buffer);
		}
		StructOffsetMap::iterator it = s_StructOffsetMap.find(sInherit);
		if (it != s_StructOffsetMap.end())
			offset = (*it).second;
	}
	initDataDTF.AppendStr("\n{\n");

	sprintf_s(buffer, "\n	//==== struct %s info init! =======================================\n", structNode->name());
	classDefDTF.AppendStr(buffer);
	classDefDTF.AppendStr( s_MakeStrings["StructNewBegin"].c_str() );
	
	count = 0;
	child = structNode->first_node();
	while (child)
	{
		count++;
		child = child->next_sibling();
	}

	MAKE_INIT(buffer, init_fmt, count);
	classDefDTF.AppendStr(buffer);

	child = structNode->first_node();
	int index = 0;
	while (child)
	{
		value = xml_getattrivalue(child, "type");
		if ( _stricmp(value, "bool") == 0)
		{
			MAKE_SETVALUE(buffer, set_fmt, index, child->name(), offset, sizeof(unsigned char), vtBool);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(unsigned char);

			PUSH_VAR(buffer, "unsigned char", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "int") == 0)
		{
			MAKE_SETVALUE(buffer, set_fmt, index, child->name(), offset, sizeof(int), vtInt);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(int);

			PUSH_VAR(buffer, "int", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "uint") == 0)
		{
			MAKE_SETVALUE(buffer, set_fmt, index, child->name(), offset, sizeof(uint), vtInt);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(int);

			PUSH_VAR(buffer, "uint", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "float") == 0)
		{
			MAKE_SETVALUE(buffer, set_fmt, index, child->name(), offset, sizeof(float), vtFloat);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(float);

			PUSH_VAR(buffer, "float", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "double") == 0)
		{
			MAKE_SETVALUE(buffer, set_fmt, index, child->name(), offset, sizeof(double), vtDouble);
			classDefDTF.AppendStr(buffer);
			offset += sizeof(double);

			PUSH_VAR(buffer, "double", child->name());
			initDataDTF.AppendStr(buffer);
		}
		else if (_stricmp(value, "string") == 0)
		{
			value = xml_getattrivalue(child, "size");
			int size = strtol(value, 0, 10);

			MAKE_SETVALUE(buffer, set_fmt, index, child->name(), offset, size, vtBuffer);
			classDefDTF.AppendStr(buffer);
			offset += size;

			PUSH_VAR_BUFFER(buffer, child->name(), size);
			initDataDTF.AppendStr(buffer);
		}
		index++;
		child = child->next_sibling();
	}
	s_StructOffsetMap.insert(std::make_pair(structNode->name(), offset));

	initDataDTF.AppendStr("};\n\n");

	classDefDTF.AppendStr( s_MakeStrings["StructNewEnd"].c_str() );
	return true;
}