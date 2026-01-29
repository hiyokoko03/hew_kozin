//// ==============================================================
//// thief.cpp	
//// 猫制御
//// 
//// 制作者:村上和樹		制作日付：2025/11/28	
//// 更新者:村上和樹		更新日付：2025/11/28
//// ==============================================================
//#include "thief.h"
//
//#include <windows.h>
//#include <DirectXMath.h>
//#include <array>
//#include <random>
//#include <vector>
//#include <algorithm>
//#include <iostream>
//
//#include "cat.h"
//#include "cube.h"
//#include "map.h"
//#include "billboard.h"
//#include "active_camera.h"
//#include "texture.h"
//#include "direct3d.h"
//#include "timer.h"
//#include "event.h"
//#include "end_game.h"
//
//using namespace DirectX;
//
//// ===============================================
//// グローバル変数
//// ===============================================
//std::array<Thief, THIEF_MAX> thief;
//Timer g_ThiefStayTimer;
//Timer g_TheifChaseTimer;
//Timer g_ThiefLookCooldown;
//Cat* pCat = GetCat();
//MapObject* objects = GetMapObject();
//
//namespace
//{
//	int g_CollisionTextureID[3] = {};
//	ActiveCamera* g_pActiveCamera = GetActiveCamera();
//	EndGame* pEndGame = GetEndGame();
//}
//
//static int GetObjectPriority(MapObject::Type type)
//{
//	switch (type)
//	{
//	case MapObject::Sofa:
//		return 5;
//
//	case MapObject::Chair:
//		return 1;
//
//	case MapObject::CoatStand:
//		return 2;
//
//	case MapObject::Plant:
//		return 4;
//
//	case MapObject::Clock:
//		return 3;
//
//	case MapObject::Shelf:
//		return 5;
//
//	case  MapObject::Vase:
//		return 2;
//
//	case  MapObject::UmbrellaStand:
//		return 1;
//
//	case MapObject::Table:
//		return 3;
//
//	default:
//		return 0;
//	}
//}
//
//struct Node {
//	int x, z;
//	float g, h;
//	Node* parent;
//};
//
//void Thief::Initialize(void)
//{
//	m_Tag = OBJECT_ENEMY;
//	m_Use = true;
//	m_Direction = Front;
//	m_Pos = { -12.0f,0.5f,1.0f };
//	m_Size = { 0.5f,1.0f,0.1f };
//	m_CollisionSize = { 0.5f,1.0f,0.5f };
//	m_CatchCollisionSize = { 1.5f,1.0f,1.5f }; 
//	m_Color = { 1.0f,1.0f,1.0f,1.0f };
//	m_State = Stay;
//	m_LookCat = false;
//	m_CatchCat = false;
//
//	m_TextureID = Texture_Load(L"resource/texture/thief.png");
//	g_CollisionTextureID[0] = g_CollisionTexture = Texture_Load(L"resource/texture/object_collision.png");
//	g_CollisionTextureID[1] = Texture_Load(L"resource/texture/look.png");
//	g_CollisionTextureID[2] = Texture_Load(L"resource/texture/over_gauge.png");
//}
//
//void Thief::Finalize(void)
//{
//
//}
//
//void Thief::Update(double elapsed_time)
//{
//	if (!m_Use)
//		return;
//
//	if (m_State == End)
//		return;
//
//	g_ThiefStayTimer.Update((float)elapsed_time);
//	g_TheifChaseTimer.Update((float)elapsed_time);
//	g_ThiefLookCooldown.Update((float)elapsed_time);
//
//	if (RealizeObject() == true)
//	{
//		g_ThiefLookCooldown.Start(15.0f);
//		m_State = Run;
//		
//	}
//	if (m_State == Run && g_ThiefLookCooldown.IsFinished())
//	{
//		m_State = Stay;
//	}
//	
//	if (m_State == Away)
//	{
//		MoveToFar((float)elapsed_time);
//	}
//
//	if (g_ThiefStayTimer.IsFinished() && m_State == Stay && m_State != CatFind && m_State != CatCatch)
//	{
//		SelectTarget();
//		m_State = Move;
//	}
//	if (m_State == Move && m_LookCat != true)
//	{
//		MoveToTarget((float)elapsed_time);
//	}
//	if (m_LookCat == true && m_State != CatFind)
//	{
//		FindPath(m_Pos, pCat->GetPos());
//
//		m_State = CatFind;
//	}
//	if (g_TheifChaseTimer.IsFinished() && m_State == CatFind && m_CatchCat == false)
//	{
//		MoveToCat((float)elapsed_time);
//	}
//	if (m_CatchCat == true && m_State != CatCatch)
//	{
//		m_LookCat = false;
//		m_State = CatCatch;
//		FindPath(m_Pos, { -14.0f, 0.0f, 0.0f });
//	}
//	if (m_State == CatCatch)
//	{
//		MoveToExit((float)elapsed_time);
//	}
//
//	Collision();
//}
//
//void Thief::Draw(void)
//{
//	if (!m_Use)
//		return;
//
//	int texture_id = -1;
//	texture_id = m_TextureID;
//	XMFLOAT2 texcood1{ 0.35f,0.0f }, texcood2{ 0.725f,0.0f }, texcood3{ 0.35f,1.0f }, texcood4{ 0.725f,1.0f };
//
//	switch (m_State)
//	{
//	case CatCatch:
//		texcood1 = { 0.725f,0.0f };
//		texcood2 = { 1.0f,0.0f };
//		texcood3 = { 0.725f,1.0f };
//		texcood4 = { 1.0f,1.0f };
//	}
//	//switch (m_Direction)
//	//{
//	//case Front:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.0f, 0.005f);
//	//	texcood2 = XMFLOAT2(0.08f, 0.005f);
//	//	texcood3 = XMFLOAT2(0.0f, 0.125f);
//	//	texcood4 = XMFLOAT2(0.08f, 0.125f);
//	//	break;
//
//	//case LeftFront:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.26f, 0.005f);
//	//	texcood2 = XMFLOAT2(0.34f, 0.005f);
//	//	texcood3 = XMFLOAT2(0.26f, 0.125f);
//	//	texcood4 = XMFLOAT2(0.34f, 0.125f);
//	//	break;
//
//	//case Left:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.0f, 0.135f);
//	//	texcood2 = XMFLOAT2(0.08f, 0.135f);
//	//	texcood3 = XMFLOAT2(0.0f, 0.26f);
//	//	texcood4 = XMFLOAT2(0.08f, 0.26f);
//	//	break;
//
//	//case LeftBack:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.26f, 0.13f);
//	//	texcood2 = XMFLOAT2(0.34f, 0.13f);
//	//	texcood3 = XMFLOAT2(0.26f, 0.25f);
//	//	texcood4 = XMFLOAT2(0.34f, 0.25f);
//	//	break;
//
//	//case Back:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.0f, 0.38f);
//	//	texcood2 = XMFLOAT2(0.08f, 0.38f);
//	//	texcood3 = XMFLOAT2(0.0f, 0.5f);
//	//	texcood4 = XMFLOAT2(0.08f, 0.5f);
//	//	break;
//
//	//case RightBack:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.26f, 0.38f);
//	//	texcood2 = XMFLOAT2(0.34f, 0.38f);
//	//	texcood3 = XMFLOAT2(0.26f, 0.5f);
//	//	texcood4 = XMFLOAT2(0.34f, 0.5f);
//	//	break;
//
//	//case Right:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.0f, 0.26f);
//	//	texcood2 = XMFLOAT2(0.08f, 0.26f);
//	//	texcood3 = XMFLOAT2(0.0f, 0.385f);
//	//	texcood4 = XMFLOAT2(0.08f, 0.385f);
//	//	break;
//
//	//case RightFront:
//	//	// 描画部分のテクスチャID
//	//	texture_id = m_TextureID;
//	//	texcood1 = XMFLOAT2(0.26f, 0.26f);
//	//	texcood2 = XMFLOAT2(0.34f, 0.26f);
//	//	texcood3 = XMFLOAT2(0.26f, 0.385f);
//	//	texcood4 = XMFLOAT2(0.34f, 0.385f);
//	//	break;
//	//}
//
//	// スケール行列 (幅, 高さ, 奥行き)
//	XMMATRIX ScaleMatrix = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
//	XMMATRIX CollisionScaleMatrix = XMMatrixScaling(m_CollisionSize.x, m_CollisionSize.y, m_CollisionSize.z);
//	XMMATRIX CatchCollisionScaleMatrix = XMMatrixScaling(m_CatchCollisionSize.x, m_CatchCollisionSize.y, m_CatchCollisionSize.z);
//
//	//ビュー変換行列を求める
//	XMVECTOR xTarget = XMLoadFloat3(&g_pActiveCamera->GetPos()) + XMLoadFloat3(&g_pActiveCamera->GetFront());
//	XMMATRIX ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&g_pActiveCamera->GetPos()), xTarget, XMLoadFloat3(&g_pActiveCamera->GetUp()));
//
//	// ビュー行列の逆行列 (カメラのワールド行列) を取得
//	XMMATRIX inv = XMMatrixInverse(nullptr, ViewMatrix);
//
//	// 回転部分だけ取り出す
//	XMFLOAT3 right, up, forward;
//	XMStoreFloat3(&right, inv.r[0]);
//	XMStoreFloat3(&up, inv.r[1]);
//	XMStoreFloat3(&forward, inv.r[2]);
//
//	// 移動成分を 0 にした行列を作り直す
//	XMMATRIX billboard_rotation = XMMATRIX(
//		XMLoadFloat3(&right),
//		XMLoadFloat3(&up),
//		XMLoadFloat3(&forward),
//		XMVectorSet(0, 0, 0, 1)
//	);
//
//	// 移動行列
//	XMMATRIX translation_matrix = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
//
//	// ワールド行列の合成
//	XMMATRIX WorldMatrix = ScaleMatrix * billboard_rotation * translation_matrix;
//	XMMATRIX CollisionMatrix = CollisionScaleMatrix * translation_matrix;
//	XMMATRIX CatchCollisionMatrix = CatchCollisionScaleMatrix * translation_matrix;
//
//
//	Direct3D_SetZEnable(true);
//	Direct3D_SetZWrite(false);
//	Direct3D_SetBlendMode(BLEND_TRANSPARENT);
//
//	// 描画
//	Billborad_Draw(texture_id, WorldMatrix, m_Color, texcood1, texcood2, texcood3, texcood4);
//
//#ifdef DEBUG || _DEBUG
//	Cube_Draw(g_CollisionTextureID[0], CollisionMatrix, { 1.0f,1.0f,1.0f,0.5f});
//	if (m_LookCat == false && m_CatchCat == false)
//	{
//		Cube_Draw(g_CollisionTextureID[0], CatchCollisionMatrix, { 0.0f,0.0f,1.0f,0.5f });
//	}
//	if (m_LookCat == true)
//	{
//		Cube_Draw(g_CollisionTextureID[1], CatchCollisionMatrix, { 1.0f,1.0f,0.0f,0.5f });
//	}
//	if (m_CatchCat == true)
//	{
//		Cube_Draw(g_CollisionTextureID[2], CatchCollisionMatrix, { 1.0f,0.0f,0.0f,0.5f });
//	}
//#endif
//}
//
//void Thief::Collision(void)
//{
//	if (m_Collision.IntersectsBoxBox(m_Pos, m_CatchCollisionSize, pCat->GetPos(), pCat->GetCollisionSize()) && 
//		m_CatchCat == false && m_LookCat == false && g_ThiefLookCooldown.IsFinished() && m_State != Run)
//	{
//		m_LookCat = true;
//		g_TheifChaseTimer.Start(3.0f);
//	}
//	if (m_Collision.IntersectsBoxBox(m_Pos, m_CatchCollisionSize, pCat->GetPos(), pCat->GetCollisionSize()) && g_TheifChaseTimer.IsFinished() && m_State != Run)
//	{
//		m_CatchCat = true;
//		pCat->SetIsCatch(true);
//	}
//}
//
//Thief::Direction Thief::GetDirectionFromCamera(const DirectX::XMFLOAT3& moveDir, const DirectX::XMFLOAT3& camPos, const DirectX::XMFLOAT3& camTarget)
//{
//	XMVECTOR move = XMVectorSet(moveDir.x, 0.0f, moveDir.z, 0.0f);
//	float moveLen = XMVectorGetX(XMVector3Length(move));
//	if (moveLen < 1e-5f)
//		return Front;
//
//	move = XMVectorScale(move, 1.0f / moveLen);
//
//	// カメラ front（Y無視）
//	XMVECTOR camP = XMLoadFloat3(&camPos);
//	XMVECTOR camT = XMLoadFloat3(&camTarget);
//	XMVECTOR front = camT - camP;
//	front = XMVectorSet(XMVectorGetX(front), 0.0f, XMVectorGetZ(front), 0.0f);
//
//	float frontLen = XMVectorGetX(XMVector3Length(front));
//	if (frontLen < 1e-5f)
//		return Front;
//
//	front = XMVectorScale(front, 1.0f / frontLen);
//
//	// 右方向（Frontを90度回転）
//	float fx = XMVectorGetX(front);
//	float fz = XMVectorGetZ(front);
//	XMVECTOR right = XMVectorSet(fz, 0.0f, -fx, 0.0f);
//
//	float forwardDot = XMVectorGetX(XMVector3Dot(move, front));
//	float rightDot = XMVectorGetX(XMVector3Dot(move, right));
//
//	float angle = atan2f(rightDot, forwardDot);
//	float deg = XMConvertToDegrees(angle);
//
//	// 後ろ方向の閾値（狭める）
//	const float BACK_RANGE = 15.0f;  // Back 正面 ±15°
//	const float BACK_SIDE_RANGE = 50.0f;  // BackSide は ±50°まで
//
//	// 後ろ
//	if (deg >= 180.0f - BACK_RANGE || deg <= -180.0f + BACK_RANGE)
//		return Back;
//
//	// 左後ろ
//	if (deg < -BACK_RANGE && deg >= -BACK_SIDE_RANGE)
//		return LeftBack;
//
//	// 右後ろ
//	if (deg > BACK_RANGE && deg <= BACK_SIDE_RANGE)
//		return RightBack;
//
//	// 以下は前側の 5 方向
//	if (deg >= -22.5f && deg < 22.5f)     return Front;
//	if (deg >= 22.5f && deg < 67.5f)     return RightFront;
//	if (deg >= 67.5f && deg < 112.5f)    return Right;
//	if (deg >= -67.5f && deg < -22.5f)    return LeftFront;
//	if (deg >= -112.5f && deg < -67.5f)   return Left;
//
//	return Front;
//}
//
//void Thief::SelectTarget()
//{
//
//	float bestScore = -FLT_MAX;
//	DirectX::XMFLOAT3 pos = m_Pos;
//
//	const float priority_weight = 1.0f;   // 優先度スコアの重み
//	const float distance_weight = 1.2f;   // 距離スコアの重み
//
//	int selectedID = -1;
//
//	for (int i = 0; i < OBJECT_MAX; i++)
//	{
//		MapObject& obj = objects[i];
//		int id = obj.GetID();
//
//		int priority = GetObjectPriority(obj.GetType());
//
//		if (priority == 0)
//		{
//			continue;
//		}
//
//		if (m_SkipCount[id] > 0)
//		{
//			continue;
//		}
//
//		float dx = obj.GetPos().x - pos.x;
//		float dy = obj.GetPos().y - pos.y;
//		float dz = obj.GetPos().z - pos.z;
//		float dist = sqrtf(dx * dx + dy * dy + dz * dz);
//
//		// 距離スコア（近いほど大きい）
//		float distance_score = 10.0f / (dist + 1.0f);
//
//		//スコア計算(優先度スコアの重み × 優先度 ＋ 距離スコアの重み × 距離スコア)
//		float score = priority_weight * priority +
//			distance_weight * distance_score;
//
//		// 最大スコアを選ぶ
//		if (score > bestScore)
//		{
//			bestScore = score;
//			selectedID = id;
//		}
//	}
//
//	// 他のスキップカウントを1減らす
//	for (int i = 0; i < OBJECT_MAX; i++)
//	{
//		if (m_SkipCount[i] > 0)
//		{
//			m_SkipCount[i]--;
//		}
//	}
//
//	// 選ばれたオブジェクトを5回スキップ
//	if (selectedID >= 0)
//	{
//		m_TargetID = selectedID;
//		m_SkipCount[selectedID] = m_SkipLimit;
//		m_TargetPos = objects[selectedID].GetTargetPos();
//		FindPath(m_Pos, m_TargetPos);
//	}
//}
//
//void Thief::SelectFar()
//{
//	float bestScore = -FLT_MAX;
//	DirectX::XMFLOAT3 pos = m_Pos;
//
//	int selectedID = -1;
//
//	for (int i = 0; i < OBJECT_MAX; i++)
//	{
//		MapObject& obj = objects[i];
//
//		if (obj.GetType() == MapObject::LeftDoor || obj.GetType() == MapObject::RightDoor)
//			continue;
//
//		int id = obj.GetID();
//
//		float dx = obj.GetPos().x - pos.x;
//		float dy = obj.GetPos().y - pos.y;
//		float dz = obj.GetPos().z - pos.z;
//		float dist = sqrtf(dx * dx + dy * dy + dz * dz);
//
//		// 距離スコア（遠いほど大きい）
//		float distance_score = (dist + 1.0f);
//
//		//スコア計算(距離スコア)
//		float score = distance_score;
//
//		// 最大スコアを選ぶ
//		if (score > bestScore)
//		{
//			bestScore = score;
//			selectedID = id;
//		}
//	}
//
//	// 選ばれたオブジェクトを5回スキップ
//	if (selectedID >= 0)
//	{
//		m_TargetID = selectedID;
//		m_SkipCount[selectedID] = m_SkipLimit;
//		m_TargetPos = objects[selectedID].GetTargetPos();
//		FindPath(m_Pos, m_TargetPos);
//	}
//}
//
//void Thief::MoveToTarget(float dt)
//{
//	const float speed = 1.2f;
//
//	if (m_Waypoints.empty()) 
//		return;
//
//	XMFLOAT3 target = m_Waypoints[m_CurrentWaypoint];
//
//	XMFLOAT3 dir{
//		target.x - m_Pos.x,
//		target.y - m_Pos.y,
//		target.z - m_Pos.z,
//	};
//
//	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
//	if (len < 0.5f)
//	{
//		m_CurrentWaypoint++;
//
//		// 最後の waypoint に到達：Stay
//		if (m_CurrentWaypoint >= (int)m_Waypoints.size())
//		{
//			g_ThiefStayTimer.Start(4.0f);
//			m_State = Stay;
//			return;
//		}
//		return;
//	}
//
//	dir.x /= len;
//	dir.z /= len;
//
//	// ここで方向を計算して反映する
//	m_Direction = GetDirectionFromCamera(dir, g_pActiveCamera->GetPos(), g_pActiveCamera->GetTarget());
//
//	m_Pos.x += dir.x * speed * dt;
//	m_Pos.z += dir.z * speed * dt;
//}
//
//void Thief::MoveToCat(float dt)
//{
//	const float speed = 1.5f;
//
//	if (m_Waypoints.empty())
//		return;
//
//	XMFLOAT3 target = m_Waypoints[m_CurrentWaypoint];
//
//	XMFLOAT3 dir{
//		target.x - m_Pos.x,
//		target.y - m_Pos.y,
//		target.z - m_Pos.z,
//	};
//
//	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
//	if (len < 0.5f)
//	{
//		m_CurrentWaypoint++;
//
//		// 最後の waypoint に到達：Stay
//		if (m_CurrentWaypoint >= (int)m_Waypoints.size())
//		{
//			m_State = Stay;
//			return;
//		}
//		return;
//	}
//
//	dir.x /= len;
//	dir.z /= len;
//
//	// ここで方向を計算して反映する
//	m_Direction = GetDirectionFromCamera(dir, g_pActiveCamera->GetPos(), g_pActiveCamera->GetTarget());
//
//	m_Pos.x += dir.x * speed * dt;
//	m_Pos.z += dir.z * speed * dt;
//}
//
//void Thief::MoveToExit(float dt)
//{
//	const float speed = 0.8f;
//
//	if (m_Waypoints.empty())
//		return;
//
//	XMFLOAT3 target = m_Waypoints[m_CurrentWaypoint];
//
//	XMFLOAT3 dir{
//		target.x - m_Pos.x,
//		target.y - m_Pos.y,
//		target.z - m_Pos.z,
//	};
//
//	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
//	if (len < 0.5f)
//	{
//		m_CurrentWaypoint++;
//
//		// 最後の waypoint に到達：Stay
//		if (m_CurrentWaypoint >= (int)m_Waypoints.size())
//		{
//			m_State = End;
//
//			//ゲームオーバーの処理
//			pEndGame->SetEnd(pEndGame->GameOver);
//
//			return;
//		}
//		return;
//	}
//
//	dir.x /= len;
//	dir.z /= len;
//
//	// ここで方向を計算して反映する
//	m_Direction = GetDirectionFromCamera(dir, g_pActiveCamera->GetPos(), g_pActiveCamera->GetTarget());
//
//	pCat->SetPos({ pCat->GetPos().x + dir.x * speed * dt, pCat->GetPos().y, pCat->GetPos().z + dir.z * speed * dt });
//	m_Pos.x += dir.x * speed * dt;
//	m_Pos.z += dir.z * speed * dt;
//}
//
//void Thief::MoveToFar(float dt)
//{
//	const float speed = 1.5f;
//
//	if (m_Waypoints.empty())
//		return;
//
//	XMFLOAT3 target = m_Waypoints[m_CurrentWaypoint];
//
//	XMFLOAT3 dir{
//		target.x - m_Pos.x,
//		target.y - m_Pos.y,
//		target.z - m_Pos.z,
//	};
//
//	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
//	if (len < 0.5f)
//	{
//		m_CurrentWaypoint++;
//
//		// 最後の waypoint に到達：Stay
//		if (m_CurrentWaypoint >= (int)m_Waypoints.size())
//		{
//			g_ThiefStayTimer.Start(10.0f);
//			m_State = Stay;
//			return;
//		}
//		return;
//	}
//
//	dir.x /= len;
//	dir.z /= len;
//
//	// ここで方向を計算して反映する
//	m_Direction = GetDirectionFromCamera(dir, g_pActiveCamera->GetPos(), g_pActiveCamera->GetTarget());
//
//	m_Pos.x += dir.x * speed * dt;
//	m_Pos.z += dir.z * speed * dt;
//}
//
//bool Thief::FindPath(const XMFLOAT3& StartPos, const XMFLOAT3& GoalPos)
//{
//	const float GRID = 0.1f;
//
//	// grid index <- world
//	auto toGrid = [&](float v) { return (int)floorf(v / GRID); };
//	// grid index -> world: 「セルの中心」を返す（ここが重要）
//	auto toWorld = [&](int g) { return g * GRID + GRID * 0.5f; };
//
//	int sx = toGrid(StartPos.x);
//	int sz = toGrid(StartPos.z);
//	int gx = toGrid(GoalPos.x);
//	int gz = toGrid(GoalPos.z);
//
//	std::vector<Node*> open;
//	std::vector<Node*> closed;
//
//	auto h = [&](int x, int z) {
//		return abs(gx - x) + abs(gz - z); // マンハッタン（軸移動優先）
//		};
//
//	Node* start = new Node{ sx, sz, 0, (float)h(sx, sz), nullptr };
//	open.push_back(start);
//
//	auto isInList = [&](std::vector<Node*>& list, int x, int z) {
//		for (auto n : list)
//			if (n->x == x && n->z == z) return true;
//		return false;
//		};
//
//	const int DIR[4][2] = {
//		{ 1,0 }, {-1,0 }, {0,1}, {0,-1}  // X/Z 軸方向のみ
//	};
//
//	Node* last = nullptr;
//
//	while (!open.empty())
//	{
//		std::sort(open.begin(), open.end(),
//			[](Node* a, Node* b) { return (a->g + a->h) < (b->g + b->h); });
//
//		Node* cur = open.front();
//		open.erase(open.begin());
//		closed.push_back(cur);
//
//		if (cur->x == gx && cur->z == gz)
//		{
//			last = cur;
//			break;
//		}
//
//		for (auto& d : DIR)
//		{
//			int nx = cur->x + d[0];
//			int nz = cur->z + d[1];
//
//			// ワールド座標に変換してブロック判定（※ toWorld はセル中心を返す）
//			XMFLOAT3 wpos{ toWorld(nx), StartPos.y, toWorld(nz) };
//			if (IsBlocked(wpos))
//				continue;
//
//			if (isInList(closed, nx, nz))
//				continue;
//
//			float ng = cur->g + 1;
//			Node* n = nullptr;
//
//			for (auto o : open)
//				if (o->x == nx && o->z == nz) n = o;
//
//			if (!n)
//			{
//				n = new Node{ nx, nz, ng, (float)h(nx,nz), cur };
//				open.push_back(n);
//			}
//			else if (ng < n->g)
//			{
//				n->g = ng;
//				n->parent = cur;
//			}
//		}
//	}
//
//	if (!last)
//	{
//		// メモリ解放（見つからなかった場合も確実に）
//		for (auto n : open) delete n;
//		for (auto n : closed) delete n;
//		return false;
//	}
//
//	// ルート復元
//	m_Waypoints.clear();
//	Node* p = last;
//	while (p)
//	{
//		m_Waypoints.push_back(XMFLOAT3(toWorld(p->x), StartPos.y, toWorld(p->z)));
//		p = p->parent;
//	}
//	std::reverse(m_Waypoints.begin(), m_Waypoints.end());
//
//	// ノード配列のメモリを解放
//	for (auto n : open) delete n;
//	for (auto n : closed) delete n;
//
//	m_CurrentWaypoint = 0;
//
//	return true;
//}
//
//bool Thief::IsBlocked(const XMFLOAT3& pos)
//{
//	Cube* pWall = GetWall();
//	MapObject* pMapObject = GetMapObject();
//
//	for (int i = 0; i < WALL_MAX; i++)
//	{
//		XMFLOAT3 WallPos = pWall[i].GetPos();
//		XMFLOAT3 WallSize = pWall[i].GetSize();
//
//		if (m_Collision.IntersectsBoxBox(pos, m_CollisionSize, WallPos, WallSize))
//		{
//			return true;
//		}
//	}
//	
//	/*for (int i = 0; i < OBJECT_MAX; i++)
//	{
//		XMFLOAT3 ObjectPos = pMapObject[i].GetPos();
//		XMFLOAT3 ObjectCollisionSize = pMapObject[i].GetSize();
//
//		if (m_Collision.IntersectsBoxBox(pos, m_Size, ObjectPos, ObjectCollisionSize))
//		{
//			return true;
//		}
//	}*/
//
//	return false;
//}
//
//bool Thief::RealizeObject()
//{
//	for (int i = 0; i < OBJECT_MAX; i++)
//	{
//		MapObject& obj = objects[i];
//
//		XMFLOAT3 dir{
//		objects[i].GetPos().x - m_Pos.x,
//		objects[i].GetPos().y - m_Pos.y,
//		objects[i].GetPos().z - m_Pos.z,
//		};
//
//		float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
//
//		if (len < 5.0f)
//		{
//			if (objects[i].GetIsMoved() == true)
//			{
//				objects[i].SetIsMoved(false);
//				if (m_CatchCat == true)
//				{
//					m_CatchCat = false;
//					m_LookCat = false;
//					pCat->SetState(Cat::Run);
//					pCat->SetIsCatch(false);
//				}
//				/*else
//				{
//					m_State = Sprize;
//				}*/
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
//
//Thief* GetThief()
//{
//	return &thief[0];
//}
