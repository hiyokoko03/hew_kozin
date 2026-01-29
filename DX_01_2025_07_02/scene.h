// ==============================================================
// scene.h	
// シーン制御
// 
// 制作者:Ishijima	Momoka		制作日付：2025/09/17	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef SCENE_H
#define	SCENE_H

// ===============================================
// 列挙体宣言
// ===============================================
enum Scene
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,

	SCENE_MAX
};

// ===============================================
// プロトタイプ宣言
// ===============================================
void	Scene_Initialize();
void	Scene_Finalize();
void	Scene_Update(double	elapsed_time);
void	Scene_Draw();

void	Scene_SetNextScene(Scene scene);
void	Scene_ChangeScene();

#endif // !SCENE_H