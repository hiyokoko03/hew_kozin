// ==============================================================
// model.cpp	
// モデル制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/12/03	
// 更新者:Kazuki Murakami		更新日付：2025/12/03
// ==============================================================
#include "model.h"

#include <string>
#include <DirectXMath.h>

#include "direct3D.h"
#include "WICTextureLoader11.h"
#include "DirectXtex.h"
#include "shader3D.h"
#include "texture.h"

using namespace DirectX;

// ===============================================
// 構造体宣言
// ===============================================
struct Vertex3D		// 頂点構造体
{
	XMFLOAT3 position;	// 頂点座標
	XMFLOAT3 normal;	// 法線
	XMFLOAT4 color;		// カラー
	XMFLOAT2 texcood;	// テクスチャ座標
};

// ===============================================
// グローバル変数
// ===============================================
static int g_TextureID = -1;	// テクスチャが無い場合に使う白テクスチャID

// ==============================================================
// モデル読み込み関数
// MODEL* ModelLoad( const char *FileName, float scale)
// 
// 型：MODEL*	モデルのポインター型
// 
// 引数１：FileName		const char*型		読み込むモデルのパス
// 引数２：scale		float型				モデル全体の拡縮
// 返り値：model		MODEL*型			モデルを返す
// ==============================================================
MODEL* ModelLoad( const char *FileName, float scale)
{
	MODEL* model = new MODEL;

	// ファイルパス保存
	const std::string modelPath( FileName );

	// Assimp を使ってモデル読み込み（左手系に変換）
	model->AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(model->AiScene);

	// メッシュ数ぶんのバッファを準備
	model->VertexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];
	model->IndexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];

	// 各メッシュごとに頂点・インデックスバッファ生成
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = model->AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			// メッシュに含まれる頂点数ぶん配列を用意
			Vertex3D* vertex = new Vertex3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				// 頂点座標
				vertex[v].position = XMFLOAT3(
					mesh->mVertices[v].x * scale,
					mesh->mVertices[v].y * scale,
					mesh->mVertices[v].z * scale
				);

				// テクスチャUV
				vertex[v].texcood = XMFLOAT2( 
					mesh->mTextureCoords[0][v].x,
					mesh->mTextureCoords[0][v].y
				);

				// カラーは白固定
				vertex[v].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				// 法線（Y/Z の入れ替えなど補正）
				vertex[v].normal = XMFLOAT3(
					mesh->mNormals[v].x,
					-mesh->mNormals[v].z,
					mesh->mNormals[v].y
				);
			}

			// 頂点バッファの設定
			D3D11_BUFFER_DESC bd{};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = vertex;

			Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->VertexBuffer[m]);

			delete[] vertex;
		}


		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			// Assimp では必ず三角形化済みなので3つ固定
			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			// インデックスバッファ作成
			D3D11_BUFFER_DESC bd{};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = index;

			Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->IndexBuffer[m]);

			delete[] index;
		}

	}

	// 埋め込みテクスチャの読み込み
	for(int i = 0; i < model->AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = model->AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;
		TexMetadata metadata;
		ScratchImage image;

		// メモリ上の画像データ(WIC形式)を読み込み
		LoadFromWICMemory(
			aitexture->pcData,
			aitexture->mWidth,
			WIC_FLAGS_NONE,
			&metadata,
			image
		);

		// GPUリソース作成
		CreateShaderResourceView(
			Direct3D_GetDevice(),
			image.GetImages(),
			image.GetImageCount(),
			metadata,
			&texture
		);

		assert(texture);

		// ファイル名をキーに保存
		model->Texture[aitexture->mFilename.data] = texture;
	}

	// テクスチャ無しの時に使う白テクスチャを読み込み
	g_TextureID = Texture_Load(L"resource/texture/white.png");

	return model;
}

// ==============================================================
// モデル解放
// void ModelRelease(MODEL* model)
// 
// 型：void
// 
// 引数１：model		MODEL*型		解放するモデル
// 返り値：void
// ==============================================================
void ModelRelease(MODEL* model)
{
	// 全メッシュの頂点・インデックスバッファ破棄
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		model->VertexBuffer[m]->Release();
		model->IndexBuffer[m]->Release();
	}

	delete[] model->VertexBuffer;
	delete[] model->IndexBuffer;

	// 読み込んだテクスチャを解放
	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : model->Texture)
	{
		pair.second->Release();
	}

	// Assimp のシーン破棄
	aiReleaseImport(model->AiScene);

	delete model;
}

// ==============================================================
// モデル描画
// void ModelDraw(const MODEL* model, const DirectX::XMMATRIX& mtxWorld)
// 
// 型：void
// 
// 引数１：model		MODEL*型				描画するモデル
// 引数２：mtxWorld		DirectX::XMMATRIX&型	ワールド行列
// 引数３：useShader	bool型					シェーダーを使用するかどうか
// 返り値：void
// ==============================================================
void ModelDraw(const MODEL* model, const DirectX::XMMATRIX& mtxWorld, bool useShader)
{
	// シェーダーパイプライン設定
	if (useShader)
	{
		Shader3D_Begin();
	}

	for (int i = 0; i < model->AiScene->mNumMeshes; i++)
	{
		// 頂点バッファを描画パイプラインに設定
		UINT stride = sizeof(Vertex3D);
		UINT offset = 0;
		Direct3D_GetContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer[i], &stride, &offset);

		// 頂点インデックスを描画パイプラインに設定
		Direct3D_GetContext()->IASetIndexBuffer(model->IndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//ワールド変換行列の設定
		Shader3D_SetWorldMatrix(mtxWorld);

		// プリミティブトポロジ設定
		Direct3D_GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		aiString texture;
		aiMaterial* aimaterial = model->AiScene->mMaterials[model->AiScene->mMeshes[i]->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0,&texture );

		if (texture.length != 0)
		{
			//テクスチャの設定
			Direct3D_GetContext()->PSSetShaderResources(0, 1, &model->Texture.at(texture.data));
			shader3D_SetMaterialDiffuse({ 1.0f,1.0f,1.0f,1.0f });
		}
		else
		{
			Texture_SetTexture(g_TextureID);
			aiColor3D diffuse;
			aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			shader3D_SetMaterialDiffuse({ diffuse.r,diffuse.g, diffuse.b,1.0f });
		}

		// ポリゴン描画命令発行
		Direct3D_GetContext()->DrawIndexed(model->AiScene->mMeshes[i]->mNumFaces * 3, 0, 0);
	}
}