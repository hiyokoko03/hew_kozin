// ==============================================================
// direct3d.h	
// Direct3D制御
// 
// 制作者:Momoka Ishijima		制作日付：2025/05/07	
// 更新者:Kazuki Murakami		更新日付：2025/11/24
// ==============================================================
#ifndef DIRECT3D_H
#define	DIRECT3D_H

#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")

#include "debug_ostream.h"

#define SAFE_RELEASE(o) if (o) { (o)->Release(); o = NULL; }

// ===============================================
// 列挙体宣言
// ===============================================
enum AlphaBlendMode
{
	BLEND_TRANSPARENT,
	BLEND_ADD,
	BLEND_MAX
};

// 2D 3Dの描画切り替え
enum RenderMode
{
	RENDER_2D = 0,
	RENDER_3D = 1
};

// ===============================================
// プロトタイプ宣言
// ===============================================
bool Direct3D_Initialize(HWND hWnd);	//Init		//Direct3Dの初期化処理
void Direct3D_Finalize();				//Uninit	//Direct3Dの終了処理

static	bool configureBackBuffer();	//バックバッファの設定・生成
static	void releaseBackBuffer();	//バックバッファの解放

void Direct3D_Clear();		//バックバッファのクリア
void Direct3D_Present();	//バックバッファの表示

unsigned int Direct3D_GetBackBufferWidth();
unsigned int Direct3D_GetBackBufferHeight();

ID3D11Device* Direct3D_GetDevice();
ID3D11DeviceContext* Direct3D_GetContext();
IDXGISwapChain* Direct3D_GetSwapChain();

void SetViewport(int n);

void Direct3D_SetAlphaBlend(AlphaBlendMode blend);	//使用終了後は元の設定にセットしなおす

void Direct3D_SetDepthStencilState(bool bEnable);

void Direct3D_SetRenderMode(RenderMode mode);
inline void Direct3D_Begin2D() { Direct3D_SetRenderMode(RENDER_2D); }
inline void Direct3D_Begin3D() { Direct3D_SetRenderMode(RENDER_3D); }

void Direct3D_SetZEnable(bool enable);
void Direct3D_SetZWrite(bool enable);
void Direct3D_SetBlendMode(AlphaBlendMode mode);

#endif // !DIRECT3D_H



