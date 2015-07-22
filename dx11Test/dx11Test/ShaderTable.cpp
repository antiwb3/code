#include "stdafx.h"
#include "DxAssist.h"
#include "shader_data.inl"
#include "ShaderTable.h"

struct ShaderBytecode
{
    const byte* Bytecode;
    uint  Len;
};

struct FxFileInfo
{
    const char* FileName;
    const char* EntryPoint;
    const char* ShaderModel;
};

enum SHADER_DATA_TYPE
{
    sdtBytecode,
    sdtFileInfo
};

struct ShaderData
{
    union 
    {
        ShaderBytecode SBytecode;
        FxFileInfo     FileInfo;
    };
    SHADER_DATA_TYPE eType;
};

struct INPUT_LAYOUT_INFO
{
    uint ElemCount;
    D3D11_INPUT_ELEMENT_DESC InputElemDesc[8];
};

#define INIT_SHADER_BYTECODE(info, bytecode, len)  \
    { \
        ShaderBytecode& SByteCode = info.SBytecode; \
        info.eType = sdtBytecode;   \
        SByteCode.Bytecode = bytecode;   \
        SByteCode.Len = len;    \
    }

#define INIT_FX_FILE_INFO(info, fileName, entryPoint, shaderModel)  \
    { \
        FxFileInfo& fileInfo = info.FileInfo; \
        info.eType = sdtFileInfo; \
        fileInfo.FileName = fileName;   \
        fileInfo.EntryPoint = entryPoint;   \
        fileInfo.ShaderModel = shaderModel; \
    }

static ShaderData s_ShaderVertexData[VERTEX_SHADER_COUNT] = {};
static ShaderData s_ShaderPixelData[PIXEL_SHADER_COUNT] = {};
static INPUT_LAYOUT_INFO s_InputLayoutInfos[LAYOUT_SHADER_COUNT] = 
{
    {
        2,
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,      0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        }
    },

    {   
        2, 
        {
            { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
        } 
    }
};

void InitShaderData()
{
    /*INIT_SHADER_BYTECODE(s_ShaderVertexData[VERTEX_SHADER_IMAGE], QuadVertexShader, sizeof(QuadVertexShader));
    INIT_FX_FILE_INFO(s_ShaderVertexData[VERTEX_SHADER_COLOR], "ui\\shader\\Tutorial04.fx", "VS", "vs_4_0");

    INIT_SHADER_BYTECODE(s_ShaderPixelData[PIXEL_SHADER_IMAGE], QuadPixelShader, sizeof(QuadPixelShader));
    INIT_FX_FILE_INFO(s_ShaderPixelData[PIXEL_SHADER_COLOR], "ui\\shader\\Tutorial04.fx", "PS", "ps_4_0");*/

    INIT_FX_FILE_INFO(s_ShaderVertexData[VERTEX_SHADER_IMAGE], "ui\\shader\\Image2D.fx", "VS", "vs_4_0");
    //INIT_FX_FILE_INFO(s_ShaderVertexData[VERTEX_SHADER_IMAGE], "ui\\shader\\Tutorial07.fx", "VS", "vs_4_0");
    INIT_FX_FILE_INFO(s_ShaderVertexData[VERTEX_SHADER_COLOR], "ui\\shader\\Tutorial04.fx", "VS", "vs_4_0");

    //INIT_FX_FILE_INFO(s_ShaderPixelData[PIXEL_SHADER_IMAGE], "ui\\shader\\Tutorial07.fx", "PS", "ps_4_0");
    INIT_FX_FILE_INFO(s_ShaderPixelData[PIXEL_SHADER_IMAGE], "ui\\shader\\Image2D.fx", "PS", "ps_4_0");
    INIT_FX_FILE_INFO(s_ShaderPixelData[PIXEL_SHADER_COLOR], "ui\\shader\\Tutorial04.fx", "PS", "ps_4_0");
}


ShaderTable::ShaderTable()
{

}

ShaderTable::~ShaderTable()
{

}

