// ==============================================================
// FontWrite.h
// フォント読み込み・描画
// https://islingtonsystem.hatenablog.jp/entry/DirectWrite_CustomFont_DirectX#%E3%82%BD%E3%83%BC%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89
// 制作者:Momoka Ishijima		制作日付：2025/11/11
// 更新者:	更新日付：//
// ==============================================================
#ifndef FONTWRITE_H
#define	FONTWRITE_H

#include	"direct3D.h"
#include	<Windows.h>
#include	<dwrite_3.h>
#include	<DirectXMath.h>
#include	<d2d1.h>
#include	<wrl.h>			//ComPtr
#include	<string>

//ライブラリ
#pragma comment(lib,"d2d1.lib")			// Direct2D用
#pragma comment(lib,"Dwrite.lib")		// DirectWrite用

// ===============================================
// マクロ定義
// ===============================================
static	constexpr	int	FONT_MAX = 128;

// ===============================================
// クラス宣言
// ===============================================
class FontWrite
{
private:
	// ===========================================
	// 構造体宣言
	// ===========================================
	struct FontFileDate
	{
		std::wstring fontpath = L"";						// フォントファイルパス
		std::wstring font = L"";							// フォント名
		IDWriteFontCollection1* fontCollection = nullptr;	// フォントコレクション
		const wchar_t* localeName = nullptr;				// ロケール名
	};

private:
	// ===========================================
	// メンバ関数
	// ===========================================
	std::wstring	StringToWString(std::string string);

	// ===========================================
	// メンバ変数
	// ===========================================
	//インターフェース宣言
	Microsoft::WRL::ComPtr<IDWriteFactory5>			m_pDWriteFactory = nullptr;			//DirectWriteファクトリ
	Microsoft::WRL::ComPtr<IDWriteTextFormat>		m_pDWriteTextFormat = nullptr;		//テキストフォーマット
	Microsoft::WRL::ComPtr<IDWriteTextLayout>		m_pDWriteTextLayout = nullptr;		//テキストレイアウト

	Microsoft::WRL::ComPtr<ID2D1Factory>			m_pD2D1Factory = nullptr;			// Direct2Dファクトリ
	Microsoft::WRL::ComPtr<IDXGISurface>			m_pBackBuffer = nullptr;			// サーフェス情報
	Microsoft::WRL::ComPtr<ID2D1RenderTarget>		m_pD2D1RenderTarget = nullptr;		// Direct2Dレンダーターゲット
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_pD2D1SolidColorBrush = nullptr;	//テキストカラー

	Microsoft::WRL::ComPtr<IDWriteFontFile>			m_pDWriteFontFile[FONT_MAX]{};		//フォントファイル

	static	FontFileDate m_FontFileDate[FONT_MAX];

public:
	// ===========================================
	// プロトタイプ宣言
	// ===========================================
	void	Initialize(IDXGISwapChain* pSwapChain);
	void	Finalize(void);

	int		LoadFontFile(const wchar_t* pFilePath);
	void	CreateTextFormat(int Index, float FontSize);
	void	TextDraw(std::string String, DirectX::XMFLOAT2 Position, D2D1_DRAW_TEXT_OPTIONS TextOptions = D2D1_DRAW_TEXT_OPTIONS_NONE);
};

#endif // !FONTWRITE_H
