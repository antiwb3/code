//---------------------------------------------------------------------------
// File:	xmlassistor.cpp
// Date:	2011.08.07
// Code:	Wang Bin
// Desc:
// modify:  Wang Bin
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "./rapidxml_print.hpp"
#include "./rapidxml_utils.hpp"
#include "fileassistor.h"
#include "xmlassistor.h"

inline const char *xml_allocstring(xml_doc &doc, const char *value, int balloc)
{
	if (balloc && value)
		return doc.allocate_string(value);

	return val;
}

ENGINE_API xml_doc *xml_createdoc(const char *path, xml_doc *retdoc)
{
	int nret = false;
	xml_doc *result = NULL;

	byte *buffer = NULL;
	char *allocm = NULL;

	PROCESS_ERROR(path);
	PROCESS_ERROR(path[0] != '\0');
	PROCESS_ERROR(retdoc);

	fileContent(path, &buffer);
	PROCESS_ERROR(buffer);

	allocm = retdoc->allocate_string((const char *)buffer);
	PROCESS_ERROR(allocm);

	nret = retdoc->parse<rapidxml::parse_default>(allocm);
	PROCESS_ERROR(nret);
	
	result = retdoc;
Exit0:
	SAFE_DELETE_ARRAY(buffer);
	return result;
}

ENGINE_API void xml_doc2content(xml_doc &doc, std::string &text)
{
	xml_node *node = doc.first_node();
	while (node)
	{
		std::string Out;
		rapidxml::print(std::back_inserter(Out), *node, 0);
		text = text + Out;

		node = node->next_sibling();
	}
}

ENGINE_API void xml_doc2content(xml_node *node, std::string &text)
{
	if (node)
	{
		rapidxml::print(std::back_inserter(text), *node, 0);
	}
}

ENGINE_API void xml_deleteNode(xml_doc &doc, const char *name)
{
	xml_node *node = NULL;

	PROCESS_ERROR(name);

	node = xml_getnode(doc, name);
	if (node)
	{
		xml_node *parent = node->parent();
		parent->remove_node(node);
	}
Exit0:
	return;
}

ENGINE_API void xml_insertnode(xml_doc &doc, xml_node *parent, xml_node *child)
{
	char *name = NULL;
	char *value = NULL;

	xml_node *node = NULL;
	xml_node *subchild = NULL;
	xml_attri *attri = NULL;
	rapidxml::node_type eType = rapidxml::node_element;

	PROCESS_ERROR(parent);
	PROCESS_ERROR(child);

	name = doc.allocate_string(child->name());
	value = doc.allocate_string(child->value());

	if (name[0] == '\0')
		eType = rapidxml::node_data;

	node = doc.allocate_node(eType, name, value);
	parent->append_node(node);

	attri = child->first_attribute();
	while (attri)
	{
		name = doc.allocate_string(attri->name());
		value = doc.allocate_string(attri->value());
		xml_attri *newattri = doc.allocate_attribute(name, value);
		node->append_attribute(newattri);

		attri = attri->next_attribute();
	}

	subchild = child->first_node();
	while (subchild)
	{
		xml_insertnode(doc, node, subchild);
		subchild = subchild->next_sibling();
	}
Exit0:
	return;
}

ENGINE_API void xml_addsibling(xml_node *changenode, xml_node *dst, int before/* = false*/)
{
	xml_node *parent = NULL;

	PROCESS_ERROR(changenode);
	PROCESS_ERROR(dst);
	PROCESS_ERROR(dst->parent());

	PROCESS_SUCCESS(changenode == dst);

	parent = changenode->parent();
	if (parent)
		parent->remove_node(changenode);

	changenode->m_parent = dst->parent();
	if (before)
	{
		if (dst->m_prev_sibling)
			dst->m_prev_sibling->m_next_sibling = changenode;
		changenode->m_prev_sibling = dst->m_prev_sibling;
		dst->m_prev_sibling = changenode;
		changenode->m_next_sibling = dst;
	}
	else
	{
		if (dst->m_next_sibling)
			dst->m_next_sibling->m_prev_sibling = changenode;
		changenode->m_next_sibling = dst->m_next_sibling;
		dst->m_next_sibling = changenode;
		changenode->m_prev_sibling = dst;
	}
	if (changenode->m_prev_sibling == NULL)
		parent->m_first_node = changenode;

	if (changenode->m_next_sibling == NULL)
		parent->m_last_node = changenode;

Exit1:
Exit0:
	return;
}

