//---------------------------------------------------------------------------
// File:	path.h
// Date:	2013.10.21
// Code:	Wang Bin
// Desc:
// modify:  Wang Bin
//---------------------------------------------------------------------------

#ifndef _PATH_ASSISTOR_H_
#define _PATH_ASSISTOR_H_

#include <Windows.h>

#ifndef ENGINE_API
    #define ENGINE_API
#endif

ENGINE_API int IsDir( const char* pszPath );
ENGINE_API int IsDirW( const wchar_t* pwszPath );
ENGINE_API int IsPathExist( const char* pszPath );
ENGINE_API int IsPathExistW( const wchar_t* pwszPath );
ENGINE_API int RemovePathSpec( char* pszPath );
ENGINE_API int RemovePathSpecW( wchar_t* pwszPath );
ENGINE_API int MakeDir( const char* pszDir );
ENGINE_API int MakeDirW( const wchar_t* pwszDir );
ENGINE_API int PathCat( char* pszDst, size_t uSize, const char* pszSrc );
ENGINE_API int PathCatW( wchar_t* pszDst, size_t uSize, const wchar_t* pszSrc);
ENGINE_API int GetFileName( const char* pszPath, char* pszResult, size_t uSize);
/*
class Task
{
    int Run(const char* pszPath, WIN32_FIND_DATAA& FindData);
    ...
}
*/
template< class T >
ENGINE_API int WalkDir( const char* pszDir, T* pTask, const char* pszFliter = NULL )
{
    if ( !pszDir || !pTask )
        return false;

    int nRet     = false;
    int nNextRet = 0;
    char szPath[ MAX_PATH ];
    WIN32_FIND_DATAA FindData;

    strncpy_s( szPath, _countof(szPath) - 3, pszDir, strlen(pszDir) );
    szPath[ _countof(szPath) - 3 ] = '\0';

    if ( pszFliter && pszFliter[ 0 ] != '\0' )
        PathCat( szPath, _countof(szPath) - 1, pszFliter );
    else
        PathCat( szPath, _countof(szPath) - 1, "*" );

    HANDLE hFile = ::FindFirstFileA( szPath, &FindData );
    if ( hFile == INVALID_HANDLE_VALUE )
        return false;
    
    if ( (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        nNextRet = ::FindNextFileA( hFile, &FindData ); //fliter cur dir
    else
        nNextRet = 1;

    while (nNextRet != 0)
    {
        if ( strcmp( FindData.cFileName, "." ) == 0  || strcmp( FindData.cFileName, ".." ) == 0 )
            goto NEXT_FILE;

        strncpy_s( szPath, _countof(szPath) - 2, pszDir, strlen(pszDir) );
        szPath[ _countof(szPath) - 2 ] = '\0';

        PathCat( szPath, _countof(szPath) - 1, FindData.cFileName  );

        nRet = pTask->Run( szPath, FindData );
        if ( nRet && (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            WalkDir( szPath, pTask, pszFliter );

NEXT_FILE:
        nNextRet = ::FindNextFileA( hFile, &FindData );
    }

    if ( hFile != INVALID_HANDLE_VALUE )
        FindClose( hFile );

    return true;
}

template< class T >
ENGINE_API int WalkDirW( const wchar_t* pwszDir, T* pTask, const wchar_t* pwszFliter = NULL )
{
    if ( !pwszDir || !pTask )
        return false;

    int nRet        = 0;
    int nNextRet    = 0;
    wchar_t wszPath[ MAX_PATH ];
    WIN32_FIND_DATAW FindData;

    wcsncpy_s( wszPath, _countof(wszPath) - 3, pwszDir, wcslen(pwszDir) );
    wszPath[ _countof(wszPath) - 3 ] = L'\0';

    if ( pwszFliter && pwszFliter[ 0 ] != L'\0' )
        PathCatW( wszPath, _countof(wszPath) - 1, pwszFliter );
    else
        PathCatW( wszPath, _countof(wszPath) - 1, L"*" );

    HANDLE hFile = ::FindFirstFileW( wszPath, &FindData );
    if ( hFile == INVALID_HANDLE_VALUE )
        return false;

    if ( (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        nNextRet = ::FindNextFileW( hFile, &FindData ); //fliter cur dir
    else
        nNextRet = 1;

    while (nNextRet != 0) // GetLastError() == ERROR_NO_MORE_FILES
    {
        if ( wcscmp( FindData.cFileName, L"." ) == 0 || wcscmp( FindData.cFileName, L".." ) == 0 )
            goto NEXT_FILE;

        wcsncpy_s( wszPath, _countof(wszPath) - 1, pwszDir, wcslen(pwszDir) );
        wszPath[ _countof(wszPath) - 1 ] = L'\0';

        PathCatW( wszPath, _countof(wszPath) - 1, FindData.cFileName );

        nRet = pTask->Run( wszPath, FindData );
        if ( nRet && (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            WalkDirW( wszPath, pTask, pwszFliter );

NEXT_FILE:
        nNextRet = ::FindNextFileW( hFile, &FindData );
    };

    if ( hFile != INVALID_HANDLE_VALUE )
        FindClose( hFile );

    return true;
}
#endif // _PATH_ASSISTOR_H_