#ifndef _DX11_MGR_H
#define _DX11_MGR_H

#include <d3d11.h>
#include <vector>

typedef std::vector<IDXGIAdapter *> IDXGIAdapterArray;

class DX11Mgr
{
public:
	DX11Mgr();
	~DX11Mgr();

	HRESULT Init(HWND hwnd);
	void	Uninit();

	void	Cleanup();

	void	SetViewports(UINT uWidth, UINT uHeight);
    void    OnClientSize(UINT uWidth, UINT uHeight);

	static HRESULT GetAllAdapters(IDXGIAdapterArray& rArray);
	static HRESULT GetDisplayModeList(IDXGIAdapter* pAdapter, int nIndex, DXGI_MODE_DESC** ppDesc);

private:
	HRESULT CreateDevice();
	HRESULT CreateSwapChain(DXGI_SWAP_CHAIN_DESC const& Desc);
	HRESULT CreateRTView();
    HRESULT CreateDepthStencilView(int width, int height);
public:
	ID3D11Device*			m_pDevice;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11DeviceContext*	m_pImmContext;
	ID3D11RenderTargetView* m_pRTView;

    ID3D11Texture2D*        m_pDepthStencil;
    ID3D11DepthStencilView* m_pDepthStencilView;
};

#endif // _DX11_MGR_H