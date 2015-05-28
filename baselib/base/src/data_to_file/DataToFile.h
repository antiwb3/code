#ifndef _DATA_TO_FILE_H
#define _DATA_TO_FILE_H

class DataToFile
{
public:
	DataToFile();
	~DataToFile();

	int Start(const char* path, unsigned int bufferSize = 2048);
	int Finish();
	int AppendStr(const char* str);

private:
	int			m_bufferSize;
	int			m_offset;
	FILE*		m_file;
	char*		m_buffer;
};
#endif // !_DATA_TO_FILE_H
