// ==============================================================
// map_object.cpp	
// マップオブジェクト描画
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/22	
// 更新者:Kazuki Murakami		更新日付：2025/11/22
// ==============================================================
#include "map_object.h"

#include <array>
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include <cfloat>

#include "game.h"
#include "direct3D.h"
#include "debug_camera.h"
#include "active_camera.h"
#include "texture.h"
#include "model.h"
#include "cube.h"
#include "billboard.h"
#include "easing.h"

using namespace DirectX;

struct ObjectMove
{
	bool moving = false;
	double elapsed = 0.0;

	XMFLOAT3 ObjectStartPos{};
	XMFLOAT3 ObjectTargetPos{};
	XMFLOAT3 ShadowStartPos{};
	XMFLOAT3 ShadowTargetPos{};
	XMFLOAT3 CameraStartPos{};
	XMFLOAT3 CameraTargetPos{};
};

namespace
{
	constexpr int OBJECT_MODEL_MAX = 14;
	constexpr double MOVE_TIME = 5.0;   // 移動にかかる時間（秒）
	constexpr float MOVE_DISTANCE = 1.0f;

	int g_CollisionTexture{};
	ObjectMove g_ObjectMove{};

	MODEL* g_pObjectModel[OBJECT_MODEL_MAX] = {};
}

