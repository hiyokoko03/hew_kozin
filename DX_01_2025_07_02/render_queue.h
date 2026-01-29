// ==============================================================
// render_queue.h	
// 描画順制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/24	
// 更新者:Kazuki Murakami		更新日付：2025/11/24
// ==============================================================
#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <vector>
#include <functional>
#include <algorithm>
#include <DirectXMath.h>

enum class RenderLayer
{
    Opaque,      // 不透明（ZWrite ON）
    Transparent  // 半透明（ZWrite OFF）
};

struct RenderItem
{
    DirectX::XMFLOAT3 pos;         // 3D 位置
    float distanceToCamera;        // カメラ距離
    RenderLayer layer;
    std::function<void()> drawFunc;
};

class RenderQueue
{
public:
    void Clear();

    void Add(
        const DirectX::XMFLOAT3& pos,
        RenderLayer layer,
        std::function<void()> func);

    // カメラ位置を与えて描画
    void DrawAll(const DirectX::XMFLOAT3& cameraPos, const DirectX::XMMATRIX& view);

private:
    std::vector<RenderItem> m_items;
};


#endif // RENDER_QUEUE_H