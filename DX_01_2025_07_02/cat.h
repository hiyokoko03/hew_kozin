// ==============================================================
// cat.h	
// 猫制御
// 
// 制作者:村上和樹		制作日付：2025/11/27	
// 更新者:村上和樹		更新日付：2025/11/27
// ==============================================================
#ifndef CAT_H
#define CAT_H

#include <DirectXMath.h>

#include "object.h"
#include "event.h"
#include "map_object.h"

// ===============================================
// グローバル変数
// ===============================================
static constexpr int CAT_MAX = 1;

// ===============================================
// クラス宣言
// ===============================================
class Cat : public GameObject {
public:
	// ===========================================
	// 列挙体宣言
	// ===========================================
	enum State {
		Idle = 0,
		Move,
		Stay,
		Break,
		Run,
		Away,
	};

	enum Direction {
		Front = 0,
		LeftFront,
		Left,
		LeftBack,
		Back,
		RightBack,
		Right,
		RightFront,
	};

private:
	// ===========================================
	// メンバ変数
	// ===========================================
	Direction m_Direction = Front;								// 猫の向いている方向
	int m_State = Idle;											// 猫の状態
	int m_TextureID = -1;										// テクスチャID
	int m_TargetID = -1;										// 現在向かう MapObject の ID
	DirectX::XMFLOAT3 m_CollisionSize{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_TargetPos{ 0,0,0 };
	//int m_SkipCount[OBJECT_MAX] = {};
	int m_SkipLimit = 7;  // 選ばれたら次の5回はスキップ
	std::vector<XMFLOAT3> m_Waypoints;
	int m_CurrentWaypoint = 0;
	bool m_Catch = false;

public:
	// ===========================================
	// プロトタイプ宣言
	// ===========================================
	void Initialize(void) override;
	void Finalize(void) override;
	void Update(double elapsed_time) override;
	void Draw(void) override;

	Direction GetDirectionFromCamera(const DirectX::XMFLOAT3& moveDir, const DirectX::XMFLOAT3& camPos, const DirectX::XMFLOAT3& camTarget);

	void SelectTarget();			// 優先度＆距離からターゲット決定
	void SelectCatFarTarget();
	void MoveToTarget(float dt);	// 目的地へ移動
	void MoveToCatFar(float dt);
	bool FindPath(const XMFLOAT3& StartPos, const XMFLOAT3& GoalPos);
	bool IsBlocked(const XMFLOAT3& pos);

	const int& GetState() const { return m_State; }
	void SetState(int State) { m_State = State; }

	const DirectX::XMFLOAT3& GetCollisionSize(void) const { return m_CollisionSize; }
	void SetCollisionSize(DirectX::XMFLOAT3 CollisionSize) { m_CollisionSize = CollisionSize; }

	const bool GetIsCatch(void) const { return m_Catch; }
	void SetIsCatch(bool Catch) { m_Catch = Catch; }
}; 

Cat* GetCat();
#endif
