// XHookAPI.cpp: implementation of the CXHookAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HookAPIX.h"
#pragma warning (disable : 4731) // “CXHookAPI::ThisCallFunction”: 框架指针寄存器“ebp”被内联程序集代码修改
namespace XLIB
{


	//////////////////////////////////////////////////////////////////////////
	//另一个LDX32

#define C_ERROR         0xFFFFFFFF
#define C_PREFIX        0x00000001
#define C_66            0x00000002
#define C_67            0x00000004
#define C_DATA66        0x00000008
#define C_DATA1         0x00000010
#define C_DATA2         0x00000020
#define C_DATA4         0x00000040
#define C_MEM67         0x00000080
#define C_MEM1          0x00000100
#define C_MEM2          0x00000200
#define C_MEM4          0x00000400
#define C_MODRM         0x00000800
#define C_DATAW0        0x00001000
#define C_FUCKINGTEST   0x00002000
#define C_TABLE_0F      0x00004000

static DWORD table_1[256] =
{
	/* 00 */   C_MODRM
	/* 01 */,  C_MODRM
	/* 02 */,  C_MODRM
	/* 03 */,  C_MODRM
	/* 04 */,  C_DATAW0
	/* 05 */,  C_DATAW0
	/* 06 */,  0
	/* 07 */,  0
	/* 08 */,  C_MODRM
	/* 09 */,  C_MODRM
	/* 0A */,  C_MODRM
	/* 0B */,  C_MODRM
	/* 0C */,  C_DATAW0
	/* 0D */,  C_DATAW0
	/* 0E */,  0
	/* 0F */,  C_TABLE_0F
	/* 10 */,  C_MODRM
	/* 11 */,  C_MODRM
	/* 12 */,  C_MODRM
	/* 13 */,  C_MODRM
	/* 14 */,  C_DATAW0
	/* 15 */,  C_DATAW0
	/* 16 */,  0
	/* 17 */,  0
	/* 18 */,  C_MODRM
	/* 19 */,  C_MODRM
	/* 1A */,  C_MODRM
	/* 1B */,  C_MODRM
	/* 1C */,  C_DATAW0
	/* 1D */,  C_DATAW0
	/* 1E */,  0
	/* 1F */,  0
	/* 20 */,  C_MODRM
	/* 21 */,  C_MODRM
	/* 22 */,  C_MODRM
	/* 23 */,  C_MODRM
	/* 24 */,  C_DATAW0
	/* 25 */,  C_DATAW0
	/* 26 */,  C_PREFIX
	/* 27 */,  0
	/* 28 */,  C_MODRM
	/* 29 */,  C_MODRM
	/* 2A */,  C_MODRM
	/* 2B */,  C_MODRM
	/* 2C */,  C_DATAW0
	/* 2D */,  C_DATAW0
	/* 2E */,  C_PREFIX
	/* 2F */,  0
	/* 30 */,  C_MODRM
	/* 31 */,  C_MODRM
	/* 32 */,  C_MODRM
	/* 33 */,  C_MODRM
	/* 34 */,  C_DATAW0
	/* 35 */,  C_DATAW0
	/* 36 */,  C_PREFIX
	/* 37 */,  0
	/* 38 */,  C_MODRM
	/* 39 */,  C_MODRM
	/* 3A */,  C_MODRM
	/* 3B */,  C_MODRM
	/* 3C */,  C_DATAW0
	/* 3D */,  C_DATAW0
	/* 3E */,  C_PREFIX
	/* 3F */,  0
	/* 40 */,  0
	/* 41 */,  0
	/* 42 */,  0
	/* 43 */,  0
	/* 44 */,  0
	/* 45 */,  0
	/* 46 */,  0
	/* 47 */,  0
	/* 48 */,  0
	/* 49 */,  0
	/* 4A */,  0
	/* 4B */,  0
	/* 4C */,  0
	/* 4D */,  0
	/* 4E */,  0
	/* 4F */,  0
	/* 50 */,  0
	/* 51 */,  0
	/* 52 */,  0
	/* 53 */,  0
	/* 54 */,  0
	/* 55 */,  0
	/* 56 */,  0
	/* 57 */,  0
	/* 58 */,  0
	/* 59 */,  0
	/* 5A */,  0
	/* 5B */,  0
	/* 5C */,  0
	/* 5D */,  0
	/* 5E */,  0
	/* 5F */,  0
	/* 60 */,  0
	/* 61 */,  0
	/* 62 */,  C_MODRM
	/* 63 */,  C_MODRM
	/* 64 */,  C_PREFIX
	/* 65 */,  C_PREFIX
	/* 66 */,  C_PREFIX+C_66
	/* 67 */,  C_PREFIX+C_67
	/* 68 */,  C_DATA66
	/* 69 */,  C_MODRM+C_DATA66
	/* 6A */,  C_DATA1
	/* 6B */,  C_MODRM+C_DATA1
	/* 6C */,  0
	/* 6D */,  0
	/* 6E */,  0
	/* 6F */,  0
	/* 70 */,  C_DATA1
	/* 71 */,  C_DATA1
	/* 72 */,  C_DATA1
	/* 73 */,  C_DATA1
	/* 74 */,  C_DATA1
	/* 75 */,  C_DATA1
	/* 76 */,  C_DATA1
	/* 77 */,  C_DATA1
	/* 78 */,  C_DATA1
	/* 79 */,  C_DATA1
	/* 7A */,  C_DATA1
	/* 7B */,  C_DATA1
	/* 7C */,  C_DATA1
	/* 7D */,  C_DATA1
	/* 7E */,  C_DATA1
	/* 7F */,  C_DATA1
	/* 80 */,  C_MODRM+C_DATA1
	/* 81 */,  C_MODRM+C_DATA66
	/* 82 */,  C_MODRM+C_DATA1
	/* 83 */,  C_MODRM+C_DATA1
	/* 84 */,  C_MODRM
	/* 85 */,  C_MODRM
	/* 86 */,  C_MODRM
	/* 87 */,  C_MODRM
	/* 88 */,  C_MODRM
	/* 89 */,  C_MODRM
	/* 8A */,  C_MODRM
	/* 8B */,  C_MODRM
	/* 8C */,  C_MODRM
	/* 8D */,  C_MODRM
	/* 8E */,  C_MODRM
	/* 8F */,  C_MODRM
	/* 90 */,  0
	/* 91 */,  0
	/* 92 */,  0
	/* 93 */,  0
	/* 94 */,  0
	/* 95 */,  0
	/* 96 */,  0
	/* 97 */,  0
	/* 98 */,  0
	/* 99 */,  0
	/* 9A */,  C_DATA66+C_MEM2
	/* 9B */,  0
	/* 9C */,  0
	/* 9D */,  0
	/* 9E */,  0
	/* 9F */,  0
	/* A0 */,  C_MEM67
	/* A1 */,  C_MEM67
	/* A2 */,  C_MEM67
	/* A3 */,  C_MEM67
	/* A4 */,  0
	/* A5 */,  0
	/* A6 */,  0
	/* A7 */,  0
	/* A8 */,  C_DATA1
	/* A9 */,  C_DATA66
	/* AA */,  0
	/* AB */,  0
	/* AC */,  0
	/* AD */,  0
	/* AE */,  0
	/* AF */,  0
	/* B0 */,  C_DATA1
	/* B1 */,  C_DATA1
	/* B2 */,  C_DATA1
	/* B3 */,  C_DATA1
	/* B4 */,  C_DATA1
	/* B5 */,  C_DATA1
	/* B6 */,  C_DATA1
	/* B7 */,  C_DATA1
	/* B8 */,  C_DATA66
	/* B9 */,  C_DATA66
	/* BA */,  C_DATA66
	/* BB */,  C_DATA66
	/* BC */,  C_DATA66
	/* BD */,  C_DATA66
	/* BE */,  C_DATA66
	/* BF */,  C_DATA66
	/* C0 */,  C_MODRM+C_DATA1
	/* C1 */,  C_MODRM+C_DATA1
	/* C2 */,  C_DATA2
	/* C3 */,  0
	/* C4 */,  C_MODRM
	/* C5 */,  C_MODRM
	/* C6 */,  C_MODRM+C_DATA66
	/* C7 */,  C_MODRM+C_DATA66
	/* C8 */,  C_DATA2+C_DATA1
	/* C9 */,  0
	/* CA */,  C_DATA2
	/* CB */,  0
	/* CC */,  0
	/* CD */,  C_DATA1+C_DATA4
	/* CE */,  0
	/* CF */,  0
	/* D0 */,  C_MODRM
	/* D1 */,  C_MODRM
	/* D2 */,  C_MODRM
	/* D3 */,  C_MODRM
	/* D4 */,  0
	/* D5 */,  0
	/* D6 */,  0
	/* D7 */,  0
	/* D8 */,  C_MODRM
	/* D9 */,  C_MODRM
	/* DA */,  C_MODRM
	/* DB */,  C_MODRM
	/* DC */,  C_MODRM
	/* DD */,  C_MODRM
	/* DE */,  C_MODRM
	/* DF */,  C_MODRM
	/* E0 */,  C_DATA1
	/* E1 */,  C_DATA1
	/* E2 */,  C_DATA1
	/* E3 */,  C_DATA1
	/* E4 */,  C_DATA1
	/* E5 */,  C_DATA1
	/* E6 */,  C_DATA1
	/* E7 */,  C_DATA1
	/* E8 */,  C_DATA66
	/* E9 */,  C_DATA66
	/* EA */,  C_DATA66+C_MEM2
	/* EB */,  C_DATA1
	/* EC */,  0
	/* ED */,  0
	/* EE */,  0
	/* EF */,  0
	/* F0 */,  C_PREFIX
	/* F1 */,  0                       // 0xF1
	/* F2 */,  C_PREFIX
	/* F3 */,  C_PREFIX
	/* F4 */,  0
	/* F5 */,  0
	/* F6 */,  C_FUCKINGTEST
	/* F7 */,  C_FUCKINGTEST
	/* F8 */,  0
	/* F9 */,  0
	/* FA */,  0
	/* FB */,  0
	/* FC */,  0
	/* FD */,  0
	/* FE */,  C_MODRM
	/* FF */,  C_MODRM
}; // table_1

static DWORD table_0F[256] =
{
	/* 00 */   C_MODRM
	/* 01 */,  C_MODRM
	/* 02 */,  C_MODRM
	/* 03 */,  C_MODRM
	/* 04 */,  -1
	/* 05 */,  -1
	/* 06 */,  0
	/* 07 */,  -1
	/* 08 */,  0
	/* 09 */,  0
	/* 0A */,  0
	/* 0B */,  0
	/* 0C */,  -1
	/* 0D */,  -1
	/* 0E */,  -1
	/* 0F */,  -1
	/* 10 */,  -1
	/* 11 */,  -1
	/* 12 */,  -1
	/* 13 */,  -1
	/* 14 */,  -1
	/* 15 */,  -1
	/* 16 */,  -1
	/* 17 */,  -1
	/* 18 */,  -1
	/* 19 */,  -1
	/* 1A */,  -1
	/* 1B */,  -1
	/* 1C */,  -1
	/* 1D */,  -1
	/* 1E */,  -1
	/* 1F */,  -1
	/* 20 */,  -1
	/* 21 */,  -1
	/* 22 */,  -1
	/* 23 */,  -1
	/* 24 */,  -1
	/* 25 */,  -1
	/* 26 */,  -1
	/* 27 */,  -1
	/* 28 */,  -1
	/* 29 */,  -1
	/* 2A */,  -1
	/* 2B */,  -1
	/* 2C */,  -1
	/* 2D */,  -1
	/* 2E */,  -1
	/* 2F */,  -1
	/* 30 */,  -1
	/* 31 */,  -1
	/* 32 */,  -1
	/* 33 */,  -1
	/* 34 */,  -1
	/* 35 */,  -1
	/* 36 */,  -1
	/* 37 */,  -1
	/* 38 */,  -1
	/* 39 */,  -1
	/* 3A */,  -1
	/* 3B */,  -1
	/* 3C */,  -1
	/* 3D */,  -1
	/* 3E */,  -1
	/* 3F */,  -1
	/* 40 */,  -1
	/* 41 */,  -1
	/* 42 */,  -1
	/* 43 */,  -1
	/* 44 */,  -1
	/* 45 */,  -1
	/* 46 */,  -1
	/* 47 */,  -1
	/* 48 */,  -1
	/* 49 */,  -1
	/* 4A */,  -1
	/* 4B */,  -1
	/* 4C */,  -1
	/* 4D */,  -1
	/* 4E */,  -1
	/* 4F */,  -1
	/* 50 */,  -1
	/* 51 */,  -1
	/* 52 */,  -1
	/* 53 */,  -1
	/* 54 */,  -1
	/* 55 */,  -1
	/* 56 */,  -1
	/* 57 */,  -1
	/* 58 */,  -1
	/* 59 */,  -1
	/* 5A */,  -1
	/* 5B */,  -1
	/* 5C */,  -1
	/* 5D */,  -1
	/* 5E */,  -1
	/* 5F */,  -1
	/* 60 */,  -1
	/* 61 */,  -1
	/* 62 */,  -1
	/* 63 */,  -1
	/* 64 */,  -1
	/* 65 */,  -1
	/* 66 */,  -1
	/* 67 */,  -1
	/* 68 */,  -1
	/* 69 */,  -1
	/* 6A */,  -1
	/* 6B */,  -1
	/* 6C */,  -1
	/* 6D */,  -1
	/* 6E */,  -1
	/* 6F */,  -1
	/* 70 */,  -1
	/* 71 */,  -1
	/* 72 */,  -1
	/* 73 */,  -1
	/* 74 */,  -1
	/* 75 */,  -1
	/* 76 */,  -1
	/* 77 */,  -1
	/* 78 */,  -1
	/* 79 */,  -1
	/* 7A */,  -1
	/* 7B */,  -1
	/* 7C */,  -1
	/* 7D */,  -1
	/* 7E */,  -1
	/* 7F */,  -1
	/* 80 */,  C_DATA66
	/* 81 */,  C_DATA66
	/* 82 */,  C_DATA66
	/* 83 */,  C_DATA66
	/* 84 */,  C_DATA66
	/* 85 */,  C_DATA66
	/* 86 */,  C_DATA66
	/* 87 */,  C_DATA66
	/* 88 */,  C_DATA66
	/* 89 */,  C_DATA66
	/* 8A */,  C_DATA66
	/* 8B */,  C_DATA66
	/* 8C */,  C_DATA66
	/* 8D */,  C_DATA66
	/* 8E */,  C_DATA66
	/* 8F */,  C_DATA66
	/* 90 */,  C_MODRM
	/* 91 */,  C_MODRM
	/* 92 */,  C_MODRM
	/* 93 */,  C_MODRM
	/* 94 */,  C_MODRM
	/* 95 */,  C_MODRM
	/* 96 */,  C_MODRM
	/* 97 */,  C_MODRM
	/* 98 */,  C_MODRM
	/* 99 */,  C_MODRM
	/* 9A */,  C_MODRM
	/* 9B */,  C_MODRM
	/* 9C */,  C_MODRM
	/* 9D */,  C_MODRM
	/* 9E */,  C_MODRM
	/* 9F */,  C_MODRM
	/* A0 */,  0
	/* A1 */,  0
	/* A2 */,  0
	/* A3 */,  C_MODRM
	/* A4 */,  C_MODRM+C_DATA1
	/* A5 */,  C_MODRM
	/* A6 */,  -1
	/* A7 */,  -1
	/* A8 */,  0
	/* A9 */,  0
	/* AA */,  0
	/* AB */,  C_MODRM
	/* AC */,  C_MODRM+C_DATA1
	/* AD */,  C_MODRM
	/* AE */,  -1
	/* AF */,  C_MODRM
	/* B0 */,  C_MODRM
	/* B1 */,  C_MODRM
	/* B2 */,  C_MODRM
	/* B3 */,  C_MODRM
	/* B4 */,  C_MODRM
	/* B5 */,  C_MODRM
	/* B6 */,  C_MODRM
	/* B7 */,  C_MODRM
	/* B8 */,  -1
	/* B9 */,  -1
	/* BA */,  C_MODRM+C_DATA1
	/* BB */,  C_MODRM
	/* BC */,  C_MODRM
	/* BD */,  C_MODRM
	/* BE */,  C_MODRM
	/* BF */,  C_MODRM
	/* C0 */,  C_MODRM
	/* C1 */,  C_MODRM
	/* C2 */,  -1
	/* C3 */,  -1
	/* C4 */,  -1
	/* C5 */,  -1
	/* C6 */,  -1
	/* C7 */,  -1
	/* C8 */,  0
	/* C9 */,  0
	/* CA */,  0
	/* CB */,  0
	/* CC */,  0
	/* CD */,  0
	/* CE */,  0
	/* CF */,  0
	/* D0 */,  -1
	/* D1 */,  -1
	/* D2 */,  -1
	/* D3 */,  -1
	/* D4 */,  -1
	/* D5 */,  -1
	/* D6 */,  -1
	/* D7 */,  -1
	/* D8 */,  -1
	/* D9 */,  -1
	/* DA */,  -1
	/* DB */,  -1
	/* DC */,  -1
	/* DD */,  -1
	/* DE */,  -1
	/* DF */,  -1
	/* E0 */,  -1
	/* E1 */,  -1
	/* E2 */,  -1
	/* E3 */,  -1
	/* E4 */,  -1
	/* E5 */,  -1
	/* E6 */,  -1
	/* E7 */,  -1
	/* E8 */,  -1
	/* E9 */,  -1
	/* EA */,  -1
	/* EB */,  -1
	/* EC */,  -1
	/* ED */,  -1
	/* EE */,  -1
	/* EF */,  -1
	/* F0 */,  -1
	/* F1 */,  -1
	/* F2 */,  -1
	/* F3 */,  -1
	/* F4 */,  -1
	/* F5 */,  -1
	/* F6 */,  -1
	/* F7 */,  -1
	/* F8 */,  -1
	/* F9 */,  -1
	/* FA */,  -1
	/* FB */,  -1
	/* FC */,  -1
	/* FD */,  -1
	/* FE */,  -1
	/* FF */,  -1
}; // table_0F


/*
函数名:CallFunction
功  能:call一个函数地址
参  数:	lpAddress函数地址
		dwArguCount参数个数
		...参数列表
例	子:CallFunction( MessageBoxA,4,0,"caption","text",MB_OK );
*/

VOID _CallFunction();
VOID _ThisCallFunction();

typedef DWORD (__cdecl *XCALLFUNCTION)( LPVOID lpAddress,DWORD dwArguCount,...);
#define XCallFunction ((XCALLFUNCTION)_CallFunction)

typedef DWORD ( __cdecl *XTHISCALLFUNCTION)( LPVOID lpThisPointer, LPVOID lpAddress, DWORD dwArguCount, ... );
#define XThisCallFunction ((XTHISCALLFUNCTION)_ThisCallFunction)

#define VALUE_ADD( address, delta ) ( (uintptr_t) (address) + (uintptr_t) (delta) )
#define VALUE_SUB( address, delta ) ( (uintptr_t) (address) - (uintptr_t) (delta) )

#define	MAKE_JMPSHORT_ADDRESS( jmpaddr )	(*LPBYTE ( VALUE_ADD( (jmpaddr), 1 ) ) + 2 + (uintptr_t) (jmpaddr) )

//MAKE_JMP_ADDRESS得到jmp跳转的实际地址
#define	MAKE_JMP_ADDRESS( jmpaddr )			(*LPDWORD ( VALUE_ADD( (jmpaddr), 1 ) ) + 5 + (uintptr_t) (jmpaddr) )

//MAKE_JMP_OFFET 得到jmp指令的偏移量
#define MAKE_JMP_OFFET( from, to )	(DWORD) ( (uintptr_t) to - (uintptr_t) from - 5 )


#define FILL_JMP( jmpaddr, from, to ) *LPDWORD VALUE_ADD( (jmpaddr), 1 ) = MAKE_JMP_OFFET( (from), (to) )

#define MakePtr(cast, ptr, AddValue) (cast)( VALUE_ADD ( (ptr), (AddValue) ) )
	

static size_t __stdcall GetOpCodeSize( BYTE* startaddress );
static int CopyCodeToBuffer( LPVOID lpCode, LPVOID lpBuffer, int* pnCodeLen );	//返回复制代码的长度
static PIMAGE_IMPORT_DESCRIPTOR GetNamedImportDescriptor(HMODULE hModule, LPCSTR lplpszImportModuleule);


DWORD CStack_ESP::GetReturnAddress()
{
	return *LPDWORD (ESP);
}

DWORD CStack_ESP::GetArgument(DWORD index)
{
	return *LPDWORD (ESP+4+index*4);
}

LPVOID* CStack_ESP::GetArguList()
{
	return (LPVOID*) (ESP+4);
}

DWORD CStack_ESP::GetEflags()
{
	return eflags;
}


CXHookAPI::CXHookAPI()
: m_pOriginFunction(0)
, m_dwOrginCodeLen(0)
, m_HookedAddr(0)
{

}

CXHookAPI::~CXHookAPI()
{

}

BOOL CXHookAPI::IsHooked()
{
	return (BOOL)( m_pOriginFunction != 0 );
}

LPVOID CXHookAPI::GetAddressByNameA( LPCSTR lpszImportModule, LPCSTR lpszFunctionName )
{
	HMODULE hModule = LoadLibraryA( lpszImportModule );
	if( 0 == hModule )
		return 0;

	return GetProcAddress( hModule, lpszFunctionName );
}

// hook virtual func  in  class virtutal table
BOOL CXHookAPI::HookVirtualFunction( LPVOID lpThisPointer, int vIndex, LPVOID lpNewFun, BOOL bCanUnHook )
{
	LPVOID* lpVirtaulTable;

	lpVirtaulTable = *( (LPVOID**) lpThisPointer );
	if( 0 == lpVirtaulTable )
		return FALSE;

	m_dwOrginCodeLen	= 0;
	m_HookedAddr		= 0;

	m_pOriginFunction = lpVirtaulTable[ vIndex ];
	if( m_pOriginFunction == lpNewFun )
		return FALSE;

	if( bCanUnHook )
	{
		m_HookedAddr = &lpVirtaulTable[ vIndex ];
		m_dwOrginCodeLen = sizeof(LPVOID);
		CopyMemory( m_pOriginCode, m_HookedAddr, m_dwOrginCodeLen );
	} 

	return WriteMemory( &lpVirtaulTable[ vIndex ], &lpNewFun, sizeof(LPVOID) );
}

BOOL CXHookAPI::HookImportTable( HMODULE hModule, LPCSTR lpszImportModule, LPCSTR lpszFunctionName, LPVOID lpNewFunction )
{
	LPVOID pFunctionAddress = 0;

	if( 0 == hModule )
		hModule = GetModuleHandle( 0 );

	if( 0 == hModule )
		return FALSE;

 	if( 0 == lpszImportModule || 0 == lpszFunctionName )
		return FALSE;

	// Get the specific import descriptor.    
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = GetNamedImportDescriptor( hModule, lpszImportModule );   
	if ( pImportDesc == NULL )
		return FALSE;   
   
	// Get the original thunk information for this DLL.  I cannot use    
	//  the thunk information stored in the pImportDesc->FirstThunk    
	//  because the that is the array that the loader has already    
	//  bashed to fix up all the imports.  This pointer gives us acess    
	//  to the function names.    

	// Get the array pointed to by the pImportDesc->FirstThunk.  This is    
	//  where I will do the actual bash.    
	PIMAGE_THUNK_DATA pRealThunk = MakePtr( PIMAGE_THUNK_DATA, hModule, pImportDesc->FirstThunk );   
	PIMAGE_THUNK_DATA pOrigThunk = 0;
	
	if( pImportDesc->OriginalFirstThunk )//OriginalFirstThunk有可能为0
	{
		pOrigThunk = MakePtr( PIMAGE_THUNK_DATA, hModule, pImportDesc->OriginalFirstThunk );   
	}
	else
	{
		pFunctionAddress = GetAddressByNameA( lpszImportModule, lpszFunctionName );
	}
	
	
	BOOL bDoHook = FALSE;  
	// Loop through and look for the one that matches the name.    
	while (1)   
	{
		if( pOrigThunk && 0 == pOrigThunk->u1.AddressOfData )
			break;
		else if( 0 == pRealThunk->u1.Function )
			break;

		if( pOrigThunk )
		{
			// Only look at those that are imported by name, not ordinal.    
			if ( IMAGE_ORDINAL_FLAG != ( pOrigThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG ) )   
			{   
				// Look get the name of this imported function.    
				PIMAGE_IMPORT_BY_NAME pByName = MakePtr( PIMAGE_IMPORT_BY_NAME, hModule, pOrigThunk->u1.AddressOfData );   
   
				// If the name starts with NULL, then just skip out now.    
				if( 0 == pByName->Name[ 0 ] ) 
					continue;

				// Determines if we do the hook.    
				if( lstrcmpiA( lpszFunctionName, (char*) pByName->Name ) == 0 )
					bDoHook = TRUE;
			}
		}
		else
		{
			if( pFunctionAddress == (LPVOID) (DWORD_PTR) pRealThunk->u1.Function )
			{
				bDoHook = TRUE;
			}
		}
		//  Might want to consider bsearch here.    
		//  See if the particular function name is in the import    
		//  list.  It might be good to consider requiring the    
		//  paHookArray to be in sorted order so bsearch could be    
		//  used so the lookup will be faster.  However, the size of    
		//  uiCount coming into this function should be rather    
		//  small but it is called for each function imported by    
		//  lpszImportModule.  
		if( bDoHook )
		{
			MEMORY_BASIC_INFORMATION mbi_thunk;  

			VirtualQuery( pRealThunk, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION) );   
			VirtualProtect( mbi_thunk.BaseAddress, mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect );   
			
			//保存hook前的代码
			m_HookedAddr = &pRealThunk->u1.Function;
			m_dwOrginCodeLen = sizeof(LPVOID);
			CopyMemory( m_pOriginCode, m_HookedAddr, m_dwOrginCodeLen );

			// Save the original address if requested.    
			m_pOriginFunction = (LPVOID) (DWORD_PTR) pRealThunk->u1.Function;
			
			// Do the actual hook.    
			if ( sizeof( pRealThunk->u1.Function ) == sizeof(ULONGLONG) )
				*(ULONGLONG*) (&pRealThunk->u1.Function) = (ULONGLONG) lpNewFunction;
			else
				*PDWORD (&pRealThunk->u1.Function) = (DWORD) (DWORD_PTR) lpNewFunction;

			// Change the protection back to what it was before I blasted.    
			DWORD dwOldProtect = 0;   
			VirtualProtect( mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect );   
			break;
		}   
   
		// Increment both tables.    
		if ( pOrigThunk )
			pOrigThunk++;   

		pRealThunk++;   
	}   
 
