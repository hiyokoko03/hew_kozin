// ==============================================================
// shader3D.h	
// 3Dシェーダー制御
// 
// 制作者:村上和樹		制作日付：2025/10/16	
// 更新者:村上和樹		更新日付：2025/10/16
// ==============================================================
#ifndef SHADER3D_H
#define	SHADER3D_H

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

bool Shader3D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader3D_Finalize();

void Shader3D_SetWorldMatrix(const DirectX::XMMATRIX& matrix);
void Shader3D_SetViewMatrix(const DirectX::XMMATRIX& matrix);
void Shader3D_SetProjMatrix(const DirectX::XMMATRIX& matrix);

void shader3D_SetMaterialDiffuse(const DirectX::XMFLOAT4 color);

void Shader3D_Begin();

#endif // SHADER_H
