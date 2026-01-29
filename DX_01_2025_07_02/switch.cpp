// ==============================================================
// switch.cpp	
// スイッチ制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/12/15	
// 更新者:Kazuki Murakami		更新日付：2025/12/15
// ==============================================================
#include "switch.h"

#include <DirectXMath.h>
#include <array>

#include "game.h"
#include "model.h"
#include "texture.h"
#include "cube.h"
#include "player.h"
#include "collision.h"
#include "lamp.h"
#include "map_object.h"
#include "map_object_shadow.h"

using namespace DirectX;

namespace
{
	static int g_CollisionTexture[2]{};

	Player* pPlayer = GetPlayer();
	Lamp* pLamp = GetLamp();
	
	MapObject* pMapObject = GetMapObject();
	//MapObjectShadow* pMapObjectShadow = GetMapObjectShadow();
}
static MODEL* g_pSwitchModel = {};

std::array<Switch, SWITCH_MAX> g_Switch = {
	// 位置, サイズ, 回転,当たり判定サイズ,当たり判定オフセット
	Switch(false,{ -7.8f,0.0f,2.8f},{ 0.15f,0.15f,0.15f }, {0.0f,0.0f,0.0f},{ 0.4f,0.6f,0.4f },{ 0.0f,0.3f,0.0f }),
	Switch(false,{-2.8f,2.25f,7.5f},{ 0.15f,0.15f,0.15f }, {0.0f,270.0f,0.0f},{ 0.4f,0.6f,0.4f },{ 0.0f,0.3f, 0.0f }),
};

void Switch::Initialize(void)
{
	for (int i = 0; i < SWITCH_MAX; i++)
	{
		g_Switch[i].SetID(i);
		g_Switch[i].m_CollisionPos = {
			g_Switch[i].m_Pos.x + g_Switch[i].m_CollisionOffset.x,
			g_Switch[i].m_Pos.y + g_Switch[i].m_CollisionOffset.y,
			g_Switch[i].m_Pos.z + g_Switch[i].m_CollisionOffset.z
		};
	}

	g_CollisionTexture[0] = Texture_Load(L"resource/texture/object_collision.png");

	g_CollisionTexture[1] = Texture_Load(L"resource/texture/look.png");

	g_pSwitchModel = ModelLoad("resource/model/switch.fbx");
}

void Switch::Finalize(void)
{

}

void Switch::Update(double elapsed_time)
{
	m_CollisionPos = {
			m_Pos.x + m_CollisionOffset.x,
			m_Pos.y + m_CollisionOffset.y,
			m_Pos.z + m_CollisionOffset.z
	};

	
}

void Switch::Draw(void)const
{
	Game* pGame = GetGame();

	// スケール行列 (幅, 高さ, 奥行き)
	XMMATRIX ScaleMatrix = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
	XMMATRIX CollisionScaleMatrix = XMMatrixScaling(m_CollisionSize.x, m_CollisionSize.y, m_CollisionSize.z);

	XMMATRIX RotationMatrix = XMMatrixRotationX(XMConvertToRadians(m_Rotation.x)) * XMMatrixRotationY(XMConvertToRadians(m_Rotation.y)) * XMMatrixRotationZ(XMConvertToRadians(m_Rotation.z));

	// 移動行列
	XMMATRIX TranslationMatrix = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	XMMATRIX CollisionTranslationMatrix = XMMatrixTranslation(m_CollisionPos.x, m_CollisionPos.y, m_CollisionPos.z);

	// ワールド行列の合成
	XMMATRIX WorldMatrix = ScaleMatrix * RotationMatrix * TranslationMatrix;
	XMMATRIX CollisionMatrix = CollisionScaleMatrix * CollisionTranslationMatrix;

	Direct3D_SetZEnable(true);
	Direct3D_SetZWrite(true);
	Direct3D_SetBlendMode(BLEND_TRANSPARENT);

	ModelDraw(g_pSwitchModel, WorldMatrix);

	if (!m_IsUse)
	{
		if (pGame->GetGameMode() == DebugMode)
		{
			Cube_Draw(g_CollisionTexture[0], CollisionMatrix, { 1.0f,1.0f,1.0f,0.5f });
		}
	}
	else
	{
		if (pGame->GetGameMode() == DebugMode)
		{
			Cube_Draw(g_CollisionTexture[1], CollisionMatrix, { 1.0f,1.0f,1.0f,0.5f });
		}
	}
}

Switch* GetSwitch()
{
	return g_Switch.data();
}