	return bDoHook;   
}

BOOL CXHookAPI::HookExportTableByID( HMODULE hModule, DWORD dwNameOrdinals, LPVOID lpNewFunction )
{	
	IMAGE_DOS_HEADER *		dosheader				= 0;
	IMAGE_OPTIONAL_HEADER * opthdr					= 0;
	PIMAGE_EXPORT_DIRECTORY exports					= 0;
	USHORT					index					= 0; 
	LPVOID					BaseAddress 			= 0;
	PUCHAR					pFuncName				= 0;
	PULONG					pAddressOfFunctions		= 0;
	PULONG					pAddressOfNames			= 0;
	PUSHORT					pAddressOfNameOrdinals	= 0;

	if( 0 == lpNewFunction )
		return FALSE;

	if( 0 == hModule )
		hModule = GetModuleHandle(0);

	BaseAddress = (LPVOID) hModule;

	dosheader	= (IMAGE_DOS_HEADER *) hModule;
	opthdr		= (IMAGE_OPTIONAL_HEADER *) ( (BYTE *) hModule + dosheader->e_lfanew + 24 );
	exports		= (PIMAGE_EXPORT_DIRECTORY) ( (BYTE *) dosheader + opthdr->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );

	pAddressOfFunctions		= (ULONG *)  ( (BYTE *) hModule + exports->AddressOfFunctions );    //函数列表
	pAddressOfNames			= (ULONG *)  ( (BYTE *) hModule + exports->AddressOfNames );		  //名字列表
	pAddressOfNameOrdinals	= (USHORT *) ( (BYTE *) hModule + exports->AddressOfNameOrdinals ); //名字相对应的函数列表中的地址项index

	//这个是查一下是按照什么方式（函数名称or函数序号）来查函数地址的
	if( dwNameOrdinals > exports->NumberOfFunctions )
		return FALSE;

	dwNameOrdinals -= exports->Base;
	index = pAddressOfNameOrdinals[ dwNameOrdinals ];

	MEMORY_BASIC_INFORMATION mbi_thunk;   
	VirtualQuery( &pAddressOfFunctions[ index ], &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION) );   
	VirtualProtect( mbi_thunk.BaseAddress, mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect );   

	m_HookedAddr = &pAddressOfFunctions[ index ];
	m_dwOrginCodeLen = sizeof(LPVOID);
	CopyMemory( m_pOriginCode, m_HookedAddr, m_dwOrginCodeLen );

	m_pOriginFunction = (LPVOID) ( (uintptr_t) hModule + pAddressOfFunctions[ index ] );
	pAddressOfFunctions[ index ] =  (DWORD) (DWORD_PTR) lpNewFunction - (DWORD) (DWORD_PTR) BaseAddress;

	DWORD dwOldProtect;   
	VirtualProtect( mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect );   

	return TRUE;
}

