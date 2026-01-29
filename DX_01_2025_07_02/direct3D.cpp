// ==============================================================
// direct3d.cpp	
// Direct3D制御
// 
// 制作者:Momoka Ishijima		制作日付：2025/05/07	
// 更新者:Kazuki Murakami		更新日付：2025/10/29
// ==============================================================
#include "direct3D.h"
		 
#include <d3d11.h>

/*追加の依存ファイル(ライブラリ)	プロパティ追加か下のコード*/
#pragma comment(lib,"d3d11.lib")
//#pragma comment(lib,"dxgi.lib")
#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib,"DirectXTex_Debug.lib")
#else
	#pragma comment(lib,"DirectXTex_Release.lib")
#endif

// ===============================================
// グローバル変数
// ===============================================
//インターフェースの宣言
static	ID3D11Device* g_pDevice = nullptr;
static	ID3D11DeviceContext* g_pDeviceContext = nullptr;
static	IDXGISwapChain* g_pSwapChain = nullptr;
//static	ID3D11BlendState* g_BlendStateMultiply = nullptr;

static	ID3D11DepthStencilState* g_pDepthStencilStateDepthDisable = nullptr;
static	ID3D11DepthStencilState* g_pDepthStencilStateDepthEnable = nullptr;

/*バックバッファ関連*/
static	ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
static	ID3D11Texture2D* g_pDepthStencilBuffer = nullptr;
static	ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
static	ID3D11BlendState* g_BlendStateMultiply = nullptr;
static	ID3D11BlendState* g_BlendStateAdd = nullptr;

static	D3D11_TEXTURE2D_DESC g_BackBufferDesc{};
static	D3D11_VIEWPORT g_Viewport[2]{ {},{} };

// 追加
static ID3D11RasterizerState* g_pRS_CullNone = nullptr;
static ID3D11RasterizerState* g_pRS_CullBack = nullptr;

bool Direct3D_Initialize(HWND hWnd)
{
	/*デバイス、スワップチェーン、コンテキスト生成*/
	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.BufferCount = 2;
	//swap_chain_desc.BufferDesc.Width = 0;
	//swap_chain_desc.BufferDesc.Height = 0;
	//ウィンドウサイズに合わせて自動的に設定される
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swap_chain_desc.OutputWindow = hWnd;
	//swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;	//処理の重さ確認用

	UINT device_flags = 0;

#if defined(DEBUG)||defined(_DEBUG)
	device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_11_1/*,
		D3D_FEATURE_LEVEL_11_0*/
	};

	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,	//D3D11_CREATE_DEVICE_BGRA_SUPPORT //device_flags 
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&swap_chain_desc,
		&g_pSwapChain,
		&g_pDevice,
		&feature_level,
		&g_pDeviceContext
	);

	if (FAILED(hr))
	{
		MessageBox(hWnd, "Direct3Dの初期化に失敗しました", "エラー", MB_OK);
		return false;
	}

	if (!configureBackBuffer())
	{
		MessageBox(hWnd, "バックバッファの設定に失敗しました", "エラー", MB_OK);
		return false;
	}

	// ブレンドステート設定
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	g_pDevice->CreateBlendState(&bd, &g_BlendStateMultiply);	//作成

	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	g_pDevice->CreateBlendState(&bd, &g_BlendStateAdd);

	Direct3D_SetAlphaBlend(BLEND_TRANSPARENT);
	
	//深度ステンシルステート
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = FALSE;
	dsd.DepthEnable = FALSE;//無効にする
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	g_pDevice->CreateDepthStencilState(&dsd, &g_pDepthStencilStateDepthDisable);

	// 
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	g_pDevice->CreateDepthStencilState(&dsd,&g_pDepthStencilStateDepthEnable);

	//true3D	false2D
	Direct3D_SetDepthStencilState(true);

	return true;
}

