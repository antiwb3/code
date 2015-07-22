#ifndef _PAINT_ENGINE_H
#define _PAINT_ENGINE_H

#include <d3d11.h>
#include "ShaderTable.h"

struct DxPaintDevice
{
    ID3D11Buffer*               VertexBuffer; // The vertex buffer, containing the quad mesh data.
    ID3D11Buffer*               IndexBuffer; // The vertex buffer, containing the quad mesh data.
    ID3D11InputLayout*          VertexLayout;    // The vertex format layout.
    ID3D11VertexShader*         VertexShader;    // The vertex shader.
    ID3D11PixelShader*          PixelShader;      // The fragment (pixel) shader.
    ID3D11Buffer*               VShaderUniforms;  // The buffer containing the uniforms (shader constants), used with the vertex shader.
   // ID3D11ShaderResourceView*   ShaderResView;    // The shader resource view of the rendered texture
};


struct RenderData 
{
    float minx;
    float miny;
    float maxx;
    float maxy;

    float uvminx;
    float uvminy;
    float uvmaxx;
    float uvmaxy;

    RenderData* nextData;
};

struct RenderImageData : public RenderData
{
    float z;
    ID3D11ShaderResourceView *iTextureView;
    XMCOLOR Color;
};

class ImageRenderer
{
public:
    ImageRenderer();
    ~ImageRenderer();

    HRESULT Init(HWND hwnd);
    HRESULT Uninit();

    HRESULT Render(RenderDevice& renderDevice, RenderImageData* data, uint count);
private:
    HRESULT UpdateBuffer(uint imageCount);
    HRESULT CreateIndexBuffer(uint imageCount, ID3D11Buffer** indexBufferOut);
    HRESULT CreateConstantBuffer();
    HRESULT FillVertexBuffer(RenderImageData* data, uint count);
private:
    uint            m_maxImageCount;
    ID3D11Buffer*   m_vertexBuffer;
    ID3D11Buffer*   m_indexBuffer;
    ID3D11Buffer*   m_coorConstantBuffer;
    ID3D11Buffer*   m_CBNeverChanges;
    ID3D11Buffer*   m_CBChangeOnResize;
    ID3D11Buffer*   m_CBChangesEveryFrame;


    ID3D11SamplerState* m_samplerStateLinear;
    ID3D11BlendState* m_blendState;
    HWND            m_hwnd;
};

class PaintEngine
{
public:
    struct Vertex
    {
        float x, y, z;
        float tu, tv;
    };

public:
    static PaintEngine& Instance();

public:
    PaintEngine();
    ~PaintEngine();

    int Init(HWND hwnd);
    void Uninit();

public:
    HWND          m_hwnd;
    ShaderTable   m_shaderTable;
    ImageRenderer m_imageRender;
};

#endif // ! _PAINT_ENGINE_H