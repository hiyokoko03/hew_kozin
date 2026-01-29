// ==============================================================
// map_object_shadow.h	
// マップオブジェクト影描画
// 
// 制作者:Kazuki Murakami		制作日付：2025/12/15	
// 更新者:Kazuki Murakami		更新日付：2026/01/27
// ==============================================================
#ifndef MAP_OBJECT_SHADOW_H
#define MAP_OBJECT_SHADOW_H
#include <DirectXMath.h>

class MapObjectShadow
{
private:
    bool m_IsUse = true;
    DirectX::XMFLOAT3 m_Pos{};
    DirectX::XMFLOAT3 m_Size{};

public:
    MapObjectShadow(
        const DirectX::XMFLOAT3& pos,
        const DirectX::XMFLOAT3& size
    );

    void Initialize();
    void Finalize();
    void Update(double elapsed_time);
    void Draw() const;

    bool IsInsideXZ(const DirectX::XMFLOAT3& p) const;

    void SetPos(const DirectX::XMFLOAT3& pos);
    const DirectX::XMFLOAT3& GetPos() const;

    void SetUse(bool use) { m_IsUse = use; }
    bool IsUse() const { return m_IsUse; }

};
#endif
