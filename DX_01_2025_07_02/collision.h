// ==============================================================
// collision.h	
// 当たり判定制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/05	
// 更新者:Kazuki Murakami		更新日付：2025/11/05
// ==============================================================
#ifndef COLLISION_H
#define COLLISION_H

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

// --------------------------------------------------------------
// オブジェクトの当たり判定用 AABB 情報
//  - position : 中心座標
//  - size     : 幅・高さ・奥行き
// --------------------------------------------------------------
struct CollisionObject
{
    XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f);
};

// --------------------------------------------------------------
// 当たり判定ユーティリティ
//  - AABB vs AABB
// --------------------------------------------------------------
class Collision
{
public:
    // AABB vs AABB
    static bool IntersectsBoxBox(
        const XMFLOAT3& posA, const XMFLOAT3& sizeA,
        const XMFLOAT3& posB, const XMFLOAT3& sizeB)
    {
        BoundingBox boxA, boxB;

        boxA.Center = posA;
        boxA.Extents = XMFLOAT3(
            sizeA.x * 0.5f,
            sizeA.y * 0.5f,
            sizeA.z * 0.5f
        );

        boxB.Center = posB;
        boxB.Extents = XMFLOAT3(
            sizeB.x * 0.5f,
            sizeB.y * 0.5f,
            sizeB.z * 0.5f
        );

        return boxA.Intersects(boxB);
    }
};

#endif