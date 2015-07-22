#include "stdafx.h"
//#include <dxgitype.h>
#include "dx11mgr.h"



DX11Mgr::DX11Mgr()
: m_pDevice(NULL)
, m_pSwapChain(NULL)
, m_pImmContext(NULL)
, m_pRTView(NULL)
{

}

DX11Mgr::~DX11Mgr()
{
}

HRESULT DX11Mgr::Init(HWND hwnd)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	RECT rc;
	DXGI_SWAP_CHAIN_DESC sd;

	GetClientRect( hwnd, &rc );

	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rc.right - rc.left;
	sd.BufferDesc.Height = rc.bottom - rc.top;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = CreateDevice();
	FailToExit0(hr);

	hr = CreateSwapChain(sd);
	FailToExit0(hr);

	hr = CreateRTView();
	FailToExit0(hr);

    hr = CreateDepthStencilView(sd.BufferDesc.Width, sd.BufferDesc.Height);
    FailToExit0(hr);

    m_pImmContext->OMSetRenderTargets( 1, &m_pRTView, m_pDepthStencilView );

	SetViewports(sd.BufferDesc.Width, sd.BufferDesc.Height);

	hResult = S_OK;
Exit0:
	return hResult; 
}

void DX11Mgr::Uninit()
{
	Cleanup();
}

void DX11Mgr::Cleanup()
{
	if (m_pImmContext) 
		m_pImmContext->ClearState();

	SAFE_RELEASE(m_pRTView);
    SAFE_RELEASE(m_pDepthStencil);
    SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pImmContext);
	//SAFE_RELEASE(m_pDevice);
}

void DX11Mgr::SetViewports(UINT uWidth, UINT uHeight)
{
	ASSERT(m_pImmContext);

	D3D11_VIEWPORT vp;

	vp.Width	= (FLOAT)uWidth;
	vp.Height	= (FLOAT)uHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pImmContext->RSSetViewports(1, &vp);
    
}

void  DX11Mgr::OnClientSize(UINT uWidth, UINT uHeight)
{
    if (m_pSwapChain)
    {
        m_pImmContext->ClearState();

        SAFE_RELEASE(m_pRTView);
        SAFE_RELEASE(m_pDepthStencil);
        SAFE_RELEASE(m_pDepthStencilView);
        m_pSwapChain->ResizeBuffers(1, uWidth, uHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        
        CreateRTView();
        CreateDepthStencilView(uWidth, uHeight);

        m_pImmContext->OMSetRenderTargets( 1, &m_pRTView, m_pDepthStencilView );
    }
    SetViewports(uWidth, uHeight);
}

HRESULT DX11Mgr::CreateDevice()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	//IDXGIAdapter* pAdapter = NULL;
	ID3D11Device*			pDevice			= NULL;
	ID3D11DeviceContext*	pDeviceContext	= NULL;

	D3D_DRIVER_TYPE DriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT nDriverTypes	= ARRAYSIZE( DriverTypes );
	UINT nLevelCount	= ARRAYSIZE( FeatureLevels );
	UINT uDeviceFlags	= 0;
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

#ifdef _DEBUG
	uDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	for( UINT uIndex = 0; uIndex < nDriverTypes; uIndex++ )
	{
		hr = D3D11CreateDevice(NULL, DriverTypes[uIndex], 
			0,  //A handle to a DLL that implements a software rasterizer
			D3D11_CREATE_DEVICE_SINGLETHREADED,  // The runtime layers to enable (see D3D11_CREATE_DEVICE_FLAG); values can be bitwise OR'd together.
			FeatureLevels, nLevelCount, 
			D3D11_SDK_VERSION, 
			&pDevice, 
			&FeatureLevel,
			&pDeviceContext);

		if( SUCCEEDED( hr ) )
			break;
	}
	FailToExit0(hr);

	m_pDevice = pDevice;
	m_pImmContext = pDeviceContext;
	hResult = S_OK;
Exit0:
	if (FAILED(hResult))
	{
		SAFE_RELEASE(pDeviceContext);
		SAFE_RELEASE(pDevice);
	}
	return hResult; 
}

