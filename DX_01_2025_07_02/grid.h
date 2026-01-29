/*==============================================================================
grid‚Ì•`‰æ[grid.h]
Ishijima Momoka		2025/10/21
==============================================================================*/
#ifndef GRID_H
#define	GRID_H

#include	"texture.h"
#include	<d3d11.h>

void	Grid_Initialize(int x_count, int z_count, float widthsize);
void	Grid_Finalize();
void	Grid_Update(double elapsed_time);
void	Grid_Draw();

#endif // !GRID_H


#pragma once
