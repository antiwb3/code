#include "stdafx.h"
#include <stack>
#include <d3dcompiler.h>
#include <D3DX11async.h>
#include "DxAssist.h"
#include "RenderStruct.h"

ID3D11Device*               DxAssist::m_device = 0;
ID3D11DeviceContext*        DxAssist::m_deviceContext = 0;



//D3D11_CPU_ACCESS_WRITE
//D3D11_USAGE_DYNAMIC
//
//typedef struct D3D11_BUFFER_DESC
//{
//    UINT ByteWidth;
//    D3D11_USAGE Usage;
//    UINT BindFlags;
//    UINT CPUAccessFlags;
//    UINT MiscFlags;
//    UINT StructureByteStride;
//} 	D3D11_BUFFER_DESC;

/*
D3D11_BIND_VERTEX_BUFFER
D3D11_BIND_INDEX_BUFFER
D3D11_BIND_CONSTANT_BUFFER

*/
static D3D11_BUFFER_DESC s_Dx11BufferDesc = {
    0,
    D3D11_USAGE_DEFAULT,
    D3D11_BIND_VERTEX_BUFFER,
    0,
    0,
    0
};


struct Vertex
{
    float x, y, z;
    float tu, tv;
};

static Vertex QuadVertices[4] = 
{
    {-1.0f, 1.0f, 0.0f,  0.0f,0.0f },
    { 1.0f, 1.0f, 0.0f,  1.0f,0.0f },
    {-1.0f,-1.0f, 0.0f,  0.0f,1.0f },
    { 1.0f,-1.0f, 0.0f,  1.0f,1.0f },
};

struct RTSaveInfo
{
    ID3D11RenderTargetView*  RTView;
    ID3D11DepthStencilView*  RTStencil;
};
typedef std::stack<RTSaveInfo> RTSaveInfoStack;

static RTSaveInfoStack s_RTSaveInfoStack;

void DxAssist::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    m_device = device;
    m_deviceContext = deviceContext;
}

ID3D11Texture2D* DxAssist::CreateTexture(
    uint        w, 
    uint        h,
    DXGI_FORMAT format      /*= DXGI_FORMAT_R8G8B8A8_UNORM*/, 
    D3D11_USAGE usage       /*= D3D11_USAGE_DEFAULT*/, 
    UINT        bindFlags   /*= (D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE)*/
)
{
    HRESULT hr = E_FAIL;

    ID3D11Device* device = m_device;
    ID3D11DeviceContext* deviceContext = m_deviceContext;
    
    ID3D11Texture2D* texture = 0;
    D3D11_TEXTURE2D_DESC texdesc;

    ASSERT(device);
    ASSERT(deviceContext);

    // Create the rendered texture.

    memset(&texdesc, 0, sizeof(texdesc));
    texdesc.Width = w;
    texdesc.Height = h;
    texdesc.MipLevels = 1;
    texdesc.ArraySize = 1;
    texdesc.Format = format;
    texdesc.SampleDesc.Count = 1;
    texdesc.SampleDesc.Quality = 0;
    texdesc.Usage = usage;
    texdesc.BindFlags = bindFlags;
    texdesc.CPUAccessFlags = 0;
    texdesc.MiscFlags = 0;

    hr = device->CreateTexture2D(&texdesc, NULL, &texture);
    KGLOG_COM_PROCESS_ERROR(hr);

Exit0:
    return texture;
}

HRESULT DxAssist::CreateRTDevice(ID3D11Texture2D* texture, DxRTDevice* RTDevice)
{
    HRESULT hr = E_FAIL;
    HRESULT hresult = E_FAIL;

    ID3D11Device* device = m_device;
    ID3D11DeviceContext* deviceContext = m_deviceContext;
    D3D11_TEXTURE2D_DESC texdesc;

    ASSERT(device);
    ASSERT(deviceContext);

    hr = device->CreateRenderTargetView(texture, NULL, &RTDevice->RTView);
    KGLOG_COM_PROCESS_ERROR(hr);

    // Create the depth/stencil buffer that is used when rendering the cube. Most parameters remain the same,
    // except the format, and bind flags.

    texture->GetDesc(&texdesc);
    texdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    
    hr = device->CreateTexture2D(&texdesc, NULL, &RTDevice->RTDepthStencilBuf);
    KGLOG_COM_PROCESS_ERROR(hr);
    
    hr = device->CreateDepthStencilView( RTDevice->RTDepthStencilBuf, NULL, &RTDevice->RTDepthStencil);
    KGLOG_COM_PROCESS_ERROR(hr);

    hresult = S_OK;
Exit0:
    if ( FAILED(hresult) )
    {
        ReleaseRTDevice(RTDevice);
    }
    return hresult;
}

HRESULT DxAssist::ReleaseRTDevice(DxRTDevice* RTDevice)
{
    SAFE_RELEASE(RTDevice->RTDepthStencilBuf);
    SAFE_RELEASE(RTDevice->RTDepthStencil);
    SAFE_RELEASE(RTDevice->RTView);

    return S_OK;
}