BOOL CXHookAPI::HookExportTable( HMODULE hModule, LPCSTR lpszFunctionName, LPVOID lpNewFunction )
{	
	IMAGE_DOS_HEADER *		dosheader				= 0;
	IMAGE_OPTIONAL_HEADER * opthdr					= 0;
	PIMAGE_EXPORT_DIRECTORY exports					= 0;
	ULONG					i						= 0;
	USHORT					index					= 0; 
	LPVOID					BaseAddress 			= 0;
	PUCHAR					pFuncName				= 0;
	PULONG					pAddressOfFunctions		= 0;
	PULONG					pAddressOfNames			= 0;
	PUSHORT					pAddressOfNameOrdinals	= 0;

	if( 0 == lpszFunctionName || 0 == lpNewFunction)
		return FALSE;

	if( 0 == hModule )
		hModule = GetModuleHandle( 0 );

	BaseAddress = (PDWORD)hModule;
 
	dosheader	= (IMAGE_DOS_HEADER *) hModule;
	opthdr		= (IMAGE_OPTIONAL_HEADER *) ( (BYTE *) hModule + dosheader->e_lfanew + 24 );
	exports		= (PIMAGE_EXPORT_DIRECTORY) ( (BYTE *) dosheader + opthdr->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );
 
	pAddressOfFunctions		= (ULONG *)  ( (BYTE *) hModule+exports->AddressOfFunctions ); //函数列表
	pAddressOfNames			= (ULONG *)  ( (BYTE *) hModule+exports->AddressOfNames );		//名字列表
	pAddressOfNameOrdinals	= (USHORT *) ( (BYTE *) hModule+exports->AddressOfNameOrdinals ); //名字相对应的函数列表中的地址项index
 
	for (i = 0; i < exports->NumberOfNames; i++) 
	{
		index = pAddressOfNameOrdinals[ i ];
		pFuncName = (PUCHAR) ( (BYTE *) hModule + pAddressOfNames[ i ] );

		if ( lstrcmpiA( (char *) pFuncName, lpszFunctionName ) == 0)
		{
			MEMORY_BASIC_INFORMATION mbi_thunk;

			VirtualQuery( &pAddressOfFunctions[ index ], &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION) );   
			VirtualProtect( mbi_thunk.BaseAddress, mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect );   
   
			m_HookedAddr = &pAddressOfFunctions[ index ];
			m_dwOrginCodeLen = sizeof(LPVOID);
			CopyMemory( m_pOriginCode,m_HookedAddr,m_dwOrginCodeLen);
			 
			m_pOriginFunction = (LPVOID) ( (uintptr_t) hModule + pAddressOfFunctions[ index ] );
			pAddressOfFunctions[ index ] = ( (DWORD) (DWORD_PTR) lpNewFunction - (DWORD) (DWORD_PTR) BaseAddress );
				
			DWORD dwOldProtect;   
			VirtualProtect( mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect);   
   
			return TRUE;
		}
	}
 
	return FALSE;
}

