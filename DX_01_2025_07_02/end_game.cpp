// ==============================================================
// end_game.cpp	
// ゲーム終了制御
// 
// 制作者:村上和樹		制作日付：2025/12/01	
// 更新者:村上和樹		更新日付：2025/12/01
// ==============================================================
#include "end_game.h"

#include "texture.h"
#include "sprite.h"

EndGame endgame;
int EndGameTextureID[2] = {};

void EndGame::Initialize(void)
{
	EndGameTextureID[0] = Texture_Load(L"resource/texture/gameclear.png");
	EndGameTextureID[1] = Texture_Load(L"resource/texture/gameover.png");
}

void EndGame::Finalize(void)
{

}

void EndGame::Update(double elpased_time)
{

}

void EndGame::Draw(void)
{
	if (m_GameState == GameClear)
	{
		Sprite_Draw(EndGameTextureID[0], 960.0f, 540.0f, 1920, 1080);
	}

	if (m_GameState == GameOver)
	{
		Sprite_Draw(EndGameTextureID[1], 960.0f, 540.0f, 1920, 1080);
	}
}

EndGame* GetEndGame()
{
	return &endgame;
}