std::array<std::vector<MapObject>, STAGE_MAX> g_MapObjects = {
	std::vector<MapObject>
	{
		// 位置, タイプ, サイズ, 回転,当たり判定サイズ,当たり判定オフセット		※影のあるオブジェクトを最初に書く
		MapObject({ -2.6f,-0.1f,12.0f},MapObject::Shelf,{ 0.15f,0.15f,0.15f }, {0.0f,270.0f,0.0f},{ 0.5f,2.5f,2.0f },{ 0.0f,0.9f,0.0f }),
		MapObject({ -12.3f,0.0f,1.4f},MapObject::Stair,{ 0.15f,0.15f,0.15f }, {0.0f,90.0f,0.0f},{ 0.85f,0.85f,1.3f },{ 0.03f,0.57f,-0.15f }),
		MapObject({ -2.5f,0.05f,7.0f},MapObject::Box,{ 0.3f,0.3f,0.3f }, {0.0f,0.0f,0.0f},{ 0.6f,1.2f,1.3f },{ 0.01f,0.65f,0.0f }),
		MapObject({ -2.5f,1.25f,7.1f},MapObject::Stair,{ 0.15f,0.15f,0.15f }, {0.0f,90.0f,0.0f},{ 0.85f,0.85f,1.3f },{ 0.03f,0.57f,-0.15f }),
		MapObject({ -14.8f,0.0f,0.0f},MapObject::LeftDoor,{ 0.4f,0.4f,0.4f }, {0.0f,270.0f,0.0f},{},{}),
		MapObject({ 0.0f,0.0f,20.8f },MapObject::RightDoor,{ 0.4f,0.4f,0.4f }, {0.0f,0.0f,0.0f},{},{}),
	},

	std::vector<MapObject>
	{
		MapObject({ -2.6f,-0.1f,12.0f},MapObject::Shelf,{ 0.15f,0.15f,0.15f }, {0.0f,270.0f,0.0f},{ 0.5f,2.5f,2.0f },{ 0.0f,0.9f,0.0f }),
		MapObject({ -12.3f,0.0f,1.4f},MapObject::Stair,{ 0.15f,0.15f,0.15f }, {0.0f,90.0f,0.0f},{ 0.85f,0.85f,1.3f },{ 0.03f,0.57f,-0.15f }),

		MapObject({ -14.8f,0.0f,0.0f},MapObject::LeftDoor,{ 0.4f,0.4f,0.4f }, {0.0f,270.0f,0.0f},{},{}),
		MapObject({ 0.0f,0.0f,20.8f },MapObject::RightDoor,{ 0.4f,0.4f,0.4f }, {0.0f,0.0f,0.0f},{},{}),
	},
	
	std::vector<MapObject>
	{
		MapObject({ -2.6f,-0.1f,12.0f},MapObject::Shelf,{ 0.15f,0.15f,0.15f }, {0.0f,270.0f,0.0f},{ 0.5f,2.5f,2.0f },{ 0.0f,0.9f,0.0f }),
		MapObject({ -12.3f,0.0f,1.4f},MapObject::Stair,{ 0.15f,0.15f,0.15f }, {0.0f,90.0f,0.0f},{ 0.85f,0.85f,1.3f },{ 0.03f,0.57f,-0.15f }),

		MapObject({ -2.5f,0.05f,7.0f},MapObject::Box,{ 0.3f,0.3f,0.3f }, {0.0f,0.0f,0.0f},{ 0.6f,1.2f,1.3f },{ 0.01f,0.65f,0.0f }),
		MapObject({ -2.5f,1.25f,7.1f},MapObject::Stair,{ 0.15f,0.15f,0.15f }, {0.0f,90.0f,0.0f},{ 0.85f,0.85f,1.3f },{ 0.03f,0.57f,-0.15f }),
		MapObject({ -14.8f,0.0f,0.0f},MapObject::LeftDoor,{ 0.4f,0.4f,0.4f }, {0.0f,270.0f,0.0f},{},{}),
		MapObject({ 0.0f,0.0f,20.8f },MapObject::RightDoor,{ 0.4f,0.4f,0.4f }, {0.0f,0.0f,0.0f},{},{}),
	},

	std::vector<MapObject>
	{
		MapObject({ -2.6f,-0.1f,12.0f},MapObject::Shelf,{ 0.15f,0.15f,0.15f }, {0.0f,270.0f,0.0f},{ 0.5f,2.5f,2.0f },{ 0.0f,0.9f,0.0f }),
		MapObject({ -12.3f,0.0f,1.4f},MapObject::Stair,{ 0.15f,0.15f,0.15f }, {0.0f,90.0f,0.0f},{ 0.85f,0.85f,1.3f },{ 0.03f,0.57f,-0.15f }),

		MapObject({ -2.5f,0.05f,7.0f},MapObject::Box,{ 0.3f,0.3f,0.3f }, {0.0f,0.0f,0.0f},{ 0.6f,1.2f,1.3f },{ 0.01f,0.65f,0.0f }),
		MapObject({ -2.5f,1.25f,7.1f},MapObject::Stair,{ 0.15f,0.15f,0.15f }, {0.0f,90.0f,0.0f},{ 0.85f,0.85f,1.3f },{ 0.03f,0.57f,-0.15f }),
		MapObject({ -14.8f,0.0f,0.0f},MapObject::LeftDoor,{ 0.4f,0.4f,0.4f }, {0.0f,270.0f,0.0f},{},{}),
		MapObject({ 0.0f,0.0f,20.8f },MapObject::RightDoor,{ 0.4f,0.4f,0.4f }, {0.0f,0.0f,0.0f},{},{}),
	},

};

MapObject::MapObject(const MapObject& other)
	: m_IsUse(other.m_IsUse)
	, m_ID(other.m_ID)
	, m_Type(other.m_Type)
	, m_Pos(other.m_Pos)
	, m_Size(other.m_Size)
	, m_Rotation(other.m_Rotation)
	, m_CollisionSize(other.m_CollisionSize)
	, m_CollisionOffset(other.m_CollisionOffset)
	, m_CollisionPos(other.m_CollisionPos)
	, m_ShadowOffsets(other.m_ShadowOffsets)
	, m_ActiveShadowIndex(other.m_ActiveShadowIndex)
{
	// Shadow実体は再生成する
	for (size_t i = 0; i < m_ShadowOffsets.size(); ++i)
	{
		const auto& off = m_ShadowOffsets[i];
		XMFLOAT3 pos{
			m_Pos.x + off.x,
			m_Pos.y + off.y,
			m_Pos.z + off.z
		};

		m_pShadows.emplace_back(
			std::make_unique<MapObjectShadow>(pos, XMFLOAT3{ 1,1,1 })
		);
	}
}