void Direct3D_Finalize()
{
	SAFE_RELEASE(g_pDepthStencilStateDepthDisable);
	SAFE_RELEASE(g_pDepthStencilStateDepthEnable);
	SAFE_RELEASE(g_BlendStateAdd);

	releaseBackBuffer();
	
	//if (g_pSwapChain)
	//{
	//	g_pSwapChain->Release();
	//	g_pSwapChain = nullptr;
	//}
	//			↓
	
	SAFE_RELEASE(g_BlendStateMultiply);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pDeviceContext);
	SAFE_RELEASE(g_pDevice);

}

void Direct3D_Clear()
{
	float clear_color[4] = { 0.2f,0.25f,0.4f,1.0f };

	g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, clear_color);
	g_pDeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//レンダーターゲットビューとデプスステンシルビューの設定
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
}

void Direct3D_Present()
{
	//スワップチェーンの表示
	g_pSwapChain->Present(1, 0);	//1,0に戻す
}

void	Direct3D_SetDepthStencilState(bool bEnable)
{
	auto state = bEnable ? g_pDepthStencilStateDepthEnable : g_pDepthStencilStateDepthDisable;
	g_pDeviceContext->OMSetDepthStencilState(state, NULL);
}

bool configureBackBuffer()
{
	HRESULT hr;

	ID3D11Texture2D* back_buffer_pointer = nullptr;

	// バックバッファの取得
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer_pointer);

	if (FAILED(hr)) {
		hal::dout << "バックバッファの取得に失敗しました" << std::endl;
		return false;
	}

	// バックバッファのレンダーターゲットビューの生成
	hr = g_pDevice->CreateRenderTargetView(back_buffer_pointer, nullptr, &g_pRenderTargetView);

	if (FAILED(hr)) {
		back_buffer_pointer->Release();
		hal::dout << "バックバッファのレンダーターゲットビューの生成に失敗しました" << std::endl;
		return false;
	}

	// バックバッファの状態（情報）を取得
	back_buffer_pointer->GetDesc(&g_BackBufferDesc);

	back_buffer_pointer->Release(); // バックバッファのポインタは不要なので解放

	// デプスステンシルバッファの生成
	D3D11_TEXTURE2D_DESC depth_stencil_desc{};
	depth_stencil_desc.Width = g_BackBufferDesc.Width;
	depth_stencil_desc.Height = g_BackBufferDesc.Height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;
	hr = g_pDevice->CreateTexture2D(&depth_stencil_desc, nullptr, &g_pDepthStencilBuffer);

	if (FAILED(hr)) {
		hal::dout << "デプスステンシルバッファの生成に失敗しました" << std::endl;
		return false;
	}

	// デプスステンシルビューの生成
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
	depth_stencil_view_desc.Format = depth_stencil_desc.Format;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	depth_stencil_view_desc.Flags = 0;
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilBuffer, &depth_stencil_view_desc, &g_pDepthStencilView);

	if (FAILED(hr)) {
		hal::dout << "デプスステンシルビューの生成に失敗しました" << std::endl;
		return false;
	}

	//ビューポートの設定
	g_Viewport[0].TopLeftX = 0.0f;
	g_Viewport[0].TopLeftY = 0.0f;
	g_Viewport[0].Width = (FLOAT)g_BackBufferDesc.Width;
	g_Viewport[0].Height = (FLOAT)g_BackBufferDesc.Height;
	g_Viewport[0].MinDepth = 0.0f;
	g_Viewport[0].MaxDepth = 1.0f;

	g_Viewport[1].TopLeftX = (FLOAT)g_BackBufferDesc.Width * 0.5f;
	g_Viewport[1].TopLeftY = 0.0f;
	g_Viewport[1].Width = (FLOAT)g_BackBufferDesc.Width * 0.5f;
	g_Viewport[1].Height = (FLOAT)g_BackBufferDesc.Height;
	g_Viewport[1].MinDepth = 0.0f;
	g_Viewport[1].MaxDepth = 1.0f;

	return true;
}

