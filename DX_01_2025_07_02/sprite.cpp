// ==============================================================
// sprite.cpp	
// スプライト制御
// 
// 制作者:Youhei Sato			制作日付：2025/05/15	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#include "sprite.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include "DirectXTex.h"

#include "direct3D.h"
#include "texture.h"
#include "shader2D.h"
#include "debug_ostream.h"

using namespace DirectX;

// ===============================================
// sprite.cpp内限定のグローバル変数
// ===============================================
namespace
{
	constexpr int NUM_VERTEX = 4; // 頂点数


	ID3D11Buffer* g_pVertexBuffer = nullptr;			// 頂点バッファ
	//static ID3D11ShaderResourceView* g_pTexture = nullptr;	//テクスチャ01(追加でテクスチャ増やせる)

	// 初期化で外部から設定されるものRelease不要。
	ID3D11Device* g_pDevice = nullptr;
	ID3D11DeviceContext* g_pContext = nullptr;

	struct Vertex	// 頂点構造体
	{
		XMFLOAT3 position;	// 頂点座標
		XMFLOAT4 color;		// 頂点カラー
		XMFLOAT2 texcoord;	//テクスチャ座標　UV座標
	};
}

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/*デバイスとデバイスコンテキストのチェック*/
	if (!pDevice || !pContext) {
		hal::dout << "Sprite_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	/*デバイスとデバイスコンテキストの保存*/
	g_pDevice = pDevice;
	g_pContext = pContext;

	/*頂点バッファ生成*/
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
}

void Sprite_Finalize(void)
{
	//SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pVertexBuffer);
}

