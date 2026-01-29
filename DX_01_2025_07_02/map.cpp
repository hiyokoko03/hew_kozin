// ==============================================================
// map.cpp	
// マップ制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/04	
// 更新者:Kazuki Murakami		更新日付：2025/12/16
// ==============================================================
#include "map.h"

#include <array>
#include <vector>

#include "game.h"
#include "map_object.h"
#include "map_object_shadow.h"
#include "cube.h"
#include "texture.h"
#include "shader3D.h"
#include "Direct3D.h"
#include "billboard.h"

using namespace DirectX;

namespace
{
	constexpr int TEXTURE_MAX = 3;
	constexpr float FLOOR_OFFSET = 2.5f;

	int g_CollisionTextureID = -1;
	int g_MapTexid[TEXTURE_MAX] = { -1 };
};

std::array<std::vector<Map>, STAGE_MAX> g_Floor{};

std::array< std::vector<Map>, STAGE_MAX> g_Walls = {
	std::vector<Map>
	{	// 使用フラグ、座標、タイプ、サイズ
		Map(true,{ -6.0f,1.5f,3.0f }, Map::Wall1, { 6.0f, 3.0f, 0.25f }),
		Map({ true,{ -12.0f,1.5f,3.0f }, Map::Wall1, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ -15.0f,1.5f,0.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,1.5f,6.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,1.5f,12.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,1.5f,18.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{  0.0f,1.5f, 21.0f }, Map::Wall1, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ 0.0f,1.5f,-3.0f }, Map::Wall2, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ -6.0f,1.5f,-3.0f }, Map::Wall2, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ -12.0f,1.5f,-3.0f }, Map::Wall2, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ 3.0f,1.5f,0.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,1.5f,6.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,1.5f,12.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,1.5f,18.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -6.0f,4.5f,3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-12.0f,4.5f,3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-15.0f,4.5f,0.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,4.5f,6.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,4.5f,12.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,4.5f,18.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{  0.0f,4.5f, 21.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ 0.0f,4.5f,-3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-6.0f,4.5f,-3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-12.0f,4.5f,-3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ 3.0f,4.5f,0.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,4.5f,6.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,4.5f,12.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,4.5f,18.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true, {  0.0f,1.5f, 16.0f }, Map::Wall4, { 6.0f, 3.0f, 0.25f } }),
		Map({ false, {  -2.0f,1.5f, 17.0f }, Map::Wall4, { 0.25f, 3.0f, 2.0f } }),
		Map({ false, {  0.5f,1.5f, 18.0f }, Map::Wall4, { 5.0f, 3.0f, 0.25f } }),
		Map({ false, {  0.5f,1.5f, 16.0f }, Map::Wall4, { 5.0f, 3.0f, 0.25f } }),
	},

	std::vector<Map>
	{
		Map(true,{ -6.0f,1.5f,3.0f }, Map::Wall1, { 6.0f, 3.0f, 0.25f }),
		Map({ true,{ -12.0f,1.5f,3.0f }, Map::Wall1, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ -15.0f,1.5f,0.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,1.5f,6.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,1.5f,12.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,1.5f,18.0f }, Map::Wall1, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{  0.0f,1.5f, 21.0f }, Map::Wall1, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{	 0.0f,1.5f,-3.0f }, Map::Wall2, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ -6.0f,1.5f,-3.0f }, Map::Wall2, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ -12.0f,1.5f,-3.0f }, Map::Wall2, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ 3.0f,1.5f,0.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,1.5f,6.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,1.5f,12.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,1.5f,18.0f }, Map::Wall2, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -6.0f,4.5f,3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-12.0f,4.5f,3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-15.0f,4.5f,0.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,4.5f,6.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,4.5f,12.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ -3.0f,4.5f,18.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{  0.0f,4.5f, 21.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ 0.0f,4.5f,-3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-6.0f,4.5f,-3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{-12.0f,4.5f,-3.0f }, Map::Wall3, { 6.0f, 3.0f, 0.25f } }),
		Map({ true,{ 3.0f,4.5f,0.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,4.5f,6.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,4.5f,12.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true,{ 3.0f,4.5f,18.0f }, Map::Wall3, { 0.25f, 3.0f, 6.0f } }),
		Map({ true, {  0.0f,1.5f, 16.0f }, Map::Wall4, { 6.0f, 3.0f, 0.25f } }),
		Map({ false, {  -2.0f,1.5f, 17.0f }, Map::Wall4, { 0.25f, 3.0f, 2.0f } }),
		Map({ false, {  0.5f,1.5f, 18.0f }, Map::Wall4, { 5.0f, 3.0f, 0.25f } }),
		Map({ false, {  0.5f,1.5f, 16.0f }, Map::Wall4, { 5.0f, 3.0f, 0.25f } }),

		Map(true,{ -2.5f,0.25f,2.0f }, Map::Wall5, { 1.0f, 0.5f, 0.25f }),

		Map(true,{ -2.0f,0.25f,2.5f }, Map::Wall5, { 0.25f, 0.5f, 1.0f }),
		Map(true,{ -3.0f,0.25f,2.5f }, Map::Wall5, { 0.25f, 0.5f, 1.0f }),

		Map(true,{ -2.5f,0.5f,3.0f }, Map::Wall5, { 1.0f, 1.0f, 0.25f }),
		Map(true,{ -1.5f,0.5f,3.0f }, Map::Wall5, { 1.0f, 1.0f, 0.25f }),
		Map(true,{ -1.0f,0.5f,3.5f }, Map::Wall5, { 0.25f, 1.0f, 1.0f }),
	},

	std::vector<Map>
	{

	},

	std::vector<Map>
	{

	},
};

