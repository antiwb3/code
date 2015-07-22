#include "stdafx.h"
#include "DxAssist.h"
#include "RenderStruct.h"
#include "PaintEngine.h"

#define VERTEX_PER_IMAGE  4
#define INDEX_PER_IMAGE 6

struct VERTEX_TEXTURE
{
    XMFLOAT3 pos;
    XMFLOAT2 texPos;
   // XMCOLOR color;
};

PaintEngine& PaintEngine::Instance()
{
    static PaintEngine s_inst;
    return s_inst;
}

PaintEngine::PaintEngine()
{

}

PaintEngine::~PaintEngine()
{

}


int PaintEngine::Init(HWND hwnd)

{
	int ret = false;
	int result = false;

    m_hwnd = hwnd;
    m_shaderTable.Load();
    m_imageRender.Init(m_hwnd);

// Exit1:
	result = true;
Exit0:
	return result;
}

void PaintEngine::Uninit()
{
    m_imageRender.Uninit();
    m_shaderTable.Unload();
}

//==== ImageRenderer ====================================================================
XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;

static int s_test2D = true;

static void Init2DCoorConstant(ID3D11Buffer* iCBNeverChanges, ID3D11Buffer* iCBChangeOnResize, ID3D11Buffer* iCBChangesEveryFrame, int width, int height)
{
    CBNeverChanges cbNeverChanges;
    CBChangeOnResize cbChangesOnResize;
    CBChangesEveryFrame cbChangesEveryFrame;

    cbNeverChanges.mView = XMMatrixIdentity();
    cbChangesOnResize.mProjection = XMMatrixOrthographicLH( (FLOAT)width, -(FLOAT)height, 0.0f, 1.0f );;

    cbChangesEveryFrame.mWorld = XMMatrixTranslation(-(FLOAT)width * 0.5f, -(FLOAT)height * 0.5f, 0.0f);
    cbChangesEveryFrame.mWorld = XMMatrixTranspose( cbChangesEveryFrame.mWorld );

    DxAssist::m_deviceContext->UpdateSubresource( iCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0 );
    DxAssist::m_deviceContext->UpdateSubresource( iCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0 );
    DxAssist::m_deviceContext->UpdateSubresource( iCBChangesEveryFrame, 0, NULL, &cbChangesEveryFrame, 0, 0 );
}

