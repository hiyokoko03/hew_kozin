/*================================================
リザルト画面[result.h]
Ishijima	Momoka	2025/09/21
================================================*/

#include	"result.h"
#include	"scene.h"
#include	"result_score.h"
//#include	"game_score.h"	//

#include	"keylogger.h"
#include	"texture.h"
#include	"sprite.h"
#include	"fade.h"
#include	"Audio.h"

#include	"direct3D.h"

static	int	g_TexResultID = -1;

static	double	g_AccumulatedTime = 0.0;
static	double	g_KeyInputTime = 0.0;

//static	int	g_ResultScore = 0;	//

enum ResultState
{
	RESULT_STATE_FADE_IN,
	RESULT_STATE_KEYINPUT_WAIT,
	RESULT_STATE_KEYINPUT_ACTION,
	RESULT_STATE_FADE_OUT,
	RESULT_STATE_MAX
};

static	ResultState	g_ResultState = RESULT_STATE_FADE_IN;

void Result_Initialize()
{
	g_TexResultID = Texture_Load(L"resource/texture/result.png");
	Fade_Start(1.0f, true);
	g_ResultState = RESULT_STATE_FADE_IN;
	//g_ResultScore = GameScore_GetScore();	//
	ResultScore_Initialize(Direct3D_GetBackBufferWidth() / 3, Direct3D_GetBackBufferHeight() / 2, 7);
}

void Result_Finalize()
{
	ResultScore_Finalize();
}

void Result_Update(double elapsed_time)
{
	g_AccumulatedTime += elapsed_time;

	switch (g_ResultState)
	{
	case RESULT_STATE_FADE_IN:
		if (Fade_GetState() == FADE_STATE_FADE_IN_FINISHED)
		{
			g_ResultState = RESULT_STATE_KEYINPUT_WAIT;
		}
		break;
	case RESULT_STATE_KEYINPUT_WAIT:
		ResultScore_Update();
		if (KeyLogger_IsTrigger(KK_ENTER))
		{
			g_ResultState = RESULT_STATE_KEYINPUT_ACTION;
			g_KeyInputTime = g_AccumulatedTime;
			//サウンド
		}
		break;
	case RESULT_STATE_KEYINPUT_ACTION:
		if (g_AccumulatedTime - g_KeyInputTime > 1.0)
		{
			g_ResultState = RESULT_STATE_FADE_OUT;
			Fade_Start(1.0f, false);
		}
		break;
	case RESULT_STATE_FADE_OUT:
		if (Fade_GetState() == FADE_STATE_FADE_OUT_FINISHED)
		{
			//再挑戦ならEnter/終了ならEsc
			Scene_SetNextScene(SCENE_GAME);
		}
		break;
	}
}

void Result_Draw()
{
	Sprite_Draw(g_TexResultID, 0.0f, 0.0f, Texture_GetWidth(g_TexResultID), Texture_GetHeight(g_TexResultID));
	ResultScore_Draw();
}

//void	Result_SetScore(int	score)
//{
//	g_ResultScore = score;
//}
