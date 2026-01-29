// ==============================================================
// light.cpp	
// ライト制御
// 
// 制作者:村上和樹		制作日付：2025/11/06	
// 更新者:村上和樹		更新日付：2025/11/06
// ==============================================================
#include "light.h"

#include <d3d11.h>
#include "direct3D.h"

using namespace DirectX;

static ID3D11Buffer* g_pPSConstantBuffer0 = nullptr;
static ID3D11Buffer* g_pPSConstantBuffer1 = nullptr;

void Light_Initialize(void)
{
	auto pDevice = Direct3D_GetDevice();

	// ピクセルシェーダーのライト設定用定数バッファの作成
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // バインドフラグ

	buffer_desc.ByteWidth = sizeof(XMFLOAT4);			// バッファのサイズ
	pDevice->CreateBuffer(&buffer_desc, nullptr, &g_pPSConstantBuffer0);

	buffer_desc.ByteWidth = sizeof(DiffuseLight);		// バッファのサイズ
	pDevice->CreateBuffer(&buffer_desc, nullptr, &g_pPSConstantBuffer1);
}

void Light_Finalize(void)
{
	SAFE_RELEASE(g_pPSConstantBuffer1);
	SAFE_RELEASE(g_pPSConstantBuffer0);
}

void Light_Update(void)
{

}

void Light_Draw(void)
{

}

void Light_SetAmbient(const DirectX::XMFLOAT4& color)
{
	// 定数バッファに行列をセット
	Direct3D_GetContext()->UpdateSubresource(g_pPSConstantBuffer0, 0, nullptr, &color, 0, 0);
	Direct3D_GetContext()->PSSetConstantBuffers(0, 1, &g_pPSConstantBuffer0);
}

void Light_SetDiffuse(const DirectX::XMFLOAT3& color, const DirectX::XMFLOAT3& direction)
{
	DiffuseLight light{
		{color.x,color.y,color.z, 1.0f},
		{direction.x, direction.y, direction.z, 1.0f}
	};

	Direct3D_GetContext()->UpdateSubresource(g_pPSConstantBuffer1, 0, nullptr, &light, 0, 0);
	Direct3D_GetContext()->PSSetConstantBuffers(1, 1, &g_pPSConstantBuffer1);
}

void Light_SetDiffuse(const DiffuseLight& light)
{
	Direct3D_GetContext()->UpdateSubresource(g_pPSConstantBuffer1, 0, nullptr, &light, 0, 0);
	Direct3D_GetContext()->PSSetConstantBuffers(1, 1, &g_pPSConstantBuffer1);
}