static void Init3DCoorConstant(ID3D11Buffer* iCBNeverChanges, ID3D11Buffer* iCBChangeOnResize, ID3D11Buffer* iCBChangesEveryFrame, int width, int height)
{
    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet( 0.0f, 3.0f, -6.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    g_View = XMMatrixLookAtLH( Eye, At, Up );

    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = XMMatrixTranspose( g_View );
    DxAssist::m_deviceContext->UpdateSubresource( iCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0 );

    // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f );

    CBChangeOnResize cbChangesOnResize;
    cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
    DxAssist::m_deviceContext->UpdateSubresource( iCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0 );

    CBChangesEveryFrame cb;
    cb.mWorld = XMMatrixTranspose( g_World );
    DxAssist::m_deviceContext->UpdateSubresource( iCBChangesEveryFrame, 0, NULL, &cb, 0, 0 );
}

ImageRenderer::ImageRenderer()
    : m_maxImageCount(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
    , m_coorConstantBuffer(0)
    , m_samplerStateLinear(0)
{

}

ImageRenderer::~ImageRenderer()
{

}

#define BLEND(enable,srcC,dstC,Cop,srcA,dstA,Aop,mask) \
    { 0, 0, {{enable, D3D11_##srcC, D3D11_##dstC, D3D11_##Cop, D3D11_##srcA, D3D11_##dstA, D3D11_##Aop, mask}} }

HRESULT ImageRenderer::Init(HWND hwnd)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    m_hwnd = hwnd;
    hr = DxAssist::CreateBuffer(0, sizeof(ConstantBuffer), D3D11_BIND_CONSTANT_BUFFER, &m_coorConstantBuffer);
    KGLOG_COM_PROCESS_ERROR(hr);

    {

        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory( &sampDesc, sizeof(sampDesc) );
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = DxAssist::m_device->CreateSamplerState( &sampDesc, &m_samplerStateLinear );
        KGLOG_COM_PROCESS_ERROR(hr);
    }
    
    {
        D3D11_BLEND_DESC blend = BLEND(1, BLEND_ONE, BLEND_INV_SRC_ALPHA, BLEND_OP_ADD,
            BLEND_ONE, BLEND_INV_SRC_ALPHA, BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL);

        DxAssist::m_device->CreateBlendState(&blend, &m_blendState);

    }
    hr = CreateConstantBuffer();
    KGLOG_COM_PROCESS_ERROR(hr);

    {
        RECT rc;
        GetClientRect( m_hwnd, &rc );
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        if (s_test2D)
            Init2DCoorConstant(m_CBNeverChanges, m_CBChangeOnResize, m_CBChangesEveryFrame, width, height);
        else
            Init3DCoorConstant(m_CBNeverChanges, m_CBChangeOnResize, m_CBChangesEveryFrame, width, height);
    }
    result = S_OK;
Exit0:
    return result;
}

HRESULT ImageRenderer::Uninit()
{
    SAFE_RELEASE(m_vertexBuffer);
    SAFE_RELEASE(m_indexBuffer);
    return S_OK;
}

HRESULT ImageRenderer::Render(RenderDevice& renderDevice, RenderImageData* data, uint count)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;
    unsigned uStride = sizeof(VERTEX_TEXTURE);
    unsigned uOffset = 0;

    uint imageCount = 0;
    RenderImageData* imageData = 0;
    RenderImageData* lastImageData = 0;
    ID3D11ShaderResourceView * iResView = 0;
    ID3D11ShaderResourceView * iLastResView = 0;

    // Update our time
    static float t = 0.0f;
    if( /*g_driverType == D3D_DRIVER_TYPE_REFERENCE*/ false )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if( dwTimeStart == 0 )
            dwTimeStart = dwTimeCur;
        t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }

    // Rotate cube around the origin
   /* g_World = XMMatrixRotationY( t );

    CBChangesEveryFrame cb;
    cb.mWorld = XMMatrixTranspose( g_World );
    DxAssist::m_deviceContext->UpdateSubresource( g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0 );*/

    const float one[] = {1,1,1,1};
   // DxAssist::SetUICoordinates(m_coorConstantBuffer, renderDevice.ViewPortW, renderDevice.ViewPortH);
    DxAssist::m_deviceContext->VSSetConstantBuffers( 0, 1, &m_CBNeverChanges );
    DxAssist::m_deviceContext->VSSetConstantBuffers( 1, 1, &m_CBChangeOnResize );
    DxAssist::m_deviceContext->VSSetConstantBuffers( 2, 1, &m_CBChangesEveryFrame );

    //DxAssist::m_deviceContext->OMSetBlendState(m_blendState, one, 0xfffffff);

    DxAssist::m_deviceContext->IASetInputLayout(renderDevice.InputLayout);
    DxAssist::m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &uStride, &uOffset);
    DxAssist::m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    DxAssist::m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    DxAssist::m_deviceContext->VSSetShader(renderDevice.VertexShader, NULL, 0);
    DxAssist::m_deviceContext->PSSetShader(renderDevice.PixelShader, NULL, 0);
    DxAssist::m_deviceContext->PSSetSamplers( 0, 1, &m_samplerStateLinear );

    imageData = data;
    if (imageData)
    {
        imageCount = 1;
        iLastResView = imageData->iTextureView;
        lastImageData = imageData;
    }

    for (int i = 1; imageData && i < count; i++)
    {
        iResView = imageData->iTextureView;
        if (iLastResView != iResView)
        {
            UpdateBuffer(imageCount);

            hr = FillVertexBuffer(lastImageData, imageCount);
            KGLOG_COM_PROCESS_ERROR(hr);

            DxAssist::m_deviceContext->PSSetShaderResources(0, 1, &iLastResView);
            DxAssist::m_deviceContext->DrawIndexed(INDEX_PER_IMAGE * imageCount, 0, 0);
            imageCount = 1;
            lastImageData = imageData;
            iLastResView  = iResView;
        }
        else
            imageCount++;
        imageData = (RenderImageData *)imageData->nextData;
    }

    if (imageCount > 0 && lastImageData)
    {
        UpdateBuffer(imageCount);

        hr = FillVertexBuffer(lastImageData, imageCount);
        KGLOG_COM_PROCESS_ERROR(hr);

        DxAssist::m_deviceContext->PSSetShaderResources(0, 1, &iLastResView);
        DxAssist::m_deviceContext->DrawIndexed(INDEX_PER_IMAGE * imageCount, 0, 0);
    }

    result = S_OK;
Exit0:
    return result;
}

HRESULT ImageRenderer::CreateConstantBuffer()
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    hr = DxAssist::m_device->CreateBuffer( &bd, NULL, &m_CBNeverChanges );
    if( FAILED( hr ) )
        return hr;

    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = DxAssist::m_device->CreateBuffer( &bd, NULL, &m_CBChangeOnResize );
    if( FAILED( hr ) )
        return hr;

    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = DxAssist::m_device->CreateBuffer( &bd, NULL, &m_CBChangesEveryFrame );
    if( FAILED( hr ) )
        return hr;

    result = S_OK;
Exit0:
    return result;
}

