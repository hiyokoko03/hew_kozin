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

#include "shadow_map.h"
#include <fstream>
#include <vector>

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

	// Shadow Mapping Resources
	ShadowMap* g_pShadowMap = nullptr;
	ID3D11VertexShader* g_pShadowVS = nullptr;
	ID3D11InputLayout* g_pShadowLayout = nullptr;
	ID3D11Buffer* g_pShadowCB = nullptr;
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

	// Initialize Shadow Map
	g_pShadowMap = new ShadowMap();
	g_pShadowMap->Initialize(Direct3D_GetDevice(), 1024, 1024);

	// Load Shadow Vertex Shader
	std::ifstream ifs("VertexShaderShadow.cso", std::ios::binary);
	if (ifs)
	{
		ifs.seekg(0, std::ios::end);
		size_t size = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		std::vector<char> binary(size);
		ifs.read(binary.data(), size);
		Direct3D_GetDevice()->CreateVertexShader(binary.data(), size, nullptr, &g_pShadowVS);

		// Create Input Layout (Same as Model)
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOOD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		Direct3D_GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), binary.data(), size, &g_pShadowLayout);
	}

	// Create Shadow Constant Buffer
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(XMFLOAT4X4) * 3; // World, View, Proj
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Direct3D_GetDevice()->CreateBuffer(&bd, nullptr, &g_pShadowCB);

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
	SAFE_RELEASE(g_pShadowCB);
	SAFE_RELEASE(g_pShadowLayout);
	SAFE_RELEASE(g_pShadowVS);
	if (g_pShadowMap) {
		g_pShadowMap->Finalize();
		delete g_pShadowMap;
		g_pShadowMap = nullptr;
	}

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
	// ===============================================
	// Shadow Pass
	// ===============================================
	Direct3D_SetRenderMode(RENDER_3D);
	g_pShadowMap->Begin(Direct3D_GetContext());

	// Calculate Light Matrices (Directional Light)
	XMVECTOR lightPos = XMVectorSet(10.0f, 20.0f, -10.0f, 1.0f); // Adjust based on scene
	XMVECTOR lightTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR lightUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX lightView = XMMatrixLookAtLH(lightPos, lightTarget, lightUp);
	// Approx scene size 20x20
	XMMATRIX lightProj = XMMatrixOrthographicLH(20.0f, 20.0f, 1.0f, 50.0f);

	// Update Shadow Constant Buffer
	struct ShadowCB {
		XMFLOAT4X4 world; // Placeholder, updated in ModelDraw/Shader3D but here we set View/Proj once
		XMFLOAT4X4 view;
		XMFLOAT4X4 proj;
	} scb;
	XMStoreFloat4x4(&scb.view, XMMatrixTranspose(lightView));
	XMStoreFloat4x4(&scb.proj, XMMatrixTranspose(lightProj));
	// Note: VS_SHADOW_BUFFER expected layout: World, LightView, LightProj.
	// But we bind global CB here. Individual objects need to update World Matrix.
	// Problem: Game drawing architecture relies on Shader3D to update World Matrix.
	// But we are skipping Shader3D_Begin.
	// Solution: We need a way to update the World Matrix for the Shadow Shader.
	// Since we can't easily hook into every Draw call, we might rely on the fact that
	// ModelDraw sets World Matrix in VSConstantBuffer0 (slot 0).
	// Our Shadow Shader expects World at b0. Perfect!
	// But we need to define the CB structure correctly.
	// VertexShaderShadow.hlsl: cbuffer VS_SHADOW_CONSTANT_BUFFER : register(b0) { world, lightView, lightProj }
	// This combines them. Standard Shader3D uses b0 for World, b1 for View, b2 for Proj.
	// To be compatible with ModelDraw (which updates b0), we should split the Shadow CB.
	// VertexShaderShadow.hlsl should match the layout of standard shader?
	// Or we just bind b0 (World) as usual. And b1 (View), b2 (Proj) for Light?
	// Let's change VertexShaderShadow.hlsl to match standard layout!
	// Then we don't need special "ShadowCB" binding, just overwrite View/Proj buffers!

	// Wait, ModelDraw calls Direct3D_GetContext()->UpdateSubresource(g_pVSConstantBuffer0...);
	// So b0 is taken care of.
	// We just need to update b1 (View) and b2 (Proj) with Light Matrices!
	// And use the Shadow VS.
	
	// Set Shadow Pass State
	Shader3D_SetShadowPass(true);
	
	// Bind Shadow VS (InputLayout reused from somewhere? No, we created g_pShadowLayout)
	Direct3D_GetContext()->VSSetShader(g_pShadowVS, nullptr, 0);
	Direct3D_GetContext()->IASetInputLayout(g_pShadowLayout);
	Direct3D_GetContext()->PSSetShader(nullptr, nullptr, 0);

	// Update View/Proj Buffers with Light Matrices
	Shader3D_SetViewMatrix(lightView);
	Shader3D_SetProjMatrix(lightProj);

	// Draw Everything for Shadow
	Map_Draw();
	for (auto& player : player) player.Draw();
	for (auto& mapObject : g_MapObjects[game.GetStageNumber()]) mapObject.Draw();
	for (auto& lamp : g_Lamp) lamp.Draw();
	for (auto& Switch : g_Switch) Switch.Draw();

	g_pShadowMap->End(Direct3D_GetContext());
	Shader3D_SetShadowPass(false);

	// ===============================================
	// Main Pass
	// ===============================================
	Direct3D_Begin3D(); // Resets Render Targets, viewport, depth

	// Restore Main Camera View/Proj (Direct3D_Begin3D doesn't do this, ActiveCamera does)
	if (game.GetCameraMode() == ACTIVE_CAMERA) {
		g_pActiveCamera->SetMatrix(); // Calls Shader3D_SetView/Proj
	}
	else {
		g_pCamera->SetMatrix();
	}

	Direct3D_SetDepthStencilState(true);

	if (game.GetGridDraw() == true)
	{
		Grid_Draw();
	}

	Light_SetAmbient({ 0.3f,0.3f,0.4f,1.0f }); // Brighter ambient for testing

	XMFLOAT3 direction;
	// Use same light direction
	XMStoreFloat3(&direction, { -10.0f,-20.0f, 10.0f }); 
	Light_SetDiffuse({ 1.0f,1.0f,1.0f }, direction);

	// Pass Light Matrices for Shadow Calculation
	Shader3D_SetLightMatrices(lightView, lightProj);

	// Bind Shadow Map to Slot 1
	ID3D11ShaderResourceView* shadowSRV = g_pShadowMap->GetSRV();
	Direct3D_GetContext()->PSSetShaderResources(1, 1, &shadowSRV);

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

	// Unbind Shadow Map
	ID3D11ShaderResourceView* nullSRV = nullptr;
	Direct3D_GetContext()->PSSetShaderResources(1, 1, &nullSRV);

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
