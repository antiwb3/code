// HookAssistor.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "APIHook.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern CAPIHook s_Kernel32Hook_WPM;
extern CAPIHook s_Kernel32Hook_RPM;

void OutputCallData(const char* pcszKey)
{
    FILE* pFile = NULL;

    pFile = fopen( "D:\\CallData.txt", "w+" );
    if ( !pFile )
        return;

    fwrite( pcszKey, sizeof(char), strlen(pcszKey), pFile );
    fclose( pFile );
}

void SaveDataFile( const char* pcszPath, LPCVOID buff, size_t uSize )
{
    FILE* pFile = NULL;

    pFile = fopen( pcszPath, "w" );
    if ( !pFile )
        return;

    fwrite( buff, sizeof(char), uSize, pFile );
    fclose( pFile );
}

typedef BOOL ( *WriteProcessMemory_tdef ) ( 
    __in      HANDLE hProcess,
    __in      LPVOID lpBaseAddress,
    __in_bcount(nSize) LPCVOID lpBuffer,
    __in      SIZE_T nSize,
    __out_opt SIZE_T * lpNumberOfBytesWritten
    );

BOOL WINAPI MyWriteProcessMemory(
  __in      HANDLE hProcess,
  __in      LPVOID lpBaseAddress,
  __in_bcount(nSize) LPCVOID lpBuffer,
  __in      SIZE_T nSize,
  __out_opt SIZE_T * lpNumberOfBytesWritten
)
{
    char buff[MAX_PATH] = {0};
    sprintf(buff, "MyWriteProcessMemory process handle:%p address:%p\n", hProcess, lpBaseAddress );
    OutputCallData( buff );

    char szPath[MAX_PATH] = {0};
    sprintf( szPath, "D:\\wdata_p(%p)_address(%p).txt", hProcess, lpBaseAddress );

    SaveDataFile( szPath, lpBuffer, nSize );
    

    return ( (WriteProcessMemory_tdef) (PROC) s_Kernel32Hook_WPM )( hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten );
}

typedef BOOL ( *ReadProcessMemory_tdef ) ( 
    __in      HANDLE hProcess,
    __in      LPCVOID lpBaseAddress,
    __out_bcount_part(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
    __in      SIZE_T nSize,
    __out_opt SIZE_T * lpNumberOfBytesRead
    );


BOOL WINAPI MyReadProcessMemory(
    __in      HANDLE hProcess,
    __in      LPCVOID lpBaseAddress,
    __out_bcount_part(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
    __in      SIZE_T nSize,
    __out_opt SIZE_T * lpNumberOfBytesRead
)
{
    ReadProcessMemory_tdef fn = ( (ReadProcessMemory_tdef) (PROC) s_Kernel32Hook_RPM );

    BOOL res = 0;
    if ( fn )
    {
        BOOL res = fn ( hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead );

        char buff[ MAX_PATH ] = {0};
        sprintf( buff, "MyReadProcessMemory process handle:%p addres:%p\n", hProcess, lpBaseAddress );
        OutputCallData( buff );

        char szPath[ MAX_PATH ] = {0};
        sprintf( szPath, "D:\\rdata_p(%p)_address(%p).txt", hProcess, lpBaseAddress );

        SaveDataFile( szPath, lpBuffer, nSize );
    }
    return res;
}
static CAPIHook s_Kernel32Hook_WPM( "kernel32.dll", "WriteProcessMemory",  (PROC) MyWriteProcessMemory );
static CAPIHook s_Kernel32Hook_RPM( "kernel32.dll", "ReadProcessMemory",   (PROC) MyReadProcessMemory );

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

