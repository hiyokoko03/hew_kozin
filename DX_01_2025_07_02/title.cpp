// ==============================================================
// title.cpp	
// タイトルシーン制御
// 
// 制作者:Momoka Ishijima		制作日付：2025/09/12
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#include	"title.h"

#include <DirectXMath.h>

#include	"keylogger.h"
#include	"texture.h"
#include	"sprite.h"
#include	"fade.h"
#include	"Audio.h"
#include	"scene.h"

// ===============================================
// 列挙体宣言
// ===============================================
enum TitleState
{
	TITLE_STATE_FADE_IN,
	TITLE_STATE_KEYINPUT_WAIT,
	TITLE_STATE_KEYINPUT_ACTION,
	TITLE_STATE_FADE_OUT,
	TITLE_STATE_MAX
};

// ===============================================
// グローバル変数
// ===============================================
static	int	g_TexTitleID = -1;
static	int	g_TexKeyID = -1;

static	double	g_AccumulatedTime = 0.0;
static	double	g_KeyInputTime = 0.0;

static	TitleState	g_TitleState = TITLE_STATE_FADE_IN;

void Title_Initialize()
{
	//g_TexTitleID = Texture_Load(L"resource/texture/mino03.png");
	g_TexTitleID = Texture_Load(L"resource/texture/title.png");
	g_TexKeyID = Texture_Load(L"resource/texture/mino03.png");
	Fade_Start(0.0f, true);

	g_TitleState = TITLE_STATE_FADE_IN;
}

void Title_Finalize()
{
}

void Title_Update(double elapsed_time)
{
	g_AccumulatedTime += elapsed_time;

	switch (g_TitleState)
	{
	case TITLE_STATE_FADE_IN:
		if (Fade_GetState() == FADE_STATE_FADE_IN_FINISHED)
		{
			g_TitleState = TITLE_STATE_KEYINPUT_WAIT;
		}
		break;
	case TITLE_STATE_KEYINPUT_WAIT:
		if (KeyLogger_IsTrigger(KK_ENTER))
		{
			g_TitleState = TITLE_STATE_KEYINPUT_ACTION;
			g_KeyInputTime = g_AccumulatedTime;
			//サウンド
		}
		break;
	case TITLE_STATE_KEYINPUT_ACTION:
		//if (g_AccumulatedTime - g_KeyInputTime > 1.0)
		//{
			g_TitleState = TITLE_STATE_FADE_OUT;
			Fade_Start(0.0f, false);
		//}
		break;
	case TITLE_STATE_FADE_OUT:
		if (Fade_GetState() == FADE_STATE_FADE_OUT_FINISHED)
		{
			Scene_SetNextScene(SCENE_GAME);
		}
		break;
	}

}

void Title_Draw()
{
	Sprite_Draw(g_TexTitleID, 0.0f, 0.0f, (float)Texture_GetWidth(g_TexTitleID), (float)Texture_GetHeight(g_TexTitleID), { 1.0f, 1.0f, 1.0f, 1.0f });
}