MapObject& MapObject::operator=(const MapObject& other)
{
	if (this == &other) 
		return *this;

	m_IsUse = other.m_IsUse;
	m_ID = other.m_ID;
	m_Type = other.m_Type;
	m_Pos = other.m_Pos;
	m_Size = other.m_Size;
	m_Rotation = other.m_Rotation;
	m_CollisionSize = other.m_CollisionSize;
	m_CollisionOffset = other.m_CollisionOffset;
	m_CollisionPos = other.m_CollisionPos;

	m_ShadowOffsets = other.m_ShadowOffsets;
	m_ShadowPos = other.m_ShadowPos;
	m_ActiveShadowIndex = other.m_ActiveShadowIndex;

	m_pShadows.clear(); // 影は持ち越さない

	for (size_t i = 0; i < m_ShadowOffsets.size(); ++i)
	{
		m_pShadows.emplace_back(
			std::make_unique<MapObjectShadow>(
				m_ShadowPos[i], XMFLOAT3{ 1,1,1 }
			)
		);
	}

	return *this;
}

MapObject::MapObject(const DirectX::XMFLOAT3& pos, Type type, const DirectX::XMFLOAT3& size, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& CollisionSize, const DirectX::XMFLOAT3& CollisonOffset)
	:m_Pos(pos), m_Type(type), m_Size(size), m_Rotation(rotation), m_CollisionSize(CollisionSize), m_CollisionOffset(CollisonOffset)
{

}

MapObject::~MapObject()
{

}

void MapObject::Initialize(void)
{
	Game* g_pGame = GetGame();

	g_CollisionTexture = Texture_Load(L"resource/texture/object_collision.png");

	g_pObjectModel[0] = ModelLoad("resource/model/01sofa.fbx");
	g_pObjectModel[1] = ModelLoad("resource/model/02chair.fbx");
	g_pObjectModel[2] = ModelLoad("resource/model/04coatstand.fbx");
	g_pObjectModel[3] = ModelLoad("resource/model/05plant.fbx");
	g_pObjectModel[4] = ModelLoad("resource/model/06clock.fbx");
	g_pObjectModel[5] = ModelLoad("resource/model/06shelf.fbx");
	g_pObjectModel[6] = ModelLoad("resource/model/07tv.fbx");
	g_pObjectModel[7] = ModelLoad("resource/model/08vase.fbx");
	g_pObjectModel[8] = ModelLoad("resource/model/09umbrellastand.fbx");
	g_pObjectModel[9] = ModelLoad("resource/model/10table.fbx");
	g_pObjectModel[10] = ModelLoad("resource/model/leftdoor.fbx");
	g_pObjectModel[11] = ModelLoad("resource/model/rightdoor.fbx");
	g_pObjectModel[12] = ModelLoad("resource/model/step_02.fbx");
	g_pObjectModel[13] = ModelLoad("resource/model/box.fbx");

	for (size_t i = 0; i < g_MapObjects[g_pGame->GetStageNumber()].size(); i++)
	{
		g_MapObjects[g_pGame->GetStageNumber()][i].SetID((int)i);
		g_MapObjects[g_pGame->GetStageNumber()][i].m_IsUse = true;
		g_MapObjects[g_pGame->GetStageNumber()][i].m_CollisionPos = { 
			g_MapObjects[g_pGame->GetStageNumber()][i].m_Pos.x + g_MapObjects[g_pGame->GetStageNumber()][i].m_CollisionOffset.x,
			g_MapObjects[g_pGame->GetStageNumber()][i].m_Pos.y + g_MapObjects[g_pGame->GetStageNumber()][i].m_CollisionOffset.y,
			g_MapObjects[g_pGame->GetStageNumber()][i].m_Pos.z + g_MapObjects[g_pGame->GetStageNumber()][i].m_CollisionOffset.z
		};
	}

	// 影の初期化より前にAddShadowを呼び出す
	auto& objects = g_MapObjects[g_pGame->GetStageNumber()];

	//						影の位置				影のサイズ
	objects[0].AddShadow({ 0.75f,0.2f,0.0f },	{ 1.0f,0.1f,2.0f },true);
	objects[0].AddShadow({ 2.0f,0.2f,2.0f }, { 1.0f,0.1f,1.3f },false);
	objects[1].AddShadow({ 1.0f,0.1f,0.0f }, { 1.0f,0.1f,1.3f }, true);

	// 影の初期化
	for (auto& obj : g_MapObjects[g_pGame->GetStageNumber()])
	{
		for (auto& s : obj.m_pShadows)
		{
			s->Initialize();
		}
	}
}