HRESULT ImageRenderer::UpdateBuffer(uint imageCount)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    D3D11_BUFFER_DESC desc;

    ID3D11Buffer*   vertexBuffer = 0;
    ID3D11Buffer*   indexBuffer = 0;

    KG_PROCESS_SUCCESS(m_maxImageCount >= imageCount);

    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth  = sizeof(VERTEX_TEXTURE) * VERTEX_PER_IMAGE * imageCount;
    desc.Usage      = D3D11_USAGE_DYNAMIC;
    desc.BindFlags  = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = DxAssist::m_device->CreateBuffer(&desc, 0, &vertexBuffer);
    KGLOG_COM_PROCESS_ERROR(hr);

    hr = CreateIndexBuffer(imageCount, &indexBuffer);
    KGLOG_COM_PROCESS_ERROR(hr);

    SAFE_RELEASE(m_indexBuffer);
    SAFE_RELEASE(m_vertexBuffer);

    m_vertexBuffer = vertexBuffer; vertexBuffer = 0;
    m_indexBuffer = indexBuffer; indexBuffer = 0;

Exit1:
    result = S_OK;
Exit0:
    SAFE_RELEASE(vertexBuffer);
    SAFE_RELEASE(indexBuffer);
    return result;
}

HRESULT ImageRenderer::CreateIndexBuffer(uint imageCount, ID3D11Buffer** indexBufferOut)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    D3D11_BUFFER_DESC desc;

    ID3D11Buffer*   iIndexBuffer = 0;
    WORD*           wIndexBuffer = 0;
    WORD*           wIndex = 0;
    WORD            index = 0;

    wIndexBuffer = new WORD[INDEX_PER_IMAGE * imageCount];
    KGLOG_PROCESS_ERROR(wIndexBuffer);

    wIndex = wIndexBuffer;
    for (unsigned i = 0; i < imageCount; ++i)
    {
        // vertex sequence of one char is:
        //  0---1
        //  | / |
        //  2---3  
        wIndex[0] = index + 0;
        wIndex[1] = index + 1;
        wIndex[2] = index + 2;

        wIndex[3] = index + 2;
        wIndex[4] = index + 1;
        wIndex[5] = index + 3;

        wIndex += 6;
        index += 4;
    }

    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth  = sizeof(*wIndexBuffer) * INDEX_PER_IMAGE * imageCount;
    desc.Usage      = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags  = D3D11_BIND_INDEX_BUFFER;

    hr = DxAssist::CreateBuffer(wIndexBuffer, desc.ByteWidth, desc.BindFlags, &iIndexBuffer, &desc);
    KGLOG_COM_PROCESS_ERROR(hr);

    *indexBufferOut = iIndexBuffer;
