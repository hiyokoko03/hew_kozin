// ==============================================================
// keylogger.h	
// キーボード入力の記録
// 
// 制作者:Ishijima	Momoka     	制作日付：2025/06/27	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef	KEYLOGGER_H
#define	KEYLOGGER_H

#include	"keyboard.h"

// ===============================================
// プロトタイプ宣言
// ===============================================
void	KeyLogger_Initialize();
void	KeyLogger_Update();

bool	KeyLogger_IsPressed(Keyboard_Keys key);
bool	KeyLogger_IsTrigger(Keyboard_Keys key);
bool	KeyLogger_IsRelease(Keyboard_Keys key);

//キーリピート	押した瞬間true離す瞬間false押してる間定期的にtrue

#endif // !KEYLOGGER_H