/*
HookAddress( LPVOID lpAddress,FILTERROUTINE pRoutine )
lpAddress:

	jmp m_pTempCode

m_pTempCode:


00402411      60            PUSHAD
00402410 >    9C            PUSHFD
00402412      54            PUSH ESP
00402413      E8 E8DBFFFF   CALL pRoutine
00402418      83C4 04       ADD ESP,4
0040241C      9D            POPFD
0040241B      61            POPAD
; m_pOriginFunction is here
	lpAddress前几个字节内容
0040241D    - E9 DEDBFFFF   JMP lpAddress+size

*/

BOOL CXHookAPI::InlineHookAddress( LPVOID lpAddress, FILTERROUTINE pRoutine )
{
	int		nPos,nLen;
	int		nCodelen = 0;
	LPBYTE	lpCode = (LPBYTE)lpAddress;
	DWORD	dwOldProtect = 0;  
	BOOL	bRet;

	if( 0 == lpAddress || 0 == pRoutine)
		return FALSE;

	nPos = 0;

	m_pTempCode[ nPos++ ] = 0x60;//PUSHAD
	m_pTempCode[ nPos++ ] = 0x9c;//PUSHFD
	m_pTempCode[ nPos++ ] = 0x54;//PUSH ESP
	m_pTempCode[ nPos ]	  = 0xe8;//CALL

	FILL_JMP( &m_pTempCode[ nPos ], &m_pTempCode[ nPos ], pRoutine );
	nPos += 5;

	m_pTempCode[ nPos++ ] = 0x83;//ADD ESP,4
	m_pTempCode[ nPos++ ] = 0xC4;
	m_pTempCode[ nPos++ ] = 0x04;
	m_pTempCode[ nPos++ ] = 0x9D;//POPFD
	m_pTempCode[ nPos++ ] = 0x61;//POPAD

	m_pOriginFunction = (LPVOID) &m_pTempCode[ nPos ];
	//将lpFunction前面大于?个字节复制到m_pTempCode
	nLen = CopyCodeToBuffer( lpCode, &m_pTempCode[ nPos ], &nCodelen );
 	nPos += nLen;

	//存保原始代码,以后恢复
	m_HookedAddr = (LPVOID) lpCode;
	m_dwOrginCodeLen = nLen;
	CopyMemory( m_pOriginCode, m_HookedAddr, m_dwOrginCodeLen);

	m_pTempCode[ nPos ] = 0xe9;//jmp
	FILL_JMP( &m_pTempCode[ nPos ], &m_pTempCode[ nPos ], (uintptr_t) lpCode + 5);
	bRet = VirtualProtect( m_pTempCode, sizeof(m_pTempCode), PAGE_EXECUTE_READWRITE, &dwOldProtect );   
	if( FALSE == bRet )
		return FALSE;

	bRet = VirtualProtect( lpCode, nLen, PAGE_EXECUTE_READWRITE, &dwOldProtect );   
	if( FALSE == bRet )
		return FALSE;
	
	//修改lpFunction前5字节为jmp xxxx
	memset( lpCode, 0x90, nCodelen );
	lpCode[ 0 ] = 0xe9;//jmp
	FILL_JMP( lpCode, lpCode, m_pTempCode );
	bRet = VirtualProtect( lpCode, nLen, dwOldProtect, &dwOldProtect );
	if( FALSE == bRet )
		return FALSE;

	return TRUE;
}

