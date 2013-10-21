//---------------------------------------------------------------------------
// File:	objectpool.h
// Date:	2013.04.14
// Code:	Wang Bin
// Desc:
// modify:  Wang Bin
//---------------------------------------------------------------------------

#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include <cassert>
#include <malloc.h>
#include <new.h>

#ifndef ENGINE_API
	#define ENGINE_API
#endif

struct MemChunk
{
	MemChunk* pNext;
	char pData[1];
};

struct MemObject
{
	union
	{
		MemObject* pNext;
		void* pData;
	};
};

enum OBJECT_ALLOC_TYPE
{
	oatMallocFree = 0,
	oatNewDelete,

#if (OBJECT_POOL_RELEASE_ENABLE)
	oatNewRelease,
#endif

};

template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType = oatNewDelete>
class ObjectPool
{
public:
	ObjectPool();
	ObjectPool(size_t nChunkObjCount, const char* cpszName = NULL);
	
	~ObjectPool();
	
	int			 Init(size_t nChunkObjCount, const char* cpszName = NULL);
	void		 Purge();

	OBJECT_TYPE* New();
	void		 Delete(void* pValue);

	
private:
	enum OBJECT_ALLOC_TYPE	m_eAllocType;

	struct MemChunk*		m_pChunkList;
	struct MemObject*		m_pFreeObjList;
	size_t				m_nChunkObjCount;
};

template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType>
ObjectPool<OBJECT_TYPE, eAllocType>::ObjectPool()
: m_eAllocType(eAllocType)
, m_pChunkList(NULL)
, m_pFreeObjList(NULL)
, m_nChunkObjCount(0)
{

}


template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType>
ObjectPool<OBJECT_TYPE, eAllocType>::ObjectPool(size_t nChunkObjCount, const char* cpszName = NULL)
: m_eAllocType(eAllocType)
, m_pChunkList(NULL)
, m_pFreeObjList(NULL)
, m_nChunkObjCount(nChunkObjCount)
{

}

template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType>
ObjectPool<OBJECT_TYPE, eAllocType>::~ObjectPool()
{
	size_t uAllocObjCount = 0;
	size_t uFreeObjCount  = 0;

	for (MemChunk* pChunk = m_pChunkList; pChunk; pChunk = pChunk->pNext)
	{
		uAllocObjCount += m_nChunkObjCount;
	}

	for (MemObject* pObject = m_pFreeObjList; pObject; pObject = pObject->pNext)
	{
		++uFreeObjCount;
	}

	if (uAllocObjCount > uFreeObjCount)
	{
		//printf("[ObjectPool] Resource Leak %d %s\n", nAllocObjectCount - nFreeObjectCount, m_szName);
	}

	Purge();
}

template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType>
int ObjectPool<OBJECT_TYPE, eAllocType>::Init(size_t nChunkObjCount, const char* cpszName = NULL)
{
	ASSERT(m_pFreeObjList == NULL);
	ASSERT(m_pChunkList == NULL);

	m_eAllocType		= eAllocType;
	m_nChunkObjCount	= nChunkObjCount;

	return true;
}

template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType>
void ObjectPool<OBJECT_TYPE, eAllocType>::Purge()
{
	while (m_pChunkList)
	{
		MemChunk* pChunk = m_pChunkList;
		m_pChunkList	 = m_pChunkList->pNext;
		free(pChunk);
	}

	m_pChunkList = NULL;
	m_pFreeObjList = NULL;
}

template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType>
OBJECT_TYPE* ObjectPool<OBJECT_TYPE, eAllocType>::New()
{
	MemObject*		pObject = NULL;
	OBJECT_TYPE*	pResult = NULL;

	if (m_pFreeObjList == NULL) 
	{
		assert(m_nChunkObjCount > 0);

		MemChunk* pChunk = (MemChunk*)(malloc(sizeof(MemChunk*) + m_nChunkObjCount * sizeof(OBJECT_TYPE)));
		if (pChunk == NULL)
		{
			// printf("[KSmallObjectPool] alloc failed %d %d %s\n", m_nChunkObjectCount, m_nObjectSize, m_szName);
			return NULL;
		}

		pChunk->pNext = m_pChunkList;
		m_pChunkList = pChunk;  

		for (size_t nIndex = 0; nIndex < m_nChunkObjCount; ++nIndex)
		{
			pObject = (MemObject*)(m_pChunkList->pData + nIndex * sizeof(OBJECT_TYPE));
			pObject->pNext = m_pFreeObjList;
			m_pFreeObjList = pObject;
		}
	}

	pObject = m_pFreeObjList;

	if (m_pFreeObjList)
		m_pFreeObjList = m_pFreeObjList->pNext;

	if (m_eAllocType == oatNewDelete)
		pResult = new(pObject) OBJECT_TYPE;
	#if (OBJECT_POOL_RELEASE_ENABLE)
	else if (m_eAllocType == oatNewRelease)
		pResult = new(pObject) OBJECT_TYPE;
	#endif
	else
		pResult = (OBJECT_TYPE *)pObject;

	return pResult;
}

template<class OBJECT_TYPE, enum OBJECT_ALLOC_TYPE eAllocType>
void ObjectPool<OBJECT_TYPE, eAllocType>::Delete(void* pValue)
{
	MemObject*		pObject  = NULL;
	OBJECT_TYPE*	pFreeObj = NULL;

	if (!pValue)
		goto Exit0;

	pObject  = (MemObject *)pValue;
	pFreeObj = (OBJECT_TYPE *)pValue;
	switch (m_eAllocType)
	{
	case oatMallocFree:
		break;
	case oatNewDelete:
		pFreeObj->~OBJECT_TYPE();
		break;
	#if (OBJECT_POOL_RELEASE_ENABLE)
	case oatNewRelease:
		pFreeObj->Release();
		break;
	#endif

	default:
		assert(true);
	}

	pObject->pNext = m_pFreeObjList;
	m_pFreeObjList = pObject;

Exit0:
	return ;
}

#endif // OBJECT_POOL_H_