void MapObject::Finalize(void)
{
	Game* g_pGame = GetGame();

	for (int i = 0; i < OBJECT_MODEL_MAX; i++)
	{
		if (g_pObjectModel[i])
		{
			ModelRelease(g_pObjectModel[i]);
			g_pObjectModel[i] = nullptr;
		}
	}

	for (auto& obj : g_MapObjects[g_pGame->GetStageNumber()])
	{
		for (auto& s : obj.m_pShadows)
		{
			s->Finalize();
		}
	}
}

void MapObject::Update(double elapsed_time)
{
	Game* g_pGame = GetGame();

	// 当たり判定位置更新
	m_CollisionPos = {
		m_Pos.x + m_CollisionOffset.x,
		m_Pos.y + m_CollisionOffset.y,
		m_Pos.z + m_CollisionOffset.z
	};

	// 影の位置更新
	for (size_t i = 0; i < m_pShadows.size(); i++)
	{
		m_ShadowPos[i] = {
			m_Pos.x + m_ShadowOffsets[i].x,
			m_Pos.y + m_ShadowOffsets[i].y,
			m_Pos.z + m_ShadowOffsets[i].z
		};
		m_pShadows[i]->SetPos(m_ShadowPos[i]);
	}

	for (auto& obj : g_MapObjects[g_pGame->GetStageNumber()])
	{
		for (auto& s : obj.m_pShadows)
		{
			s->Update(elapsed_time);
		}
	}
}

void MapObject::Draw(void)const
{
	Game* g_pGame = GetGame();

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

	switch(m_Type)
	{
	case Sofa:
		ModelDraw(g_pObjectModel[0], WorldMatrix);
		break;

	case Chair:
		ModelDraw(g_pObjectModel[1], WorldMatrix);
		break;

	case CoatStand:
		ModelDraw(g_pObjectModel[2], WorldMatrix);
		break;

	case Plant:
		ModelDraw(g_pObjectModel[3], WorldMatrix);
		break;

	case Clock:
		ModelDraw(g_pObjectModel[4], WorldMatrix);
		break;

	case Shelf:
		ModelDraw(g_pObjectModel[5], WorldMatrix);

		if(g_pGame->GetGameMode() == DebugMode)
		{
			Cube_Draw(g_CollisionTexture, CollisionMatrix, { 1.0f,1.0f,1.0f,0.5f });
		}
		break;

	case TV:
		ModelDraw(g_pObjectModel[6], WorldMatrix);
		break;

	case Vase:
		ModelDraw(g_pObjectModel[7], WorldMatrix);
		break;

	case UmbrellaStand:
		ModelDraw(g_pObjectModel[8], WorldMatrix);
		break;

	case Table:
		ModelDraw(g_pObjectModel[9], WorldMatrix);
		break;

	case Stair:
		XMMATRIX Box1CollisionMatrix = XMMatrixScaling(m_CollisionSize.x, m_CollisionSize.y / 2, m_CollisionSize.z) * XMMatrixTranslation(m_CollisionPos.x, m_CollisionPos.y - 0.3f, m_CollisionPos.z);
		XMMATRIX Box2CollisionMatrix = XMMatrixScaling(m_CollisionSize.x, m_CollisionSize.y / 2, m_CollisionSize.z / 2) * XMMatrixTranslation(m_CollisionPos.x, m_CollisionPos.y + 0.15f, m_CollisionPos.z + 0.3f);

		ModelDraw(g_pObjectModel[12], WorldMatrix);
		if (g_pGame->GetGameMode() == DebugMode)
		{
			Cube_Draw(g_CollisionTexture, Box1CollisionMatrix, { 1.0f,1.0f,1.0f,0.5f });
			Cube_Draw(g_CollisionTexture, Box2CollisionMatrix, { 1.0f,1.0f,1.0f,0.5f });
		}
		break;

	case Box:
		ModelDraw(g_pObjectModel[13], WorldMatrix);
		if (g_pGame->GetGameMode() == DebugMode)
		{
			Cube_Draw(g_CollisionTexture, CollisionMatrix, { 1.0f,1.0f,1.0f,0.5f });
		}
		break;

	case LeftDoor:
		ModelDraw(g_pObjectModel[10], WorldMatrix);
		break;

	case RightDoor:
		ModelDraw(g_pObjectModel[11], WorldMatrix);
		break;
	}

	// 影描画
	for (auto& obj : g_MapObjects[g_pGame->GetStageNumber()])
	{
		for (auto& s : obj.m_pShadows)
		{
			s->Draw();
		}
	}
}

