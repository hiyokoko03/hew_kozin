// ==============================================================
// scene.cpp	
// シーン制御
// 
// 制作者:Ishijima	Momoka		制作日付：2025/09/17	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#include	"scene.h"

#include	"title.h"
#include	"game.h"
#include	"result.h"

// ===============================================
// グローバル変数
// ===============================================
static	Scene g_Scene = SCENE_GAME;	//開始シーンの設定
//static	Scene g_Scene = SCENE_RESULT;	//開始シーンの設定
static	Scene	g_SceneNext = g_Scene;	//

void Scene_Initialize()
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Initialize();
		break;
	case SCENE_GAME:
		Game_Initialize();
		break;
	case SCENE_RESULT:
		Result_Initialize();
		break;
	default:
		break;
	}

	/*
	g_Initialize[g_Scene]();
	*/
}

void Scene_Finalize()
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Finalize();
		break;
	case SCENE_GAME:
		Game_Finalize();
		break;
	case SCENE_RESULT:
		Result_Finalize();
		break;
	default:
		break;
	}
}

void Scene_Update(double elapsed_time)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Update(elapsed_time);
		break;
	case SCENE_GAME:
		Game_Update(elapsed_time);
		break;
	case SCENE_RESULT:
		Result_Update(elapsed_time);
		break;
	default:
		break;
	}
}

void Scene_Draw()
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Draw();
		break;
	case SCENE_GAME:
		Game_Draw();
		break;
	case SCENE_RESULT:
		Result_Draw();
		break;
	default:
		break;
	}
}

void Scene_SetNextScene(Scene scene)
{
	g_SceneNext = scene;
}

void Scene_ChangeScene()
{
	//シーンを切り替え
	if (g_Scene != g_SceneNext)
	{
		//ファイナライズ
		Scene_Finalize();
		//次シーンをセット
		g_Scene = g_SceneNext;
		//初期化
		Scene_Initialize();
	}
}
