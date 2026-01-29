/*==============================================
	リザルトのスコア表示[result_score.h]
	Ishijima	Momoka		2025/09/22
==============================================*/
#ifndef RESULT_SCORE_H
#define	RESULT_SCORE_H


void	ResultScore_Initialize(float	x, float	y, int	digit);
void	ResultScore_Finalize();
void	ResultScore_Update();
void	ResultScore_Draw();

void	ResultScore_Reset();

#endif // !RESULT_SCORE_H