ENGINE_API xml_node *xml_getnode(xml_doc &doc, const char *name, int nlayer/* = INT_MAX*/)
{
	xml_node *child = NULL;

	PROCESS_ERROR(name);

	child = doc.first_node();
	while (child)
	{
		if (!strcmp(child->name(), name))
			return child;
		
		if (nlayer - 1 > 0)
		{
			xml_node *result = xml_getnode(child, name, nlayer - 1);
			if (result)
				return result;
		}
		child = child->next_sibling();
	}

Exit0:
	return NULL;
}

ENGINE_API xml_node *xml_getnode(xml_node *parent, const char *name, int nlayer /*= INT_MAX*/)
{
	xml_node *child = NULL;

	PROCESS_ERROR(parent);
	PROCESS_ERROR(name);

	child = parent->first_node();
	while (child)
	{
		if (!strcmp(child->name(), name))
			return child;

		if (nlayer - 1 > 0)
		{
			xml_node *node = xml_getnode(child, name, nlayer - 1);
			if (node)
				return node;
		}
		child = child->next_sibling();
	}
Exit0:
	return NULL;
}

ENGINE_API xml_node *xml_getnode(xml_doc &doc, const char *attriname, const char *attrivalue, int nlayer /*= INT_MAX*/)
{
	xml_node *child = NULL;

	PROCESS_ERROR(attriname);
	PROCESS_ERROR(attrivalue);

	child = doc.first_node();
	while (child)
	{
		char *value = xml_getattrivalue(child, attriname);
		if (!strcmp(value, attrivalue))
			return child;

		if (nlayer - 1 > 0)
		{
			xml_node *result = xml_getnode(child, attriname, attrivalue, nlayer - 1);
			if (result)
				return result;
		}
		child = child->next_sibling();
	}

Exit0:
	return NULL;
}

ENGINE_API xml_node *xml_getnode(xml_node *parent, const char *attriname, const char *attrivalue, int nlayer /*= INT_MAX*/)
{
	xml_node *child = NULL;

	PROCESS_ERROR(parent);
	PROCESS_ERROR(attriname);
	PROCESS_ERROR(attrivalue);

	child = parent->first_node();
	while (child)
	{
		char *value = xml_getattrivalue(child, attriname);
		if (!strcmp(value, attrivalue))
			return child;

		if (nlayer - 1 > 0)
		{
			xml_node *node = xml_getnode(child, attriname, attrivalue, nlayer - 1);
			if (node)
				return node;
		}
		child = child->next_sibling();
	}
Exit0:
	return NULL;
}

ENGINE_API xml_node *xml_appendnode(xml_doc &doc, xml_node *parent, const char *key, const char *value/* = 0*/, int balloc /*= true*/)
{
	xml_node *node = NULL;
	const char *newkey = NULL;
	const char *newvalue = NULL;
	rapidxml::node_type eType = rapidxml::node_element;

	PROCESS_ERROR(parent);
	PROCESS_ERROR(key);

	newkey = xml_allocstring(doc, key, balloc);
	if (!newkey)
		eType = rapidxml::node_data;

	node = doc.allocate_node(eType, newkey);
	parent->append_node(node);

	newvalue = xml_allocstring(doc, value, balloc);
	if (newvalue)
		node->value(newvalue);

	return node;
Exit0:
	return NULL;
}

