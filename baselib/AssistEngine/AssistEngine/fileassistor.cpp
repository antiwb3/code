//---------------------------------------------------------------------------
// File:	fileassistor.cpp
// Date:	2011.08.07
// Code:	Wangbin
// Desc:
// modify: Wangbin
//---------------------------------------------------------------------------

#include "stdafx.h"
#include <Windows.h>
#include "fileassistor.h"

#if !defined(ASSISTOR_NO_STDLIB)
	#include <cstdlib>      // For std::size_t
	#include <assert.h>      // For assert
	#include <tchar.h>
#endif

#ifdef _MSC_VER
#define FA_TEMP_DISABLE_WARNING(warningCode, expression)   \
	__pragma(warning(push))                             \
	__pragma(warning(disable:warningCode))              \
	expression                                          \
	__pragma(warning(pop))
#else
#define FA_TEMP_DISABLE_WARNING(warningCode, expression)   \
	expression
#endif 

#define FA_WHILE_FALSE_NO_WARNING  \
	FA_TEMP_DISABLE_WARNING(4127, while (false))

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	\
    do {	\
	    if ((p)) {	\
		    delete[] (p);	\
		    p = 0; \
	    }	\
    } FA_WHILE_FALSE_NO_WARNING
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) \
    do {	\
	    if ((p)) {	\
		    delete (p);	\
		    p = 0; \
	    }	\
    } FA_WHILE_FALSE_NO_WARNING
#endif

#ifndef SAFE_FILE_CLOSE
#define SAFE_FILE_CLOSE(file) \
	do  \
	{   \
		if ((file))       \
		{                       \
			fclose (file);	\
			file = 0;		\
		}                       \
	} FA_WHILE_FALSE_NO_WARNING
#endif

#ifndef jn2Exit0
#define jn2Exit0(Condition) \
	do  \
	{   \
		if (!(Condition))       \
		{                       \
			goto Exit0;         \
		}                       \
	} FA_WHILE_FALSE_NO_WARNING
#endif

#ifndef ERROR_ID
#define ERROR_ID -1
#endif

ENGINE_API size_t fileSize(FILE* file)
{
	assert(file);

	long org = ftell(file);
	fseek(file, 0, SEEK_END);
	long size = ftell(file);;
	fseek(file, org, SEEK_SET);

	return (size_t)size;
}

ENGINE_API int fileContent(FILE* file, unsigned char** content, size_t offset /*= 0*/, int readbytes /*= -1*/)
{
	int	   ret			  =  ERROR_ID;
	size_t filesize		  =  0;
	size_t rsize	      =  0;
	size_t orgoffset	  =  0;
	unsigned char* buffer =  0;

	jn2Exit0(file);
	jn2Exit0(content);

	orgoffset = ftell(file);
	fseek(file, (long)offset, SEEK_SET);

	filesize = fileSize(file);
	jn2Exit0(filesize >= 0);
	jn2Exit0(offset < filesize);

	filesize = ((readbytes != -1) ? readbytes : filesize);
	buffer = new unsigned char[filesize + 1];
	jn2Exit0(buffer);

	rsize = fread(buffer, sizeof(unsigned char), filesize, file);
	jn2Exit0(ferror(file) == 0);
	jn2Exit0(rsize <= filesize);
	buffer[rsize] = '\0';

	*content = buffer;
	ret = (int)rsize;
Exit0:
	if (ret == ERROR_ID)
		SAFE_DELETE_ARRAY(buffer);

	if (file)
		fseek(file, (long)orgoffset, SEEK_SET);

	return ret;
}

ENGINE_API int fileContent(const char* filename, unsigned char** content, size_t offset /*= 0*/, int readbytes /*= -1*/)
{
	int			  retc	  = false;
	int			  result  = ERROR_ID;
	size_t		  rsize   = 0;
	FILE*		  file    = 0;
	unsigned char* buffer = 0;
	
	jn2Exit0(filename);
	jn2Exit0(filename[0] != '\0');
	jn2Exit0(content);

	retc = fopen_s(&file, filename, "rb");
	jn2Exit0(retc == 0);
	jn2Exit0(file);

	rsize = fileContent(file, &buffer, offset, readbytes);
	jn2Exit0(rsize != -1);

	*content = buffer;
	result   = (int)rsize;
Exit0:
	if (result == ERROR_ID)
		SAFE_DELETE_ARRAY(buffer);

	SAFE_FILE_CLOSE(file);
	return result;
}

