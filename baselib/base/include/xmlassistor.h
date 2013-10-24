//---------------------------------------------------------------------------
// File:	xmlassistor.h
// Date:	2011.08.07
// Code:	Wang Bin
// Desc:
// modify:  Wang Bin
//---------------------------------------------------------------------------
#ifndef XML_ASSISTOR_H_
#define XML_ASSISTOR_H_

#include "./rapidxml.hpp"
#ifndef byte
	typedef unsigned char byte;
#endif

#ifndef ENGINE_API
	#define ENGINE_API
#endif


ENGINE_API typedef rapidxml::xml_document<char>		xml_doc;
ENGINE_API typedef rapidxml::xml_document<wchar_t> 	xml_docw;
ENGINE_API typedef rapidxml::xml_node<char>			xml_node;
ENGINE_API typedef rapidxml::xml_node<wchar_t>		xml_nodew;
ENGINE_API typedef rapidxml::xml_attribute<char>	xml_attri;
ENGINE_API typedef rapidxml::xml_attribute<wchar_t>	xml_attriw;

ENGINE_API xml_doc *xml_createdoc(const char *path, xml_doc *retdoc);

ENGINE_API void xml_doc2content(xml_doc &doc, std::string &text);

ENGINE_API void xml_doc2content(xml_node *node, std::string &text);

ENGINE_API void xml_deleteNode(xml_doc &doc, const char *name);

ENGINE_API void xml_insertnode(xml_doc &doc, xml_node *parent, xml_node *child);

ENGINE_API void xml_addsibling(xml_node *changenode, xml_node *dst, int before = false);

ENGINE_API char *xml_getattrivalue(xml_node *node, const char *name, const char *defvalue = 0);

ENGINE_API xml_node *xml_getnode(xml_doc &doc, const char *name, int nlayer = INT_MAX);

ENGINE_API xml_node *xml_getnode(xml_node *parent, const char *name, int nlayer = INT_MAX);

ENGINE_API xml_node *xml_getnode(xml_doc &doc, const char *attriname, const char *attrivalue, int nlayer = INT_MAX);

ENGINE_API xml_node *xml_getnode(xml_node *parent, const char *attriname, const char *attrivalue, int nlayer = INT_MAX);

ENGINE_API xml_node *xml_appendnode(xml_doc &doc, xml_node *parent, const char *key, const char *value = 0, int balloc = true);

ENGINE_API xml_attri *xml_appendattri(xml_doc &doc, xml_node *node, const char *name, const char *value, int balloc = true);

#endif // XML_ASSISTOR_H_


