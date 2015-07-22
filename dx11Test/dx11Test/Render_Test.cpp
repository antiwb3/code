#include "stdafx.h"
#include "DxAssist.h"
#include "PaintEngine.h"
#include "dxTutorial07Test.h"
#include "Render_Test.h"

struct TextureDesc
{        
    ID3D11Texture2D*          pTexture;
    ID3D11ShaderResourceView* pView;
    RenderImageData*          RenderData;
    RenderDevice              RenderDC;
};

typedef std::vector<TextureDesc> TextureDescArray;
static TextureDescArray s_TextureDescArray;

static int s_EnableTestTutorial07 = false;
static void TestTutorial07(HWND hwnd)
{
    Tutorial07_Test::InitDevice(hwnd);
    s_EnableTestTutorial07 = true;
};

RenderTest& RenderTest::Instance()
{
    static RenderTest s_inst;
    return s_inst;
}

RenderTest::RenderTest()
{

}

RenderTest::~RenderTest()
{
    
}

int RenderTest::Init(HWND hwnd)
{
	int ret = false;
	int result = false;
 
    m_hwnd = hwnd;
#ifndef DISABLE_TEST_RENDER
    TestSwfToTexture();
    TestTutorial07(hwnd);
#endif

// Exit1:
	result = true;
Exit0:
	return result;
}

void RenderTest::Render(double lasttime, double nowtime)
{
    if (s_EnableTestTutorial07)
        Tutorial07_Test::Render();

    for (TextureDescArray::iterator it = s_TextureDescArray.begin(); it != s_TextureDescArray.end(); it++)
    {
        PaintEngine::Instance().m_imageRender.Render(it->RenderDC, it->RenderData, 1);
    }

}

void RenderTest::TestSwfToTexture()
{
    ID3D11Texture2D* texture = 0;
    ID3D11ShaderResourceView* resView = 0;
    TextureDesc textureDesc;
    const int TEXTURE_W = 512;
    const int TEXTURE_H = 512;
    texture = DxAssist::CreateTexture(TEXTURE_W, TEXTURE_H, DXGI_FORMAT_R8G8B8A8_UNORM);
    KGLOG_COM_PROCESS_ERROR(texture);

    D3DX11CreateShaderResourceViewFromFile( DxAssist::m_device, "ui\\image\\Lighthouse.jpg", NULL, NULL, &resView, NULL );
    

    //DxAssist::m_device->CreateShaderResourceView( texture, 0, &resView);

    RenderImageData* renderData = new RenderImageData;
    //renderData->minx = -0.5;
    //renderData->miny = 0.5;
    //renderData->maxx = 0.5;
    //renderData->maxy = -0.5;

    renderData->minx = 10;
    renderData->miny = 10;
    renderData->maxx = 10 + TEXTURE_W;
    renderData->maxy = 10 + TEXTURE_H;

    renderData->uvminx = 0.0f;
    renderData->uvminy = 0.0f;
    renderData->uvmaxx = 1.0f;
    renderData->uvmaxy = 1.0f;
    renderData->z = 0.0f;
    renderData->iTextureView = resView;
    renderData->Color.r = 255;
    renderData->Color.g = 255;
    renderData->Color.b = 255;
    renderData->Color.a = 255;
    renderData->nextData = 0;

    textureDesc.RenderData = renderData;
    textureDesc.pTexture = texture;
    textureDesc.pView = resView;
    {
         RECT rc;
        GetClientRect( m_hwnd, &rc );
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        ShaderTable & shaderTable =  PaintEngine::Instance().m_shaderTable;
        textureDesc.RenderDC.ViewPortW = width;
        textureDesc.RenderDC.ViewPortH = width;
        textureDesc.RenderDC.VertexShader = shaderTable.GetVertexShader(VERTEX_SHADER_IMAGE);
        textureDesc.RenderDC.PixelShader = shaderTable.GetPixelShader(PIXEL_SHADER_IMAGE);
        textureDesc.RenderDC.InputLayout = shaderTable.GetLayoutShader(LAYOUT_SHADER_IMAGE);
    }

    s_TextureDescArray.push_back(textureDesc);

    //SWFToTexture* swfToTexture = SWFToTexture::Create(0, texture, 1024, 1024);
    //s_SwfToTextureArray.push_back(swfToTexture);

Exit0:
    return;
}