void releaseBackBuffer()
{
	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencilBuffer);
	SAFE_RELEASE(g_pDepthStencilView);
}

unsigned int Direct3D_GetBackBufferWidth()
{
	return g_BackBufferDesc.Width;
}

unsigned int Direct3D_GetBackBufferHeight()
{
	return g_BackBufferDesc.Height;
}

ID3D11Device* Direct3D_GetDevice()
{
	return g_pDevice;
}

ID3D11DeviceContext* Direct3D_GetContext()
{
	return g_pDeviceContext;
}

IDXGISwapChain* Direct3D_GetSwapChain()
{
	return g_pSwapChain;
}

void SetViewport(int n)
{
	g_pDeviceContext->RSSetViewports(1, &g_Viewport[n]);
}

void Direct3D_SetAlphaBlend(AlphaBlendMode blend)
{

	float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* arg{};

	switch (blend)
	{
	case	BLEND_TRANSPARENT:
		arg = g_BlendStateMultiply;
		break;
	case	BLEND_ADD:
		arg = g_BlendStateAdd;
		break;
	}

	g_pDeviceContext->OMSetBlendState(g_BlendStateMultiply, blend_factor, 0xffffffff);	//設定

}

void Direct3D_SetRenderMode(RenderMode mode)
{
	// 深度ステンシル
	if (mode == RENDER_3D)
	{
		Direct3D_SetDepthStencilState(true);   // 深度有効
		if (g_pRS_CullBack)
		{
			g_pDeviceContext->RSSetState(g_pRS_CullBack);
		}
	}
	else // RENDER_2D
	{
		Direct3D_SetDepthStencilState(false);  // 深度無効
		if (g_pRS_CullNone)
		{
			g_pDeviceContext->RSSetState(g_pRS_CullNone);
		}
	}

	// ブレンド（必要ならここで都度変える）
	// ひとまず2D/3Dともに透過ブレンドを既定
	Direct3D_SetAlphaBlend(BLEND_TRANSPARENT);

	// ※ 射影/View 行列の切替は各描画パス側で行う想定
}

// ===============================================
// 深度テスト ON/OFF
// ===============================================
void Direct3D_SetZEnable(bool enable)
{
	if (enable)
	{
		g_pDeviceContext->OMSetDepthStencilState(
			g_pDepthStencilStateDepthEnable,
			0
		);
	}
	else
	{
		g_pDeviceContext->OMSetDepthStencilState(
			g_pDepthStencilStateDepthDisable,
			0
		);
	}
}

// ===============================================
// 深度書き込み ON/OFF
// ===============================================
void Direct3D_SetZWrite(bool enable)
{
	D3D11_DEPTH_STENCIL_DESC desc{};
	desc.StencilEnable = FALSE;

	if (enable)
	{
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
	}
	else
	{
		desc.DepthEnable = TRUE;                    // 深度テストは ON のまま
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 書き込みだけ OFF
		desc.DepthFunc = D3D11_COMPARISON_LESS;
	}

	ID3D11DepthStencilState* newState = nullptr;
	g_pDevice->CreateDepthStencilState(&desc, &newState);

	g_pDeviceContext->OMSetDepthStencilState(newState, 0);

	SAFE_RELEASE(newState);
}

// ===============================================
// ブレンド方式設定（透明 / 加算）
// ===============================================
void Direct3D_SetBlendMode(AlphaBlendMode mode)
{
	float blend_factor[4] = { 0,0,0,0 };

	ID3D11BlendState* state = nullptr;

	switch (mode)
	{
	case BLEND_TRANSPARENT:
		state = g_BlendStateMultiply;
		break;

	case BLEND_ADD:
		state = g_BlendStateAdd;
		break;
	}

	g_pDeviceContext->OMSetBlendState(state, blend_factor, 0xffffffff);
}