ENGINE_API int fileContent(const wchar_t* filename, unsigned char** content, size_t offset/* = 0*/, int readbytes /*= -1*/)
{
	int			   retc   = false;
	int			   result = ERROR_ID;
	size_t		   rsize  = 0;
	FILE*		   file   = 0;
	unsigned char* buffer = 0;

	jn2Exit0(filename);
	jn2Exit0(filename[0] != L'\0');
	jn2Exit0(content);

	retc = _wfopen_s(&file, filename, L"rb");
	jn2Exit0(retc == 0);
	jn2Exit0(file);

	rsize = fileContent(file, &buffer, offset, readbytes);
	jn2Exit0(rsize != -1);

	*content = buffer;
	result   = (int)rsize;
Exit0:
	if (result == ERROR_ID)
		SAFE_DELETE_ARRAY(buffer);

	SAFE_FILE_CLOSE(file);
	return result;
}
/*
    功能：实现文件编码格式的判断
    通过一个文件的最前面三个字节，可以判断出该的编码类型：
    ANSI：　　　　　　　　无格式定义；(第一个字节开始就是文件内容)
    Unicode： 　　　　　　前两个字节为FFFE；
    Unicode big endian：　前两字节为FEFF；　
    UTF-8：　 　　　　　　前两字节为EFBB，第三字节为BF
*/

ENGINE_API int fileContentW(const wchar_t* filename, wchar_t** content)
{
	int			   retc     = false;
	int			   result   = ERROR_ID;
	size_t		   rsize    = 0;
	size_t		   wsize    = 0;
	FILE*		   file     = 0;
	wchar_t*	   wcontent = 0;
	unsigned char* buffer   = 0;
	CODE_PAGE	   codepage = cpInvalid;

	jn2Exit0(filename);
	jn2Exit0(filename[0] != L'\0');
	jn2Exit0(content);

	retc = _wfopen_s(&file, filename, L"rb");
	jn2Exit0(retc == 0);
	jn2Exit0(file);

	codepage = codePage(file);
	jn2Exit0(codepage != cpInvalid);

	rsize = fileContent(file, &buffer);
	jn2Exit0(rsize != -1);
	jn2Exit0(buffer);
	
	wsize = (size_t)translateWidechar(buffer, rsize, &wcontent, codepage, true);
	jn2Exit0(wsize != -1);
	jn2Exit0(wcontent);
	
	*content = wcontent;
	result   = (int)wsize;
Exit0:
	if (result == ERROR_ID)
		SAFE_DELETE_ARRAY(wcontent);

	SAFE_DELETE_ARRAY(buffer);
	SAFE_FILE_CLOSE(file);
	return result;
}

ENGINE_API int codepage2bytecount(CODE_PAGE codepage)
{
	switch(codepage)
	{
	case cpAnsi:
		return 0;
	case cpUnicode:
		return 2;
	case cpUBE:
		return 2;
	case cpUTF8:
		return 3;
		break;
	default:
		return ERROR_ID;
	}
	return ERROR_ID;
}

ENGINE_API int translateWidechar(unsigned char* buffer, size_t size, wchar_t** content, CODE_PAGE codepage /*= cpAnsi*/, int existhead /*= false*/)
{
	int		 retc		= false;
	int		 result		= ERROR_ID;
	int		 headoffset = 0;
	size_t	 wsize	    = 0;
	wchar_t* wbuffer	= 0;

	jn2Exit0(buffer);
	jn2Exit0(codepage >= cpBegin);
	jn2Exit0(codepage < cpEnd);
	jn2Exit0(size > 0);

	if (existhead)
		headoffset = codepage2bytecount(codepage);

	jn2Exit0(headoffset != -1);
	jn2Exit0((size_t)headoffset <= size);

	if (codepage == cpUTF8)
	{
		unsigned char* startbuffer = buffer + headoffset;
		int lastsize = (int)(size - headoffset);

		wsize = ::MultiByteToWideChar(CP_UTF8, 0, (const char *)startbuffer, lastsize, NULL, 0);
		jn2Exit0(wsize > 0);

		wbuffer = new wchar_t[wsize + 1];
		jn2Exit0(wbuffer);

		::MultiByteToWideChar(CP_UTF8, 0, (const char *)startbuffer, lastsize, wbuffer, (int)wsize);
		wbuffer[wsize] = L'\0';
	}
	else if (codepage == cpUnicode)
	{
		unsigned char* startbuffer = buffer + headoffset;
		wsize = size - headoffset;

		wbuffer = new wchar_t[wsize + 1];
		jn2Exit0(wbuffer);

		memcpy(wbuffer, startbuffer, wsize); 
		wbuffer[size] = L'\0';
	}
	else if (codepage == cpUBE)
	{
		
	}
	else
	{
		wsize = ::MultiByteToWideChar(CP_ACP, 0, (const char *)buffer, (int)size, NULL, 0);
		jn2Exit0(wsize > 0);

		wbuffer = new wchar_t[wsize + 1];
		jn2Exit0(wbuffer);

		::MultiByteToWideChar(CP_ACP, 0, (const char *)buffer, (int)size, wbuffer, (int)wsize);
		wbuffer[wsize] = L'\0';
	}

	*content = wbuffer;
	result   = (int)wsize;
Exit0:
	if (result == ERROR_ID)
		SAFE_DELETE_ARRAY(wbuffer);

	return result;
}

