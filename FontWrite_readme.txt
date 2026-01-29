//direct3D.cppのInitialize内、
"D3D11CreateDeviceAndSwapChain"の
第四引数(device_flags)を
"D3D11_CREATE_DEVICE_BGRA_SUPPORT"に変更

//GetSwapChainを作成
IDXGISwapChain* Direct3D_GetSwapChain()
{
	return g_pSwapChain;
}

//Game.cppでInitializeとFinalize宣言

使用したいcppの

Initializeで

管理番号(int) = FontWrite_LoadFontFile(L"パス指定");

Drawで

CreateTextFormat(管理番号,フォントサイズ);
でフォントとサイズを指定

FontWrite_Draw("文字列",{x,y});
で表示する文字と位置を指定