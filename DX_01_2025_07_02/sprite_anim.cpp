// ==============================================================
// sprite_anim.cpp	
// スプライトアニメーション制御
// 
// 制作者:Ishijima Momoka		制作日付：2025/06/??	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#include	"sprite_anim.h"
#include	"texture.h"
#include	"sprite.h"
//#include	"system_timer.h"

//static int g_TextureId = -1;
//static constexpr int ANIM_PATTERN_MAX = 13;
//static int g_pattern = 0;
//static double g_time;
//static AnimPattern* pAP = nullptr;

//void SpriteAnim_Initialize()
//{
//	pAP = new AnimPattern(Texture_Load(L"kokosozai.png"),
//		13, 0.1, { 0,0 }, { 32,32 });	//コマ数、コマ秒、
//	g_pattern = 0;
//	g_time = 0.0;
//	//g_TextureId = Texture_Load(L"kokosozai.png");
//	//g_time = SystemTimer_GetTime();
//}

//void SpriteAnim_Finalize()
//{
//
//}

//void SpriteAnim_Update(double elapsed_time)
//{
//	//if (SystemTimer_GetTime() - g_time >= 1.0f)
//	//{
//	//	g_pattern = (g_pattern + 1) % ANIM_PATTERN_MAX;
//	//	g_time = SystemTimer_GetTime();
//	//}
//	
//	//g_time += elapsed_time;
//	//
//	//if (g_time >= 0.1)
//	//{
//	//	g_pattern = (g_pattern + 1) % pAP->GetPatternMax();
//	//	g_time -= 0.1;
//	//}
//}
//
//void SpriteAnim_Draw(float x, float y)
//{
//	//Sprite_Draw(g_TextureId, x, y, 200.0f, 200.0f, 32 * g_pattern, 32 * 1, 32, 32);
//	pAP->Draw(x, y, 256, 256, g_pattern);
//	
//}

void AnimPattern::Draw(float dx, float dy, float dw, float dh, int pattern_num, float angle, DirectX::XMFLOAT4 color)
{
	//int pattern = pattern_num % m_pattern_max;

	Sprite_Draw(m_texture_id, dx, dy, dw, dh,
		m_start_position.x + m_pattern_size.x * (pattern_num%m_horizontal_pattern_max),	//pattern_numをどっちも改造
		m_start_position.y + m_pattern_size.y * (pattern_num/m_horizontal_pattern_max),	//?
		m_pattern_size.x, m_pattern_size.y,
		 angle, color);
}

void AnimPatternPlayer::Update(double elapsed_time)
{
	m_accumulated_time += elapsed_time;

	if (m_accumulated_time >= m_pAnimPattern->GetSecondPerPattern())
	{
		m_pattern = (m_pattern + 1) % m_pAnimPattern->GetPatternMax();
		m_accumulated_time -= m_pAnimPattern->GetSecondPerPattern();
	}
}

void AnimPatternPlayer::Draw(float dx, float dy, float dw, float dh, float angle, DirectX::XMFLOAT4 color)
{
	m_pAnimPattern->Draw(dx, dy, dw, dh, m_pattern, angle, color);
}