void Map_Initialize(void)
{
	Game* g_pGame = GetGame();

	g_MapTexid[0] = Texture_Load(L"resource/texture/black.png");
	g_MapTexid[1] = Texture_Load(L"resource/texture/wall.png");
	g_CollisionTextureID = Texture_Load(L"resource/texture/object_collision.png");

	StateFloor_Set(g_pGame->GetStageNumber());

	for (size_t i = 0; i < g_Walls[g_pGame->GetStageNumber()].size(); i++)
	{
		g_Walls[g_pGame->GetStageNumber()][i].SetIndex(i);
	}
}

void Map_Finalize(void)
{
	
}

void Map_Update(double elapsed_time)
{
	Game* g_pGame = GetGame();
	MapObject* g_pMapObject = GetMapObject();

	switch (g_pGame->GetStageNumber())
	{
	case Stage1:
		
		break;
	}
}

void Map_Draw(void)
{
	Game* g_pGame = GetGame();

	for (size_t i = 0; i < g_Walls[g_pGame->GetStageNumber()].size(); i++)
	{
		g_Walls[g_pGame->GetStageNumber()][i].Draw();
	}

	for (size_t i = 0; i < g_Floor[g_pGame->GetStageNumber()].size(); i++)
	{
		g_Floor[g_pGame->GetStageNumber()][i].Draw();
	}
}

void StateFloor_Set(int StageNum)
{
	switch (StageNum)
	{
	case Stage1:
		// 床配置						使用フラグ、座標、タイプ、サイズ
		g_Floor[Stage1].emplace_back(Map({ true,{ 0.0f,-10.0f,0.0f }, Map::WorldBottom, { 512.0f, 0.1f, 512.0f } }));

		//3つ分
		for (int i = 0; i < 3; i++)
		{
			//6X6で構成されるタイル
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					//X方向にずらしながら
					g_Floor[Stage1].emplace_back(Map({ true,{ (float)j - (FLOOR_OFFSET + 6.0f * i), 0.0f, (float)k - FLOOR_OFFSET }, Map::Floor, { 1.0f, 0.1f, 1.0f } }));
				}
			}
		}

		//3つ分(原点の分を除いて1から)
		for (int i = 1; i < 3; i++)
		{
			//6X6で構成されるタイル
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					//Z方向にずらしながら
					g_Floor[Stage1].emplace_back(Map({ true,{ (float)j - FLOOR_OFFSET, 0.0f, (float)k - (FLOOR_OFFSET - 6.0f * i) }, Map::Floor, { 1.0f, 0.1f, 1.0f } }));
				}
			}
		}

		//穴あき床タイル
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				if (k == 1 || k == 2)
				{
					continue;
				}

				//Z方向にずらしながら
				g_Floor[Stage1].emplace_back(Map({ true,{ (float)j - FLOOR_OFFSET, 0.0f, (float)k - (FLOOR_OFFSET - 6.0f * 3.0f) }, Map::Floor, { 1.0f, 0.1f, 1.0f } }));
			}
		}
		break;

	case Stage2:
		// 床配置						使用フラグ、座標、タイプ、サイズ
		g_Floor[Stage2].emplace_back(Map({ true,{ 0.0f,-10.0f,0.0f }, Map::WorldBottom, { 512.0f, 0.1f, 512.0f } }));

		//3つ分
		for (int i = 0; i < 3; i++)
		{
			//6X6で構成されるタイル
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					//X方向にずらしながら
					g_Floor[Stage2].emplace_back(Map({ true,{ (float)j - (FLOOR_OFFSET + 6.0f * i), 0.0f, (float)k - FLOOR_OFFSET }, Map::Floor, { 1.0f, 0.1f, 1.0f } }));
				}
			}
		}

		//3つ分(原点の分を除いて1から)
		for (int i = 1; i < 3; i++)
		{
			//6X6で構成されるタイル
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					//Z方向にずらしながら
					g_Floor[Stage2].emplace_back(Map({ true,{ (float)j - FLOOR_OFFSET, 0.0f, (float)k - (FLOOR_OFFSET - 6.0f * i) }, Map::Floor, { 1.0f, 0.1f, 1.0f } }));
				}
			}
		}

		//穴あき床タイル
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				if (k == 1 || k == 2)
				{
					continue;
				}

				//Z方向にずらしながら
				g_Floor[Stage2].emplace_back(Map({ true,{ (float)j - FLOOR_OFFSET, 0.0f, (float)k - (FLOOR_OFFSET - 6.0f * 3.0f) }, Map::Floor, { 1.0f, 0.1f, 1.0f } }));
			}
		}
		break;
	}
}

