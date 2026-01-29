// ==============================================================
// light.h	
// ライト制御
// 
// 制作者:村上和樹		制作日付：2025/11/06	
// 更新者:村上和樹		更新日付：2025/11/06
// ==============================================================
#ifndef LIHGT_H
#define LIGHT_H

#include <DirectXMath.h>

struct DiffuseLight {
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 direction;
};



void Light_Initialize(void);
void Light_Finalize(void);
void Light_Update(void);
void Light_Draw(void);

void Light_SetAmbient(const DirectX::XMFLOAT4& color);
void Light_SetDiffuse(const DirectX::XMFLOAT3& color,const DirectX::XMFLOAT3& direction);
void Light_SetDiffuse(const DiffuseLight& light);

#endif