LPVOID CXHookAPI::GetOriginFunction()
{
	return m_pOriginFunction;
}

/*
HookFunction( LPVOID lpFunction,LPVOID lpNewFunction )

lpFunction:
	jmp lpNewFunction

lpNewFunction:
CallFunction()
{
  m_pOriginFunction = m_pTempCode;
}

m_pTempCode:
	lpFunction前几个字节内容
	jmp lpFunction+5
*/
BOOL CXHookAPI::InlineHookFunction( LPVOID lpFunction, LPVOID lpNewFunction )
{
	int		nLen			= 0;
	LPBYTE	lpCode			= 0;
	DWORD	dwOldProtect	= 0;  
	BOOL	bRet			= FALSE;
	int		nCodelen		= 0;

	if( 0 == lpFunction || 0 == lpNewFunction)
		return FALSE;

	lpCode = (LPBYTE) lpFunction;

	//将lpFunction前面大于?个字节复制到m_pTempCode
	nLen = CopyCodeToBuffer( lpFunction, m_pTempCode, &nCodelen );

	//存保原始代码,以后恢复
	m_HookedAddr		= (LPVOID) lpCode;
	m_dwOrginCodeLen	= nLen;
	CopyMemory( m_pOriginCode, m_HookedAddr, m_dwOrginCodeLen);

 	//修改m_pTempCode为jmp xxxx
	m_pTempCode[ nLen ] = 0xe9; // 在最后加一句调到原函数 接近着被修改的内容下面的地址 = 当前函数地址 + 5 的位置，修改了前面5个字节的内容
	FILL_JMP( &m_pTempCode[ nLen ], &m_pTempCode[ nLen ], (uintptr_t) lpFunction + 5 );

	bRet = VirtualProtect( m_pTempCode, sizeof(m_pTempCode), PAGE_EXECUTE_READWRITE, &dwOldProtect );   
	if( FALSE == bRet )
		return FALSE;

	m_pOriginFunction = (LPVOID) m_pTempCode;

	bRet = VirtualProtect( lpCode, nLen, PAGE_EXECUTE_READWRITE, &dwOldProtect );   
	if( FALSE == bRet )
		return FALSE;

	//修改lpFunction前?字节为jmp xxxx
	memset( lpCode, 0x90, nCodelen );
	lpCode[0] = 0xe9; //jmp
	FILL_JMP( lpCode, lpCode, lpNewFunction );

	// jmp far 立即数:   跳转到偏移当前的某个位置 
	// 跳转的地址 =下一条指令地址（Eip ）+ 立即数（偏移位置） = 当前指令地址（ EIP） + 立即数+ 指令字节数(1 + 4(地址字节数 ) = 5)
	// *LPDWORD((DWORD)lpCode + 1) = ((DWORD)lpNewFunction - (DWORD)lpCode - 5);

	bRet = VirtualProtect( lpCode, nLen, dwOldProtect, &dwOldProtect );   
	if( FALSE == bRet )
		return FALSE;

	return TRUE;
}

