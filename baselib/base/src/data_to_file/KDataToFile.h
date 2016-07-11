#pragma once

class KDataToFile
{
public:
	KDataToFile();
	~KDataToFile();

	int Start(const char* path, size_t bufferSize = 2048);
	int Finish();
	int AppendData(BYTE* data, size_t len);
	int WriteFileHeader(BYTE* data, size_t len);
	int Flush();
	size_t GetDataLen();

private:
	size_t		m_dataLen;
	int			m_bufferSize;
	int			m_offset;
	FILE*		m_file;
	BYTE*		m_buffer;
};
