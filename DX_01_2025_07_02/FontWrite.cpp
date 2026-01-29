// ==============================================================
// FontWrite.cpp
// フォント描画関係
// https://islingtonsystem.hatenablog.jp/entry/DirectWrite_CustomFont_DirectX#%E3%82%BD%E3%83%BC%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89
// 制作者:Momoka Ishijima		制作日付：2025/11/11
// 更新者:	更新日付：//
// ==============================================================
#include	"FontWrite.h"

#include	<wrl.h>			//ComPtr
#include	<dwrite_3.h>	//DirectWrite
#include	<string>
#include	<d2d1.h>

//ライブラリ
#pragma comment(lib,"d2d1.lib")			// Direct2D用
#pragma comment(lib,"Dwrite.lib")		// DirectWrite用

#include	"direct3D.h"
#include	"debug_ostream.h"

using	namespace	Microsoft;

/*フォント設定データ*/
/*struct FontDate
{
	int	fontindex;							// 使用フォントファイルデータindex指定
	DWRITE_FONT_WEIGHT fontWeight;			// フォントの太さ
	DWRITE_FONT_STYLE fontStyle;			// フォントスタイル
	DWRITE_FONT_STRETCH fontStretch;		// フォント幅
	FLOAT fontSize;							// フォントサイズ
	DWRITE_TEXT_ALIGNMENT textAlignment;	// テキストの配置
	D2D1_COLOR_F Color;						// フォント色
	// 
	D2D1_COLOR_F shadowColor;				// 影色
	D2D1_POINT_2F shadowOffset;				// 影オフセット
};*/

FontWrite::FontFileDate FontWrite::m_FontFileDate[FONT_MAX];

//文字列変換	str("")->wstr(L"")
std::wstring	FontWrite::StringToWString(std::string string)
{
	// SJIS → wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, cpUCS2, iBufferSize);

	// stringの生成
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// バッファの破棄
	delete[] cpUCS2;

	// 変換結果を返す
	return(oRet);

}

void	FontWrite::Initialize(IDXGISwapChain* pSwapChain)
{
	HRESULT hr;

	//Direct2Dファクトリー初期化
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pD2D1Factory.GetAddressOf());
	if (FAILED(hr))
	{
		hal::dout << "FontWrite：Direct2Dファクトリの初期化に失敗しました" << std::endl;
		//return false;
	}

	//バックバッファ取得
	hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pBackBuffer));
	if (FAILED(hr))
	{
		hal::dout << "FontWrite：バックバッファの取得に失敗しました" << std::endl;
		//return false;
	}

	//DPI
	DXGI_SWAP_CHAIN_DESC desc{};
	pSwapChain->GetDesc(&desc);
	HWND hwnd = desc.OutputWindow;
	FLOAT dpiX, dpiY;

	if (hwnd)
	{
		UINT dpi = GetDpiForWindow(hwnd);
		dpiX = dpiY = static_cast<FLOAT>(dpi);
	}
	else
	{
		dpiX = dpiY = 96.0f;
	}

	//レンダーターゲット作成
	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX, dpiY);

	//サーフェスに描画するレンダーターゲット作成
	hr = m_pD2D1Factory->CreateDxgiSurfaceRenderTarget(
		m_pBackBuffer.Get(),
		&props,
		m_pD2D1RenderTarget.GetAddressOf());
	if (FAILED(hr))
	{
		hal::dout << "FontWrite：レンダーターゲット作成に失敗しました" << std::endl;
		//return false;
	}

	//テキストのアンチエイリアシングモードの設定
	m_pD2D1RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

	//IDWriteFactoryの作成
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), reinterpret_cast<IUnknown**>(m_pDWriteFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		hal::dout << "FontWrite：DWriteのファクトリ作成に失敗しました" << std::endl;
		//return false;
	}

	//後で変更可にします
	//色	一旦黒
	m_pD2D1RenderTarget->CreateSolidColorBrush
	(
		D2D1::ColorF(D2D1::ColorF::Black),
		m_pD2D1SolidColorBrush.GetAddressOf());
}

void	FontWrite::Finalize(void)
{
	for (FontFileDate& file : m_FontFileDate)
	{
		SAFE_RELEASE(file.fontCollection);
	}
}

