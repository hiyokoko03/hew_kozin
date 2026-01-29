#include	"UI.h"
#include	"texture.h"
#include	"sprite.h"
#include	"FontWrite.h"

#include	"direct3D.h"
#include	"DirectXMath.h"

#include	"keylogger.h"

FontWrite font{};

int		g_TokeiBan = -1;
int		g_TokeiHari = -1;
int		g_Font = 0;
int		g_TextBox = -1;

float	tokeitime = 0.0f;
float	rote = -DirectX::XMConvertToRadians(360.0f / 12);
float	angle = 0.0f;
float	hari_angle = DirectX::XMConvertToRadians((360.0f / 12)*8);

static	float	textbox_x{};
static	float	textbox_y{};
static	float	textfont_x{};
static	float	textfont_y{};

const	std::string	g_fontward = "ƒRƒ“ƒRƒ“test";

int		i;
const	std::string	g_fontwards[5]
{
	"0",
	"1",
	"2",
	"3",
	"4",
};

void	UI_Init()
{
	font.Initialize(Direct3D_GetSwapChain());

	g_Font = font.LoadFontFile(L"resource/font/komorebi-gothic.ttf");
	g_TextBox = Texture_Load(L"resource/texture/UI/TextBox.png");

	g_TokeiBan = Texture_Load(L"resource/texture/UI/ClockFace.png");
	g_TokeiHari = Texture_Load(L"resource/texture/UI/ClockHand.png");

	textbox_x = Direct3D_GetBackBufferWidth() * 0.5f;
	textbox_y = Direct3D_GetBackBufferHeight() * 0.8f;

	textfont_x = textbox_x - Texture_GetWidth(g_TextBox) * 0.4f;
	textfont_y = textbox_y - Texture_GetHeight(g_TextBox) * 0.05f;
}

void	UI_Fina()
{
	font.Finalize();
}

void	UI_Update(double elp)
{
	//tokeitime += elp;
	//if (tokeitime > 1.0)
	//{
	//	angle += rote;
	//	tokeitime = 0.0f;
	//}
	tokeitime += elp;

	if (tokeitime > 1.0f)
	{
		UI_TokeiRoteUpdate();
	}

	UI_TextUpdate();
}

void	UI_Draw()
{
	UI_TokeiDraw();
	UI_TextDraw();
}

void	UI_TokeiDraw()
{
	Sprite_Draw(g_TokeiBan, Direct3D_GetBackBufferWidth(), 0.0f, Texture_GetWidth(g_TokeiBan), Texture_GetHeight(g_TokeiBan), angle);
	Sprite_Draw(g_TokeiHari, Direct3D_GetBackBufferWidth(), 0.0f, Texture_GetWidth(g_TokeiHari), Texture_GetHeight(g_TokeiHari), hari_angle);
}

void UI_TokeiRoteUpdate()
{
	if (tokeitime > 1.0)
	{
		angle += rote;
		tokeitime = 0.0f;
	}
}

void	UI_TokeiReset()
{
	tokeitime = 0.0f;
	angle = 0.0f;
}

void	UI_TextDraw()
{
	font.CreateTextFormat(g_Font, 40.0f);

	Sprite_Draw(g_TextBox, textbox_x, textbox_y, Texture_GetWidth(g_TextBox), Texture_GetHeight(g_TextBox));

	font.TextDraw(g_fontward, {textfont_x,textfont_y});
}

void	UI_TextUpdate()
{
	if (KeyLogger_IsTrigger(KK_ENTER))
	{
		i++;
	}
}
