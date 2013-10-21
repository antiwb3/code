//---------------------------------------------------------------------------
// File:	fileassistor.h
// Date:	2013.04.14
// Code:	Wang Bin
// Desc:
// modify: 
//---------------------------------------------------------------------------

#ifndef PROJ_CONF_H_
#define PROJ_CONF_H_

#if defined(ASSISTOR_DLL)

#if defined(ASSISTOR_EXPORTS) || defined(ASSISTOR_LIB)

#define ENGINE_API		__declspec(dllexport)
#define ENGINE_API	__declspec(dllexport)


#else
#define ENGINE_API		__declspec(dllimport)
#define ENGINE_API	__declspec(dllimport)

#endif

#else

#define ENGINE_API		

#define ENGINE_API	extern

#endif

#define OBJECT_POOL_RELEASE_ENABLE 0

#endif // PROJ_CONF_H_