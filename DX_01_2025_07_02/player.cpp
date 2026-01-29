// ==============================================================
// player.cpp	
// プレイヤー制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/10/29	
// 更新者:Kazuki Murakami		更新日付：2025/11/25
// ==============================================================
#include "player.h"

#include <array>
#include <DirectXMath.h>

#include "game.h"
#include "direct3D.h"
#include "texture.h"
#include "sprite.h"
#include "billboard.h"
#include "collision.h"
#include "keylogger.h"
#include "easing.h"
#include "debug_camera.h"
#include "active_camera.h"
#include "map.h"
#include "map_object.h"
#include "switch.h"
#include "cube.h"
#include "timer.h"


using namespace DirectX;

// ===============================================
// 定数定義
// ===============================================
namespace
{
	int g_CollisionTexture = -1;

	constexpr float GRAVITY = -0.015f;
	constexpr float MOVE_SPEED = 0.07f;
	constexpr float JUMP_POWER = 0.2f;   // ジャンプ初速

	constexpr float START_POS_X = -14.0f;
	constexpr float START_POS_Y = 0.15f;
	constexpr float START_POS_Z = 0.0f;

	constexpr float IDLE_WIDTH = 1.0f;
	constexpr float IDLE_HEIGHT = 1.0f;
	constexpr float IDLE_DEPTH = 1.0f;

	constexpr float SHADOW_WIDTH = 200.0f;
	constexpr float SHADOW_HEIGHT = 200.0f;
	constexpr float SHADOW_DEPTH = 0.1f;

	Game* g_pGame = GetGame();
	ActiveCamera* cam = GetActiveCamera();
	Switch* pSwitch = GetSwitch();
	
	XMFLOAT3 OldCamPos = {};
}

// ===============================================
// グローバル変数
// ===============================================
std::array<Player, PLAYER_MAX> player;
Timer g_SwitchCooldown;

// ===============================================
// プレイヤー初期化処理
// void Player::Initialize(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Player::Initialize(void)
{
	updateFunc = &Player::UpdateMove;

	g_CollisionTexture = Texture_Load(L"resource/texture/object_collision.png");
	m_TextureID[0] = Texture_Load(L"resource/texture/cat.png");
	m_Model = ModelLoad("resource/model/Cat_B.fbx",0.1f);

	m_Tag = OBJECT_PLAYER;
	m_Use = true;
	m_Pos = XMFLOAT3(START_POS_X, START_POS_Y, START_POS_Z);
	m_OldPos = m_Pos;
	m_Vel = XMFLOAT3(MOVE_SPEED, JUMP_POWER, MOVE_SPEED);
	m_Size = XMFLOAT3(IDLE_WIDTH, IDLE_HEIGHT,IDLE_DEPTH);
	m_Direction = Player::Front;
	m_Form = Player::FORM_IDLE;
	m_OnGround = false;
	m_Jump = true;
	m_ShadowChange = false;
	m_Color = XMFLOAT4(0.0f,0.0f,0.0f,1.0f);
	m_CollisionSize = XMFLOAT3(0.3f, 0.3f,0.3f);
	m_Shadow.Initialize();

	return;
}

// ===============================================
// プレイヤー終了処理
// void Player::Finalize(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Player::Finalize(void)
{
	m_Shadow.Finalize();
	return;
}

// ===============================================
// プレイヤー更新処理
// void Player::Update(double elpased_time)
// 
// 型：void
// 返り値：無し
// 引数：elpased_time	double型	経過時間
// ===============================================
void Player::Update(double elpased_time)
{
	g_SwitchCooldown.Update((float)elpased_time);

	(this->*updateFunc)(); // 現在の状態の関数を呼ぶ

	if (m_Jump && m_Shadow.GetIsActive() == false)
	{
		m_Vel.y += GRAVITY;   // 加速度
		m_Pos.y += m_Vel.y;  // 速度反映
	}
	if (!m_OnGround && !m_Jump && m_Shadow.GetIsActive() == false)
	{
		m_Pos.y -= 0.1f;
	}

	Collision();

	m_Shadow.Update(elpased_time);

	return;
}

