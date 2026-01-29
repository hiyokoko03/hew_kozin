// ==============================================================
// map.h	
// マップ制御
// 
// 制作者:村上和樹		制作日付：2025/11/04	
// 更新者:村上和樹		更新日付：2025/11/26
// ==============================================================
#ifndef MAP_H
#define MAP_H

#include <DirectXMath.h>
#include <vector>

#include "collision.h"

static constexpr int SHADOW_FLOOR_MAX = 4;

static int g_ShadowOwnerIndex[SHADOW_FLOOR_MAX] = { -1 };

// ===============================================
// クラス宣言
// ===============================================
class Map {
public:
	enum Type {
		WorldBottom = 0,
		Floor,
		Wall1,
		Wall2,
		Wall3,
		Wall4,
		Wall5,
		ShadowFloor
	};

private:
	Type m_Type{ Floor };
	int m_Index;
	bool m_IsUse = false;
	DirectX::XMFLOAT3 m_Pos{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_Size{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT2 TexUV[4]{};
	Collision m_Collision;

public:
	Map() {};
	Map(const bool& IsUse, const DirectX::XMFLOAT3& pos, Type type,const DirectX::XMFLOAT3& size)
		:m_IsUse(IsUse), m_Pos(pos), m_Type(type),m_Size(size)
	{

	}

	void Draw(void)const;

	const int& GetIndex(void) { return m_Index; }
	void SetIndex(int index) { m_Index = index; }

	const bool& GetIsUse(void) const { return m_IsUse; }
	void SetIsUse(bool use) { m_IsUse = use; }

	const DirectX::XMFLOAT3& GetPos(void) const { return m_Pos; }
	void SetPos(DirectX::XMFLOAT3 pos) { m_Pos = pos; }

	const DirectX::XMFLOAT3& GetSize(void) const { return m_Size; }
	void SetSize(DirectX::XMFLOAT3 size) { m_Size = size; }

	const Collision& GetCollision(void) const { return m_Collision; }

	const std::vector<Map>& GetWalls(void);
	const std::vector<Map>& GetFloors(void);
};

void Map_Initialize(void);
void Map_Finalize(void);
void Map_Update(double elapsed_time);
void Map_Draw(void);

void StateFloor_Set(int StageNum);


Map* GetWall(void);
Map* GetFloor(void);

class MapObject;

MapObject* GetObjectInShadowByDistance(const DirectX::XMFLOAT3& playerPos);


#endif