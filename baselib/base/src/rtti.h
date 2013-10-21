#ifndef WIDGET_PARSER_DEFINE_H_
#define WIDGET_PARSER_DEFINE_H_

#include "parsexmldefine.h"
#include "windowtype.h"

class ObjectParam{};

typedef ObjectParam *(*FunCreate)();
class RTTI
{
public:
    RTTI(
        int nClassID, 
        FunCreate fnCreate,
    );
public:
    int m_nClassID;
    FunCreate m_fnCreate;
};

template<class T, class Base, int nClassID>
class SupportRTTI : public Base
{
public:
    SupportRTTI();
    virtual ~SupportRTTI();
    virtual RTTI *GetRTTI();
    
    static T* Create();
private:
    static RTTI m_sRTTI;
};

template<class T, class Base, int nClassID>
SupportRTTI<T, Base, nClassID>::SupportRTTI()
{

}

template<class T, class Base, int nClassID>
SupportRTTI<T, Base, nClassID>::~SupportRTTI()
{

}

template<class T, class Base, int nClassID>
RTTI SupportRTTI<T, Base, nClassID>::m_sRTTI(
    nClassID, 
    (FunCreate)T::Create, 
);

template<class T, class Base, int nClassID>
RTTI *SupportRTTI<T, Base, nClassID>::GetRTTI()
{
    return &m_sRTTI;
}

int g_RegisterRTTI(int nClassID, RTTI *pRTTI);
const RTTI *g_GetRTTI(int nClassID);

#endif //WIDGET_PARSER_DEFINE_H_
