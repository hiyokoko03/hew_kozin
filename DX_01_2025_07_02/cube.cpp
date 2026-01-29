// ==============================================================
// cube.cpp	
// 3Dオブジェクト描画
// 
// 制作者:村上和樹		制作日付：2025/10/16	
// 更新者:村上和樹		更新日付：2025/11/26
// ==============================================================
#include <DirectXMath.h>
#include "cube.h"
#include "direct3D.h"
#include "shader3D.h"
#include "texture.h"
//#include "debug_camera.h"
#include "keylogger.h"
#include "debug_ostream.h"

using namespace DirectX;

// ===============================================
// 定数定義
// ===============================================
namespace
{
	constexpr int NUM_VERTEX = 24;	// 頂点数
	constexpr int NUM_INDEX = 36;	// インデックス数
}

static ID3D11Buffer* g_pVertex3DBuffer = nullptr;	 // 頂点バッファ
static ID3D11Buffer* g_pIndex3DBuffer = nullptr;	 // 頂点バッファ


// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

// 頂点構造体
struct Vertex3D
{
	XMFLOAT3 position;	// 頂点座標
	XMFLOAT3 normal;	// 頂点座標
	XMFLOAT4 color;		//カラー
	XMFLOAT2 texcood;	//テクスチャ座標
};

static Vertex3D g_CubeVerTex[] = {
	//正面
	{{-0.5f, 0.5f, -0.5f},{ 0.0f, 0.0f, -1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f} },
	{{ 0.5f, 0.5f, -0.5f},{ 0.0f, 0.0f, -1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 0.0f}},
	{{-0.5f,-0.5f, -0.5f},{ 0.0f, 0.0f, -1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.5f} },
	{{ 0.5f,-0.5f, -0.5f},{ 0.0f, 0.0f, -1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 0.5f}},


	//右
	{{ 0.5f, 0.5f, -0.5f},{ 1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 0.0f}},
	{{ 0.5f, 0.5f,  0.5f},{ 1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 0.0f}},
	{{ 0.5f,-0.5f, -0.5f},{ 1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 0.5f}},
	{{ 0.5f,-0.5f,  0.5f},{ 1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 0.5f}},

	//背面
	{{ 0.5f, 0.5f, 0.5f},{ 0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 0.5f} },
	{{-0.5f, 0.5f, 0.5f},{ 0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.99f, 0.5f}},
	{{ 0.5f,-0.5f, 0.5f},{ 0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 1.0f} },
	{{-0.5f,-0.5f, 0.5f},{ 0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.67f, 1.0f}},

	//左
	{{-0.5f, 0.5f,  0.5f},{ -1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 0.5f}},
	{{-0.5f, 0.5f, -0.5f},{ -1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 0.5f}},
	{{-0.5f,-0.5f,  0.5f},{ -1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 1.0f}},
	{{-0.5f,-0.5f, -0.5f},{ -1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 1.0f}},

	//上
	{{-0.5f, 0.5f, 0.5f},{ 0.0f, 1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 0.0f}},
	{{ 0.5f, 0.5f, 0.5f},{ 0.0f, 1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.99f, 0.0f}},
	{{-0.5f, 0.5f,-0.5f},{ 0.0f, 1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 0.5f}},
	{{ 0.5f, 0.5f,-0.5f},{ 0.0f, 1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.99f, 0.5f}},

	//下
	{{-0.5f,-0.5f,-0.5f},{ 0.0f, -1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 0.5f}},
	{{ 0.5f,-0.5f,-0.5f},{ 0.0f, -1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 0.5f}},
	{{-0.5f,-0.5f, 0.5f},{ 0.0f, -1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.33f, 1.0f}},
	{{ 0.5f,-0.5f, 0.5f},{ 0.0f, -1.0f, 0.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.66f, 1.0f}},

};

static unsigned short g_CubeIndex[] = {
	 0, 1, 2, 1, 3, 2,
	 4, 5, 6, 5, 7, 6,
	 8, 9,10, 9,11,10,
	12,13,14,13,15,14,
	16,17,18,17,19,18,
	20,21,22,21,23,22
};