ENGINE_API xml_attri *xml_appendattri(xml_doc &doc, xml_node *node, const char *name, const char *value, int balloc /*= true*/)
{
	PROCESS_ERROR(node);
	PROCESS_ERROR(name);
	PROCESS_ERROR(value);

	xml_attri *attri = NULL;
	const char *newvalue = NULL;

	attri = node->first_attribute(name);
	if (!attri)
	{
		const char *key = xml_allocstring(doc, name, balloc);
		attri = doc.allocate_attribute(key);
		node->append_attribute(attri);
	}

	newvalue = xml_allocstring(doc, value, balloc);
	if (newvalue)
		attri->value(newvalue);

	return attri;
Exit0:
	return NULL;
}

ENGINE_API char *xml_getattrivalue(xml_node *node, const char *name, const char *defvalue /*= 0*/)
{
	xml_attri *attri = NULL;
	
	PROCESS_ERROR(node);
	PROCESS_ERROR(name);

	attri = node->first_attribute(name);
	
	return attri ? attri->value() : (char *)defvalue;
Exit0:
	return NULL;
}

ENGINE_API int xml_clonenode(xml_doc& doc, xml_node* lh_node, xml_node* rh_node, int onlychild /*= false*/)
{
    xml_node*  child_node   = NULL;
    xml_attri* attri        = NULL;
    xml_node*  new_node     = NULL;

    assert(lh_node);
    assert(rh_node);

    if (!onlychild)
    {
        attri = rh_node->first_attribute();
        while (attri)
        {
            xml_appendattri(doc, lh_node, attri->name(), attri->value());
            attri = attri->next_attribute();
        }
    }

    child_node = rh_node->first_node();
    while (child_node)
    {
        new_node = xml_appendnode(doc, lh_node, child_node->name(), child_node->value(), true, child_node->m_type);
        if (new_node)
        {
            xml_clonenode(doc, new_node, child_node);
        }
        child_node = child_node->next_sibling();
    }
    return true;
}

ENGINE_API xml_node* xml_createchild(xml_doc& doc, xml_node* parent_node, xml_node* data_node)
{
    xml_node* new_node = NULL;

    assert(parent_node);
    assert(data_node);

    new_node = xml_appendnode(doc, parent_node, data_node->name());
    if (new_node)
        xml_clonenode(doc, new_node, data_node);

    return new_node;
}

ENGINE_API int   xmlBool_getattri(xml_node *node, const char* name, int def /*= false*/)
{
    char* value = NULL;
    int   res   = def;

    assert(node);
    assert(name);

    value   = xml_getattrivalue(node, name, NULL);
    res     = ( value ? ( xml_value2bool(value, def) ) : (def) );
    return res;
}

ENGINE_API int   xmlInt_getattri(xml_node *node, const char* name, int def /*= 0*/)
{
    char* value = NULL;
    int   res   = 0;

    assert(node);
    assert(name);

    value   = xml_getattrivalue(node, name, NULL);
    res     = ( value ? ( (int)strtoul(value, NULL, 0) ) : (def) );
    return res;
}

ENGINE_API float xmlFloat_getattri(xml_node *node, const char* name, float def /*= 0.0f*/)
{
    char* value = NULL;
    float res   = 0.0f;

    assert(node);
    assert(name);

    value   = xml_getattrivalue(node, name, NULL);
    res     = ( value ? ( (float)strtod(value, NULL) ) : (def) );
    return res;
}

ENGINE_API double xmlDouble_getattri(xml_node *node, const char* name, double def /*= 0.0f*/)
{
    char*   value = NULL;
    double  res   = 0.0;

    assert(node);
    assert(name);

    value   = xml_getattrivalue(node, name, NULL);
    res     = ( value ? ( (double)strtod(value, NULL) ) : (def) );
    return res;
}

ENGINE_API int xml_value2bool(const char *value, int defaultvalue)
{
    KGLOG_PROCESS_ERROR(value);

    if (!_tcsicmp(value, "true") || !_tcsicmp(value, "1"))
    {
        return true;
    }
    else if (!_tcsicmp(value, "false") || !_tcsicmp(value, "0"))
    {
        return false;
    }
Exit0:
    return defaultvalue;
}
