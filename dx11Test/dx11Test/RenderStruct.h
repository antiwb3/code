#ifndef _RENDER_STRUCT_H
#define _RENDER_STRUCT_H

#include <d3d11.h>

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};

struct ConstantBuffer
{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
};


enum RENDER_COORDINATES_TYPE
{
    rct2D,
    rct3D,
};

struct CBNeverChanges
{
    XMMATRIX mView;
};

struct CBChangeOnResize
{
    XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    //XMFLOAT4 vMeshColor;
};

#endif // ! _RENDER_STRUCT_H