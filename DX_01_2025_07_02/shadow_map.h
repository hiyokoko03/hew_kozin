#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <d3d11.h>
#include <DirectXMath.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	HRESULT Initialize(ID3D11Device* device, UINT width, UINT height);
	void Finalize();

	// Begin rendering to the shadow map (clears depth)
	void Begin(ID3D11DeviceContext* context);

	// End rendering (scoping only, currently does nothing but good for API consistency)
	void End(ID3D11DeviceContext* context);

	ID3D11ShaderResourceView* GetSRV() const { return m_pShaderResourceView; }
	ID3D11DepthStencilView* GetDSV() const { return m_pDepthStencilView; }
	const D3D11_VIEWPORT& GetViewport() const { return m_Viewport; }

private:
	UINT m_Width;
	UINT m_Height;

	ID3D11ShaderResourceView* m_pShaderResourceView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_Viewport;
};

#endif // SHADOW_MAP_H