HRESULT ShaderTable::Load()
{
	HRESULT hr = E_FAIL;
	HRESULT result = E_FAIL;

    static int s_initShaderData = false;

    if (!s_initShaderData)
    {
        InitShaderData();
    }

    for (int i = 0 ; i < _countof(s_ShaderVertexData); i++)
    {
        ShaderData& data = s_ShaderVertexData[i];
        switch (data.eType)
        {
        case sdtBytecode:
            {
                ShaderBytecode const& SBytecode = data.SBytecode;
                hr = DxAssist::m_device->CreateVertexShader(SBytecode.Bytecode, SBytecode.Len, 0, &m_vertex[i]);
                KGLOG_COM_PROCESS_ERROR(hr);

                hr = DxAssist::CreateInputLayout(
                    SBytecode.Bytecode, 
                    SBytecode.Len,
                    &m_layout[i], 
                    s_InputLayoutInfos[i].InputElemDesc, 
                    s_InputLayoutInfos[i].ElemCount
                );
                KGLOG_COM_PROCESS_ERROR(hr);
            }
            break;
        case sdtFileInfo:
            {
                 char dir[MAX_PATH] = {0};
                 ::GetCurrentDirectory(_countof(dir), dir);
                ID3DBlob* vsblob = 0;
                FxFileInfo const& fileInfo = data.FileInfo;
                hr = DxAssist::CreateVertexShader(fileInfo.FileName, fileInfo.EntryPoint, fileInfo.ShaderModel, &m_vertex[i], &vsblob);
                KGLOG_COM_PROCESS_ERROR(hr);

                hr = DxAssist::m_device->CreateInputLayout( s_InputLayoutInfos[i].InputElemDesc, s_InputLayoutInfos[i].ElemCount, 
                    vsblob->GetBufferPointer(),
                    vsblob->GetBufferSize(), &m_layout[i] );    
                SAFE_RELEASE(vsblob);

                KGLOG_COM_PROCESS_ERROR(hr);

            }
            break;
        default:
            break;
        }
    }

    for (int i = 0 ; i < _countof(s_ShaderPixelData); i++)
    {
        ShaderData& data = s_ShaderPixelData[i];
        switch (data.eType)
        {
        case sdtBytecode:
            {
                ShaderBytecode const& SBytecode = data.SBytecode;
                hr = DxAssist::m_device->CreatePixelShader(SBytecode.Bytecode, SBytecode.Len, 0, &m_pixel[i]);
                KGLOG_COM_PROCESS_ERROR(hr);
            }
            break;
        case sdtFileInfo:
            {
                FxFileInfo const& fileInfo = data.FileInfo;
                hr = DxAssist::CreatePixelShader(fileInfo.FileName, fileInfo.EntryPoint, fileInfo.ShaderModel, &m_pixel[i]);
                KGLOG_COM_PROCESS_ERROR(hr);
            }
            break;
        default:
            break;
        }
    }
// Exit1:
	result = S_OK;
Exit0:
    if (FAILED(result))
    {
        Unload();
    }
	return result;
}

void ShaderTable::Unload()
{
    for (int i = 0; i < _countof(m_layout); i++)
    {
        SAFE_RELEASE(m_layout[i]);
    }

    for (int i = 0; i < _countof(m_pixel); i++)
    {
        SAFE_RELEASE(m_pixel[i]);
    }

    for (int i = 0; i < _countof(m_vertex); i++)
    {
        SAFE_RELEASE(m_vertex[i]);
    }
}   

ID3D11VertexShader* ShaderTable::GetVertexShader(VERTEX_SHADER etype)
{
    if (etype >= 0 && etype < VERTEX_SHADER_COUNT)
    {
        return m_vertex[etype];
    }
    return NULL;
}

ID3D11PixelShader*  ShaderTable::GetPixelShader(PIXEL_SHADER etype)
{
    if (etype >= 0 && etype < PIXEL_SHADER_COUNT)
    {
        return m_pixel[etype];
    }
    return NULL;
}

ID3D11InputLayout*  ShaderTable::GetLayoutShader(LAYOUT_SHADER etype)
{
    if (etype >= 0 && etype < LAYOUT_SHADER_COUNT)
    {
        return m_layout[etype];
    }
    return NULL;
}