// ==============================================================
// fade.cpp	
// フェード制御
// 
// 制作者:Youhei Sato      		制作日付：2025/09/10	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#include	"fade.h"

#include	"direct3D.h"
#include	"sprite.h"
#include	"texture.h"

using namespace DirectX;

// ===============================================
// グローバル変数
// ===============================================
static	FadeState	g_FadeState = FADE_STATE_NONE;
static	double	g_FadeTime = 0.0;
static	double	g_AccumlatedTime = 0.0;
static	double	g_FadeStartTime = 0.0;
static	XMFLOAT4	g_FadeColor = { 0.0f,0.0f,0.0f,0.0f };
static	int	g_TexID = -1;

void Fade_Initialize()
{
	g_FadeState = FADE_STATE_NONE;
	g_AccumlatedTime = 0.0;

	g_TexID = Texture_Load(L"resource/texture/88.jpg");
}

void Fade_Finalize()
{
}

void Fade_Update(double elapsed_time)
{
	//時間計測とステート管理
	if (g_FadeState == FADE_STATE_NONE || g_FadeState == FADE_STATE_FADE_IN_FINISHED || g_FadeState == FADE_STATE_FADE_OUT_FINISHED)
	{
		return;
	}

	//Fadeが実行されている時間
	double	lifeTime = g_AccumlatedTime - g_FadeStartTime;

	//透明度の計算
	float	alpha = static_cast<float>(lifeTime / g_FadeTime);
	if (g_FadeState == FADE_STATE_FADE_IN)
	{
		alpha = 1.0f - alpha;
	}
	g_FadeColor.w = alpha;

	if (g_FadeTime <= lifeTime)
	{
		//三項演算子
		g_FadeState = g_FadeState == FADE_STATE_FADE_IN ? FADE_STATE_FADE_IN_FINISHED : FADE_STATE_FADE_OUT_FINISHED;
		/*if (g_FadeState == FADE_STATE_FADE_IN)
		{
			g_FadeState = FADE_STATE_FADE_IN_FINISHED;
		}
		else
		{
			g_FadeState = FADE_STATE_FADE_OUT_FINISHED;
		}*/
	}

	g_AccumlatedTime += elapsed_time;
}

void Fade_Draw()
{
	if (g_FadeState == FADE_STATE_NONE || g_FadeState == FADE_STATE_FADE_IN_FINISHED )
	{
		return;
	}

	Sprite_Draw(g_TexID, 0.0f, 0.0f,
		Direct3D_GetBackBufferWidth(),
		Direct3D_GetBackBufferHeight(),
		g_FadeColor);
}

void Fade_Start(double fade_time, bool isFadeIn, DirectX::XMFLOAT4 fade_color)
{
	g_FadeTime = fade_time;
	//g_FadeState = isFadeIn ? FADE_STATE_FADE_IN : FADE_STATE_FADE_OUT;
	if (isFadeIn)
	{
		g_FadeState = FADE_STATE_FADE_IN;
	}
	else
	{
		g_FadeState = FADE_STATE_FADE_OUT;
	}
	g_FadeStartTime = g_AccumlatedTime;
	g_FadeColor = fade_color;
}

FadeState Fade_GetState()
{
	return g_FadeState;
}
