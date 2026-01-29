/*=================================================================
エフェクト制御・管理[game_effect.cpp]
Ishijima	Momoka
2025/09/19
=================================================================*/
#include "game_effect.h"
#include	"texture.h"
#include	"sprite.h"
#include	"direct3D.h"

struct Effect
{
	DirectX::XMFLOAT2 position;
	float size;
	double lifeTime;
	double birthTime;
	DirectX::XMFLOAT4 color;
};

static	constexpr	int	EFFECT_MAX = 128;
static	Effect	g_Effects[EFFECT_MAX]{};
static	int	g_EffectCount{ 0 };
static	double	g_AccumulatedTime = 0.0;
static	int	g_TexID = -1;

void Effect_Initialize()
{
	g_TexID = Texture_Load(L"resource/texture/effect000 1.jpg");

	g_EffectCount = 0.0;
	g_AccumulatedTime = 0.0;
}

void Effect_Finalize()
{
}

void Effect_Update(double elpased_time)
{
	g_AccumulatedTime += elpased_time;

	//寿命判定
	for (int i = g_EffectCount; i >= 0; i--)
	{
		double	time = g_AccumulatedTime - g_Effects[i].birthTime;//経過生存時間
		if (g_Effects[i].lifeTime <= time)
		{
			g_Effects[i] = g_Effects[--g_EffectCount];
		}
	}
}

void Effect_Draw()
{
	//Direct3D_SetAlphaBlend(BLEND_ADD);

	for (int i = 0; i < g_EffectCount; i++)
	{
		double	time = g_AccumulatedTime - g_Effects[i].birthTime;
		float	ratio = time / g_Effects[i].lifeTime;
		float	alpha = 1.0f - ratio;
		float	size = g_Effects[i].size * (1.0f - ratio);

		float	half = g_Effects[i].size * 0.5f;
		float	dx = g_Effects[i].position.x + half - size * 0.5f;
		float	dy = g_Effects[i].position.y + half - size * 0.5f;

		DirectX::XMFLOAT4 color = g_Effects[i].color;
		color.w = ratio;

		Sprite_Draw(g_TexID, dx, dy, size, size,
			0.0f, 0.0f, Texture_GetWidth(g_TexID), Texture_GetHeight(g_TexID),
			color);
	}

	//Direct3D_SetAlphaBlend(BLEND_TRANSPARENT);
}

void Effect_Create(const DirectX::XMFLOAT2& position, float size, double lifeTime, const DirectX::XMFLOAT4& color)
{
	if (g_EffectCount >= EFFECT_MAX)	return;

	g_Effects[g_EffectCount].position = position;
	g_Effects[g_EffectCount].size = size;
	g_Effects[g_EffectCount].lifeTime = lifeTime;
	g_Effects[g_EffectCount].birthTime = g_AccumulatedTime;
	g_Effects[g_EffectCount].color = color;
	g_EffectCount++;
}
