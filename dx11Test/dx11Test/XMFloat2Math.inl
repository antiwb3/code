#include <xnamath.h>

inline void XMFLOAT2_SUB(XMFLOAT2 const& a, XMFLOAT2 const& b, XMFLOAT2& value)
{
    XMStoreFloat2 (&value,  XMLoadFloat2(&a) - XMLoadFloat2(&b) );
}

inline void XMFLOAT2_Add(XMFLOAT2 const& a, XMFLOAT2 const& b, XMFLOAT2& value)
{
    XMStoreFloat2 (&value,  XMLoadFloat2(&a) + XMLoadFloat2(&b) );
}

inline void XMFLOAT2_Multi(XMFLOAT2 const& a, XMFLOAT2 const& b, XMFLOAT2& value)
{
    XMStoreFloat2 (&value,  XMLoadFloat2(&a) * XMLoadFloat2(&b) );
}

inline void XMFLOAT2_Multi(XMFLOAT2 const& a, float b, XMFLOAT2& value)
{
    XMStoreFloat2 (&value,  XMLoadFloat2(&a) * b );
}


inline float XMFLOAT2_Dot(XMFLOAT2 const& a, XMFLOAT2 const& b)
{
    return XMVectorGetX ( XMVector2Dot(XMLoadFloat2(&a), XMLoadFloat2(&b)) );
}

inline float XMFLOAT2_Cross(XMFLOAT2 const& a, XMFLOAT2 const& b)
{
    return XMVectorGetX ( XMVector2Cross(XMLoadFloat2(&a), XMLoadFloat2(&b)) );
}

inline float XMFLOAT2_Length(XMFLOAT2 const& a)
{
    return XMVectorGetX( XMVector2Length( XMLoadFloat2(&a) ) );
}

inline float XMFLOAT2_LengthSq(XMFLOAT2 const& a)
{
    return XMVectorGetX( XMVector2LengthSq( XMLoadFloat2(&a) ) );
}

inline void XMFLOAT2_Normalize(XMFLOAT2 const& a, XMFLOAT2& value)
{
    XMStoreFloat2 ( &value,  XMVector2Normalize( XMLoadFloat2(&a) ) );
}
