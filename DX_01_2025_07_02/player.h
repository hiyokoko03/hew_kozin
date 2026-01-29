// ==============================================================
// player.h	
// プレイヤー制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/10/29	
// 更新者:Kazuki Murakami		更新日付：2025/11/25
// ==============================================================
#ifndef PLAYER_H
#define PLAYER_H

#include <DirectXMath.h>
#include <iostream>

#include "object.h"
#include "shadow.h"
#include "model.h" 

// ===============================================
// マクロ定義
// ===============================================
static constexpr int PLAYER_MAX = 1;
static constexpr int TEXTURE_MAX = 8;

// ===============================================
// クラス宣言
// ===============================================
class Player : public GameObject {
public:
	// ===========================================
	// 列挙体宣言
	// ===========================================
	enum Form {
		FORM_IDLE = 0,
		FORM_SHADOW
	};

	enum Direction {
		Front = 315,
		LeftFront = 0,
		Left = 45,
		LeftBack = 90,
		Back = 135,
		RightBack = 180,
		Right = 225,
		RightFront = 270,
	};

private:
	// ===========================================
	// メンバ関数
	// ===========================================
	void (Player::* updateFunc)(); // メンバ関数ポインタ

	void UpdateMove();
	void UpdateAction();

	// ===========================================
	// メンバ変数
	// ===========================================
	int m_TextureID[TEXTURE_MAX] = {};								// テクスチャID配列
	MODEL* m_Model{};
	Shadow m_Shadow;												// 影のインスタンスを持つ
	int m_Direction = Front;										// プレイヤーの向いている方向
	Form m_Form = FORM_IDLE;										// プレイヤーの形態
	bool m_OnGround = true;											// 接地判定
	bool m_Jump = false;											// ジャンプ中かどうか
	bool m_ShadowChange = false;									// 影の形態に変化したかどうか
	DirectX::XMFLOAT3 m_CollisionSize{ 0.0f,0.0f,0.0f };	// 地形とプレイヤーの当たり判定のサイズ
	

public:
	// ===========================================
	// プロトタイプ宣言
	// ===========================================
	void Initialize(void)override;
	void Finalize(void)override;
	void Update(double elpased_time)override;
	void Draw(void)override;

	void Formation(double elpased_time);
	void Collision();

	void ChangeStateToMove() { updateFunc = &Player::UpdateMove; }
	void ChangeStateToAction() { updateFunc = &Player::UpdateAction; }

	// ===========================================
	// ゲッター、セッター
	// ===========================================
	const int& GetDirection(void) const { return m_Direction; }
	void SetDirection(int direction) { m_Direction = direction; }

	const Form& GetForms(void) const { return m_Form; }
	void SetForms(Form Form) { m_Form = Form; }

	const bool& GetIsJump(void) const { return m_Jump; }
	void SetIsJump(bool jump) { m_Jump = jump; }

	const bool& GetIsShadowChange(void) const { return m_ShadowChange; }
	void SetIsShadowChange(bool change) { m_ShadowChange = change; }

	const DirectX::XMFLOAT4& GetColor(void) const { return m_Color; }
	void SetColor(DirectX::XMFLOAT4 color) { m_Color = color; }

	const DirectX::XMFLOAT3& GetToTerrainCollisionSize(void) const { return m_CollisionSize; }
	void SetToTerrainCollisionSize(DirectX::XMFLOAT3 size) { m_CollisionSize = size; }

	Shadow& GetShadow() { return m_Shadow; }
};

Player* GetPlayer(void);
#endif