#ifndef GAME_SCORE_H
#define	GAME_SCORE_H

void	GameScore_Initialize(float	x, float	y, int	digit);
void	GameScore_Finalize();
void	GameScore_Update();
void	GameScore_Draw();

void	GameScore_AddScore(int score);
void	GameScore_Reset();

int		GameScore_GetScore();

#endif // !GAME_SCORE_H

