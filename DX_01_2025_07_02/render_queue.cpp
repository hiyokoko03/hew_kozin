// ==============================================================
// render_queue.cpp	
// 描画順制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/24	
// 更新者:Kazuki Murakami		更新日付：2025/11/24
// ==============================================================
#include "render_queue.h"

#include "direct3D.h"
#include <cmath>

using namespace DirectX;

void RenderQueue::Clear()
{
    m_items.clear();
}

void RenderQueue::Add(
    const DirectX::XMFLOAT3& pos,
    RenderLayer layer,
    std::function<void()> func)
{
    RenderItem item;
    item.pos = pos;
    item.layer = layer;
    item.drawFunc = func;
    item.distanceToCamera = 0.0f; // 後で更新

    m_items.push_back(item);
}

void RenderQueue::DrawAll(const DirectX::XMFLOAT3& cameraPos, const DirectX::XMMATRIX& view)
{
    std::vector<RenderItem> opaques;
    std::vector<RenderItem> transparents;

    // ビュー空間Zを計算（透明物のみ使う）
    for (auto& item : m_items)
    {
        XMVECTOR wp = XMLoadFloat3(&item.pos);
        XMVECTOR vp = XMVector3TransformCoord(wp, view);
        item.distanceToCamera = XMVectorGetZ(vp);

        if (item.layer == RenderLayer::Opaque)
            opaques.push_back(item);
        else
            transparents.push_back(item);
    }

    // 不透明はソートしない（必要なら少しだけ近→遠でもよい）
    // std::sort(opaques.begin(), opaques.end(), ... ); 
    // ← 絶対にやらないほうが安全

    // 透明のみソート
    std::sort(transparents.begin(), transparents.end(),
        [](auto& a, auto& b)
        {
            return a.distanceToCamera > b.distanceToCamera; // 遠→近
        });

    // --------------------
    // 描画
    // --------------------
    Direct3D_SetZWrite(true);
    for (auto& item : opaques)
        item.drawFunc();

    Direct3D_SetZWrite(false);
    for (auto& item : transparents)
        item.drawFunc();

    Direct3D_SetZWrite(true);
}