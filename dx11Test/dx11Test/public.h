/*********************************************************************************
Comment Type          : Header File
Output                : No
File Name             : public.h
Create Date           : 2011-7-3 10:34:47
Comment               : 
*********************************************************************************/

#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <assert.h>

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

#ifndef FalseToExit0
#define FalseToExit0(Condition) \
    do  \
    {   \
    if (!(Condition))       \
        {                       \
        goto Exit0;         \
        }                       \
    } FA_WHILE_FALSE_NO_WARNING
#endif

#ifndef TrueToExit1
#define TrueToExit1(Condition) \
    do  \
    {   \
    if (!(Condition))       \
        {                       \
        goto Exit1;         \
        }                       \
    } FA_WHILE_FALSE_NO_WARNING
#endif


#ifndef FailToExit0
#define FailToExit0(Condition) \
    do  \
    {   \
    if (FAILED(Condition))       \
        {                       \
        goto Exit0;         \
        }                       \
    } FA_WHILE_FALSE_NO_WARNING
#endif

#ifndef SuccToExit1
#define SuccToExit1(Condition) \
    do  \
    {   \
    if (SUCCEEDED(Condition))       \
        {                       \
        goto Exit1;         \
        }                       \
    } FA_WHILE_FALSE_NO_WARNING
#endif

#define KG_PROCESS_ERROR(Condition)     \
    do                                      \
{                                       \
    if (!(Condition))                   \
    {                                   \
    goto Exit0;                      \
    }                                   \
} FA_WHILE_FALSE_NO_WARNING


////////////////////////////////////////////////////////////////////////////////
#define KGLOG_PROCESS_ERROR(Condition)  \
    do                                      \
{                                       \
    if (!(Condition))                   \
    {                                   \
    printf(                   \
    "error: at line %u in %ls", \
    __LINE__, __FUNCTION__  \
    );                              \
    goto Exit0;                     \
    }                                   \
} FA_WHILE_FALSE_NO_WARNING

#define KGLOG_COM_PROCESS_ERROR(Condition) \
    do  \
    {   \
    if (FAILED(Condition))  \
        {                       \
        printf(                   \
        "error: at line %u in %ls", \
        __LINE__, __FUNCTION__  \
        );                      \
        goto Exit0;         \
        }                       \
    } FA_WHILE_FALSE_NO_WARNING

#define KG_COM_PROCESS_ERROR KGLOG_COM_PROCESS_ERROR

// TODO: reference additional headers your program requires here

#define KG_ASSERT_EXIT(Condition)       \
    do                                      \
{                                       \
    if (!(Condition))                   \
    {                                   \
    ASSERT(Condition);               \
    goto Exit0;                      \
    }                                   \
} FA_WHILE_FALSE_NO_WARNING

#define KG_COM_RELEASE(pInterface) \
    do  \
    {   \
    if (pInterface)                 \
        {                               \
        (pInterface)->Release();    \
        (pInterface) = NULL;        \
        }                               \
    } FA_WHILE_FALSE_NO_WARNING

#define KG_PROCESS_ERROR(Condition)  \
    do                                      \
{                                       \
    if (!(Condition))                   \
    {                                   \
    goto Exit0;                     \
    }                                   \
} FA_WHILE_FALSE_NO_WARNING

#define KG_PROCESS_SUCCESS(Condition)   \
    do                                      \
{                                       \
    if (Condition)                      \
    {                                   \
    goto Exit1;                      \
    }                                   \
} FA_WHILE_FALSE_NO_WARNING

#endif

