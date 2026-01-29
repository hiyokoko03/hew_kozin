// ==============================================================
// background.cpp
// Direct3D設定全般
// 
// 制作者:石沢山		    制作日付：2025/10/29	
// 更新者:Kazuki Murakami	更新日付：2025/11/05
// ==============================================================
#include "background.h"

#include <DirectXMath.h>
#include <array>

#include "texture.h"
#include "sprite.h"
using namespace DirectX;

// ===============================================
// マクロ定義
// ===============================================
static constexpr float MOVE_SPEED = 0.0f;
static constexpr float JUMP_SPEED = 0.0f;
static constexpr float IDLE_SIZE_X = 1920.0f;
static constexpr float IDLE_SIZE_Y = 1080.0f;

// ===============================================
// グローバル変数
// ===============================================
std::array<Background, BACKGROUND_MAX> background;

// ===============================================
// 背景初期化処理
// void Background::Initialize(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Background::Initialize(void)
{
	m_TextureID = Texture_Load(L"resource/texture/background.jpg"); // テクスチャ読み込み
    m_Use = true;                                                   // 使用フラグ
    m_Pos = XMFLOAT3(960.0f, 540.0f,1.0f);                               // 座標
    m_Vel = XMFLOAT3(MOVE_SPEED, JUMP_SPEED,1.0f);                       // 移動速度
    m_Size = XMFLOAT3(IDLE_SIZE_X, IDLE_SIZE_Y,1.0f);                    // サイズ
    
    return;
}

// ===============================================
// 背景終了処理
// void Background::Finalize(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Background::Finalize(void)
{
    return;
}

// ===============================================
// 背景更新処理
// void Background::Update(double elpased_time)
// 
// 型：void
// 返り値：無し
// 引数：elpased_time     double型     経過時間
// ===============================================
void Background::Update(double elpased_time)
{
    if (m_Use == true)
    {
        // 位置を更新
        m_Pos.x += m_Vel.x;
        m_Pos.y += m_Vel.y;

        // 画面外に出たら位置をリセット
        if (m_Pos.x <= -m_Size.x || m_Pos.x >= m_Size.x)
        {
            m_Pos.x = 0.0f;
        }
    }

    return;
}

// ===============================================
// 背景描画処理
// void Background::Draw(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Background::Draw(void)
{
    if (m_Use == true)
    {
        Sprite_Draw(m_TextureID, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y,
            false, 0.0f, XMFLOAT2{ 0.0f,0.0f }, XMFLOAT2{ 1.0f,0.0f }, XMFLOAT2{ 0.0f,1.0f }, XMFLOAT2{ 1.0f,1.0f });
    }

    return;
}

// ===============================================
// 背景ゲッター
// Background* Background::GetBackground(void)
// 
// 型：Background*		    Backgroundクラスポインター型
// 返り値：&background[0]	 Background配列の先頭アドレス
// 引数：無し
// ===============================================
Background* Background::GetBackground(void)
{
    // 背景データへのポインタを返す
    return &background[0];
}