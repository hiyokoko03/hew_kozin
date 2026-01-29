// ==============================================================
// billboard.cpp	
// 板ポリゴン描画
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/18	
// 更新者:Kazuki Murakami		更新日付：2025/11/18
// ==============================================================
#include "billboard.h"

#include <DirectXMath.h>
#include <d3d11.h>

#include "direct3D.h"
#include "debug_camera.h" 
#include "shader3D.h"
#include "texture.h"
#include "debug_ostream.h"

using namespace DirectX;

// 頂点構造体
struct Vertex3D {
	XMFLOAT3 position;	// 頂点座標
	XMFLOAT3 normal;	// 法線ベクトル
	XMFLOAT4 color;	    //カラー
	XMFLOAT2 texcood;	//テクスチャ座標
};

static constexpr int NUM_VERTEX = 4;	// 頂点数 (四角形)
static constexpr int NUM_INDEX = 6;	// インデックス数 (三角形2つ)

static ID3D11Buffer* g_pBillboardVertex3DBuffer = nullptr;	 // 頂点バッファ
static ID3D11Buffer* g_pBillboardIndex3DBuffer = nullptr;	 // 頂点バッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static Vertex3D g_BillboardVerTex[NUM_VERTEX] = {
    // 位置			          // 法線 (Z方向を向く) // カラー (白)				  // UV座標
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 左上
    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // 右上
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // 左下
    {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 右下
};

static unsigned short g_BillboardIndex[NUM_INDEX] = {
    0, 1, 2, // 1つ目の三角形 (左上、右上、左下)
    1, 3, 2  // 2つ目の三角形 (右上、右下、左下)
};

void Billborad_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext) {
        hal::dout << "Billboard_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
        return;
    }

    // デバイスとデバイスコンテキストの保存
    g_pDevice = pDevice;
    g_pContext = pContext;

    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;							//DEFAULT はじめに書き込んだら使わない　　DYNAMIC　常に書き換える
    bd.ByteWidth = sizeof(Vertex3D) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;

    //頂点バッファへ流し込むデータの設定
    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = g_BillboardVerTex;

    g_pDevice->CreateBuffer(&bd, &sd, &g_pBillboardVertex3DBuffer);

    // インデックスバッファ生成
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned short) * NUM_INDEX;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    sd.pSysMem = g_BillboardIndex;

    g_pDevice->CreateBuffer(&bd, &sd, &g_pBillboardIndex3DBuffer);
}

void Billborad_Finalize()
{
    // リソース解放
    SAFE_RELEASE(g_pBillboardIndex3DBuffer);
    SAFE_RELEASE(g_pBillboardVertex3DBuffer);
}

void Billborad_Update(double elapsed_time)
{

}

void Billborad_Draw(int texid, const XMMATRIX& mtxWorld, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2 texcood1, DirectX::XMFLOAT2 texcood2, DirectX::XMFLOAT2 texcood3, DirectX::XMFLOAT2 texcood4)
{  
    if (!g_pContext || !g_pBillboardVertex3DBuffer) return;

    // 更新用の頂点データを用意（元の頂点配列をコピーしてUVだけ上書き）
    Vertex3D temp[NUM_VERTEX]{};
    for (int i = 0; i < NUM_VERTEX; ++i)
    {
        temp[i] = g_BillboardVerTex[i];
    }

    temp[0].texcood = texcood1; // 左上
    temp[1].texcood = texcood2; // 右上
    temp[2].texcood = texcood3; // 左下
    temp[3].texcood = texcood4; // 右下

    // 頂点バッファへ書き込む
    D3D11_MAPPED_SUBRESOURCE mapped;
    g_pContext->Map(g_pBillboardVertex3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    
     // バッファサイズは sizeof(Vertex3D) * NUM_VERTEX
     memcpy(mapped.pData, temp, sizeof(temp));
     g_pContext->Unmap(g_pBillboardVertex3DBuffer, 0);

    // 板ポリを描画
    Shader3D_Begin();

    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex3D);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pBillboardVertex3DBuffer, &stride, &offset);
    g_pContext->IASetIndexBuffer(g_pBillboardIndex3DBuffer, DXGI_FORMAT_R16_UINT, 0);

    // ワールド変換行列の設定
    Shader3D_SetWorldMatrix(mtxWorld);

    // プリミティブトポロジ設定
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // テクスチャの設定
    Texture_SetTexture(texid);

    // マテリアルカラーの設定 
    shader3D_SetMaterialDiffuse(color);

    g_pContext->DrawIndexed(NUM_INDEX, 0, 0);
}