int		FontWrite::LoadFontFile(const wchar_t* pFilePath)
{
	HRESULT hr;

	for (int i = 0; i < FONT_MAX; i++)
	{
		if (m_FontFileDate[i].fontCollection)continue;

		if (m_FontFileDate[i].fontpath == pFilePath)
		{
			return i;
		}
	}

	for (int i = 0; i < FONT_MAX; i++)
	{

		if (m_FontFileDate[i].fontCollection)continue;

		//ファイル読み込み
		hr = m_pDWriteFactory->CreateFontFileReference(pFilePath, nullptr, m_pDWriteFontFile[i].GetAddressOf());
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォントファイルの読み込みに失敗しました" << std::endl;
			return -1;
		}

		//読み込み済みフォントファイルパス情報保存
		m_FontFileDate[i].fontpath = pFilePath;

		WRL::ComPtr<IDWriteFontSetBuilder1>	pDWriteFontSetBuilder1 = nullptr;
		WRL::ComPtr<IDWriteFontSet>	pDWriteFontSet = nullptr;

		//フォントセットビルダー作成
		hr = m_pDWriteFactory->CreateFontSetBuilder(pDWriteFontSetBuilder1.GetAddressOf());
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォントセットのビルダー作成に失敗しました" << std::endl;
			return -1;
		}

		// フォントファイルをビルダーに登録
		hr = pDWriteFontSetBuilder1.Get()->AddFontFile(m_pDWriteFontFile[i].Get());
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：ビルダーへのフォントファイル登録に失敗しました" << std::endl;
			return -1;
		}

		//フォントセット生成
		hr = pDWriteFontSetBuilder1->CreateFontSet(pDWriteFontSet.GetAddressOf());
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォントセットの作成に失敗しました" << std::endl;
			return -1;
		}

		//フォントコレクション生成
		hr = m_pDWriteFactory->CreateFontCollectionFromFontSet(pDWriteFontSet.Get(), &m_FontFileDate[i].fontCollection);
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォントコレクションの作成に失敗しました" << std::endl;
			return -1;
		}

		//フォントファミリー名取得()フォント名
		WRL::ComPtr<IDWriteFontFamily> pDWriteFontFamily = nullptr;
		hr = m_FontFileDate[i].fontCollection->GetFontFamily(0, pDWriteFontFamily.GetAddressOf());
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォント名の取得に失敗しました" << std::endl;
			return -1;
		}

		//フォントファミリー名の一覧を取得
		WRL::ComPtr <IDWriteLocalizedStrings> familyNames = nullptr;
		hr = pDWriteFontFamily->GetFamilyNames(familyNames.GetAddressOf());
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォントファミリー名の取得に失敗しました" << std::endl;
			return -1;
		}

		// 指定されたロケールに対応するインデックスを検索・ロケール名情報保存
		UINT32 index = 0;
		BOOL exists = FALSE;
		hr = familyNames->FindLocaleName(L"ja-jp", &index, &exists);
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォントのロケールの取得に失敗しました(ja-jp)" << std::endl;
			return -1;
		}
		m_FontFileDate[i].localeName = L"jp-ja";

		if (exists == FALSE)
		{
			hr = familyNames->FindLocaleName(L"en-us", &index, &exists);
			if (FAILED(hr))
			{
				hal::dout << "FontWrite：フォントのロケールの取得に失敗しました(en-us)" << std::endl;
				return -1;
			}
			m_FontFileDate[i].localeName = L"en-us";
		}
		if (exists == FALSE)
		{
			index = 0;
		}

		// フォントファミリー名の長さを取得
		UINT32 length = 0;
		//result = familyNames->GetStringLength(index, &length);
		hr = familyNames->GetStringLength(i, &length);
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォント名の長さの取得に失敗しました" << std::endl;
			return -1;
		}

		// フォントファミリー名の取得
		std::wstring name(length + 1, L'\0');
		//result = familyNames->GetString(index, &name[0], length + 1);
		hr = familyNames->GetString(i, &name[0], length + 1);
		if (FAILED(hr))
		{
			hal::dout << "FontWrite：フォント名の取得に失敗しました" << std::endl;
			return -1;
		}

		//フォント名の情報保存
		m_FontFileDate[i].font = name;


		return i;
	}

	return -1;
	
}

void	FontWrite::CreateTextFormat(int Index, float FontSize)
{
	HRESULT hr;

	//TextFormat生成
	hr = m_pDWriteFactory->CreateTextFormat(
		m_FontFileDate[Index].font.c_str(),
		m_FontFileDate[Index].fontCollection,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		FontSize,
		m_FontFileDate[Index].localeName,
		m_pDWriteTextFormat.GetAddressOf());
	if (FAILED(hr))
	{
		hal::dout << "FontWrite：テキストフォーマットの生成に失敗しました" << std::endl;
		//return false;
	}
}

void FontWrite::TextDraw(std::string String, DirectX::XMFLOAT2 Position, D2D1_DRAW_TEXT_OPTIONS TextOptions)
{
	HRESULT hr;

	//文字列変換	str("")->wstr(L"")
	std::wstring wstr = StringToWString(String.c_str());

	//レンダーのターゲットサイズの取得
	D2D1_SIZE_F TargetSize = m_pD2D1RenderTarget->GetSize();

	//テキストレイアウト作成
	hr = m_pDWriteFactory->CreateTextLayout
	(
		wstr.c_str(), wstr.size(),
		m_pDWriteTextFormat.Get(),
		TargetSize.width, TargetSize.height,
		m_pDWriteTextLayout.GetAddressOf());

	if (FAILED(hr))
	{
		hal::dout << "FontWrite：テキストレイアウトの作成に失敗しました" << std::endl;
		//return false;
	}

	//描画位置確定
	D2D1_POINT_2F pounts;
	pounts.x = Position.x;
	pounts.y = Position.y;

	//色設定
	m_pD2D1SolidColorBrush->SetColor({ 0.2f,0.2f,1.0f,1.0f });

	//描画開始
	m_pD2D1RenderTarget->BeginDraw();

	//(かげ)

	//描画処理
	m_pD2D1RenderTarget->DrawTextLayout
	(
		pounts,
		m_pDWriteTextLayout.Get(),
		m_pD2D1SolidColorBrush.Get(),
		TextOptions);

	//描画終了
	m_pD2D1RenderTarget->EndDraw();
}