// ===============================================
// プレイヤー描画処理
// void Player::Draw(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Player::Draw(void)
{
	Game* pGame = GetGame();

	if (!m_Use)
		return;

	// スケール行列 (幅, 高さ, 奥行き)
	XMMATRIX ScaleMatrix = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);

	// 回転行列
	XMMATRIX RotateMatrix = XMMatrixRotationY(XMConvertToRadians((float)m_Direction));

	// 移動行列
	XMMATRIX TranslationMatrix = XMMatrixTranslation(m_Pos.x, m_Pos.y - 0.15f, m_Pos.z);

	// ワールド行列の合成
	XMMATRIX WorldMatrix = ScaleMatrix * RotateMatrix * TranslationMatrix;

	Direct3D_SetZEnable(true);
	Direct3D_SetZWrite(false);
	Direct3D_SetBlendMode(BLEND_TRANSPARENT);

	// 描画
	if (m_Form != FORM_SHADOW)
	{
		ModelDraw(m_Model, WorldMatrix);
		if(pGame->GetGameMode() == DebugMode)
		{
			Cube_Draw(g_CollisionTexture, XMMatrixScaling(m_CollisionSize.x, m_CollisionSize.y, m_CollisionSize.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z), { 1.0f,1.0f,1.0f,0.5f });
		}
	}

	Direct3D_SetDepthStencilState(false);
	Direct3D_SetDepthStencilState(true);

	return;
}