HRESULT DX11Mgr::CreateDepthStencilView(int width, int height)
{
    HRESULT hr = E_FAIL;
    HRESULT hResult = E_FAIL;

    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = m_pDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
    if( FAILED( hr ) )
        goto Exit0;

    {
        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory( &descDSV, sizeof(descDSV) );
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        hr = m_pDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
        if( FAILED( hr ) )
            goto Exit0;
    }

    hResult = S_OK;
Exit0:
    return hResult; 
}

HRESULT DX11Mgr::CreateSwapChain(DXGI_SWAP_CHAIN_DESC const& Desc)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	IDXGIFactory* pFactory = NULL; 
	IDXGISwapChain* pSwapChain = NULL;

	ASSERT(m_pDevice);

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory) ,(void**)&pFactory);
	FailToExit0(hr);

	hr = pFactory->CreateSwapChain(m_pDevice, (DXGI_SWAP_CHAIN_DESC *)&Desc, &pSwapChain);
	FailToExit0(hr);

	m_pSwapChain = pSwapChain;
	hResult = S_OK;
Exit0:
	SAFE_RELEASE(pFactory);
	return hResult; 
}

HRESULT DX11Mgr::CreateRTView()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	ID3D11Texture2D* pBackBuffer = NULL;

	ASSERT(m_pDevice);
	ASSERT(m_pImmContext);
	ASSERT(m_pSwapChain);

	// Create a render target view
	
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	FailToExit0(hr);

	hr = m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRTView );
	FailToExit0(hr);

	hResult = S_OK;
Exit0:
	SAFE_RELEASE(pBackBuffer);
	return hResult; 
}

HRESULT DX11Mgr::GetAllAdapters(IDXGIAdapterArray& rArray)
{
	HRESULT hr		= E_FAIL;
	HRESULT hResult = E_FAIL;

	UINT	uIndex  = 0;

	IDXGIAdapter* pAdapter = NULL; 
	IDXGIFactory* pFactory = NULL; 

	// Create a DXGIFactory object.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory) ,(void**)&pFactory);
	FailToExit0(hr);

	uIndex = 0;
	while (pFactory->EnumAdapters(uIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		rArray.push_back(pAdapter);
		uIndex++;
	}

	hResult = S_OK;
Exit0:
	SAFE_RELEASE(pFactory);
	return hResult; 

}

static HRESULT GetDisplayModeList(IDXGIAdapter* pAdapter, int nIndex, DXGI_MODE_DESC** ppDesc)
{
	HRESULT hr		= E_FAIL;
	HRESULT hResult = E_FAIL;
	UINT	nModes	= 0;

	IDXGIOutput*	pOutput		= NULL; 
	DXGI_MODE_DESC* pDisModes	= NULL;
	DXGI_FORMAT		format		= DXGI_FORMAT_R32G32B32A32_FLOAT;

	FalseToExit0(pAdapter);
	FalseToExit0(ppDesc);

	hr = pAdapter->EnumOutputs(nIndex, &pOutput);
	FailToExit0(hr);
	FalseToExit0(pOutput);

	hr = pOutput->GetDisplayModeList( format, 0, &nModes, NULL);
	FalseToExit0(hr);

	pDisModes = new DXGI_MODE_DESC[nModes]; 
	FalseToExit0(pDisModes);

	hr = pOutput->GetDisplayModeList( format, 0, &nModes, pDisModes);

	*ppDesc = pDisModes;
	hResult = S_OK;
Exit0:
	if (FAILED(hResult))
	{
		SAFE_DELETE(pDisModes);
	}
	SAFE_RELEASE(pOutput);
	return hResult; 
}