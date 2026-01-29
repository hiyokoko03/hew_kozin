// ==============================================================
// thief.h	
// 泥棒制御
// 
// 制作者:村上和樹		制作日付：2025/11/28	
// 更新者:村上和樹		更新日付：2025/11/28
// ==============================================================
#ifndef THIEF_H
#define THIEF_H

#include <DirectXMath.h>

#include "object.h"
#include "event.h"
#include "map_object.h"

// ===============================================
// グローバル変数
// ===============================================
static constexpr int THIEF_MAX = 1;

// ===============================================
// クラス宣言
// ===============================================
class Thief : public GameObject {
public:
	// ===========================================
	// 列挙体宣言
	// ===========================================
	enum State {
		Idle = 0,
		Move,
		Stay,
		CatFind,
		CatCatch,
		Run,
		Sprize,
		Away,
		End,
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
	Direction m_Direction = Front;								// 泥棒の向いている方向
	int m_State = Idle;											// 泥棒の状態
	int m_TextureID = -1;										// テクスチャID
	int m_TargetID = -1;										// 現在向かう MapObject の ID
	DirectX::XMFLOAT3 m_CollisionSize{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CatchCollisionSize{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_TargetPos{ 0.0f,0.0f,0.0f };
	//int m_SkipCount[OBJECT_MAX] = {};
	int m_SkipLimit = 7;  // 選ばれたら次の5回はスキップ
	std::vector<XMFLOAT3> m_Waypoints;
	int m_CurrentWaypoint = 0;
	bool m_LookCat = false;
	bool m_CatchCat = false;

public:
	// ===========================================
	// プロトタイプ宣言
	// ===========================================
	void Initialize(void) override;
	void Finalize(void) override;
	void Update(double elapsed_time) override;
	void Draw(void) override;

	void Collision(void);
	Direction GetDirectionFromCamera(const DirectX::XMFLOAT3& moveDir, const DirectX::XMFLOAT3& camPos, const DirectX::XMFLOAT3& camTarget);

	void SelectTarget();			// 優先度＆距離からターゲット決定
	void SelectFar();
	void MoveToTarget(float dt);	// 目的地へ移動
	void MoveToCat(float dt);
	void MoveToExit(float dt);
	void MoveToFar(float dt);
	bool FindPath(const XMFLOAT3& StartPos, const XMFLOAT3& GoalPos);
	bool IsBlocked(const XMFLOAT3& pos);
	bool RealizeObject();
};

Thief* GetThief();
#endif