// ===============================================
// プレイヤー当たり判定処理
// void Player::Collision(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Player::Collision(void)
{
	bool onAnyGround = false;

	// ===============================================
	// ステージ壁床とプレイヤーの当たり判定
	// ===============================================
	Map* pWall = GetWall();
	Map* pFloor = GetFloor();

	for (size_t i = 0; i < pWall->GetWalls().size(); i++)
	{
		if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, (pWall + i)->GetPos(), (pWall + i)->GetSize()) && (pWall + i)->GetIsUse() == true)
		{
			m_Pos.x = m_OldPos.x;
			m_Pos.z = m_OldPos.z;

			cam->SetPos(OldCamPos);
		}
	}

	
	for (size_t i = 0; i < pFloor->GetFloors().size(); i++)
	{
		if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, (pFloor + i)->GetPos(), (pFloor + i)->GetSize()))
		{
			m_OnGround = true;
			m_Jump = false;
			m_Vel.y = 0.0f;

			m_Pos.y += 0.1f;

			onAnyGround = true;
		}
	}

	for (int i = 0; i < SWITCH_MAX; i++)
	{
		if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, (pSwitch + i)->GetCollisionPos(), (pSwitch + i)->GetCollisionSize()) && g_SwitchCooldown.IsFinished())
		{
			if (KeyLogger_IsPressed(KK_F))
			{
				(pSwitch + i)->SetIsUse((pSwitch + i)->GetIsUse() ^ true);
				g_SwitchCooldown.Start(0.5);
			}
		}
	}

	MapObject* pMapObject = GetMapObject();

	for (int i = 0; i < pMapObject->GetMapObjects().size(); i++)
	{
		float PlayerTop = m_Pos.y - m_CollisionSize.y;
		float PlayerBottom = m_Pos.y + m_CollisionSize.y;
		float PlayerRight = m_Pos.x + m_CollisionSize.x;
		float PlayerLeft = m_Pos.x - m_CollisionSize.x;
		float PlayerBack = m_Pos.z + m_CollisionSize.z;
		float PlayerFront = m_Pos.z - m_CollisionSize.z;

		float OldPlayerTop = m_OldPos.y - m_CollisionSize.y;
		float OldPlayerBottom = m_OldPos.y + m_CollisionSize.y;
		float OldPlayerRight = m_OldPos.x + m_CollisionSize.x;
		float OldPlayerLeft = m_OldPos.x - m_CollisionSize.x;
		float OldPlayerBack = m_OldPos.z + m_CollisionSize.z;
		float OldPlayerFront = m_OldPos.z - m_CollisionSize.z;

		float MapObjectTop = (pMapObject + i)->GetCollisionPos().y - (pMapObject + i)->GetCollisionSize().y;
		float MapObjectBottom = (pMapObject + i)->GetCollisionPos().y + (pMapObject + i)->GetCollisionSize().y;
		float MapObjectRight = (pMapObject + i)->GetCollisionPos().x + (pMapObject + i)->GetCollisionSize().x;
		float MapObjectLeft = (pMapObject + i)->GetCollisionPos().x - (pMapObject + i)->GetCollisionSize().x;
		float MapObjectBack = (pMapObject + i)->GetCollisionPos().z + (pMapObject + i)->GetCollisionSize().z;
		float MapObjectFront = (pMapObject + i)->GetCollisionPos().z - (pMapObject + i)->GetCollisionSize().z;

		switch ((pMapObject + i)->GetType())
		{
		case MapObject::Sofa:

			break;

		case MapObject::Chair:

			break;

		case MapObject::CoatStand:

			break;

		case MapObject::Plant:

			break;

		case MapObject::Clock:

			break;

		case MapObject::Shelf:
			if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, (pMapObject + i)->GetCollisionPos(), (pMapObject + i)->GetCollisionSize()))
			{
 				m_Pos.x= m_OldPos.x;
				m_Pos.z = m_OldPos.z;

				cam->SetPos(OldCamPos);
			}
			if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, (pMapObject + i)->GetCollisionPos(), (pMapObject + i)->GetCollisionSize()))
			{
				// キャラクターの左右に当たったら
				if (PlayerLeft <= MapObjectRight || PlayerRight >= MapObjectLeft)
				{
					// 接触した瞬間
					m_Pos.x = m_OldPos.x;

					cam->SetPos({ OldCamPos.x, cam->GetPos().y, cam->GetPos().z });
				}

				// キャラクターの前後に当たったら
				if (PlayerBack >= MapObjectFront || PlayerFront <= MapObjectBack)
				{
					m_Pos.z = m_OldPos.z;

					cam->SetPos({ cam->GetPos().x, cam->GetPos().y,  OldCamPos.z });
				}

				// 足がついた
				if (PlayerBottom >= MapObjectTop)
				{
					// 着地した瞬間
					m_OnGround = true;
					m_Jump = false;
					m_Vel.y = 0.0f;

					// 座標を少し浮かせる
					m_Pos.y += 0.1f;
					onAnyGround = true;
				}

				// 頭ぶつけた
				// 前のフレームがまだ触れていない、かつ今のフレームが触れた
				if (PlayerTop <= MapObjectBottom)
				{
					m_Pos.y = m_OldPos.y;

					cam->SetPos({ cam->GetPos().x, OldCamPos.y, cam->GetPos().z });
				}
			}
			break;

		case MapObject::TV:

			break;

		case MapObject::Vase:

			break;

		case MapObject::UmbrellaStand:

			break;

		case MapObject::Table:

			break;

		case MapObject::Stair:
			if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, { (pMapObject + i)->GetCollisionPos().x,(pMapObject + i)->GetCollisionPos().y - 0.3f, (pMapObject + i)->GetCollisionPos().z },
				{ (pMapObject + i)->GetCollisionSize().x,(pMapObject + i)->GetCollisionSize().y / 2 ,(pMapObject + i)->GetCollisionSize().z }))
			{
				// キャラクターの左右に当たったら
				if (PlayerLeft <= MapObjectRight ||  PlayerRight >= MapObjectLeft)
				{
					// 接触した瞬間
					m_Pos.x = m_OldPos.x;

					cam->SetPos({ OldCamPos.x, cam->GetPos().y, cam->GetPos().z });
				}

				// キャラクターの前後に当たったら
				if (PlayerBack >= MapObjectFront || PlayerFront <= MapObjectBack)
				{
					m_Pos.z = m_OldPos.z;

					cam->SetPos({ cam->GetPos().x, cam->GetPos().y,  OldCamPos.z });
				}

				// 足がついた
				if (PlayerBottom >= MapObjectTop)
				{
					// 着地した瞬間
					m_OnGround = true;
					m_Jump = false;
					m_Vel.y = 0.0f;

					// 座標を少し浮かせる
					m_Pos.y += 0.1f;
					onAnyGround = true;
				}

				// 頭ぶつけた
				// 前のフレームがまだ触れていない、かつ今のフレームが触れた
				if (PlayerTop <= MapObjectBottom)
				{
					m_Pos.y = m_OldPos.y;

					cam->SetPos({ cam->GetPos().x, OldCamPos.y, cam->GetPos().z });
				}
			}
			if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, { (pMapObject + i)->GetCollisionPos().x,(pMapObject + i)->GetCollisionPos().y + 0.15f, (pMapObject + i)->GetCollisionPos().z + 0.3f },
				{ (pMapObject + i)->GetCollisionSize().x,(pMapObject + i)->GetCollisionSize().y / 2 ,(pMapObject + i)->GetCollisionSize().z / 2 }))
			{
				// キャラクターの左右に当たったら
				if (PlayerLeft <= MapObjectRight || PlayerRight >= MapObjectLeft)
				{
					// 接触した瞬間
					m_Pos.x = m_OldPos.x;

					cam->SetPos({ OldCamPos.x, cam->GetPos().y, cam->GetPos().z });
				}

				// キャラクターの前後に当たったら
				if (PlayerBack >= MapObjectFront || PlayerFront <= MapObjectBack)
				{
					m_Pos.z = m_OldPos.z;

					cam->SetPos({ cam->GetPos().x, cam->GetPos().y,  OldCamPos.z });
				}

				// 足がついた
				// 前のフレームがまだ触れていない、かつ今のフレームが触れた
				if (PlayerBottom >= MapObjectTop)
				{
					// 着地した瞬間
					m_OnGround = true;
					m_Jump = false;
					m_Vel.y = 0.0f;

					// 座標を少し浮かせる
					m_Pos.y += 0.1f;
					onAnyGround = true;
				}

				// 頭ぶつけた
				// 前のフレームがまだ触れていない、かつ今のフレームが触れた
				if (PlayerTop <= MapObjectBottom)
				{
					m_Pos.y = m_OldPos.y;

					cam->SetPos({ cam->GetPos().x, OldCamPos.y, cam->GetPos().z });
				}
			}
			break;

		case MapObject::Box:
			if (m_Collision.IntersectsBoxBox(m_Pos, m_CollisionSize, (pMapObject + i)->GetCollisionPos(), (pMapObject + i)->GetCollisionSize()))
			{
				m_Pos.x = m_OldPos.x;
				m_Pos.z = m_OldPos.z;

				cam->SetPos(OldCamPos);
			}
			break;

		case MapObject::LeftDoor:

			break;

		case MapObject::RightDoor:

			break;
		}
	}

	if (!onAnyGround)
	{
		// どの Floor / Object にも当たっていない
		m_OnGround = false;
	}
	
	return;
}