void Sprite_Draw(int texid, float x, float y, float width, float height, DirectX::XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader2D_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	Shader2D_SetWorldMatrix(XMMatrixIdentity());

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	v[0].position = { -0.5f , -0.5f , 0.0f };
	v[1].position = { +0.5f , -0.5f , 0.0f };
	v[2].position = { -0.5f , +0.5f , 0.0f };
	v[3].position = { +0.5f , +0.5f , 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	/*テクスチャ張り位置*/
	v[0].texcoord = { 0.0f, 0.0f };
	v[1].texcoord = { 1.0f, 0.0f };
	v[2].texcoord = { 0.0f, 1.0f };
	v[3].texcoord = { 1.0f, 1.0f };

	//テクスチャの設定
	Texture_SetTexture(texid);

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 頂点シェーダーにワールド変換行列を設定//XMMatrixIdentity()は戻り値が単位行列
	XMMATRIX scaling = XMMatrixScaling(width, height, 1.0f);
	XMMATRIX rotation = XMMatrixRotationZ(0);//演算するならXMMATRIX、しないならXMFLOAT
	XMMATRIX translation = XMMatrixTranslation(x, y, 0.0f);
	Shader2D_SetWorldMatrix(scaling * rotation * translation);

	//// 頂点シェーダーにプロジェクション変換行列を設定
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	//色味調整
	Shader2D_SetColor({ 1.0f,1.0f,1.0f,1.0f });

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float x, float y, float width, float height, int txx, int txy, int txw, int txh, DirectX::XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader2D_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	Shader2D_SetWorldMatrix(XMMatrixIdentity());

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	// 頂点位置
	v[0].position = { -0.5f , -0.5f , 0.0f };
	v[1].position = { +0.5f , -0.5f , 0.0f };
	v[2].position = { -0.5f , +0.5f , 0.0f };
	v[3].position = { +0.5f , +0.5f , 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	/*テクスチャ張り位置*/
	//v[0].texcoord = { 0.0f + b,0.0f + b };
	//v[1].texcoord = { 1.0f + b,0.0f + b };
	//v[2].texcoord = { 0.0f + b,1.0f + b };
	//v[3].texcoord = { 1.0f + b,1.0f + b };

	float tsw = (float)Texture_GetWidth(texid);
	float tsh = (float)Texture_GetHeight(texid);

	float x0 = txx / tsw;
	float y0 = txy / tsh;
	float x1 = (txx + txw) / tsw;
	float y1 = (txy + txh) / tsh;

	v[0].texcoord = { x0,y0 };
	v[1].texcoord = { x1,y0 };
	v[2].texcoord = { x0,y1 };
	v[3].texcoord = { x1,y1 };

	//テクスチャの設定
	Texture_SetTexture(texid);

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 頂点シェーダーにワールド変換行列を設定
	Shader2D_SetWorldMatrix(XMMatrixIdentity());	//XMMatrixIdentity()は戻り値が単位行列

	//// 頂点シェーダーにプロジェクション変換行列を設定
	//const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	//const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	//GMと同じ座標指定にしたい場合

	//色味調整
	Shader2D_SetColor({ 1.0f,1.0f,1.0f,1.0f });

	// プリミティブトポロジ設定		/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float x, float y, float width, float height, int txx, int txy, int txw, int txh, float angle, DirectX::XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader2D_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	Shader2D_SetWorldMatrix(XMMatrixIdentity());

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	// 頂点位置
	v[0].position = { -0.5f , -0.5f , 0.0f };
	v[1].position = { +0.5f , -0.5f , 0.0f };
	v[2].position = { -0.5f , +0.5f , 0.0f };
	v[3].position = { +0.5f , +0.5f , 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	float tsw = (float)Texture_GetWidth(texid);
	float tsh = (float)Texture_GetHeight(texid);

	float x0 = txx / tsw;
	float y0 = txy / tsh;
	float x1 = (txx + txw) / tsw;
	float y1 = (txy + txh) / tsh;

	v[0].texcoord = { x0,y0 };
	v[1].texcoord = { x1,y0 };
	v[2].texcoord = { x0,y1 };
	v[3].texcoord = { x1,y1 };

	//テクスチャの設定
	Texture_SetTexture(texid);

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 頂点シェーダーにワールド変換行列を設定//XMMatrixIdentity()は戻り値が単位行列
	XMMATRIX scaling = XMMatrixScaling(width, height, 1.0f);
	XMMATRIX rotation = XMMatrixRotationZ(angle);//演算するならXMMATRIX、しないならXMFLOAT
	XMMATRIX translation = XMMatrixTranslation(x, y, 0.0f);
	Shader2D_SetWorldMatrix(scaling * rotation * translation);

	//// 頂点シェーダーにプロジェクション変換行列を設定
	//const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	//const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	//GMと同じ座標指定にしたい場合

	//色味調整
	Shader2D_SetColor({ 1.0f,1.0f,1.0f,1.0f });

	// プリミティブトポロジ設定		/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float x, float y, float width, float height, bool flip, float angle, DirectX::XMFLOAT2 texcood1, DirectX::XMFLOAT2 texcood2, DirectX::XMFLOAT2 texcood3, DirectX::XMFLOAT2 texcood4, DirectX::XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader2D_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	Shader2D_SetWorldMatrix(XMMatrixIdentity());

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	// 頂点位置
	v[0].position = { -0.5f , -0.5f , 0.0f };
	v[1].position = { +0.5f , -0.5f , 0.0f };
	v[2].position = { -0.5f , +0.5f , 0.0f };
	v[3].position = { +0.5f , +0.5f , 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	if (flip)
	{
		v[0].texcoord = texcood2;
		v[1].texcoord = texcood1;
		v[2].texcoord = texcood4;
		v[3].texcoord = texcood3;
	}
	else //flip
	{
		v[0].texcoord = texcood1;
		v[1].texcoord = texcood2;
		v[2].texcoord = texcood3;
		v[3].texcoord = texcood4;
	}

	//テクスチャの設定
	Texture_SetTexture(texid);

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 頂点シェーダーにワールド変換行列を設定//XMMatrixIdentity()は戻り値が単位行列
	XMMATRIX scaling = XMMatrixScaling(width, height, 1.0f);
	XMMATRIX rotation = XMMatrixRotationZ(angle);//演算するならXMMATRIX、しないならXMFLOAT
	XMMATRIX translation = XMMatrixTranslation(x, y, 0.0f);
	Shader2D_SetWorldMatrix(scaling * rotation * translation);

	//// 頂点シェーダーにプロジェクション変換行列を設定
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	//色味調整
	Shader2D_SetColor({ 1.0f,1.0f,1.0f,1.0f });

	// プリミティブトポロジ設定		/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float x, float y, float width, float height, float angle, DirectX::XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader2D_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	Shader2D_SetWorldMatrix(XMMatrixIdentity());

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	// 頂点位置
	v[0].position = { -0.5f , -0.5f , 0.0f };
	v[1].position = { +0.5f , -0.5f , 0.0f };
	v[2].position = { -0.5f , +0.5f , 0.0f };
	v[3].position = { +0.5f , +0.5f , 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	/*テクスチャ張り位置*/
	v[0].texcoord = { 0.0f, 0.0f };
	v[1].texcoord = { 1.0f, 0.0f };
	v[2].texcoord = { 0.0f, 1.0f };
	v[3].texcoord = { 1.0f, 1.0f };

	//テクスチャの設定
	Texture_SetTexture(texid);

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 頂点シェーダーにワールド変換行列を設定//XMMatrixIdentity()は戻り値が単位行列
	XMMATRIX scaling = XMMatrixScaling(width, height, 1.0f);
	XMMATRIX rotation = XMMatrixRotationZ(angle);//演算するならXMMATRIX、しないならXMFLOAT
	XMMATRIX translation = XMMatrixTranslation(x, y, 0.0f);
	Shader2D_SetWorldMatrix(scaling * rotation * translation);

	//// 頂点シェーダーにプロジェクション変換行列を設定
	Shader2D_SetProjMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	//GMと同じ座標指定にしたい場合

	//色味調整
	Shader2D_SetColor({ 1.0f,1.0f,1.0f,1.0f });

	// プリミティブトポロジ設定		/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}
