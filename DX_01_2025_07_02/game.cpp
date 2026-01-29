// ==============================================================
// game.cpp	
// ゲームシーン制御
// 
// 制作者:Momoka Ishijima		制作日付：2025/??/??	
// 更新者:Kazuki Murakami		更新日付：2025/12/16
// ==============================================================
#include "game.h"

#include <iostream>
#include <DirectXMath.h>
#include <array>

#include "scene.h"

#include "debug_camera.h"
#include "active_camera.h"
#include "grid.h"

#include "object.h"
#include "player.h"
#include "cat.h"
#include "thief.h"
#include "background.h"
#include "map.h"
#include "map_object.h"
#include "map_object_shadow.h"
#include "lamp.h"
#include "switch.h"

#include "render_queue.h"
#include "direct3D.h"
#include "sprite.h"
#include "billboard.h"
#include "fade.h"
#include "cube.h"
#include "light.h"
#include "model.h"
#include "keylogger.h"
#include "end_game.h"
#include "timer.h"
#include "UI.h"

using namespace DirectX;

// ===============================================
// グローバル変数
// ===============================================
extern std::array<Player, PLAYER_MAX>player;
extern std::array<Cat, CAT_MAX> cat;
extern std::array<Thief, THIEF_MAX> thief;
extern std::array<Background, BACKGROUND_MAX> background;
extern std::array<std::vector<MapObject>, STAGE_MAX> g_MapObjects;
extern std::array<Lamp, LAMP_MAX> g_Lamp;
extern std::array<Switch, SWITCH_MAX> g_Switch;

namespace
{
	Game game;
	RenderQueue g_renderQueue;
	Timer g_EndTimer;

	DebugCamera* g_pCamera = GetDebugCamera();
	ActiveCamera* g_pActiveCamera = GetActiveCamera();
	EndGame* g_pEndGame = GetEndGame();
}

// ===============================================
// ゲーム初期化処理
// void Game_Initialize(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Game_Initialize(void)
{	
	// デバッグ項目の設定
	game.SetGameMode(DebugMode);	// モード設定
	game.SetGridDraw(game.GetGameMode() == DebugMode ? true : false);		// グリッド非表示

	// デフォルトカメラモードをアクティブカメラに設定
	game.SetCameraMode(ACTIVE_CAMERA);

	g_EndTimer.Start(4000.0f);

	Grid_Initialize(100, 100, 1.0f);

	g_pEndGame->Initialize();

	Light_Initialize();

	UI_Init();

	Fade_Start(0.0f, true);

	for (auto& player : player)
	{
		player.Initialize();
	}

	for (auto& mapObject : g_MapObjects[game.GetStageNumber()])
	{
		mapObject.Initialize();
	}

	Map_Initialize();

	for (auto& Switch : g_Switch)
	{
		Switch.Initialize();
	}

	for (auto& background : background)
	{
		background.Initialize();
	}

	for (auto& lamp : g_Lamp)
	{
		lamp.Initialize();
	}

	return;
}

// ===============================================
// ゲーム終了処理
// void Game_Finalize(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Game_Finalize(void)
{
	Light_Finalize();

	Map_Finalize();

	//Title_Finalize();
	g_pEndGame->Finalize();

	Cube_Finalize();
	Grid_Finalize();

	for (auto& player : player)
	{
		player.Finalize();
	}

	for (auto& mapObject : g_MapObjects[game.GetStageNumber()])
	{
		mapObject.Finalize();
	}

	for (auto& Switch : g_Switch)
	{
		Switch.Finalize();
	}

	for (auto& background : background)
	{
		background.Finalize();
	}

	for (auto& lamp : g_Lamp)
	{
		lamp.Finalize();
	}
	UI_Fina();

	return;
}

