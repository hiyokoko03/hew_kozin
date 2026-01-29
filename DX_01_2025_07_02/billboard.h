// ==============================================================
// billboard.h	
// 板ポリゴン描画
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/18	
// 更新者:Kazuki Murakami		更新日付：2025/11/18
// ==============================================================
#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <d3d11.h>
#include <DirectXMath.h>

void Billborad_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Billborad_Finalize();
void Billborad_Update(double elapsed_time);
void Billborad_Draw(int texid, const DirectX::XMMATRIX& mtxWorld, DirectX::XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f},
	DirectX::XMFLOAT2 texcood1 = { 0.0f,0.0f }, DirectX::XMFLOAT2 texcood2 = { 1.0f,0.0f }, DirectX::XMFLOAT2 texcood3 = { 0.0f,1.0f }, DirectX::XMFLOAT2 texcood4 = {1.0f,1.0f});

#endif
