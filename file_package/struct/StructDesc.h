#ifndef _STRUCT_DESC_H
#define _STRUCT_DESC_H

#include <vector>
#include "typedef_ps.h"

struct STRUCT_INFO
{
	const char* Name;
	int			Offset;
	int			BytesCount;
	int			VType;
};

class StructDesc
{
public:
	StructDesc();
	~StructDesc();

public:
	int		Resize(uint count);
	int		SetValue(uint index, const char* name, int offset, int bytes, int type);
	
	int		GetInfo(uint index, STRUCT_INFO* info);
	int		GetVarCount();
public:
	int		SetInt(byte* data, uint index, int value);
	int		SetUInt(byte* data, uint index, uint value);
	int		SetFloat(byte* data, uint index, float value);
	int		SetDouble(byte* data, uint index, double value);
	int		SetBuffer(byte* data, uint index, const char* src);

	int		GetInt(byte* data, uint index, int* retValue);
	int		GetUInt(byte* data, uint index, uint* retValue);
	float	GetFloat(byte* data, uint index, float* retValue);
	float	GetDouble(byte* data, uint index, double* retValue);
	const char* GetBuffer(byte* data, uint index);

private:
	uint						m_varCount;
	std::vector<std::string>	m_varNames;
	std::vector<int>			m_varOffsets;
	std::vector<int>			m_varBytes;
	std::vector<int>			m_varTypes;

};

#endif // ! _STRUCT_DESC_H