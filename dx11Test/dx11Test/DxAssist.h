#ifndef _DX_ASSIST_H
#define _DX_ASSIST_H

#include <d3d11.h>

struct DxRTDevice
{
    ID3D11RenderTargetView*     RTView;
    ID3D11Texture2D*            RTDepthStencilBuf;
    ID3D11DepthStencilView*     RTDepthStencil;
};

//
//struct 
//{
//    byte*  VertexData;
//    uint   VertexDataSize;
//
//    uint   ConstantsBufferSize;
//    byte*  VSBuffer;
//    byte*  VSBufferSize;
//
//    byte*  PSBuffer;
//    byte*  PSBufferSize;
//
//};

class DxAssist
{
public:
    static void             Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

    static ID3D11Texture2D* CreateTexture(
                                uint        w, 
                                uint        h,
                                DXGI_FORMAT format      = DXGI_FORMAT_R8G8B8A8_UNORM, 
                                D3D11_USAGE usage       = D3D11_USAGE_DEFAULT, 
                                UINT        bindFlags   = (D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE)
                            );

    static HRESULT          CreateRTDevice(ID3D11Texture2D* texture, DxRTDevice* RTDevice);
    static HRESULT          ReleaseRTDevice(DxRTDevice* RTDevice);

    static HRESULT          SetRenderTarget(DxRTDevice* RTDevice, uint w, uint h);
    static HRESULT          RestoreRenderTarget();
    static void             GetTextureSize(ID3D11Texture2D* texture, uint& w, uint& h);

    static void             Init3DCoorMatrix(int width, int height, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
    static void             Init2DCoorMatrix(int width, int height, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
    static HRESULT          CompileShaderFromFile(const char* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut);
    
    /*
        CreateBuffer(device, vertexData, vertexDataSize, D3D11_BIND_VERTEX_BUFFER, buffer);  // for vertex
        CreateBuffer(device, vertexData, vertexDataSize, D3D11_BIND_INDEX_BUFFER, buffer);   // for index
        CreateBuffer(device, 0, vertexDataSize, D3D11_BIND_CONSTANT_BUFFER, buffer);// for constant     
    */
    static HRESULT          CreateBuffer(const void* vertexBuffer, uint vertexBufferLen, UINT bindFlags, ID3D11Buffer** buffer, D3D11_BUFFER_DESC* bd = 0);
    static HRESULT          CreateInputLayout(const void* shaderBytecode, uint bytecodeLen, ID3D11InputLayout** vertexLayout, D3D11_INPUT_ELEMENT_DESC* elems = 0, uint elemCount = 0);

    static HRESULT          CreateVertexShader(const char* fileName, const char* entryPoint, const char* shaderModel, ID3D11VertexShader** vertexShader, ID3DBlob** blobOut = 0);
    static HRESULT          CreatePixelShader(const char* fileName, const char* entryPoint, const char* shaderModel, ID3D11PixelShader** pixelShader);

    static void             SetUICoordinates(ID3D11Buffer* constantBuffer, uint w, uint h);
    static void             Set3DCoordinates(ID3D11Buffer* constantBuffer, uint w, uint h);
public:
    static ID3D11Device*               m_device;
    static ID3D11DeviceContext*        m_deviceContext;
};

#endif //! _DX_ASSIST_H