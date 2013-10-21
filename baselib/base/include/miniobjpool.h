#ifndef MINI_OBJ_POOL_H
#define MINI_OBJ_POOL_H

#ifndef ENGINE_API
	#define ENGINE_API
#endif

class ENGINE_API MiniObjPool
{
public:
	MiniObjPool();
	MiniObjPool(const char* cpszName, size_t nObjSize, size_t nChunkObjCount = 32);
	~MiniObjPool();

	int		Init(size_t nObjSize, size_t nChunkObjCount = 32);

	void*	Alloc();
	int		Free(void* pValue);

	void	Purge();

private:
	const char*		m_pszName;

    struct MemChunk*	m_pChunkList;
    struct MemObject*  m_pFreeObjList;

    size_t			m_nChunkObjCount;
	size_t			m_nObjSize;
};

#endif // MINI_OBJ_POOL_H