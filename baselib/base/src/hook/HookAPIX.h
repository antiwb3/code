#pragma once

namespace XLIB
{


class CStack_ESP
{
public:
	//以下三个函数，当hook的是函数头时，才有效
	DWORD	GetReturnAddress();//取出函数返回地址
	DWORD	GetArgument( DWORD index );//base 0 取出参数
	LPVOID*	GetArguList();//返回第一个参数的地址
	DWORD	GetEflags();

//pushad
//pushfd
	DWORD	eflags;
	DWORD	EDI;
	DWORD	ESI;
	DWORD	EBP;
	DWORD	ESP;
	DWORD	EBX;
	DWORD	EDX;
	DWORD	ECX;
	DWORD	EAX;

};

typedef VOID (__cdecl *FILTERROUTINE)( CStack_ESP* pStack );

class CXHookAPI  
{
public:
	CXHookAPI();
	~CXHookAPI();

	BOOL	IsHooked();
	DWORD	CallFunction( DWORD dwArguCount, ... );
	DWORD	ThisCallFunction( LPVOID lpThisPointer, DWORD dwArguCount, ... );	//this指针使用ecx传递

	BOOL	HookVirtualFunction( LPVOID lpThisPointer, int vIndex, LPVOID lpNewFun, BOOL bCanUnHook = FALSE);	//hook一个虚函数,lpThisPointer为C++类对象指针,vIndex为虚函数索引,从0开始

	BOOL	HookImportTable( HMODULE hModule, LPCSTR lpszImportModule, LPCSTR lpszFunctionName, LPVOID lpNewFunction );

	BOOL	HookExportTableByID( HMODULE hModule, DWORD dwNameOrdinals, LPVOID lpNewFunction );
	BOOL	HookExportTable( HMODULE hModule, LPCSTR lpszFunctionName, LPVOID lpNewFunction );
	
	BOOL	InlineHookFunction( LPVOID lpFunction,LPVOID lpNewFunction );
	BOOL	InlineHookAddress( LPVOID lpAddress, FILTERROUTINE pRoutine );	//hook 一个地址,安装一个过滤器
	LPVOID	GetOriginFunction();
	BOOL	UnHook();

	static LPVOID	GetAddressByNameA( LPCSTR lpszModule, LPCSTR lpszFunctionName );
	static BOOL		WriteMemory( LPVOID lpAddress, LPVOID lpData, int nSize );

private:
	LPVOID		m_pOriginFunction;
	BYTE		m_pTempCode[30];

	LPVOID		m_HookedAddr;		//被hook的地址
	BYTE		m_pOriginCode[20];	//原始代码
	DWORD		m_dwOrginCodeLen;
};

}