DWORD CXHookAPI::CallFunction(DWORD dwArguCount,...)
{
	DWORD	dwOriginFunction = (DWORD) (DWORD_PTR) m_pOriginFunction;
	DWORD	dwRetValue;
	__asm
	{
		//保存寄存器
		pushad
		pushfd
		
		lea ebx,dwArguCount	//edx=第一个参数的地址
		mov eax,dwOriginFunction//eax=函数地址

		//保存栈
		push ebp
		mov ebp,esp
		
		mov ecx,[ebx]	//ecx=dwArguCount
LOOPPUSH:
		cmp ecx,0
		jz EX

		push [ebx+4*ecx];
		dec ecx
		jmp LOOPPUSH
EX:
		call eax

		//恢复栈
		mov esp,ebp
		pop ebp
		mov dwRetValue,eax
		//恢复寄存器
		popfd
		popad
	}
	return dwRetValue;
}

DWORD CXHookAPI::ThisCallFunction( LPVOID lpThisPointer, DWORD dwArguCount, ... )
{
	DWORD	dwOriginFunction = (DWORD) (DWORD_PTR) m_pOriginFunction;
	DWORD	dwRetValue;
	__asm
	{
		//保存寄存器
		pushad
		pushfd
		
		lea ebx,dwArguCount	//edx=第一个参数的地址
		mov eax,dwOriginFunction//eax=函数地址
		mov edx,lpThisPointer

		//保存栈
		push ebp
		mov ebp,esp
		
		mov ecx,[ebx]	//ecx=dwArguCount
LOOPPUSH:
		cmp ecx,0
		jz EX

		push [ebx+4*ecx];
		dec ecx
		jmp LOOPPUSH
EX:
		mov ecx,edx
		call eax

		//恢复栈
		mov esp,ebp
		pop ebp
		mov dwRetValue,eax
		//恢复寄存器
		popfd
		popad
	}
	return 0;
}