const DirectX::XMFLOAT3 MapObject::GetShadowPos() const
{
	if (m_ShadowOffsets.empty())
		return m_Pos;

	if (m_ActiveShadowIndex < 0 ||
		m_ActiveShadowIndex >= (int)m_ShadowOffsets.size())
	{
		return m_Pos;
	}

	const auto& off = m_ShadowOffsets[m_ActiveShadowIndex];

	return {
		m_Pos.x + off.x,
		m_Pos.y + off.y,
		m_Pos.z + off.z
	};
}

void MapObject::AddShadow(const XMFLOAT3& offset, const XMFLOAT3& size, bool use)
{
	m_ShadowOffsets.emplace_back(offset);

	XMFLOAT3 worldPos{
		m_Pos.x + offset.x,
		m_Pos.y + offset.y,
		m_Pos.z + offset.z
	};

	m_ShadowPos.emplace_back(worldPos);   

	auto shadow = std::make_unique<MapObjectShadow>(worldPos, size);
	shadow->SetUse(use); 

	m_pShadows.emplace_back(std::move(shadow));

	if (m_ActiveShadowIndex == -1)
	{
		m_ActiveShadowIndex = 0;
	}
}

bool MapObject::IsPlayerInShadow(const XMFLOAT3& playerPos) const
{
	for (size_t i = 0; i < m_pShadows.size(); i++)
	{
		if (m_pShadows[i]->IsInsideXZ(playerPos))
		{
			const_cast<MapObject*>(this)->m_ActiveShadowIndex = (int)i;
			return true;
		}
	}
	return false;
}

MapObject* GetObjectInShadow(const XMFLOAT3& playerPos)
{
	Game* g_pGame = GetGame();
	auto& objects = g_MapObjects[g_pGame->GetStageNumber()];

	for (auto& obj : objects)
	{
		if (obj.HasShadow() && obj.IsPlayerInShadow(playerPos))
		{
			return &obj;
		}
	}
	return nullptr;
}

bool MapObject::HasShadow() const
{
	return !m_pShadows.empty();
}

const std::vector<MapObject>& MapObject::GetMapObjects(void)
{
	Game* g_pGame = GetGame();
	return g_MapObjects[g_pGame->GetStageNumber()];
}

MapObject* GetMapObject(void)
{
	Game* g_pGame = GetGame();

	return  g_MapObjects[g_pGame->GetStageNumber()].data();
}