/******************************************************************************
Module:  APIHook.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/

#include "stdafx.h"
#include "CmnHdr.h"
#include <ImageHlp.h>
#pragma comment(lib, "ImageHlp")

#include "APIHook.h"
#include "Toolhelp.h"
#include <StrSafe.h>
#include <shlwapi.h>

typedef HMODULE (*FN_GETMODULEHANDLEA)(LPCSTR lpModuleName);

/////////////////////////////////////////////////////////////////////////////////


// The head of the linked-list of CAPIHook objects
CAPIHook* CAPIHook::sm_pHead = NULL;

// By default, the module containing the CAPIHook() is not hooked
BOOL CAPIHook::ExcludeAPIHookMod = TRUE; 


///////////////////////////////////////////////////////////////////////////////

// NOTE: This function must NOT be inlined

CAPIHook::CAPIHook(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook) 
{
    m_pNext  = sm_pHead;    // The next node was at the head
    sm_pHead = this;        // This node is now at the head

    // Save information about this hooked function
    m_pszCalleeModName   = pszCalleeModName;
    m_pszFuncName        = pszFuncName;
    m_pfnHook            = pfnHook;

    DWORD pid = GetCurrentProcessId();

    HMODULE hMod = GetModuleHandleRaw(pszCalleeModName);
    m_pfnOrig = GetProcAddressRaw(GetModuleHandleRaw(pszCalleeModName), m_pszFuncName);

    // If function does not exit,... bye bye
    // This happens when the module is not already loaded
    if (m_pfnOrig == NULL)
    {
        wchar_t szPathname[MAX_PATH];
        GetModuleFileNameW(NULL, szPathname, _countof(szPathname));
        wchar_t sz[1024];
        StringCchPrintfW(sz, _countof(sz), 
            TEXT("[%4u - %s] impossible to find %S\r\n"), 
            GetCurrentProcessId(), szPathname, pszFuncName);
        OutputDebugString(sz);
        return;
    }

#ifdef _DEBUG
    // This section was used for debugging sessions when Explorer died as 
    // a folder content was requested
    // 
    //static BOOL s_bFirstTime = TRUE;
    //if (s_bFirstTime)
    //{
    //   s_bFirstTime = FALSE;

    //   wchar_t szPathname[MAX_PATH];
    //   GetModuleFileNameW(NULL, szPathname, _countof(szPathname));
    //   wchar_t* pszExeFile = wcsrchr(szPathname, L'\\') + 1;
    //   OutputDebugStringW(L"Injected in ");
    //   OutputDebugStringW(pszExeFile);
    //   if (_wcsicmp(pszExeFile, L"Explorer.EXE") == 0)
    //   {
    //      DebugBreak();
    //   }
    //   OutputDebugStringW(L"\n   --> ");
    //   StringCchPrintfW(szPathname, _countof(szPathname), L"%S", pszFuncName);
    //   OutputDebugStringW(szPathname);
    //   OutputDebugStringW(L"\n");
    //}
#endif

    // Hook this function in all currently loaded modules
    ReplaceIATInAllMods(m_pszCalleeModName, m_pfnOrig, m_pfnHook);
}


///////////////////////////////////////////////////////////////////////////////


CAPIHook::~CAPIHook() 
{
    // Unhook this function from all modules
    ReplaceIATInAllMods(m_pszCalleeModName, m_pfnHook, m_pfnOrig);

    // Remove this object from the linked list
    CAPIHook* p = sm_pHead; 
    if (p == this) 
    {     // Removing the head node
        sm_pHead = p->m_pNext; 
    } else 
    {
        BOOL bFound = FALSE;
        // Walk list from head and fix pointers
        for (; !bFound && (p->m_pNext != NULL); p = p->m_pNext) 
        {
            if (p->m_pNext == this) 
            { 
                // Make the node that points to us point to our next node
                p->m_pNext = p->m_pNext->m_pNext; 
                bFound = TRUE;
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////


// NOTE: This function must NOT be inlined
FARPROC CAPIHook::GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName) {

    return(::GetProcAddress(hmod, pszProcName));
}
HMODULE WINAPI CAPIHook::GetModuleHandleRaw(PCSTR pszProcName)
{
    return(::GetModuleHandleA(pszProcName));
}
///////////////////////////////////////////////////////////////////////////////


// Returns the HMODULE that contains the specified memory address
static HMODULE ModuleFromAddress(PVOID pv) {

    MEMORY_BASIC_INFORMATION mbi;
    return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0) 
        ? (HMODULE) mbi.AllocationBase : NULL);
}


///////////////////////////////////////////////////////////////////////////////


void CAPIHook::ReplaceIATInAllMods( PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew ) 
{
    HMODULE hmodThisMod = ExcludeAPIHookMod ? ModuleFromAddress( ReplaceIATInAllMods ) : NULL;

    DWORD pid = GetCurrentProcessId();

    // Get the list of modules in this process
    CToolhelp th( TH32CS_SNAPMODULE, GetCurrentProcessId() );

    MODULEENTRY32 me = { sizeof( me ) };
    for ( BOOL bOk = th.ModuleFirst( &me ); bOk; bOk = th.ModuleNext( &me ) ) {

        // NOTE: We don't hook functions in our own module
        if ( me.hModule != hmodThisMod ) {

            // Hook this function in this module
            ReplaceIATInOneMod( pszCalleeModName, pfnCurrent, pfnNew, me.hModule );
        }
    }
}


///////////////////////////////////////////////////////////////////////////////


// Handle unexpected exceptions if the module is unloaded
LONG WINAPI InvalidReadExceptionFilter(PEXCEPTION_POINTERS pep) {

    // handle all unexpected exceptions because we simply don't patch
    // any module in that case
    LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;

    // Note: pep->ExceptionRecord->ExceptionCode has 0xc0000005 as a value

    return(lDisposition);
}


void CAPIHook::ReplaceIATInOneMod(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller) 
{
   // Get the address of the module's import section
   ULONG ulSize = 0;

   // An exception was triggered by Explorer (when browsing the content of 
   // a folder) into imagehlp.dll. It looks like one module was unloaded...
   // Maybe some threading problem: the list of modules from Toolhelp might 
   // not be accurate if FreeLibrary is called during the enumeration.
   PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;
   __try {
       pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ImageDirectoryEntryToData(
           hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize );
   } 
   __except (InvalidReadExceptionFilter(GetExceptionInformation())) {
       // Nothing to do in here, thread continues to run normally
       // with NULL for pImportDesc 
   }

   if (pImportDesc == NULL)
       return;  // This module has no import section or is no longer loaded

   // Find the import descriptor containing references to callee's functions
   for (; pImportDesc->Name; pImportDesc++) 
   {
       PSTR pszModName = (PSTR) ( (PBYTE) hmodCaller + pImportDesc->Name );
       if ( lstrcmpiA( pszModName, pszCalleeModName ) == 0 ) 
       {
           // Get caller's import address table (IAT) for the callee's functions
           PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA) ( (PBYTE) hmodCaller + pImportDesc->FirstThunk );

           // Replace current function address with new function address
           for ( ; pThunk->u1.Function; pThunk++ ) 
           {
               // Get the address of the function address
               PROC* ppfn = (PROC*) &pThunk->u1.Function;

               // Is this the function we're looking for?
               BOOL bFound = ( *ppfn == pfnCurrent );
               if ( bFound ) 
               {
                   if (!WriteProcessMemory( GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL ) && 
                       ( ERROR_NOACCESS == GetLastError() ) ) 
                   {
                       DWORD dwOldProtect;
                       if (VirtualProtect(ppfn, sizeof(pfnNew), PAGE_WRITECOPY, &dwOldProtect)) 
                       {
                           WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL);
                           VirtualProtect(ppfn, sizeof(pfnNew), dwOldProtect, &dwOldProtect);
                       }
                   }
                   return;  // We did it, get out
               }
           }
       }  // Each import section is parsed until the right entry is found and patched
   }
}


///////////////////////////////////////////////////////////////////////////////


void CAPIHook::ReplaceEATInOneMod( HMODULE hmod, PCSTR pszFunctionName, PROC pfnNew ) 
{
   // Get the address of the module's export section
   ULONG ulSize = 0;
   PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;

   __try {
       pExportDir = (PIMAGE_EXPORT_DIRECTORY) ImageDirectoryEntryToData(
           hmod, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &ulSize );
   } 
   __except (InvalidReadExceptionFilter(GetExceptionInformation())) {
       // Nothing to do in here, thread continues to run normally
       // with NULL for pExportDir 
   }

   if (pExportDir == NULL)
       return;

   PDWORD   pdwNamesRvas            = (PDWORD) ((PBYTE) hmod + pExportDir->AddressOfNames);
   PWORD    pdwNameOrdinals         = (PWORD) ((PBYTE) hmod + pExportDir->AddressOfNameOrdinals);
   PDWORD   pdwFunctionAddresses    = (PDWORD) ((PBYTE) hmod + pExportDir->AddressOfFunctions);

   // Walk the array of this module's function names 
   for (DWORD n = 0; n < pExportDir->NumberOfNames; n++) 
   {
       PSTR pszFuncName = (PSTR) ((PBYTE) hmod + pdwNamesRvas[n]);

       if ( lstrcmpiA( pszFuncName, pszFunctionName ) != 0 ) 
           continue;

       // We found the specified function
       // --> Get this function's ordinal value
       WORD ordinal = pdwNameOrdinals[ n ];

       // Get the address of this function's address
       PROC* ppfn = (PROC*) &pdwFunctionAddresses[ ordinal ];

       // Turn the new address into an RVA
       pfnNew = (PROC) ( (PBYTE) pfnNew - (PBYTE) hmod );

       // Replace current function address with new function address
       if ( !WriteProcessMemory( GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL ) && 
           ( ERROR_NOACCESS == GetLastError() ) ) 
       {
           DWORD dwOldProtect;
           if ( VirtualProtect( ppfn, sizeof(pfnNew), PAGE_WRITECOPY, &dwOldProtect ) ) 
           {
               WriteProcessMemory( GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL );
               VirtualProtect( ppfn, sizeof(pfnNew), dwOldProtect, &dwOldProtect );
           }
       }
       break;  // We did it, get out
   }
}


///////////////////////////////////////////////////////////////////////////////
// Hook LoadLibrary functions and GetProcAddress so that hooked functions
// are handled correctly if these functions are called.

//CAPIHook CAPIHook::sm_LoadLibraryA  ("Kernel32.dll", "LoadLibraryA",   
//                                     (PROC) CAPIHook::LoadLibraryA);
//
//CAPIHook CAPIHook::sm_LoadLibraryW  ("Kernel32.dll", "LoadLibraryW",   
//                                     (PROC) CAPIHook::LoadLibraryW);
//
//CAPIHook CAPIHook::sm_LoadLibraryExA("Kernel32.dll", "LoadLibraryExA", 
//                                     (PROC) CAPIHook::LoadLibraryExA);
//
//CAPIHook CAPIHook::sm_LoadLibraryExW("Kernel32.dll", "LoadLibraryExW", 
//                                     (PROC) CAPIHook::LoadLibraryExW);

CAPIHook CAPIHook::sm_GetProcAddress("Kernel32.dll", "GetProcAddress", 
                                     (PROC) CAPIHook::GetProcAddress);

CAPIHook CAPIHook::sm_WritePrivateProfileString("Kernel32.dll", "WritePrivateProfileStringA", 
                                     (PROC) CAPIHook::WritePrivateProfileString);

//CAPIHook CAPIHook::sm_GetModuleHandleA("Kernel32.dll", "GetModuleHandleA", (PROC) CAPIHook::GetModuleHandleA);

///////////////////////////////////////////////////////////////////////////////


void CAPIHook::FixupNewlyLoadedModule( HMODULE hmod, DWORD dwFlags ) {

    if ((hmod != NULL) &&   // Do not hook our own module
        (hmod != ModuleFromAddress(FixupNewlyLoadedModule)) && 
        ((dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0)
        //((dwFlags & LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE) == 0) &&
        //((dwFlags & LOAD_LIBRARY_AS_IMAGE_RESOURCE) == 0)
        ) 
    {
        for (CAPIHook* p = sm_pHead; p != NULL; p = p->m_pNext) {
            if (p->m_pfnOrig != NULL) 
            {
                ReplaceIATInAllMods( p->m_pszCalleeModName, p->m_pfnOrig, p->m_pfnHook );  
            } 
            else 
            {
#ifdef _DEBUG
                wchar_t szPathname[MAX_PATH];
                GetModuleFileNameW(NULL, szPathname, _countof(szPathname));
                wchar_t sz[1024];
                StringCchPrintfW(sz, _countof(sz), 
                    TEXT("[%4u - %s] impossible to find %S\r\n"), 
                    GetCurrentProcessId(), szPathname, p->m_pszCalleeModName);
                OutputDebugString(sz);
#endif
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
typedef BOOL (*fnWritePrivateProfileStringA)(LPCWSTR lpAppName,LPCWSTR lpKeyName,LPCWSTR lpString,LPCWSTR lpFileName);
BOOL WINAPI CAPIHook::WritePrivateProfileString( LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName )
{
    return ((fnWritePrivateProfileStringA)(PROC)(sm_WritePrivateProfileString))(lpAppName, lpKeyName, lpString, lpFileName);
}


HMODULE WINAPI CAPIHook::GetModuleHandleA( LPCSTR lpModuleName)
{
    HMODULE hmod = GetModuleHandleRaw(lpModuleName);
    return(hmod);
}

HMODULE WINAPI CAPIHook::LoadLibraryA(PCSTR pszModulePath) 
{
    DWORD   pid     = GetCurrentProcessId();
    HMODULE hmod    = ::LoadLibraryA(pszModulePath);
    FixupNewlyLoadedModule(hmod, 0);

    return(hmod);
}


///////////////////////////////////////////////////////////////////////////////


HMODULE WINAPI CAPIHook::LoadLibraryW(PCWSTR pszModulePath) 
{
    DWORD   pid     = GetCurrentProcessId();
    HMODULE hmod    = ::LoadLibraryW(pszModulePath);
    FixupNewlyLoadedModule(hmod, 0);

    return (hmod);
}


///////////////////////////////////////////////////////////////////////////////


HMODULE WINAPI CAPIHook::LoadLibraryExA( PCSTR pszModulePath, HANDLE hFile, DWORD dwFlags ) 
{
    DWORD pid = GetCurrentProcessId();

    HMODULE hmod = ::LoadLibraryExA( pszModulePath, hFile, dwFlags );
    FixupNewlyLoadedModule( hmod, dwFlags );
    return (hmod);
}


///////////////////////////////////////////////////////////////////////////////


HMODULE WINAPI CAPIHook::LoadLibraryExW( PCWSTR pszModulePath, HANDLE hFile, DWORD dwFlags ) 
{
    DWORD pid = GetCurrentProcessId();

    HMODULE hmod = ::LoadLibraryExW( pszModulePath, hFile, dwFlags );
    FixupNewlyLoadedModule( hmod, dwFlags );
    return (hmod);
}


///////////////////////////////////////////////////////////////////////////////


FARPROC WINAPI CAPIHook::GetProcAddress( HMODULE hmod, PCSTR pszProcName ) {

    FARPROC pfn = GetProcAddressRaw( hmod, pszProcName );

    // Is it one of the functions that we want hooked?
    CAPIHook* p = sm_pHead;
    for  (; ( pfn != NULL ) && ( p != NULL ); p = p->m_pNext ) 
    {
        if ( pfn == p->m_pfnOrig ) 
        {
            // The address to return matches an address we want to hook
            // Return the hook function address instead
            pfn = p->m_pfnHook;
            break;
        }
    }

    return(pfn);
}


void OutputExportAddress(PCSTR pszCalleeModName, PCSTR filename) 
{
    HMODULE hmod = CAPIHook::GetModuleHandleRaw(pszCalleeModName);
    if (!hmod)
        return;

    ULONG ulSize;

    PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;
    __try {
        pExportDir = (PIMAGE_EXPORT_DIRECTORY) ImageDirectoryEntryToData(
            hmod, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &ulSize);
    } 
    __except (InvalidReadExceptionFilter(GetExceptionInformation())) {
    }

    if (pExportDir == NULL)
        return;  // This module has no export section or is unloaded

    FILE* file = 0;
    file = fopen(filename, "w");

    PDWORD pdwNamesRvas = (PDWORD) ((PBYTE) hmod + pExportDir->AddressOfNames);
    PWORD pdwNameOrdinals = (PWORD) ((PBYTE) hmod + pExportDir->AddressOfNameOrdinals);
    PDWORD pdwFunctionAddresses = (PDWORD) ((PBYTE) hmod + pExportDir->AddressOfFunctions);

    for (DWORD n = 0; n < pExportDir->NumberOfNames; n++) {
        PSTR pszFuncName = (PSTR) ((PBYTE) hmod + pdwNamesRvas[n]);

        WORD ordinal = pdwNameOrdinals[n];

        PROC* ppfn = (PROC*) &pdwFunctionAddresses[ordinal];
        fprintf(file, "fun:%s, address:%x=%x\n", pszFuncName, ppfn, *ppfn);
    }
    if (file)
        fclose(file);
}


void OutputImportAddress(PCSTR pszCalleeModName, PCSTR pszImportModule, PCSTR filename) {

    HMODULE hmodCaller = CAPIHook::GetModuleHandleRaw(pszCalleeModName);
    if (!hmodCaller)
        return;
   // Get the address of the module's import section
   ULONG ulSize;

                                          
   PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;
   __try {
       pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ImageDirectoryEntryToData(
           hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
   } 
   __except (InvalidReadExceptionFilter(GetExceptionInformation())) {
       // Nothing to do in here, thread continues to run normally
       // with NULL for pImportDesc 
   }

   if (pImportDesc == NULL)
       return;  // This module has no import section or is no longer loaded


   FILE* file = 0;
   file = fopen(filename, "w");
   // Find the import descriptor containing references to callee's functions
   for (; pImportDesc->Name; pImportDesc++) {
       PSTR pszModName = (PSTR) ((PBYTE) hmodCaller + pImportDesc->Name);
       if (lstrcmpiA(pszModName, pszImportModule) == 0) {
           PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA) ((PBYTE) hmodCaller + pImportDesc->FirstThunk);

           for (; pThunk->u1.Function; pThunk++) {

               PROC* ppfn = (PROC*) &pThunk->u1.Function;
               fprintf(file, "address:%x=%x\n", ppfn, *ppfn);
           }
           break;
       }
   }
   if (file)
    fclose(file);
}

//////////////////////////////// End of File //////////////////////////////////