HRESULT DxAssist::SetRenderTarget(DxRTDevice* RTDevice, uint w, uint h)
{
    HRESULT hresult = E_FAIL;

    ID3D11Device* device = m_device;
    ID3D11DeviceContext* deviceContext = m_deviceContext;

    ID3D11RenderTargetView*  poldRT = 0;
    ID3D11DepthStencilView*  poldDS = 0;
    D3D11_VIEWPORT vp;
    const float clear[] = { 0, 0, 0, 90/255.0f}; //red,green,blue,alpha

    ASSERT(device);
    ASSERT(deviceContext);

    KGLOG_PROCESS_ERROR(RTDevice);
    KGLOG_PROCESS_ERROR(RTDevice->RTView);
    KGLOG_PROCESS_ERROR(RTDevice->RTDepthStencil);
    KGLOG_PROCESS_ERROR(RTDevice->RTDepthStencilBuf);

    deviceContext->OMGetRenderTargets(1, &poldRT, &poldDS );
    deviceContext->OMSetRenderTargets(1, &RTDevice->RTView, RTDevice->RTDepthStencil);

    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width    = (float)w;
    vp.Height   = (float)h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    deviceContext->RSSetViewports(1, &vp);
    
    deviceContext->ClearRenderTargetView(RTDevice->RTView, clear);
    deviceContext->ClearDepthStencilView(RTDevice->RTDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    {
        RTSaveInfo info;
        info.RTView = poldRT;
        info.RTStencil = poldDS;
        s_RTSaveInfoStack.push(info);
    }
    hresult = S_OK;
Exit0:
    return hresult;
}

HRESULT DxAssist::RestoreRenderTarget()
{
    ASSERT(s_RTSaveInfoStack.size() > 0);
    
    HRESULT hr = E_FAIL;
    HRESULT hresult = E_FAIL;
    ID3D11DeviceContext* deviceContext = m_deviceContext;

    KGLOG_PROCESS_ERROR(deviceContext);

    {
        RTSaveInfo info = s_RTSaveInfoStack.top();
        s_RTSaveInfoStack.pop();

        deviceContext->OMSetRenderTargets(1, &info.RTView, info.RTStencil);
        SAFE_RELEASE(info.RTView);
        SAFE_RELEASE(info.RTStencil);
    }

    hresult = S_OK;
Exit0:
    return hresult;
}

void DxAssist::GetTextureSize(ID3D11Texture2D* texture, uint& w, uint& h)
{
    if (!texture)
        return;

    D3D11_TEXTURE2D_DESC texdesc;
    texture->GetDesc(&texdesc);

    w = texdesc.Width;
    h = texdesc.Height;
}


void DxAssist::Init3DCoorMatrix(int width, int height, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
    // Initialize the world matrix
    worldMatrix = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    viewMatrix = XMMatrixLookAtLH( Eye, At, Up );

    // Initialize the projection matrix
    projectionMatrix = XMMatrixPerspectiveFovLH( XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f );
}


void DxAssist::Init2DCoorMatrix(int width, int height, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
    // Initialize the world matrix
    worldMatrix = XMMatrixIdentity();

    // Initialize the view matrix
    viewMatrix = XMMatrixIdentity();

    // Initialize the projection matrix
    projectionMatrix = XMMatrixPerspectiveLH( (FLOAT)width, -(FLOAT)height, 0.0f, 1.0f );
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT DxAssist::CompileShaderFromFile( const char* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut )
{
    HRESULT hr = S_OK;

    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* errorBlob = 0;
    hr = D3DX11CompileFromFile( fileName, NULL, NULL, entryPoint, shaderModel, 
        shaderFlags, 0, NULL, blobOut, &errorBlob, NULL );
    if( FAILED(hr) )
    {
        if( errorBlob != NULL )
            OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
        if( errorBlob ) errorBlob->Release();
        return hr;
    }
    if( errorBlob ) errorBlob->Release();

    return S_OK;
}


HRESULT DxAssist::CreateBuffer(const void* vertexBuffer, uint vertexBufferLen, UINT bindFlags, ID3D11Buffer** buffer, D3D11_BUFFER_DESC* bd /*= 0*/)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    D3D11_BUFFER_DESC bufferDesc;

    KGLOG_PROCESS_ERROR(m_device);
    KGLOG_PROCESS_ERROR(buffer);
    KGLOG_PROCESS_ERROR((*buffer) == 0);

    if (!bd)
    {
        bufferDesc = s_Dx11BufferDesc;
        bd = &bufferDesc;
    }

    bd->ByteWidth = vertexBufferLen;
    bd->BindFlags = bindFlags;

    if (vertexBuffer)
    {
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory( &InitData, sizeof(InitData) );
        InitData.pSysMem = vertexBuffer;
        hr = m_device->CreateBuffer( bd, &InitData, buffer );
        KGLOG_COM_PROCESS_ERROR(hr);
    }
    else
    {
        hr = m_device->CreateBuffer( bd, 0, buffer );
        KGLOG_COM_PROCESS_ERROR(hr);
    }

    // Exit1:
    result = S_OK;
Exit0:
    return result;
}


HRESULT DxAssist::CreateInputLayout(const void* shaderBytecode, uint bytecodeLen, ID3D11InputLayout** vertexLayout, D3D11_INPUT_ELEMENT_DESC* elems /*= 0*/, uint elemCount /*= 0*/)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    D3D11_BUFFER_DESC bufferDesc;

    static D3D11_INPUT_ELEMENT_DESC s_elements[] =
    {
        {"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    KGLOG_PROCESS_ERROR(m_device);
    KGLOG_PROCESS_ERROR(vertexLayout);
    KGLOG_PROCESS_ERROR((*vertexLayout) == 0);

    if (!elems)
    {
        elems = s_elements;
        elemCount = 2;
    }

    hr = m_device->CreateInputLayout(elems, elemCount, shaderBytecode, bytecodeLen, vertexLayout);
    KGLOG_COM_PROCESS_ERROR(hr);

    // Exit1:
    result = S_OK;
Exit0:
    return result;
}

HRESULT DxAssist::CreateVertexShader(const char* fileName, const char* entryPoint, const char* shaderModel, ID3D11VertexShader** vertexShader, ID3DBlob** blobOut /*= 0*/)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    ID3DBlob* vsBlob = NULL;

    KGLOG_PROCESS_ERROR(m_device);
    KGLOG_PROCESS_ERROR(entryPoint);
    KGLOG_PROCESS_ERROR(shaderModel);
    KGLOG_PROCESS_ERROR(vertexShader);
    KGLOG_PROCESS_ERROR((*vertexShader) == 0);

    hr = CompileShaderFromFile( fileName, entryPoint, shaderModel, &vsBlob );
    KGLOG_COM_PROCESS_ERROR(hr);

    hr = m_device->CreateVertexShader( vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, vertexShader );
    KGLOG_COM_PROCESS_ERROR(hr);

    if (blobOut)
    {
        *blobOut = vsBlob;
    }
    // Exit1:
    result = S_OK;
Exit0:
    if ( !(SUCCEEDED(result) && blobOut) )
        SAFE_RELEASE(vsBlob);

    return result;
}

HRESULT DxAssist::CreatePixelShader(const char* fileName, const char* entryPoint, const char* shaderModel, ID3D11PixelShader** pixelShader)
{
    HRESULT hr = E_FAIL;
    HRESULT result = E_FAIL;

    ID3DBlob* vsBlob = NULL;

    KGLOG_PROCESS_ERROR(m_device);
    KGLOG_PROCESS_ERROR(entryPoint);
    KGLOG_PROCESS_ERROR(shaderModel);
    KGLOG_PROCESS_ERROR(pixelShader);
    KGLOG_PROCESS_ERROR((*pixelShader) == 0);

    hr = CompileShaderFromFile( fileName, entryPoint, shaderModel, &vsBlob );
    KGLOG_COM_PROCESS_ERROR(hr);

    hr = m_device->CreatePixelShader( vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, pixelShader );
    KGLOG_COM_PROCESS_ERROR(hr);

    // Exit1:
    result = S_OK;
Exit0:
    SAFE_RELEASE(vsBlob);
    return result;
}

void DxAssist::SetUICoordinates(ID3D11Buffer* constantBuffer, uint w, uint h)
{
    static XMMATRIX projectionMatrix;

    projectionMatrix = XMMatrixOrthographicLH( (FLOAT)w, -(FLOAT)h, 0.0f, 1.0f );

    ConstantBuffer cb;
    cb.mWorld = XMMatrixTranslation(-(FLOAT)w * 0.5f, -(FLOAT)h * 0.5f, 0.0f);
    cb.mView = XMMatrixIdentity();
    cb.mProjection = projectionMatrix ;

    m_deviceContext->UpdateSubresource( constantBuffer, 0, NULL, &cb, 0, 0 );
    m_deviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer );
}


void DxAssist::Set3DCoordinates(ID3D11Buffer* constantBuffer, uint w, uint h)
{
    ConstantBuffer cb;

    // Initialize the world matrices
    cb.mWorld = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet( 0.0f, 3.0f, -6.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    cb.mView  = XMMatrixLookAtLH( Eye, At, Up );

    cb.mView = XMMatrixTranspose( cb.mView );
    cb.mProjection = XMMatrixPerspectiveFovLH( XM_PIDIV4, w / (FLOAT)h, 0.01f, 100.0f );
    cb.mProjection = XMMatrixTranspose( cb.mProjection );
    m_deviceContext->UpdateSubresource( constantBuffer, 0, NULL, &cb, 0, 0 );
    m_deviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer );
}
