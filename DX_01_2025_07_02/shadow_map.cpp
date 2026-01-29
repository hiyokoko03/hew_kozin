#include "shadow_map.h"
#include "direct3D.h" // For safe release macros if needed, or just include standard lib

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#endif

ShadowMap::ShadowMap()
	: m_Width(0)
	, m_Height(0)
	, m_pShaderResourceView(nullptr)
	, m_pDepthStencilView(nullptr)
{
	m_Viewport = {};
}

ShadowMap::~ShadowMap()
{
	Finalize();
}

HRESULT ShadowMap::Initialize(ID3D11Device* device, UINT width, UINT height)
{
	m_Width = width;
	m_Height = height;

	HRESULT hr;

	// 1. Create Depth Texture
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = m_Width;
	texDesc.Height = m_Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	// Use R24G8 or D24_S8. For SRV, we need a typeless format or R24_UNORM.
	// Common practice: R24G8_TYPELESS for texture, D24_UNORM_S8_UINT for DSV, R24_UNORM_X8_TYPELESS for SRV
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap = nullptr;
	hr = device->CreateTexture2D(&texDesc, nullptr, &depthMap);
	if (FAILED(hr)) return hr;

	// 2. Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthMap, &dsvDesc, &m_pDepthStencilView);
	if (FAILED(hr)) {
		depthMap->Release();
		return hr;
	}

	// 3. Create Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(depthMap, &srvDesc, &m_pShaderResourceView);
	depthMap->Release(); // Done with texture, views hold references
	if (FAILED(hr)) return hr;

	// 4. Setup Viewport
	m_Viewport.Width = (float)m_Width;
	m_Viewport.Height = (float)m_Height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;

	return S_OK;
}

void ShadowMap::Finalize()
{
	SAFE_RELEASE(m_pShaderResourceView);
	SAFE_RELEASE(m_pDepthStencilView);
}

void ShadowMap::Begin(ID3D11DeviceContext* context)
{
	// Bind DSV, null rendering to color (Depth Only pass)
	ID3D11RenderTargetView* nullRTV = nullptr;
	context->OMSetRenderTargets(0, &nullRTV, m_pDepthStencilView);

	// Clear Depth
	context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Set Viewport
	context->RSSetViewports(1, &m_Viewport);
}

void ShadowMap::End(ID3D11DeviceContext* context)
{
	// Usually reset render target to back buffer here, 
	// but the main loop handles that via Direct3D_Begin3D/Clear.
	// So we might just unbind the shadow map as a best practice?
	// But OMSetRenderTargets(0, nullptr, nullptr) is enough.
}
