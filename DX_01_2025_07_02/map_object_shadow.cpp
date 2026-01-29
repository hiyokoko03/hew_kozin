// ==============================================================
// map_object_shadow.cpp	
// マップオブジェクト影描画
// 
// 制作者:Kazuki Murakami		制作日付：2025/12/15	
// 更新者:Kazuki Murakami		更新日付：2025/12/15
// ==============================================================
#include "map_object_shadow.h"

#include "billboard.h"
#include "texture.h"
#include "cube.h"

using namespace DirectX;

namespace
{
    static int g_ObjectShadowTextureID = -1;
}

MapObjectShadow::MapObjectShadow(
    const XMFLOAT3& pos,
    const XMFLOAT3& size
) : m_Pos(pos), m_Size(size)
{
}

bool MapObjectShadow::IsInsideXZ(const XMFLOAT3& p) const
{
    if (!m_IsUse) 
        return false;

    float hx = m_Size.x * 0.5f;
    float hz = m_Size.z * 0.5f;

    return
        p.x >= m_Pos.x - hx && p.x <= m_Pos.x + hx &&
        p.z >= m_Pos.z - hz && p.z <= m_Pos.z + hz;
}

void MapObjectShadow::Initialize()
{
    g_ObjectShadowTextureID = Texture_Load(L"resource/texture/black.png");
}

void MapObjectShadow::Finalize()
{

}

void MapObjectShadow::Update(double elapsed_time)
{

}

void MapObjectShadow::Draw() const
{
    if (!m_IsUse) 
        return;

    XMMATRIX mtxWorld{};
    int MapTextureID = -1;
    XMFLOAT4 Color{ 1.0f,1.0f,1.0f,0.5f };

    mtxWorld = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z) * XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
    MapTextureID = g_ObjectShadowTextureID;

    Cube_Draw(MapTextureID, mtxWorld, Color);
}

void MapObjectShadow::SetPos(const XMFLOAT3& pos)
{
    m_Pos = pos;
}

const XMFLOAT3& MapObjectShadow::GetPos() const
{
    return m_Pos;
}
