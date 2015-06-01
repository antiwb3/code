#include "stdafx.h"
#include "DataToFile.h"

static int CopyToBuffer(char* buffer, int& offset, int bufferLen, const char* src)
{
	int srcLen = (int)strlen(src);
	int ret = strncpy_s( buffer + offset,  bufferLen - 1 - offset, src, strlen(src) );
	if (ret != 0)
		return false;

	offset += srcLen;
	buffer[ offset ] = '\0';
	return true;
}

DataToFile::DataToFile()
: m_bufferSize(0)
, m_offset(0)
, m_file(0)
, m_buffer(0)
{

}

DataToFile::~DataToFile()
{
	if (m_buffer)
		delete[] m_buffer;
}

int DataToFile::Start(const char* path, unsigned int bufferSize /*= 2048*/)
{
	int ret = false;
	int result = false;

	ret = fopen_s(&m_file, path, "wb");
	if (ret != 0)
		goto Exit0;

	if ((int)bufferSize > m_bufferSize)
	{
		if (m_buffer)
			delete[] m_buffer;

		m_buffer = new char[bufferSize];
		if (!m_buffer)
			goto Exit0;

		m_bufferSize = bufferSize;
	}
	m_offset = 0;

// Exit1:
	result = true;
Exit0:
	if (!result)
	{
		fclose(m_file);
	}
	return result;
}

int DataToFile::Finish()
{
	if (m_file && m_offset > 0)
	{
		fwrite(m_buffer, sizeof(char), m_offset, m_file);
		m_offset = 0;
	}

	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
	}
	return true;
}

int DataToFile::AppendStr(const char* str)
{
	if (!str || !m_file)
		return false;

	int len = (int)strlen(str);
	if (len >= m_bufferSize)
	{
		fwrite(str, sizeof(char), len, m_file);
	}
	else if (m_bufferSize - len - m_offset > 1)
	{
		CopyToBuffer(m_buffer, m_offset, m_bufferSize, str);
	}
	else
	{
		fwrite(m_buffer, sizeof(char), m_offset, m_file);
		m_offset = 0;

		CopyToBuffer(m_buffer, m_offset, m_bufferSize, str);
	}

	return true;
}
