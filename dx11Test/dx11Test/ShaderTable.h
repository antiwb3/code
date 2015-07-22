#ifndef _SHADER_TABLE_H
#define _SHADER_TABLE_H

#include <D3DX11.h>

enum VERTEX_SHADER
{
    VERTEX_SHADER_IMAGE,
    VERTEX_SHADER_COLOR,
    VERTEX_SHADER_COUNT
};

enum PIXEL_SHADER
{
    PIXEL_SHADER_IMAGE,
    PIXEL_SHADER_COLOR,
    PIXEL_SHADER_COUNT
};

enum LAYOUT_SHADER
{
    LAYOUT_SHADER_IMAGE,
    LAYOUT_SHADER_COLOR,
    LAYOUT_SHADER_COUNT
};

struct RenderDevice
{
    uint                ViewPortW;
    uint                ViewPortH;
    ID3D11VertexShader* VertexShader;
    ID3D11PixelShader*  PixelShader;
    ID3D11InputLayout*  InputLayout;
};

class ShaderTable
{
public:
    ShaderTable();
    ~ShaderTable();

    HRESULT Load();
    void Unload();

    ID3D11VertexShader* GetVertexShader(VERTEX_SHADER etype);
    ID3D11PixelShader*  GetPixelShader(PIXEL_SHADER etype);
    ID3D11InputLayout*  GetLayoutShader(LAYOUT_SHADER etype);

public:
    ID3D11VertexShader* m_vertex[VERTEX_SHADER_COUNT];
    ID3D11PixelShader*  m_pixel[PIXEL_SHADER_COUNT];
    ID3D11InputLayout*  m_layout[LAYOUT_SHADER_COUNT];
};


#endif // ! _SHADER_TABLE_H