void Map::Draw(void) const
{
	if (m_IsUse == false)
		return;
	int MapTextureID = -1;
	XMMATRIX mtxWorld{};
	XMFLOAT4 Color{1.0f,1.0f,1.0f,1.0f};
	XMFLOAT4 ChangeColor{ 1.0f,1.0f,1.0f,1.0f };

	switch (m_Type)
	{
	case Map::WorldBottom:
	{
		MapTextureID = g_MapTexid[0];
		mtxWorld =  XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
		Color = { 1.0f,1.0f,1.0f,0.0f };
		ChangeColor = { 1.0f,1.0f,1.0f,0.0f };
		break;
	}
	case Map::Floor:
	{
		MapTextureID = g_MapTexid[1];
		mtxWorld =  XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
		break;
	}
	case  Map::Wall1:
	{
		MapTextureID = g_MapTexid[1];
		mtxWorld = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
		ChangeColor = { 0.0f,0.0f,0.0f,1.0f };
		break;
	}
	case  Map::Wall2:
	{
		Direct3D_SetZEnable(true);
		Direct3D_SetZWrite(false);

		MapTextureID = g_MapTexid[1];
		mtxWorld = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
		Color = { 1.0f,1.0f,1.0f,0.5f };
		ChangeColor = { 1.0f, 1.0f, 1.0f, 0.5f };
		break;
	}
	case Map::Wall3:
	{
		Direct3D_SetZEnable(true);
		Direct3D_SetZWrite(false);

		MapTextureID = g_MapTexid[1];
		mtxWorld = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
		Color = { 1.0f,1.0f,1.0f,0.0f };
		ChangeColor = { 1.0f, 1.0f, 1.0f, 0.0f };
		break;
	}
	case Map::Wall4:
	{
		Direct3D_SetZEnable(true);
		Direct3D_SetZWrite(false);

		MapTextureID = g_MapTexid[1];
		mtxWorld = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
		Color = { 1.0f,1.0f,1.0f,0.0f };
		ChangeColor = { 1.0f, 1.0f, 1.0f, 0.0f };
		break;
	}
	case Map::Wall5:
	{
		MapTextureID = g_MapTexid[1];
		mtxWorld = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
		ChangeColor = { 1.0f,1.0f,1.0f,1.0f };
		break;
	}
	default:
		break;

	}

	Cube_Draw(MapTextureID, mtxWorld, TOP, Color, ChangeColor);

	Direct3D_SetDepthStencilState(false);
	Direct3D_SetDepthStencilState(true);
}

const std::vector<Map>& Map::GetWalls(void)
{
	Game* g_pGame = GetGame();
	return g_Walls[g_pGame->GetStageNumber()];
}

const std::vector<Map>& Map::GetFloors(void)
{
	Game* g_pGame = GetGame();
	return g_Floor[g_pGame->GetStageNumber()];
}

Map* GetWall(void)
{
	Game* g_pGame = GetGame();
	return g_Walls[g_pGame->GetStageNumber()].data();
}

Map* GetFloor(void)
{
	Game* g_pGame = GetGame();
	return g_Floor[g_pGame->GetStageNumber()].data();
}
