#include "stdafx.h"
#include <stdio.h>
#include "KDataToFile.h"

static int CopyToBuffer(BYTE* buffer, int& offset, int bufferLen, BYTE* data, int len)
{
	int ret = memcpy_s( (void *)(buffer + offset), bufferLen - offset, (void *)data, len );
	ASSERT(ret == 0);
	if (ret != 0)
		return false;

	offset += (int)len;

	return len;
}

KDataToFile::KDataToFile()
	: m_bufferSize(0)
	, m_offset(0)
	, m_file(0)
	, m_buffer(0)
{

}

KDataToFile::~KDataToFile()
{
	if (m_buffer)
		delete[] m_buffer;
}

int KDataToFile::Start(const char* path, size_t bufferSize /*= 2048*/)
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

		m_buffer = new BYTE[bufferSize];
		if (!m_buffer)
			goto Exit0;

		m_bufferSize = (int)bufferSize;
	}
	m_offset = 0;
	m_dataLen = 0;
	// Exit1:
	result = true;
Exit0:
	if (!result)
	{
		fclose(m_file);
	}
	return result;
}

int KDataToFile::Finish()
{
	Flush();
	
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
	}
	return true;
}

int KDataToFile::AppendData(BYTE* data, size_t len)
{
	if (!data || !m_file || len == 0)
		return false;

	int nlen = (int)len;
	if (nlen >= m_bufferSize)
	{
		Flush();
		fwrite(data, sizeof(BYTE), nlen, m_file);
	}
	else if (nlen + m_offset < m_bufferSize)
	{
		CopyToBuffer(m_buffer, m_offset, m_bufferSize, data, nlen);
	}
	else
	{
		Flush();

		CopyToBuffer(m_buffer, m_offset, m_bufferSize, data, nlen);
	}
	m_dataLen += nlen;

	return true;
}

int KDataToFile::WriteFileHeader(BYTE* data, size_t len)
{
	if (!data || !m_file || len == 0)
		return false;

	
	long orgoffset = ftell(m_file);
	if (orgoffset == 0)
	{
		fwrite(data, sizeof(BYTE), len, m_file);
	}
	else
	{
		fseek(m_file, (long)0, SEEK_SET);
		fwrite(data, sizeof(BYTE), len, m_file);
		fseek(m_file, (long)orgoffset, SEEK_SET);
	}
	return true;
}

int KDataToFile::Flush()
{
	if (m_file && m_offset > 0)
	{
		fwrite(m_buffer, sizeof(BYTE), m_offset, m_file);
		m_offset = 0;
	}

	return true;
}

size_t KDataToFile::GetDataLen()
{
	return m_dataLen;
}