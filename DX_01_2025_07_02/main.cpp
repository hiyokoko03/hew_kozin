// ==============================================================
// main.cpp	
// 繧ｦ繧｣繝ｳ繝峨え陦ｨ遉ｺ
// 
// 蛻ｶ菴懆・Momoka Ishijima		蛻ｶ菴懈律莉假ｼ・025/05/07	
// 譖ｴ譁ｰ閠・Kazuki Murakami		譖ｴ譁ｰ譌･莉假ｼ・025/10/29
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
// 繧ｦ繧｣繝ｳ繝峨え諠・ｱ
// ================================================================
static	constexpr char WINDOW_CLASS[] = "GameWindow";		//繝｡繧､繝ｳ繧ｦ繧｣繝ｳ繝峨え縺ｮ繧ｯ繝ｩ繧ｹ蜷・
static	constexpr char TITLE[] = "繧ｲ繝ｼ繝繧ｦ繧｣繝ｳ繝峨え";		//繧ｿ繧､繝医Ν繝舌・縺ｮ繝・く繧ｹ繝・

// ================================================================
//	繧ｦ繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣	繝励Ο繝医ち繧､繝怜ｮ｣險
// ================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ================================================================
//	繝｡繧､繝ｳ髢｢謨ｰ繧ｨ繝ｳ繝医Μ繝ｼ繝昴う繝ｳ繝・
// ================================================================
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPSTR /*lpCmdLine*/, _In_ int nCmdShow)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//DPI繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	/*繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ縺ｮ逋ｻ骭ｲ*/
	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);							//讒矩菴薙し繧､繧ｺ逋ｻ骭ｲ
	wcex.lpfnWndProc = WndProc;									//繧ｦ繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣髢｢謨ｰ縺ｮ繝昴う繝ｳ繧ｿ逋ｻ骭ｲ
	wcex.hInstance = hInstance;									//
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);			//
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);				//
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			//
	wcex.lpszMenuName = nullptr;								//繝｡繝九Η繝ｼ縺ｯ菴懊ｉ縺ｪ縺・/繝・・繝ｫ繝舌・
	wcex.lpszClassName = WINDOW_CLASS;							//
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);	//

	RegisterClassEx(&wcex);

	RECT window_rect = { 0, 0, 1920, 1080 };//螳滄圀縺ｫ謠冗判縺吶ｋ鬆伜沺

	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);//繧ｦ繧｣繝ｳ繝峨え繧ｹ繧ｿ繧､繝ｫ謖・ｮ・

	AdjustWindowRect(&window_rect, window_style, FALSE);//蠎ｧ讓呵ｨ育ｮ・

	//繧ｦ繧｣繝ｳ繝峨え縺ｮ蟷・→鬮倥＆繧定ｨ育ｮ・
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//逕ｻ髱｢荳ｭ螟ｮ縺ｫ蜃ｺ縺吶◆繧√・險育ｮ・
	int window_x = std::max((desktop_width - window_width) / 2, 0);
	int window_y = std::max((desktop_height - window_height) / 2, 0);

	float angle = 0.0f;

	/*繝｡繧､繝ｳ繧ｦ繧｣繝ｳ繝峨え縺ｮ菴懈・*/
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

	/*蜷・ｨｮ蛻晄悄蛹・/
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
			//繧ｷ繧ｧ繝ｼ繝繝ｼ蛻晄悄蛹・
			Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
		}
	}

	//3?繧ｷ繧ｧ繝ｼ繝繝ｼ蛻晄悄蛹・
	if (!Shader3D_Initialize(Direct3D_GetDevice(), Direct3D_GetContext()))
	{
		PostQuitMessage(0);
	}

	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetContext(),
		L"resource/texture/consolab_ascii_512.png",		//繝輔か繝ｳ繝亥錐
		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
		0.0f, 0.0f,	//蟾ｦ荳雁ｺｧ讓・X Y
		0, 0,
		0.0f, 0.0f
	);


	Mouse_SetVisible(false);	//繝槭え繧ｹ豸医∴繧・
	Fade_Initialize();

	Billborad_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Cube_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

	Fade_Start(0.0f, false);/**/

	Scene_Initialize();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//譎りｨ郁ｨ域ｸｬ逕ｨ
	double	exec_last_time = 0.0;
	double	fps_last_time = 0.0;
	double	current_time = 0.0;
	ULONG	frame_count = 0;
	double	fps = 0.0;

	exec_last_time = fps_last_time = SystemTimer_GetTime();

	// Message Loop
	MSG msg;
	
	/* 繝｡繝・そ繝ｼ繧ｸ繝ｫ繝ｼ繝・&& 繧ｲ繝ｼ繝繝ｫ繝ｼ繝・*/
	do
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{//繧ｦ繧｣繝ｳ繝峨え繝｡繝・そ繝ｼ繧ｸ縺梧擂縺ｦ縺溘ｉ螳溯｡・
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{//繧ｲ繝ｼ繝縺ｮ蜃ｦ逅・

			current_time = SystemTimer_GetTime();		//繧ｷ繧ｹ繝・Β譎ょ綾繧貞叙蠕・
			double	elapsed_time = current_time - fps_last_time;//fps逕ｨ邨碁℃譎る俣
			//fps險域ｸｬ
			if (elapsed_time >= 1.0)	//1遘偵＃縺ｨ縺ｫ螳溯｡・
			{
				fps = frame_count / elapsed_time;
				fps_last_time = current_time;//FPS繧呈ｸｬ螳壹＠縺滓凾蛻ｻ
				frame_count = 0;	//繧ｫ繧ｦ繝ｳ繝医ｒ繧ｯ繝ｪ繧｢
			}

			//1/60遘偵＃縺ｨ縺ｫ螳溯｡・
			elapsed_time = current_time - exec_last_time;//繧ｲ繝ｼ繝譖ｴ譁ｰ逕ｨ邨碁℃譎る俣
			if ((elapsed_time) >= (1.0 / 60.0))
			{
				exec_last_time = current_time;	//蜃ｦ逅・＠縺滓凾髢薙ｒ陦ｨ遉ｺ

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

	/*邨ゆｺ・・逅・(蛻晄悄蛹悶→騾・・↓)*/
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
	//繝輔ぃ繧､繝翫Λ繧､繧ｺ螟壼・雜ｳ繧翫↑縺・

	return (int)msg.wParam;
}

// ================================================================
// 繧ｦ繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣
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
		if (MessageBoxA(hWnd, "Really quit?", "Confirm", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}

	//case WM_KEYDOWN:
	//	//Esc繧ｭ繝ｼ縺梧款縺輔ｌ縺溘→縺・
	//	if (wParam == VK_ESCAPE)
	//	{
	//		//髢峨§繧九・繧ｿ繝ｳ縺梧款縺輔ｌ縺溘→縺阪・繝｡繝・そ繝ｼ繧ｸ騾∽ｿ｡
	//		SendMessage(hWnd, WM_CLOSE, 0, 0);
	//	}
		break;
	case WM_DESTROY:		//繧ｦ繧｣繝ｳ繝峨え縺ｮ遐ｴ螢翫Γ繝・そ繝ｼ繧ｸ
		PostQuitMessage(0);	//WM_QUIT繝｡繝・そ繝ｼ繧ｸ縺ｮ騾∽ｿ｡
		break;
	default:
		//騾壼ｸｸ縺ｮ繝｡繝・そ繝ｼ繧ｸ蜃ｦ逅・・縺薙・髢｢謨ｰ縺ｫ莉ｻ縺帙ｋ
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

