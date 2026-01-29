// ==============================================================
// fade.h	
// フェード制御
// 
// 制作者:Youhei Sato      		制作日付：2025/09/10	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef FADE_H
#define	FADE_H

#include	<DirectXMath.h>

// ===============================================
// 列挙体宣言
// ===============================================
enum FadeState
{
	FADE_STATE_NONE,

	FADE_STATE_FADE_OUT,
	FADE_STATE_FADE_OUT_FINISHED,
	FADE_STATE_FADE_IN,
	FADE_STATE_FADE_IN_FINISHED,

	FADE_STATE_MAX
};

// ===============================================
// プロトタイプ宣言
// ===============================================
void	Fade_Initialize();
void	Fade_Finalize();
void	Fade_Update(double elapsed_time);
void	Fade_Draw();

void	Fade_Start(double fade_time, bool isFadeIn, DirectX::XMFLOAT4 fade_color = { 0.0f,0.0f,0.0f,1.0f});

FadeState Fade_GetState();

#endif // !FADE_H

