// ==============================================================
// sprite.h	
// スプライト制御
// 
// 制作者:Youhei Sato			制作日付：2025/05/15	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>

// ===============================================
// プロトタイプ宣言
// ===============================================
void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);

void Sprite_Draw(int texid, float x, float y, float width, float height, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

void Sprite_Draw(int texid, float x, float y, float width, float height, int txx, int txy, int txw, int txh, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

void Sprite_Draw(int texid, float x, float y, float width, float height, int txx, int txy, int txw, int txh, float angle, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

void Sprite_Draw(int texid, float x, float y, float width, float height, bool flip, float angle, 
	DirectX::XMFLOAT2 texcood1, DirectX::XMFLOAT2 texcood2, DirectX::XMFLOAT2 texcood3, DirectX::XMFLOAT2 texcood4, 
	DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }
);

void Sprite_Draw(int texid, float x, float y, float width, float height, float angle, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

#endif // SPRITE_H
