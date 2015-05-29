#include "stdafx.h"
#include "DynamicStruct.h"

DynamicStruct::DynamicStruct()
	: m_varCount(0)
{

}

DynamicStruct::~DynamicStruct()
{

}

int DynamicStruct::Resize(uint count)
{
	if (count > 0)
	{
		m_varNames.resize(count);
		m_varOffsets.resize(count);
		m_varBytes.resize(count);
		m_varTypes.resize(count);
		m_varCount = count;
	}
	return true;
}

int DynamicStruct::SetValue(uint index, const char* name, int offset, int bytes, int type)
{
	if (index  < m_varCount)
	{
		m_varNames[index]	= name;
		m_varOffsets[index] = offset;
		m_varBytes[index]	= bytes;
		m_varTypes[index]	= type;
		return true;
	}
	return false;
}

int DynamicStruct::GetInfo(uint index, STRUCT_INFO* info)
{
	if (info && index < m_varCount)
	{
		info->Name = m_varNames[index].c_str();
		info->Offset = m_varOffsets[index];
		info->BytesCount = m_varBytes[index];
		info->VType = (int)m_varTypes[index];
		return true;
	}
	return false;
}

int DynamicStruct::GetVarCount()
{
	return m_varCount;
}

int DynamicStruct::SetInt(byte* data, uint index, int value)
{
	if (index  < m_varCount)
	{
		int* ptr = (int *)( data + m_varOffsets[ index ] );
		(*ptr) = value;
		return true;
	}
	return false;
}

int	DynamicStruct::SetUInt(byte* data, uint index, uint value)
{
	if (index  < m_varCount)
	{
		uint* ptr = (uint *)( data + m_varOffsets[ index ] );
		(*ptr) = value;
		return true;
	}
	return false;
}

int DynamicStruct::SetFloat(byte* data, uint index, float value)
{
	if (index < m_varCount)
	{
		float* ptr = (float *)( data + m_varOffsets[ index ] );
		(*ptr) = value;
		return true;
	}
	return false;
}

int DynamicStruct::SetDouble(byte* data, uint index, double value)
{
	if (index  < m_varCount)
	{
		double* ptr = (double *)( data + m_varOffsets[ index ] );
		(*ptr) = value;
		return true;
	}
	return false;
}

int DynamicStruct::SetBuffer(byte* data, uint index, const char* src)
{
	if (data && src && index < m_varCount)
	{
		char* ptr = (char *)( data + m_varOffsets[ index ] );

		int srcLen = (int ) strlen(src);
		int ret = strncpy_s ( ptr, m_varBytes[index] - 1, src, strlen(src) );
		if (ret != 0)
			return false;

		ptr[ m_varBytes[index] ] = '\0';
		return true;
	}
	return false;
}

int DynamicStruct::GetInt(byte* data, uint index, int* retValue)
{
	if (data && retValue && index < m_varCount)
	{
		int* ptr = (int *)( data + m_varOffsets[ index ] );
		(*retValue) = (*ptr);
		return true;
	}
	return false;
}

int DynamicStruct::GetUInt(byte* data, uint index, uint* retValue)
{
	if (data && retValue && index < m_varCount)
	{
		uint* ptr = (uint *)( data + m_varOffsets[ index ] );
		(*retValue) = (*ptr);
		return true;
	}
	return false;
}


float DynamicStruct::GetFloat(byte* data, uint index, float* retValue)
{
	if (data && retValue && index < m_varCount)
	{
		float* ptr = (float *)( data + m_varOffsets[ index ] );
		(*retValue) = (*ptr);
		return true;
	}
	return false;
}

float DynamicStruct::GetDouble(byte* data, uint index, double* retValue)
{
	if (data && retValue && index < m_varCount)
	{
		double* ptr = (double *)( data + m_varOffsets[ index ] );
		(*retValue) = (*ptr);
		return true;
	}
	return false;
}

const char* DynamicStruct::GetBuffer(byte* data, uint index)
{
	if (data && index < m_varCount)
	{
		char* ptr = (char *)( data + m_varOffsets[ index ] );
		return ptr;
	}
	return NULL;
}