BOOL CXHookAPI::WriteMemory( LPVOID lpAddress, LPVOID lpData, int nSize )
{
	BOOL	bRet;
	DWORD	dwOldProtect;

	bRet = VirtualProtect( lpAddress, nSize, PAGE_EXECUTE_READWRITE, &dwOldProtect );   
	if( FALSE == bRet )
		return FALSE;

	CopyMemory( lpAddress, lpData, nSize );

	bRet = VirtualProtect( lpAddress, nSize, dwOldProtect, &dwOldProtect );   
	if( FALSE == bRet )
		return FALSE;

	return TRUE;
}

BOOL CXHookAPI::UnHook()
{
	if( 0 == m_HookedAddr || 0 == m_dwOrginCodeLen )
		return FALSE;

	if( IsBadReadPtr( m_HookedAddr,m_dwOrginCodeLen ) ) 
		return FALSE;

	return WriteMemory( m_HookedAddr, m_pOriginCode, m_dwOrginCodeLen );
}

__declspec(naked) VOID _CallFunction()
{
	
	__asm
	{
		push ebp
		mov ebp,esp
		//[ebp+4]=返回地址
		//[ebp+8]函数地址
		//[ebp+8+4]第一个参数

		//保存栈及寄存器
		push 0			//开个空间来保存esp值[ebp-4]
		
		pushfd
		mov [ebp-4],esp//[ebp-4]保存着当前esp地址

		mov eax,[ebp+0x0c]
LOOPPUSH:
		
		push [ebp+0x0c+4*eax]
		dec eax
		jnz LOOPPUSH

		call [ebp+8]
		
		//恢复栈及寄存器
		mov esp,[ebp-4]//[ebp-4]保存着当前esp地址
		popfd
		
		mov esp,ebp
		pop ebp
		retn
	}
}

__declspec(naked) VOID _ThisCallFunction()
{
	__asm
	{
		push ebp
		mov ebp,esp
		//[ebp+4]=this指针
		//[ebp+4+4]=函数地址
		//[ebp+8+4]函数地址,
		//[ebp+8+4+4]第一个参数,

		//保存栈及寄存器
		push 0			//开个空间来保存esp值[ebp-4]
		push ecx
		pushfd
		mov [ebp-4],esp//[ebp-4]保存着当前esp地址

		mov eax,[ebp+0x10]
LOOPPUSH:
		
		push [ebp+0x10+4*eax]
		dec eax
		jnz LOOPPUSH
	
		mov ecx,[ebp+8]
		call [ebp+0x0c]

		//恢复栈及寄存器
		mov esp,[ebp-4]//[ebp-4]保存着当前esp地址
		popfd
		pop ecx
		
		mov esp,ebp
		pop ebp
		retn
	}
}

