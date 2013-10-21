//---------------------------------------------------------------------------
// File:	fileassistor.h
// Date:	2011.08.07
// Code:	Wang Bin
// Desc:
// modify:  Wang Bin
//---------------------------------------------------------------------------

#ifndef FILE_ASSISTOR_H_
#define FILE_ASSISTOR_H_


#ifndef ENGINE_API
	#define ENGINE_API
#endif

ENGINE_API enum CODE_PAGE
{
	cpInvalid = 0,
	cpBegin,
	cpAnsi = cpBegin,
	cpUnicode,
	cpUBE,	  // uncode big endian
	cpUTF8,
	cpEnd,
};

ENGINE_API size_t fileSize(FILE* file);

ENGINE_API int    fileContent(FILE* file, unsigned char** content, size_t offset = 0, int readbytes = -1);

ENGINE_API int    fileContent(const char *filename, unsigned char** content, size_t offset = 0, int readbytes = -1);

ENGINE_API int	fileContent(const wchar_t* filename, unsigned char** content, size_t offset = 0, int readbytes = -1);

ENGINE_API int	fileContentW(const wchar_t *path, wchar_t **ppcontent);

ENGINE_API int	translateWidechar(unsigned char* buffer,  size_t size, wchar_t** content, CODE_PAGE codepage = cpAnsi, int existhead = false);

ENGINE_API int 	readLine(const char *content, int bytes, int& nextline);	// return the line length, if return -1 is error

ENGINE_API int	readLineW(const wchar_t *content, int bytes, int& nextline); // return the line length, if return -1 is error

ENGINE_API CODE_PAGE codePage(FILE* file);

ENGINE_API CODE_PAGE codePage(const char* filename);

ENGINE_API CODE_PAGE codePage(const wchar_t* filename);

#endif // FILE_ASSISTOR_H_