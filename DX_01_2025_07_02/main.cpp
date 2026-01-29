// ==============================================================
// main.cpp	
// ウィンドウ表示
// 
// 制作者:Momoka Ishijima		制作日付：2025/05/07	
// 更新者:Kazuki Murakami		更新日付：2025/10/29
// ==============================================================
#include <SDKDDKVer.h>
#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <algorithm>
#include <sstream>

#pragma comment(lib,"xinput.lib")

#include "direct3D.h"
#include "shader2D.h"
#include "shader3D.h"
#include "texture.h"
#include "sprite.h"
#include "sprite_anim.h"
		 
#include "scene.h"
		 
#include "system_timer.h"
#include "fade.h"
#include "Audio.h"
#include "keylogger.h"
#include "mouse.h"
		 
#include "debug_ostream.h"
#include "debug_text.h"
		 
#include "cube.h"
#include "billboard.h"


// ================================================================
// ウィンドウ情報
// ================================================================
static	constexpr char WINDOW_CLASS[] = "GameWindow";		//メインウィンドウのクラス名
static	constexpr char TITLE[] = "ゲームウィンドウ";		//タイトルバーのテキスト

// ================================================================
//	ウィンドウプロシージャ	プロトタイプ宣言
// ================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ================================================================
//	メイン関数エントリーポイント
// ================================================================
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPSTR /*lpCmdLine*/, _In_ int nCmdShow)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//DPIスケーリング
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	/*ウィンドウクラスの登録*/
	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);							//構造体サイズ登録
	wcex.lpfnWndProc = WndProc;									//ウィンドウプロシージャ関数のポインタ登録
	wcex.hInstance = hInstance;									//
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);			//
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);				//
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			//
	wcex.lpszMenuName = nullptr;								//メニューは作らない//ツールバー
	wcex.lpszClassName = WINDOW_CLASS;							//
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);	//

	RegisterClassEx(&wcex);

	RECT window_rect = { 0, 0, 1920, 1080 };//実際に描画する領域

	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);//ウィンドウスタイル指定

	AdjustWindowRect(&window_rect, window_style, FALSE);//座標計算

	//ウィンドウの幅と高さを計算
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//画面中央に出すための計算
	int window_x = std::max((desktop_width - window_width) / 2, 0);
	int window_y = std::max((desktop_height - window_height) / 2, 0);

	float angle = 0.0f;

	/*メインウィンドウの作成*/
	HWND hWnd = CreateWindow(
		WINDOW_CLASS, 
		TITLE, 
		window_style,
		window_x, 
		window_y, 
		/*800*//*window_width*/desktop_width,
		/*500*//*window_height*/desktop_height,
		nullptr, 
		nullptr, 
		hInstance, 
		nullptr
	);

	/*各種初期化*/
	/**/
	SystemTimer_Initialize();
	KeyLogger_Initialize();
	Audio_Initialize();

	if (!Direct3D_Initialize(hWnd))
	{
		PostQuitMessage(0);
	}
	else
	{
		if (!Shader2D_Initialize(Direct3D_GetDevice(), Direct3D_GetContext()))
		{
			PostQuitMessage(0);
		}
		else
		{
			Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
			//シェーダー初期化
			Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
		}
	}

	//3?シェーダー初期化
	Shader3D_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetContext(),
		L"resource/texture/consolab_ascii_512.png",		//フォント名
		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
		0.0f, 0.0f,	//左上座標 X Y
		0, 0,
		0.0f, 0.0f
	);


	Mouse_SetVisible(false);	//マウス消える
	Fade_Initialize();

	Billborad_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Cube_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

	Fade_Start(0.0f, false);/**/

	Scene_Initialize();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//時計計測用
	double	exec_last_time = 0.0;
	double	fps_last_time = 0.0;
	double	current_time = 0.0;
	ULONG	frame_count = 0;
	double	fps = 0.0;

	exec_last_time = fps_last_time = SystemTimer_GetTime();

	/*メッセージループ*/
	MSG msg;
	
	/* メッセージループ && ゲームループ */
	do
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{//ウィンドウメッセージが来てたら実行
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{//ゲームの処理

			current_time = SystemTimer_GetTime();		//システム時刻を取得
			double	elapsed_time = current_time - fps_last_time;//fps用経過時間
			//fps計測
			if (elapsed_time >= 1.0)	//1秒ごとに実行
			{
				fps = frame_count / elapsed_time;
				fps_last_time = current_time;//FPSを測定した時刻
				frame_count = 0;	//カウントをクリア
			}

			//1/60秒ごとに実行
			elapsed_time = current_time - exec_last_time;//ゲーム更新用経過時間
			if ((elapsed_time) >= (1.0 / 60.0))
			{
				exec_last_time = current_time;	//処理した時間を表示

				KeyLogger_Update();

				Scene_Update(elapsed_time);
				Fade_Update(elapsed_time);

				Direct3D_Clear();
				SetViewport(0);

				Scene_Draw();

				Fade_Draw();

#if defined(DEBUG) || defined(_DEBUG)
				std::stringstream ss;
				ss << "fps:" << fps << std::endl;
				dt.SetText(ss.str().c_str());
				dt.Draw();
				dt.Clear();
#endif

				Direct3D_Present();

				frame_count++;

				Scene_ChangeScene();
			}
		}
	} while (msg.message != WM_QUIT);

	/*終了処理	(初期化と逆順に)*/
	//SpriteAnim_Finalize();

	Scene_Finalize();

	Fade_Finalize();
	Sprite_Finalize();
	Texture_Finalize();
	Shader2D_Finalize();
	Direct3D_Finalize();
	Mouse_Finalize();

	Cube_Finalize();
	Billborad_Finalize();
	Shader3D_Finalize();
	//ファイナライズ多分足りない

	return (int)msg.wParam;
}

// ================================================================
// ウィンドウプロシージャ
// ================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		Keyboard_ProcessMessage(message, wParam, lParam);
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(message, wParam, lParam);
         break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}

	//case WM_KEYDOWN:
	//	//Escキーが押されたとき
	//	if (wParam == VK_ESCAPE)
	//	{
	//		//閉じるボタンが押されたときのメッセージ送信
	//		SendMessage(hWnd, WM_CLOSE, 0, 0);
	//	}
		break;
	case WM_DESTROY:		//ウィンドウの破壊メッセージ
		PostQuitMessage(0);	//WM_QUITメッセージの送信
		break;
	default:
		//通常のメッセージ処理はこの関数に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

