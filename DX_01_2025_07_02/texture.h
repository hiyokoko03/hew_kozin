// ==============================================================
// texture.h	
// テクスチャ制御
// 
// 制作者:IshijimaMomoka		制作日付：2025/06/06	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef	TEXTURE_H
#define	TEXTURE_H

#include	<d3d11.h>
#include	<DirectXMath.h>

// ===============================================
// プロトタイプ宣言
// ===============================================
void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	//テクスチャ初期化処理
void Texture_Finalize();	//テクスチャ終了処理

// ================================================================================

/*テクスチャ読み込み*/
// 引数
// pFileName	画像ファイル名
// bMipMap		MipMap作成フラグ：作成時 true
// 
// 戻り値	テクスチャ管理番号	/	読み込み失敗時は -1
int Texture_Load(const wchar_t* pFileName, bool bMipMap = true);

void Texture_SetTexture(int TextureID);

//テクスチャ全解放
void Texture_AllRelease();
// 部分開放は各自

// 縦横セットver
DirectX::XMUINT2 Texture_GetSize(int TextureID);

//縦横個々ver
const unsigned int& Texture_GetWidth(int TextureID);
const unsigned int& Texture_GetHeight(int TextureID);

#endif // !TEXTURE_H











