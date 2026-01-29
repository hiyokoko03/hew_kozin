// ==============================================================
// texture.cpp	
// テクスチャ制御
// 
// 制作者:IshijimaMomoka		制作日付：2025/06/06	
// 更新者:Kazuki Murakami		更新日付：2025/11/12
// ==============================================================
#include	"texture.h"

#include	<string>		//std::string使用

#include	"direct3D.h"	//SAFE_RELEASE使用
#include "DirectXTex.h"

using namespace DirectX;

// ===============================================
// shader2D.cpp内限定のグローバル変数
// ===============================================
namespace
{
	constexpr int TEXTURE_MAX = 1024;	//テクスチャ管理上限

	struct Texture
	{
		std::wstring filename;
		unsigned int width = 0;
		unsigned int height = 0;
		ID3D11ShaderResourceView* pTexture = nullptr;
	};

	Texture g_Textures[TEXTURE_MAX];

	// 初期化で外部から設定されるもの。Release不要。
	ID3D11Device* g_pDevice = nullptr;
	ID3D11DeviceContext* g_pContext = nullptr;
}

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;

}

void Texture_Finalize()
{
	Texture_AllRelease();
}

int Texture_Load(const wchar_t* pFileName, bool bMipMap)
{
	//読み込み済みか確認
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (!g_Textures[i].pTexture)continue;

		if (g_Textures[i].filename == pFileName)
		{
			return i;
		}
	}

	//管理処理
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//空いている場所(管理番号)を探す
		if (g_Textures[i].pTexture)continue;
	
		//テクスチャのファイルからの読み込み
		TexMetadata metadata;
		ScratchImage image;

		//画像ファイル読み込み
		LoadFromWICFile(pFileName, WIC_FLAGS_NONE, &metadata, image);

		//画像サイズ取得
		g_Textures[i].width = (unsigned int)metadata.width;
		g_Textures[i].height = (unsigned int)metadata.height;

		if (bMipMap)
		{
			//ミップマップ生成
			ScratchImage mipChain;
			GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
			//TEX_FILTER_BOX	|	TEX_FILTER_FORCE_NON_WIC
			image = std::move(mipChain);
			metadata = image.GetMetadata();
		}

		HRESULT hr = CreateShaderResourceView(g_pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Textures[i].pTexture);

		if (FAILED(hr))
		{
			MessageBox(nullptr, "テクスチャの読み込みに失敗しました", "エラー", MB_OK);
			break;
		}

		// ファイル名の保存
		g_Textures[i].filename = pFileName;

		return i;
	}

	return -1;
}

void Texture_SetTexture(int TextureID)
{
	if (TextureID < 0)
	{
		//テクスチャの読み込みが失敗してる
		return;
	}

	//テクスチャ設定
	g_pContext->PSSetShaderResources(0, 1, &g_Textures[TextureID].pTexture);
}

void Texture_AllRelease()
{
	for (Texture& t : g_Textures)
	{
		SAFE_RELEASE(t.pTexture);
	}
}

DirectX::XMUINT2 Texture_GetSize(int TextureID)
{
	return { g_Textures[TextureID].width, g_Textures[TextureID].height };
}

const unsigned int& Texture_GetWidth(int TextureID)
{
	return g_Textures[TextureID].width;
}

const unsigned int& Texture_GetHeight(int TextureID)
{
	return g_Textures[TextureID].height;
}