void Cube_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// デバイスとデバイスコンテキストのチェック
	if (!pDevice || !pContext) {
		hal::dout << "Cube_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;		//DEFAULT はじめに書き込んだら使わない　　DYNAMIC　常に書き換える
	bd.ByteWidth = sizeof(Vertex3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	//頂点バッファへ流し込むデータの設定
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = g_CubeVerTex;

	g_pDevice->CreateBuffer(&bd, &sd, &g_pVertex3DBuffer);

	// インデックスバッファ生成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned short) * NUM_INDEX;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	sd.pSysMem = g_CubeIndex;

	g_pDevice->CreateBuffer(&bd, &sd, &g_pIndex3DBuffer);
}

void Cube_Finalize()
{
	SAFE_RELEASE(g_pIndex3DBuffer);
	SAFE_RELEASE(g_pVertex3DBuffer);
}

void Cube_Update(double elapsed_time)
{

}

void Cube_Draw(int texid, const DirectX::XMMATRIX& mtxWorld, const DirectX::XMFLOAT4& Color)
{
	Vertex3D temp[NUM_VERTEX]{};
	for (int i = 0; i < NUM_VERTEX; ++i)
	{
		temp[i] = g_CubeVerTex[i];
		temp[i].color = Color;
	}

	// 頂点バッファへ書き込む
	D3D11_MAPPED_SUBRESOURCE mapped;
	g_pContext->Map(g_pVertex3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	// バッファサイズは sizeof(Vertex3D) * NUM_VERTEX
	memcpy(mapped.pData, temp, sizeof(temp));
	g_pContext->Unmap(g_pVertex3DBuffer, 0);

	// シェーダーを描画パイプラインに設定
	Shader3D_Begin();

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertex3DBuffer, &stride, &offset);

	// 頂点インデックスを描画パイプラインに設定
	g_pContext->IASetIndexBuffer(g_pIndex3DBuffer, DXGI_FORMAT_R16_UINT, 0);

	//ワールド変換行列の設定
	Shader3D_SetWorldMatrix(mtxWorld);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//テクスチャの設定
	Texture_SetTexture(texid);

	//マテリアルカラーの設定
	shader3D_SetMaterialDiffuse({ 1.0f,1.0f,1.0f,1.0f });

	// ポリゴン描画命令発行
	g_pContext->DrawIndexed(NUM_INDEX, 0, 0);
}

void Cube_Draw(int texid, const DirectX::XMMATRIX& mtxWorld, CubeFace ChangeFace, const DirectX::XMFLOAT4& Color, const DirectX::XMFLOAT4& ChangeColor)
{
	Vertex3D temp[NUM_VERTEX]{};
	for (int i = 0; i < NUM_VERTEX; ++i)
	{
		temp[i] = g_CubeVerTex[i];
		temp[i].color = Color;
	}

	if (ChangeFace >= 0)
	{
		for (int i = 0; i < 4; i++)
		{
			temp[ChangeFace * 4 + i].color = ChangeColor;
		}
	}

	// 頂点バッファへ書き込む
	D3D11_MAPPED_SUBRESOURCE mapped;
	g_pContext->Map(g_pVertex3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	// バッファサイズは sizeof(Vertex3D) * NUM_VERTEX
	memcpy(mapped.pData, temp, sizeof(temp));
	g_pContext->Unmap(g_pVertex3DBuffer, 0);

	// シェーダーを描画パイプラインに設定
	Shader3D_Begin();

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertex3DBuffer, &stride, &offset);

	// 頂点インデックスを描画パイプラインに設定
	g_pContext->IASetIndexBuffer(g_pIndex3DBuffer, DXGI_FORMAT_R16_UINT, 0);

	//ワールド変換行列の設定
	Shader3D_SetWorldMatrix(mtxWorld);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//テクスチャの設定
	Texture_SetTexture(texid);

	//マテリアルカラーの設定
	shader3D_SetMaterialDiffuse({ 1.0f,1.0f,1.0f,1.0f });

	// ポリゴン描画命令発行
	g_pContext->DrawIndexed(NUM_INDEX, 0, 0);
}

void Cube_Draw(int texid, const XMMATRIX& mtxWorld)
{
	// シェーダーを描画パイプラインに設定
	Shader3D_Begin();

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertex3DBuffer, &stride, &offset);

	// 頂点インデックスを描画パイプラインに設定
	g_pContext->IASetIndexBuffer(g_pIndex3DBuffer, DXGI_FORMAT_R16_UINT, 0);

	//ワールド変換行列の設定
	Shader3D_SetWorldMatrix(mtxWorld);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//テクスチャの設定
	Texture_SetTexture(texid);

	//マテリアルカラーの設定
	shader3D_SetMaterialDiffuse({ 1.0f,1.0f,1.0f,1.0f });

	// ポリゴン描画命令発行
	g_pContext->DrawIndexed(NUM_INDEX, 0, 0);
}