/*==============================================================================
gridの描画[grid.cpp]
Ishijima Momoka		2025/10/21
==============================================================================*/
#include	"grid.h"

#include	<DirectXMath.h>

#include	"debug_ostream.h"
#include	"direct3D.h"
#include	"shader3D.h"
#include	"keylogger.h"

using namespace DirectX;

//カメラ移動用
static	float	g_x = 2.0f;
static	float	g_y = 5.0f;
static	float	g_z = -5.0f;

static ID3D11Buffer* g_pVertexBuffer = nullptr;			// 頂点バッファ

// 注意！初期化で外部から設定されるもの。Release不要。	GetDevice GetContextいけるで
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

//頂点数
static	int	g_NumVertex = 0;

// 頂点構造体
struct Vertex3D
{
	XMFLOAT3 position;	// 頂点座標
	XMFLOAT4 color;		// 頂点カラー
	//XMFLOAT2 texcoord;	//テクスチャ座標　UV座標
};

void Grid_Initialize(int x_count, int z_count, float widthsize)
{

	/*デバイスとデバイスコンテキストの保存*/
	g_pDevice = Direct3D_GetDevice();
	g_pContext = Direct3D_GetContext();

	/*デバイスとデバイスコンテキストのチェック*/
	if (!g_pDevice || !g_pContext) {
		hal::dout << "Sprite_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	//頂点数算出
	const int X_VERTEX_COUNT = x_count + 1;
	const int Z_VERTEX_COUNT = z_count + 1;

	g_NumVertex = X_VERTEX_COUNT * 2 + Z_VERTEX_COUNT * 2;

	/*頂点バッファ生成*/		/*頂点バッファに触らない設定*/
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex3D) * g_NumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	auto* p_GridVertex = new Vertex3D[g_NumVertex];

	//グリッド線の頂点情報書き込み
	//端の座標算出
	float	start_x = x_count * widthsize * -0.5f;
	float	start_z = z_count * widthsize * -0.5f;

	float	end_x = x_count * widthsize * 0.5f;
	float	end_z = z_count * widthsize * 0.5f;

	int index = 0;

	for(int x=0;x<X_VERTEX_COUNT;x++)
	{

		//始点
		p_GridVertex[index].position = { start_x + widthsize * x, 0.0f, start_z };
		p_GridVertex[index].color = { 0.0f,1.0f,0.0f,1.0f };
		//終点
		p_GridVertex[index + 1].position = { start_x + widthsize * x, 0.0f, end_z };
		p_GridVertex[index + 1].color = { 0.0f,1.0f,0.0f,1.0f };

		index += 2;
	}

	for (int z = 0; z < Z_VERTEX_COUNT; z++)
	{

		//始点
		p_GridVertex[index].position = { start_x, 0.0f, start_z + widthsize * z };
		p_GridVertex[index].color = { 0.0f,1.0f,0.0f,1.0f };
		//終点
		p_GridVertex[index + 1].position = { end_x, 0.0f, start_z + widthsize * z };
		p_GridVertex[index + 1].color = { 0.0f,1.0f,0.0f,1.0f };

		index += 2;
	}


	//頂点バッファへコピーするデータの設定
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = p_GridVertex;

	g_pDevice->CreateBuffer(&bd, &sd, &g_pVertexBuffer);

	delete[] p_GridVertex;
}

void Grid_Update(double elapsed_time)
{

}

void Grid_Finalize()
{
	SAFE_RELEASE(g_pVertexBuffer);
}

void Grid_Draw()
{
	//シェーダーパイプライン流し込み
	Shader3D_Begin();

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//ワールド変換行列
	// 頂点シェーダーにワールド変換行列を設定
	Shader3D_SetWorldMatrix(XMMatrixIdentity());	//XMMatrixIdentity()は戻り値が単位行列

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// ポリゴン描画命令発行
	g_pContext->Draw(g_NumVertex, 0);
}