// ===============================================
// プレイヤー移動更新処理
// void Player::UpdateWalk(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Player::UpdateMove(void)
{
	m_OldPos = m_Pos;
	OldCamPos = cam->GetPos();

	// カメラのベクトルを正規化しておく
	XMVECTOR front = XMVector3Normalize(XMLoadFloat3(&cam->GetFront()));
	XMVECTOR right = XMVector3Normalize(XMLoadFloat3(&cam->GetRight()));

	// Y成分を打ち消す
	front = XMVectorSetY(front, 0.0f);
	right = XMVectorSetY(right, 0.0f);

	// 再正規化
	front = XMVector3Normalize(front);
	right = XMVector3Normalize(right);

	XMVECTOR move = XMVectorZero();

	// 右上移動
	if (KeyLogger_IsPressed(KK_W) && KeyLogger_IsPressed(KK_D))
	{
		move += XMVector3Normalize({ 0.0f,0.0f,1.0f });
		m_Direction = RightBack;
	}
	// 左上移動
	else if (KeyLogger_IsPressed(KK_W) && KeyLogger_IsPressed(KK_A))
	{
		move += XMVector3Normalize({ -1.0f,0.0f,0.0f});
		m_Direction = LeftBack;
	}
	// 右下移動
	else if (KeyLogger_IsPressed(KK_S) && KeyLogger_IsPressed(KK_D))
	{
		move += XMVector3Normalize({ 1.0f,0.0f,0.0f });
		m_Direction = RightFront;
	}
	// 左下移動
	else if (KeyLogger_IsPressed(KK_S) && KeyLogger_IsPressed(KK_A))
	{
		move += XMVector3Normalize({ 0.0f,0.0f,-1.0f });
		m_Direction = LeftFront;
	}
	// 上移動
	else if (KeyLogger_IsPressed(KK_W))
	{
		move += front;
		m_Direction = Back;
	}
	// 下移動
	else if (KeyLogger_IsPressed(KK_S))
	{
		move -= front;
		m_Direction = Front;
	}
	// 右移動
	else if (KeyLogger_IsPressed(KK_D))
	{
		move += right;
		m_Direction = Right;
	}
	// 左移動
	else if (KeyLogger_IsPressed(KK_A))
	{
		move -= right;
		m_Direction = Left;
	}

	if (KeyLogger_IsTrigger(KK_SPACE) && m_Jump == false)
	{
		m_Jump = true;
		m_OnGround = false;
		m_Vel.y = JUMP_POWER;  // 初速を与える
	}

	if (!XMVector3Equal(move, XMVectorZero()))
	{
		move = XMVector3Normalize(move) * ((m_Vel.x + m_Vel.z) * 0.5);
	}

	// プレイヤー位置へ適用
	XMFLOAT3 m;
	XMStoreFloat3(&m, move);

	if (m_Shadow.GetIsActive() == false)
	{
		cam->SetPos(XMFLOAT3(cam->GetPos().x + m.x, cam->GetPos().y + m.y, cam->GetPos().z + m.z));
	}

	m_Pos.x += m.x;
	m_Pos.y += m.y;
	m_Pos.z += m.z;
}


// ===============================================
// プレイヤーアクション更新処理
// void Player::UpdateAction(void) 
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Player::UpdateAction(void)
{
	
	return;
}

// ===============================================
// プレイヤーゲッター
// Player* GetPlayer(void)
// 
// 型：Player*			Playerクラスポインター型
// 返り値：&player[0]	Player配列の先頭アドレス
// 引数：無し
// ===============================================
Player* GetPlayer(void)
{
	return player.data();
}

