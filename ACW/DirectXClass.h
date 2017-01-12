#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
class DirectXClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	DirectXClass();
	DirectXClass(const DirectXClass&) = default;
	DirectXClass& operator=(const DirectXClass&) = default;

	~DirectXClass();
	bool DirectXInitialize(const HWND hwnd,const int ScreenWidth,const int ScreenHeight,const bool Fullscreen);
	void BeginScene();
	void EndScene() const;
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	void TurnOnAlphaBlending() const;
	void TurnOffAlphaBlending() const;


	void TurnOnParticleBlend() const;


	void TurnZBufferOn() const;
	void TurnZBufferOff() const;

	void SetWireFrame() const;
	void SetFill() const ; 
	void Cleanup();
	void GetProjectionMatrix(SimpleMath::Matrix&) const;
	void GetWorldMatrix(SimpleMath::Matrix&) const;
	void GetOrthoMatrix(SimpleMath::Matrix&) const;
	void ResetViewPort();
	ID3D11DepthStencilView* GetDepthStencilView() const;
	void SetBackBufferRenderTarget() const;
private:
	ID3D11Device *					m_dev;              // the device interface
	ID3D11DeviceContext*			m_devcon;    // the device context interface
	IDXGISwapChain *				m_swapchain;   // the pointer to the swap chain interface
	DXGI_SWAP_CHAIN_DESC			m_desc; //swap chain description
	IDXGIFactory*					m_factory;
	IDXGIAdapter*					m_adapter;
	IDXGIOutput*					m_adapterOutput;
	unsigned int					m_numModes, i, m_numerator, m_denominator;
	DXGI_MODE_DESC*					m_displayModeList;
	ID3D11Texture2D *				m_backBufferPointer;
	D3D11_TEXTURE2D_DESC			m_depthStencilBufferDescription;
	D3D11_DEPTH_STENCIL_DESC		m_depthStencilDescription;
	ID3D11RenderTargetView*			m_renderTargetView;
	ID3D11Texture2D*				m_depthStencilBuffer;
	ID3D11DepthStencilState*		m_depthStencilState;
	ID3D11DepthStencilView*			m_depthStencilView;
	ID3D11RasterizerState*			m_rasterizerState;
	ID3D11RasterizerState*			m_wireframeRasterizer;

	D3D11_RASTERIZER_DESC			m_rasterizerDescription;
	D3D11_DEPTH_STENCIL_VIEW_DESC	m_DepthStencilViewDescription;
	D3D_FEATURE_LEVEL				m_featureLevel;
	D3D11_VIEWPORT				m_viewPort;


	D3D11_BLEND_DESC				m_blendDesc;
	D3D11_BLEND_DESC				m_particleblendDesc;
	ID3D11BlendState* m_ParticleEnable;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ID3D11DepthStencilState* m_depthDisabledStencilState;


	float color[4];
	//these will be going to camera
	SimpleMath::Matrix m_projectionMatrix;
	//SimpleMath::Matrix m_worldMatrix;
	//SimpleMath::Matrix m_orthoMatrix;
	//ID3D11Texture2D* m_depthStencilBuffer;
	//2d rendering
	//ID3D11RasterizerState* m_rasterState;
};