static size_t __stdcall GetOpCodeSize(BYTE* iptr0)
{
	BYTE* iptr = iptr0;

	DWORD f = 0;

prefix:
	BYTE b = *iptr++;

	f |= table_1[b];

	if ( f & C_FUCKINGTEST )
		if (((*iptr)&0x38)==0x00)   // ttt
			f=C_MODRM+C_DATAW0;       // TEST
		else
			f=C_MODRM;                // NOT,NEG,MUL,IMUL,DIV,IDIV

	if ( f & C_TABLE_0F )
	{
		b = *iptr++;
		f = table_0F[b];
	}

	if ( f == C_ERROR )
	{
		//printf("error in %02X\n",b);
		return C_ERROR;
	}

	if  ( f & C_PREFIX )
	{
		f &= ~C_PREFIX;
		goto prefix;
	}

	if (f&C_DATAW0) if (b&0x01) f|=C_DATA66; else f|=C_DATA1;

	if (f&C_MODRM)
	{
		b = *iptr++;
		BYTE mod = b & 0xC0;
		BYTE rm  = b & 0x07;
		if (mod!=0xC0)
		{
			if (f&C_67)         // modrm16
			{
				if ((mod==0x00)&&(rm==0x06)) f|=C_MEM2;
				if (mod==0x40) f|=C_MEM1;
				if (mod==0x80) f|=C_MEM2;
			}
			else                // modrm32
			{
				if (mod==0x40) f|=C_MEM1;
				if (mod==0x80) f|=C_MEM4;
				if (rm==0x04) rm = (*iptr++) & 0x07;    // rm<-sib.base
				if ((rm==0x05)&&(mod==0x00)) f|=C_MEM4;
			}
		}
	} // C_MODRM

	if (f&C_MEM67)  if (f&C_67) f|=C_MEM2;  else f|=C_MEM4;
	if (f&C_DATA66) if (f&C_66) f|=C_DATA2; else f|=C_DATA4;

	if (f&C_MEM1)  iptr++;
	if (f&C_MEM2)  iptr+=2;
	if (f&C_MEM4)  iptr+=4;

	if (f&C_DATA1) iptr++;
	if (f&C_DATA2) iptr+=2;
	if (f&C_DATA4) iptr+=4;

	return (size_t)(iptr - iptr0);
}


static int CopyCodeToBuffer( LPVOID lpCode, LPVOID lpBuffer, int* pnCodeLen )
{
	DWORD	dwCodeLen		= 0;
	LPBYTE	pCode			= (LPBYTE) lpCode;
	LPBYTE	pBuffer			= (LPBYTE) lpBuffer;
	DWORD	dwCodeOffset	= 0;
	DWORD	dwBuffOffset	= 0;

	while( dwCodeOffset < 5 )
	{
		dwCodeLen = (DWORD) GetOpCodeSize( &pCode[dwCodeOffset] );
		CopyMemory( &pBuffer[dwBuffOffset], &pCode[dwCodeOffset], dwCodeLen );

		if( 0xEB == pBuffer[dwBuffOffset] ) //jmp short
		{
			pBuffer[dwBuffOffset] = 0xE9;
			FILL_JMP( &pBuffer[dwBuffOffset], &pBuffer[dwBuffOffset], MAKE_JMPSHORT_ADDRESS( &pCode[dwCodeOffset] ) );

			dwCodeOffset	+= dwCodeLen;	
			dwCodeLen		= 5;
			dwBuffOffset	+= dwCodeLen;
			continue;
		}
		else if( 0xe9 == pBuffer[dwBuffOffset] )//jmp far
		{
			FILL_JMP( &pBuffer[dwBuffOffset], &pBuffer[dwBuffOffset], MAKE_JMP_ADDRESS( &pCode[dwCodeOffset] ) );
		}
		else if( 0xe8 == pBuffer[dwBuffOffset] )//call far
		{
			FILL_JMP( &pBuffer[dwBuffOffset], &pBuffer[dwBuffOffset], MAKE_JMP_ADDRESS( &pCode[dwCodeOffset] ) );
		}

		dwBuffOffset += dwCodeLen;
		dwCodeOffset += dwCodeLen;
	}

	if(pnCodeLen)
		*pnCodeLen = dwCodeOffset;

	return dwBuffOffset;
}

static PIMAGE_IMPORT_DESCRIPTOR GetNamedImportDescriptor( HMODULE hModule, LPCSTR lplpszImportModuleule )   
{   

	if ( ( lplpszImportModuleule == NULL ) || ( hModule == NULL ) ) 
		return NULL;  

	// Get the Dos header.    
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER) hModule;   

	// Is this the MZ header?    
	if ( IsBadReadPtr( pDOSHeader, sizeof(IMAGE_DOS_HEADER) ) || ( pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE ) ) 
		return NULL;     

	// Get the PE header.    
	PIMAGE_NT_HEADERS pNTHeader = MakePtr( PIMAGE_NT_HEADERS, pDOSHeader, pDOSHeader->e_lfanew );   

	// Is this a real PE image?    
	if ( IsBadReadPtr( pNTHeader, sizeof(IMAGE_NT_HEADERS) ) || ( pNTHeader->Signature != IMAGE_NT_SIGNATURE ) )  
		return NULL;   


	// If there is no imports section, leave now.    
	if ( pNTHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress == 0 )
		return NULL;    


	// Get the pointer to the imports section.    
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = MakePtr( PIMAGE_IMPORT_DESCRIPTOR, pDOSHeader, pNTHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );   
	while (pImportDesc->Name)   
	{   
		PSTR szCurrMod = MakePtr( PSTR, pDOSHeader, pImportDesc->Name );   

		if ( lstrcmpiA( szCurrMod, lplpszImportModuleule ) == 0 )
			break; // Found it.      

		// Look at the next one.    
		pImportDesc++;   
	}   
	// If the name is NULL, then the module is not imported.    
	if ( pImportDesc->Name == NULL )
		return NULL;   

	// All OK, Jumpmaster!    
	return pImportDesc;   
}

}