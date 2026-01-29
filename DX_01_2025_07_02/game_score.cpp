#include	"game_score.h"
#include	"texture.h"
#include	"sprite.h"
#include	<algorithm>

//#include	"result.h"

static	int	g_ScoreTexID = -1;
static	int	g_Score = 0;
static	int	g_ShowScore = 0;
static	int	g_DigitMax = 0;
static	float	g_Dx = 0.0f;
static	float	g_Dy = 0.0f;
static	int	g_CountStopValie = 0;

static	constexpr	float	Tex_Width = 512;
static	constexpr	float	Tex_Height = 512;

static	constexpr	float	NumberWidth = 32.0f;
static	constexpr	float	NumberHeight = 32.0f;

//static	constexpr	int		PartXCount = 16;
//static	constexpr	int		PartYCount = 16;
static	constexpr	int		PartCount = 16;

void	GameNumberDraw(int number, float x, float y)
{
	/*number.png*/
	//Sprite_Draw(g_ScoreTexID, x, y,
	//	32.0f, 32.0f,
	//	(number % 5) * (Tex_Width / 5),(number / 5) * (Tex_Height / 5),
	//	Tex_Width / 5,Tex_Height / 5);

	/*consolab_ascii_512.png*/
	//横16*縦16？
	
	Sprite_Draw(g_ScoreTexID, x, y,
		NumberWidth, NumberHeight,
		number * (Texture_GetWidth(g_ScoreTexID) / PartCount), Texture_GetHeight(g_ScoreTexID) / PartCount,
		Texture_GetWidth(g_ScoreTexID) / PartCount, Texture_GetHeight(g_ScoreTexID) / PartCount
	);


}


void GameScore_Initialize(float	x, float	y, int	digit)
{
	//g_ScoreTexID = Texture_Load(L"resource/texture/number.png");
	g_ScoreTexID = Texture_Load(L"resource/texture/consolab_ascii_512.png");

	g_Score = 0;
	//GameScore_Reset();
	g_ShowScore = 0;
	g_DigitMax = digit;
	g_Dx = x;
	g_Dy = y;

	//カンスト
	g_CountStopValie = 1;
	for (int i = 0; i < g_DigitMax; i++)
	{
		g_CountStopValie *= 10;
	}
	g_CountStopValie--;
}

void GameScore_Finalize()
{
}

void GameScore_Update()
{
	if (g_ShowScore < g_Score)
	{
		g_ShowScore++;
	}
}

void GameScore_Draw()
{
	int	temp = std::min(g_ShowScore, g_CountStopValie);

	for (int i = 0; i < g_DigitMax; i++)
	{
		int dd = g_DigitMax - 1 - i;
		int n = temp % 10;
		//numberDraw(n, g_Dx + (Tex_Width / PartCount) * dd, g_Dy);
		GameNumberDraw(n, g_Dx + NumberWidth * dd, g_Dy);
		temp /= 10;
	}
}

void GameScore_AddScore(int score)
{
	g_ShowScore = g_Score;
	g_Score += score;
}

void GameScore_Reset()
{
	g_Score = 0;
}

int GameScore_GetScore()
{
	return	g_ShowScore;
}

