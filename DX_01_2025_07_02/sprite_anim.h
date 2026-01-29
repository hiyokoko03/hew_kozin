// ==============================================================
// sprite_anim.h	
// スプライトアニメーション制御
// 
// 制作者:Ishijima Momoka		制作日付：2025/06/??	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef	SPRITE_ANIM_H
#define	SPRITE_ANIM_H

#include	<DirectXMath.h>

//void	SpriteAnim_Initialize();
//void	SpriteAnim_Finalize();
//
//void	SpriteAnim_Update(double elapsed_time);
//void	SpriteAnim_Draw(float x, float y);

// ===============================================
// クラス宣言
// ===============================================
class AnimPattern
{
private:
	int		m_texture_id;
	int		m_pattern_max;
	int		m_horizontal_pattern_max;
	double	m_second_per_pattern;
	DirectX::XMUINT2	m_start_position;
	DirectX::XMUINT2	m_pattern_size;	//widthとheight
	bool	m_is_loop;

public:
	AnimPattern() = delete;
	AnimPattern(int texid, int pattern_max, int horizontal_max, double second_per_pattern, DirectX::XMUINT2 start_position, DirectX::XMUINT2 pattern_size, bool is_loop = true)
		:m_texture_id(texid),
		m_pattern_max(pattern_max),
		m_horizontal_pattern_max(horizontal_max),
		m_second_per_pattern(second_per_pattern),
		m_start_position(start_position),
		m_pattern_size(pattern_size),
		m_is_loop(is_loop)
	{
	}

	int		GetPatternMax() { return m_pattern_max; }
	double	GetSecondPerPattern() { return m_second_per_pattern; }
	bool	IsLoop() { return m_is_loop; }

	void	Draw(float dx, float dy, float dw, float dh, int pattern_num, float angle = { 0.0f }, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
};

class AnimPatternPlayer
{
private:
	int		m_pattern = 0;		//再生するパターン番号
	double	m_accumulated_time = 0.0;		//累積時間
	AnimPattern* m_pAnimPattern = nullptr;

public:
	AnimPatternPlayer() = delete;
	AnimPatternPlayer(AnimPattern* pAnimPattern)
		:m_pAnimPattern(pAnimPattern)
	{
	}

	void	Update(double elapsed_time);

	void	Draw(float dx, float dy, float dw, float dh, float angle = { 0.0f }, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

};


#endif // SPRITE_ANIM_H