#ifndef	UI_H
#define	UI_H

#include	"texture.h"
#include	"sprite.h"
#include	"FontWrite.h"

#include	"direct3D.h"


void	UI_Init();
void	UI_Fina();
void	UI_Update(double elp);
void	UI_Draw();

void	UI_TokeiDraw();
void	UI_TokeiRoteUpdate();
void	UI_TokeiReset();

void	UI_TextDraw();

void	UI_TextUpdate();

#endif // !UI_H
