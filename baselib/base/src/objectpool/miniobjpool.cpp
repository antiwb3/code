#include "stdafx.h"
#include <malloc.h>
#include "./miniobjpool.h"

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

MiniObjPool::MiniObjPool()
: m_pszName("")
, m_pChunkList(NULL)
, m_pFreeObjList(NULL)
, m_nChunkObjCount(0)
, m_nObjSize(0)
{
}

MiniObjPool::MiniObjPool(const char* cpszName, size_t nObjSize, size_t nChunkObjCount)
: m_pszName(cpszName ? cpszName : "")
, m_pChunkList(NULL)
, m_pFreeObjList(NULL)
, m_nChunkObjCount(nChunkObjCount)
, m_nObjSize(nObjSize)
{
}

MiniObjPool::~MiniObjPool()
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
		//printf("[KSmallObjectPool] Resource Leak %d %s\n", nAllocObjectCount - nFreeObjectCount, m_szName);
	}

	Purge();
}

int MiniObjPool::Init(size_t nObjSize, size_t nChunkObjCount)
{
    ASSERT(m_pFreeObjList == NULL);
    ASSERT(m_pChunkList == NULL);
    ASSERT(nObjSize >= sizeof(MemObject*));

    m_nObjSize			= nObjSize;
    m_nChunkObjCount	= nChunkObjCount;

    return true;
}

void* MiniObjPool::Alloc()
{
    MemObject* pObject = NULL;

    if (m_pFreeObjList == NULL) 
    {
        ASSERT(m_nObjSize > 0);
        ASSERT(m_nChunkObjCount > 0);

        MemChunk* pChunk = (MemChunk*)(malloc(sizeof(MemChunk*) + m_nChunkObjCount * m_nObjSize));
        if (pChunk == NULL)
        {
           // printf("[KSmallObjectPool] alloc failed %d %d %s\n", m_nChunkObjectCount, m_nObjectSize, m_szName);
            return NULL;
        }

        pChunk->pNext = m_pChunkList;
        m_pChunkList = pChunk;  

        for (size_t nIndex = 0; nIndex < m_nChunkObjCount; ++nIndex)
        {
            pObject = (MemObject*)(m_pChunkList->pData + nIndex * m_nObjSize);
            pObject->pNext = m_pFreeObjList;
            m_pFreeObjList = pObject;
        }
    }

    pObject = m_pFreeObjList;

    if (m_pFreeObjList)
        m_pFreeObjList = m_pFreeObjList->pNext;

    return (void*)pObject;
}

int MiniObjPool::Free(void* pValue)
{
    int nResult = false;
    MemObject* pObject = NULL;

    if (!pValue)
		goto Exit0;


    pObject = (MemObject*)pValue;
    
    pObject->pNext = m_pFreeObjList;
    m_pFreeObjList = pObject;

    nResult = true;
Exit0:
    return nResult;
}

void MiniObjPool::Purge()
{
    while (m_pChunkList)
    {
        MemChunk* pChunk = m_pChunkList;
        m_pChunkList = m_pChunkList->pNext;
        free(pChunk);
    }

    m_pChunkList = NULL;
    m_pFreeObjList = NULL;
}
