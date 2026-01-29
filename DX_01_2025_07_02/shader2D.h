// ==============================================================
// shader.h	
// シェーダー2D制御
// 
// 制作者:Youhei Sato			制作日付：2025/05/15	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef SHADER2D_H
#define	SHADER2D_H

#include <d3d11.h>
#include <DirectXMath.h>

// ===============================================
// プロトタイプ宣言
// ===============================================
bool Shader2D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader2D_Finalize();

void Shader2D_SetProjMatrix(const DirectX::XMMATRIX& matrix);
void Shader2D_SetWorldMatrix(const DirectX::XMMATRIX& matrix);

// void Shader_SetPosition(const DirectX::XMFLOAT4& position);
void Shader2D_SetColor(const DirectX::XMFLOAT4& color);

void Shader2D_Begin();

#endif // SHADER2D_H
