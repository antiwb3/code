#include "stdafx.h"
#include <assert.h>
#include "rtti.h"

#define RTTI_COUNT 20
static RTTI *g_pRTTIMgr[RTTI_COUNT];

int g_RegisterRTTI(int nClassID, RTTI *pRTTI)
{
    int nResult = false;

    assert(nClassID >= 0);
    assert(nClassID < RTTI_COUNT);

    g_pRTTIMgr[nClassID] = pRTTI;

    nResult = true;
Exit_Error:
    return nResult;
}

const RTTI *g_GetRTTI(int nClassID)
{
    assert(nClassID >= 0);
    assert(nClassID < RTTI_COUNT);

Exit_Error:
    return g_pRTTIMgr[nClassID];
}

RTTI::RTTI(
    int nClassID, 
    FunCreate fnCreate, 
)
: m_nClassID(nClassID)
, m_fnCreate(fnCreate)
{
    g_RegisterRTTI(nClassID, this);
}