// ===============================================
// ゲーム更新処理
// void Game_Update(double elpased_time)
// 
// 型：void
// 返り値：無し
// 引数：elpased_time		double型	経過時間
// ===============================================
void Game_Update(double elapsed_time)
{
	// ===============================
	// End / UI 系
	// ===============================
	g_EndTimer.Update((float)elapsed_time);
	UI_Update(elapsed_time);

	if (g_EndTimer.IsFinished())
	{
		g_pEndGame->SetEnd(g_pEndGame->GameOver);
	}

	// ===============================
	// ゲームオーバー中
	// ===============================
	if (g_pEndGame->GetEnd() == g_pEndGame->GameOver)
	{
		g_EndTimer.Pause();

		if (KeyLogger_IsTrigger(KK_ENTER))
		{
			Scene_SetNextScene(SCENE_TITLE);
		}
		return; // ← ここ重要：下の通常更新を止める
	}

	// ===============================
	// 通常ゲーム更新
	// ===============================

	// カメラ切り替え
	if (KeyLogger_IsPressed(KK_D1) && game.GetGameMode() == DebugMode)
	{
		game.SetCameraMode(ACTIVE_CAMERA);
	}
	if (KeyLogger_IsPressed(KK_D2) && game.GetGameMode() == DebugMode)
	{
		game.SetCameraMode(DEBUG_CAMERA);
	}

	if (game.GetCameraMode() == ACTIVE_CAMERA)
	{
		g_pActiveCamera->Update(elapsed_time);
	}
	else //(game.GetCameraMode() == DEBUG_CAMERA)
	{
		g_pCamera->Update(elapsed_time);
	}

	// グリッド表示
	if (KeyLogger_IsPressed(KK_D3) && game.GetGameMode() == DebugMode)
	{
		game.SetGridDraw(true);
	}
	if (KeyLogger_IsPressed(KK_D4) && game.GetGameMode() == DebugMode)
	{
		game.SetGridDraw(false);
	}

	// ===============================
	// オブジェクト更新
	// ===============================
	Map_Update(elapsed_time);
	g_pEndGame->Update(elapsed_time);

	Grid_Update(elapsed_time);
	Billborad_Update(elapsed_time);

	for (auto& player : player)
	{
		player.Update(elapsed_time);
	}

	for (auto& mapObject : g_MapObjects[game.GetStageNumber()])
	{
		mapObject.Update(elapsed_time);
	}

	for (auto& Switch : g_Switch)
	{
		Switch.Update(elapsed_time);
	}

	for (auto& background : background)
	{
		background.Update(elapsed_time);
	}

	for (auto& lamp : g_Lamp)
	{
		lamp.Update(elapsed_time);
	}
}

// ===============================================
// ゲーム描画処理
// void Game_Draw(void)
// 
// 型：void
// 返り値：無し
// 引数：無し
// ===============================================
void Game_Draw(void)
{
	if (game.GetCameraMode() == ACTIVE_CAMERA)
	{
		g_pActiveCamera->SetMatrix();
	}
	if (game.GetCameraMode() == DEBUG_CAMERA)
	{
		g_pCamera->SetMatrix();
	}	

	Direct3D_Begin3D();
	Direct3D_SetDepthStencilState(true);

	if (game.GetGridDraw() == true)
	{
		Grid_Draw();
	}

	Light_SetAmbient({ 0.1f,0.02f,-0.02f,1.0f });

	XMFLOAT3 direction;
	XMStoreFloat3(&direction, { -5.0f,-1.5f, 5.0f });
	Light_SetDiffuse({ 1.0f,1.0f,1.0f }, direction);

	Map_Draw();

	g_renderQueue.Clear();

	for (auto& player : player)
	{
		g_renderQueue.Add(
			player.GetPos(),
			RenderLayer::Transparent,
			[&]() { player.Draw(); }
		);
	}

	for (auto& mapObject : g_MapObjects[game.GetStageNumber()])
	{
		g_renderQueue.Add(
			mapObject.GetPos(),
			RenderLayer::Transparent,
			[&]() { mapObject.Draw(); }
		);
	}

	for (auto& lamp : g_Lamp)
	{
		g_renderQueue.Add(
			lamp.GetPos(),
			RenderLayer::Transparent,
			[&]() { lamp.Draw(); }
		);
	}

	for (auto& Switch : g_Switch)
	{
		g_renderQueue.Add(
			Switch.GetPos(),
			RenderLayer::Transparent,
			[&]() { Switch.Draw(); }
		);
	}

	XMVECTOR xTarget = XMLoadFloat3(&g_pActiveCamera->GetPos()) + XMLoadFloat3(&g_pActiveCamera->GetFront());
	XMMATRIX ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&g_pActiveCamera->GetPos()), xTarget, XMLoadFloat3(&g_pActiveCamera->GetUp()));

	// 描画
	g_renderQueue.DrawAll(g_pActiveCamera->GetPos(),ViewMatrix);

	Direct3D_Begin2D();

	UI_Draw();

	Direct3D_SetDepthStencilState(false);

	//　プレイヤー影ゲージ描画
	for (auto player : player)
	{
		player.GetShadow().Draw();
	}

	g_pEndGame->Draw();

	Direct3D_SetDepthStencilState(true);

	return;
}

Game* GetGame(void)
{
	return &game;
}
