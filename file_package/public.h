/*********************************************************************************
Comment Type          : Header File
Output                : No
File Name             : public.h
Create Date           : 2011-7-3 10:34:47
Comment               : 
*********************************************************************************/

#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#ifndef ASSERT
#define ASSERT assert
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if (p) { delete (p); (p) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if (p) { delete[] (p); (p) = NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = NULL; }
#endif

#ifndef SAFE_CLOSE
#define SAFE_CLOSE(p) if (p) { (p)->Close(); (p) = NULL; }
#endif

#ifndef SAFE_FILE_CLOSE
#define SAFE_FILE_CLOSE(p) if (p) { fclose(p); (p) = NULL; }
#endif

#ifndef SAFE_DESTORY
#define SAFE_DESTORY(p) if (p) { p->Destory(); (p) = NULL; }
#endif

#ifndef PROCESS_ERROR
#define PROCESS_ERROR(condition) if (!(condition)) { goto Exit0; }
#endif

#ifndef PROCESS_SUCCESS
#define PROCESS_SUCCESS(condition) if (condition) { goto Exit1; }
#endif

#ifndef COM_PROCESS_ERROR
#define COM_PROCESS_ERROR(condition) if (FAILED(condition)) { goto Exit0; }
#endif

#ifndef COM_PROCESS_SUCCESS
#define COM_PROCESS_SUCCESS(condition) if (SUCCEEDED(condition)) { goto Exit1; }
#endif

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam) ((int)(short)LOWORD(lParam))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam) ((int)(short)HIWORD(lParam))
#endif

#ifndef MAKE_LPARAM
#define MAKE_LPARAM(x, y) ((x & 0x0000FFFF) | ((y & 0x0000FFFF) << 16))
#endif

#ifndef INSTANCE
#define INSTANCE(CLASS) CLASS::Instance()
#endif

#endif
