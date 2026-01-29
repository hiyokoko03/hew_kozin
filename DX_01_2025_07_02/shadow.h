// ==============================================================
// shadow.h
// 影クラス
// 
// 制作者:Kazuki Murakami   制作日付：2025/11/10	
// 更新者:Kazuki Murakami	更新日付：2025/11/10
// ==============================================================
#ifndef SHADOW_H
#define SHADOW_H

#include "object.h"

class Player;

// ===============================================
// クラス宣言
// ===============================================
class Shadow : public GameObject {
private:
	bool m_Active = false;			// 発動中かどうか
	float m_Gauge = 100.0f;			// ゲージ
	float m_GaugeLimit = 0.0f;		// ゲージ上限
	int m_TextureID = -1;			// テクスチャID

public:

	// ===========================================
	// プロトタイプ宣言
	// ===========================================
	void Initialize(void)override;
	void Finalize(void)override;
	void Update(double elpased_time)override;
	void Draw(void)override;
	void ShadowGaugeDraw(void);

	const bool& GetIsActive(void) const { return m_Active; }
	void SetIsActive(bool active) { m_Active = active; }
	const float& GetGage(void) const { return m_Gauge; }
	void SetGage(float gage) { m_Gauge = gage; }
	
};

Shadow* GetShadow();
#endif
