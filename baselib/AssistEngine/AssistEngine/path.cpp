#include "stdafx.h"
#include <io.h>
#include <stdlib.h>
#include "path.h"

int IsDir( const char* pszPath )
{
    int bDir = false;

    WIN32_FIND_DATAA FindData;

    HANDLE hFile = FindFirstFileA( pszPath, &FindData );
    if ( hFile != INVALID_HANDLE_VALUE && (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        bDir = true;

    if ( hFile != INVALID_HANDLE_VALUE )
        FindClose(hFile);

    return bDir;
}


int IsDirW( const wchar_t* pwszPath )
{
    int bDir = false;

    WIN32_FIND_DATAW FindData;

    HANDLE hFile = FindFirstFileW( pwszPath, &FindData );
    if ( hFile != INVALID_HANDLE_VALUE && (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        bDir = true;

    if ( hFile != INVALID_HANDLE_VALUE )
        FindClose(hFile);

    return bDir;
}


int IsPathExist( const char* pszPath )
{
    return (_access( pszPath, 0 ) == 0);
}

int IsPathExistW( const wchar_t* pwszPath )
{
    return _waccess( pwszPath, 0 ) == 0;
}

int RemovePathSpec( char* pszPath )
{
    if ( !pszPath || pszPath[0] == '\0' )
        return 0;

    size_t uLen = strlen(pszPath);
    if ( pszPath[ uLen - 1 ] == '\\' || pszPath[ uLen - 1 ] == '/' )
        uLen--;

    for (int i = (int) (uLen -  1); i >= 0; i--)
    {
        if ( pszPath[ i ] == '\\' || pszPath[ i ] == '/' )
        {
            pszPath[ i ] = '\0';
            return i;
        }
    }
    return 0;
}


int RemovePathSpecW( wchar_t* pwszPath )
{
    if ( !pwszPath || pwszPath[0] == L'\0' )
        return 0;

    size_t uLen = wcslen(pwszPath);
    if ( pwszPath[ uLen - 1 ] == L'\\' || pwszPath[ uLen - 1 ] == L'/' )
        uLen--;

    for (int i = (int) (uLen -  1); i >= 0; i--)
    {
        if ( pwszPath[ i ] == L'\\' || pwszPath[ i ] == L'/' )
        {
            pwszPath[ i ] = L'\0';
            return i;
        }
    }
    return 0;
}

int MakeDir( const char* pszDir )
{
    if ( !pszDir )
        return false;

    if ( IsPathExist( pszDir ) ) 
        return true;

    int nRet = 0;
    char szPrevDir[MAX_PATH];

    strncpy_s( szPrevDir,  _countof(szPrevDir) - 1, pszDir, strlen(pszDir) );
    szPrevDir[ _countof(szPrevDir) - 1 ] = '\0';

    nRet = RemovePathSpec( szPrevDir );
    if ( nRet == 0 )
        return false;

    if ( !IsPathExist( szPrevDir ) )
    {
        MakeDir( szPrevDir );
    }
    return ::CreateDirectoryA( pszDir, NULL );
}


int MakeDirW( const wchar_t* pwszDir )
{
    if ( !pwszDir )
        return false;

    if ( IsPathExistW( pwszDir ) ) 
        return true;

    int nRet = 0;
    wchar_t wszPrevDir[MAX_PATH];

    wcsncpy_s( wszPrevDir, _countof(wszPrevDir) - 1, pwszDir, wcslen(pwszDir) );
    wszPrevDir[ _countof(wszPrevDir) - 1 ] = L'\0';

    nRet = RemovePathSpecW( wszPrevDir );
    if ( nRet == 0 )
        return false;

    if ( !IsPathExistW( wszPrevDir ) )
    {
        MakeDirW( wszPrevDir );
    }
    return ::CreateDirectoryW( pwszDir, NULL );
}

int PathCat( char* pszDst, size_t uSize, const char* pszSrc )
{
    if ( !pszDst || !pszSrc )
        return false;

    char* pszSrcA = (char* )pszSrc;

    size_t uDstLen = strlen(pszDst);
    size_t uSrcLen = strlen(pszSrcA);
    
    if ( uDstLen < 1 )
        return false;

    if (uSrcLen > 0 && pszSrcA[0] == '\\' || pszSrcA[0] == '/')
    {
        pszSrcA++;
        uSrcLen--;
    }

    if ( pszDst[ uDstLen - 1 ] != '/' && pszDst[ uDstLen - 1 ] != '\\')
    {
        if ( uSize < uDstLen + 1 )
            return false;

        pszDst[ uDstLen ] = '\\';
        uDstLen++;
        pszDst[ uDstLen ] = '\0';
    }

    if ( uSize < uDstLen + uSrcLen + 1 ) // + '\0'
        return false;
    
    return ( strcat_s( pszDst, uSize, pszSrcA ) == 0 );
}

int PathCatW( wchar_t* pwszDst, size_t uSize, const wchar_t* pwszSrc)
{
    if ( !pwszDst || !pwszSrc )
        return false;

    wchar_t* pwszSrcA = (wchar_t* )pwszSrc;

    size_t uDstLen = wcslen(pwszDst);
    size_t uSrcLen = wcslen(pwszSrcA);

    if ( uDstLen < 1 )
        return false;

    if (uSrcLen > 0 && pwszSrcA[0] == '\\' || pwszSrcA[0] == '/')
    {
        pwszSrcA++;
        uSrcLen--;
    }

    if ( pwszDst[ uDstLen - 1 ] != L'/' && pwszDst[ uDstLen - 1 ] != L'\\')
    {
        if ( uSize < uDstLen + 1 )
            return false;

        pwszDst[ uDstLen ] = L'\\';
        uDstLen++;
        pwszDst[ uDstLen ] = L'\0';
    }

    if ( uSize < uDstLen + uSrcLen + 1 ) // + '\0'
        return false;

    return ( wcscat_s( pwszDst, uSize, pwszSrcA ) == 0 );
}

int GetFileName( const char* pszPath, char* pszResult, size_t uSize)
{
	if ( !pszPath || !pszResult )
		return false;

	size_t uLen = strlen(pszPath);
	if (uLen > 0 && pszPath[uLen - 1] == '\\' || pszPath[uLen - 1] == '/')
	{
		uLen--;
	}

	size_t uCount = 0;
	for (int i = (int)uLen - 1; i >= 0; i--)
	{
		if ( pszPath[i] == '/' || pszPath[i] == '\\' )
		{
			if (uCount == 0 || uCount > uSize)
				return false;

			strncpy_s(pszResult, uSize, (pszPath + i + 1) , uCount);
			break;
		}
		else
			uCount++;
	}
	return true;
}