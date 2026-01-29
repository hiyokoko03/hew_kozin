// ==============================================================
// cube.h	
// Cube描画
// 
// 制作者:村上和樹		制作日付：2025/10/16	
// 更新者:村上和樹		更新日付：2025/11/26
// ==============================================================
#ifndef CUBE_H
#define CUBE_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <array>

enum CubeFace {
	NONE = -1,
	FRONT = 0,
	RIGHT,
	BACK,
	LEFT,
	TOP,
	BOTTOM,
};

void Cube_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Cube_Finalize();
void Cube_Update(double elapsed_time);
void Cube_Draw(int texid, const DirectX::XMMATRIX& mtxWorld, const DirectX::XMFLOAT4& Color);

void Cube_Draw(int texid,const DirectX::XMMATRIX& mtxWorld, CubeFace ChangeFace,
	const DirectX::XMFLOAT4& Color = { 1.0f,1.0f,1.0f,1.0f }, const DirectX::XMFLOAT4& ChangeColor = { 1.0f,1.0f,1.0f,1.0f }
);

#endif //CUBE_H