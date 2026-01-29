/*==============================================
	リザルトのスコア表示[result_score.h]
	Ishijima	Momoka		2025/09/22
==============================================*/
#include	"result_score.h"
#include	"game_score.h"

#include	"texture.h"
#include	"sprite.h"

static	int		g_ScoreTexID = -1;
static	int		g_ResultScore = 0;
static	int		g_DigitMax = 0;
static	float	g_Dx = 0.0f;
static	float	g_Dy = 0.0f;

static	constexpr	float	NumberWidth = 64.0f;
static	constexpr	float	NumberHeight = 64.0f;

static	constexpr	int		PartCount = 16;

void	ResultNumberDraw(int number, float x, float y)
{
	Sprite_Draw(g_ScoreTexID, x, y,
		NumberWidth, NumberHeight,
		number * (Texture_GetWidth(g_ScoreTexID) / PartCount), Texture_GetHeight(g_ScoreTexID) / PartCount,
		Texture_GetWidth(g_ScoreTexID) / PartCount, Texture_GetHeight(g_ScoreTexID) / PartCount
	);
}

void ResultScore_Initialize(float x, float y, int digit)
{
	g_ScoreTexID = Texture_Load(L"resource/texture/consolab_ascii_512.png");

	g_ResultScore = GameScore_GetScore();
	g_DigitMax = digit;
	g_Dx = x;
	g_Dy = y;


}

void ResultScore_Finalize()
{
}

void ResultScore_Update()
{
}

void ResultScore_Draw()
{
	int		temp = g_ResultScore;

	for (int i = 0; i < g_DigitMax; i++)
	{
		int	Digit = g_DigitMax - 1 - i;
		int	n = temp % 10;
		ResultNumberDraw(n, g_Dx + NumberWidth * Digit, g_Dy);
		temp /= 10;
	}
}

void ResultScore_Reset()
{
	g_ResultScore = 0;
}