Exit1:
    result = S_OK;
Exit0:
    if (FAILED(result))
    {
        SAFE_RELEASE(iIndexBuffer);
    }
    SAFE_DELETE_ARRAY(wIndexBuffer);
    return result;
}


HRESULT ImageRenderer::FillVertexBuffer(RenderImageData* data, uint count)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    int mapFlag = false;
    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    VERTEX_TEXTURE *vertexs = NULL;

    hr = DxAssist::m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
    KGLOG_COM_PROCESS_ERROR(hr);
    mapFlag = true;

    vertexs = (VERTEX_TEXTURE *)mappedBuffer.pData;
    //PTR_ASSERT(pVertexs);
    //ASSERT(MappedBuffer.RowPitch >= (_VERTEX_PER_IMAGE * sizeof(*pVertexs) * Group.size()));

    RenderImageData* imageData = data;
    for (int i = 0; imageData && i < count; i++)
    {

        const RenderImageData &Image = *imageData;
        // vertex sequence of one image is:
        //  0---1
        //  | / |
        //  2---3 

        if (s_test2D)
        {
             vertexs[0].pos.x = Image.minx;
             vertexs[0].pos.y = Image.miny;
             vertexs[0].pos.z = Image.z;
             vertexs[0].texPos.x = Image.uvminx;
             vertexs[0].texPos.y = Image.uvminy;
             //vertexs[0].color = Image.Color;

             vertexs[1].pos.x = Image.maxx;
             vertexs[1].pos.y = Image.miny;
             vertexs[1].pos.z = Image.z;
             vertexs[1].texPos.x = Image.uvmaxx;
             vertexs[1].texPos.y = Image.uvminy;
             //vertexs[1].color = Image.Color;

             vertexs[2].pos.x = Image.minx;
             vertexs[2].pos.y = Image.maxy;
             vertexs[2].pos.z = Image.z;
             vertexs[2].texPos.x = Image.uvminx;
             vertexs[2].texPos.y = Image.uvmaxy;
             //vertexs[2].color = Image.Color;

             vertexs[3].pos.x = Image.maxx;
             vertexs[3].pos.y = Image.maxy;
             vertexs[3].pos.z = Image.z;
             vertexs[3].texPos.x = Image.uvmaxx;
             vertexs[3].texPos.y = Image.uvmaxy;
            // vertexs[3].color = Image.Color;
        }
        else
        {
            vertexs[0].pos.x = -1.0f;
            vertexs[0].pos.y = 1.0f;
            vertexs[0].pos.z = -1.0;
            vertexs[0].texPos.x = 0;
            vertexs[0].texPos.y = 0;

            vertexs[1].pos.x = 1.0f;
            vertexs[1].pos.y = 1.0f;
            vertexs[1].pos.z = -1;
            vertexs[1].texPos.x = 1;
            vertexs[1].texPos.y = 0;

            vertexs[2].pos.x = -1.0f;
            vertexs[2].pos.y = -1;
            vertexs[2].pos.z = -1;
            vertexs[2].texPos.x = 1;
            vertexs[2].texPos.y = 1;

            vertexs[3].pos.x = 1;
            vertexs[3].pos.y = -1;
            vertexs[3].pos.z = -1;
            vertexs[3].texPos.x = 0;
            vertexs[3].texPos.y = 1;
        }

        vertexs += 4;

        imageData = (RenderImageData*)imageData->nextData;
    }

    result = S_OK;
Exit0:
    if (mapFlag)
    {
        DxAssist::m_deviceContext->Unmap(m_vertexBuffer, 0);
        mapFlag = false;
    }
    return result;
}

