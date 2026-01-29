// ==============================================================
// game.h	
// ゲームシーン制御
// 
// 制作者:Momoka Ishijima		制作日付：2025/??/??	
// 更新者:Kazuki Murakami		更新日付：2025/12/16
// ==============================================================
#ifndef GAME_H
#define	GAME_H

// ===============================================
// 定数定義
// ===============================================
static constexpr unsigned int STAGE_MAX = 4;

enum GameMode{
	DebugMode = 0,
	ReleaseMode,
};

enum CameraMode
{
	ACTIVE_CAMERA = 0,
	DEBUG_CAMERA,
};

enum StageNumber
{
	Stage1 = 0,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
};


class Game {
private:
	GameMode m_GameMode{ ReleaseMode };
	int m_CameraMode{ ACTIVE_CAMERA };
	int m_StageNumber{ Stage1 };
	bool m_GridDraw{ false };

public:
	const GameMode& GetGameMode(void) const { return m_GameMode; }
	void SetGameMode(GameMode mode) { m_GameMode = mode; }

	const int& GetCameraMode(void) const { return m_CameraMode; }
	void SetCameraMode(int mode) { m_CameraMode = mode; }

	const int& GetStageNumber(void) const { return m_StageNumber; }
	void SetStageNumber(int number) { m_StageNumber = number; }

	const bool& GetGridDraw(void) const { return m_GridDraw; }
	void SetGridDraw(bool draw) { m_GridDraw = draw; }
};

// ===============================================
// プロトタイプ宣言
// ===============================================
void Game_Initialize(void);
void Game_Finalize(void);
void Game_Update(double elpased_time);
void Game_Draw(void);

Game* GetGame(void);

#endif // !GAME_H