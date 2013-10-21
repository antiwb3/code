#pragma once

namespace XLIB
{


class CStack_ESP
{
public:
	//����������������hook���Ǻ���ͷʱ������Ч
	DWORD	GetReturnAddress();//ȡ���������ص�ַ
	DWORD	GetArgument( DWORD index );//base 0 ȡ������
	LPVOID*	GetArguList();//���ص�һ�������ĵ�ַ
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
	DWORD	ThisCallFunction( LPVOID lpThisPointer, DWORD dwArguCount, ... );	//thisָ��ʹ��ecx����

	BOOL	HookVirtualFunction( LPVOID lpThisPointer, int vIndex, LPVOID lpNewFun, BOOL bCanUnHook = FALSE);	//hookһ���麯��,lpThisPointerΪC++�����ָ��,vIndexΪ�麯������,��0��ʼ

	BOOL	HookImportTable( HMODULE hModule, LPCSTR lpszImportModule, LPCSTR lpszFunctionName, LPVOID lpNewFunction );

	BOOL	HookExportTableByID( HMODULE hModule, DWORD dwNameOrdinals, LPVOID lpNewFunction );
	BOOL	HookExportTable( HMODULE hModule, LPCSTR lpszFunctionName, LPVOID lpNewFunction );
	
	BOOL	InlineHookFunction( LPVOID lpFunction,LPVOID lpNewFunction );
	BOOL	InlineHookAddress( LPVOID lpAddress, FILTERROUTINE pRoutine );	//hook һ����ַ,��װһ��������
	LPVOID	GetOriginFunction();
	BOOL	UnHook();

	static LPVOID	GetAddressByNameA( LPCSTR lpszModule, LPCSTR lpszFunctionName );
	static BOOL		WriteMemory( LPVOID lpAddress, LPVOID lpData, int nSize );

private:
	LPVOID		m_pOriginFunction;
	BYTE		m_pTempCode[30];

	LPVOID		m_HookedAddr;		//��hook�ĵ�ַ
	BYTE		m_pOriginCode[20];	//ԭʼ����
	DWORD		m_dwOrginCodeLen;
};

}