ENGINE_API int readLine(const char *content, int bytes, int& nextline)
{
    int retc	= false;
    int result	= -1;
	int count	= 0;   

	if (!content)
		goto Exit0;

	nextline = 0;
	while (nextline < bytes && content[nextline] != '\r' && content[nextline] != '\n')
	{
		nextline++;
	}
	count = nextline;

	if ((nextline + 1 < bytes) && content[nextline] == '\r' && content[nextline + 1] == '\n')
		nextline += 2;
	else
		nextline += 1;
	
    result = count;
Exit0:
    return result;
}

ENGINE_API int readLineW(const wchar_t *content, int bytes, int& nextline)
{
	int retc	= false;
	int result	= -1;
	int count	= 0;   

	if (!content)
		goto Exit0;

	nextline = 0;
	while (nextline < bytes && content[nextline] != L'\r' && content[nextline] != L'\n')
	{
		nextline++;
	}
	count = nextline;

	if ((nextline + 1 < bytes) && content[nextline] == L'\r' && content[nextline + 1] == L'\n')
		nextline += 2;
	else
		nextline += 1;

	result = count;
Exit0:
	return result;
}

ENGINE_API CODE_PAGE codePage(FILE *file)
{
	CODE_PAGE eResult = cpInvalid;

	const int HEAD_SIZE = 3;
	unsigned char bhead[HEAD_SIZE] = {0};
	
	jn2Exit0(file);

	fread(bhead, sizeof(unsigned char), HEAD_SIZE, file);
	jn2Exit0(ferror(file) == 0);

	if (bhead[0] == 0xFF && bhead[1] == 0xFE)
	{
		eResult = cpUnicode;
	}
	else if (bhead[0] == 0xFE && bhead[1] == 0xFF)
	{
		eResult = cpUBE;
	}
	else if (bhead[0] == 0xEF && bhead[1] == 0xBB && bhead[2] == 0xBF)
	{
		eResult = cpUTF8;
	}
	else
	{
		eResult = cpAnsi;
	}
Exit0:
	return eResult;
}

ENGINE_API CODE_PAGE codePage(const char *filename)
{
	int nretcode = false;
	CODE_PAGE eResult = cpInvalid;

	int nsize = 0;
	FILE *file = NULL;
	unsigned char *buffer = NULL;

	jn2Exit0(filename);
	jn2Exit0(filename[0] != '\0');

	nretcode = fopen_s(&file, filename, "rb");
	jn2Exit0(nretcode == 0);
	jn2Exit0(file);

	eResult = codePage(file);
	jn2Exit0(eResult != cpInvalid);
Exit0:
	SAFE_FILE_CLOSE(file);
	return eResult;
}

ENGINE_API CODE_PAGE codePage(const wchar_t *filename)
{
	int nret = false;
	CODE_PAGE eResult = cpInvalid;

	int nsize = 0;
	FILE *file = NULL;
	unsigned char *buffer = NULL;
	
	jn2Exit0(filename);
	jn2Exit0(filename[0] != L'\0');

	nret = _wfopen_s(&file, filename, L"rb");
	jn2Exit0(nret == 0);
	jn2Exit0(file);

	eResult = codePage(file);
	jn2Exit0(eResult != cpInvalid);

Exit0:
	SAFE_FILE_CLOSE(file);
	return eResult;
}
