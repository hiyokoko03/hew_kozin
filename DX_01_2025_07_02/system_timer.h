// ==============================================================
// system_timer.h	
// システムタイマー制御
// 
// 制作者:Youhei Sato      		制作日付：2018/06/17	
// 更新者:Kazuki Murakami		更新日付：2025/10/28
// ==============================================================
#ifndef SYSTEM_TIMER_H_
#define SYSTEM_TIMER_H_

// ===============================================
// プロトタイプ宣言
// ===============================================
// システムタイマーの初期化
void SystemTimer_Initialize(void);

// システムタイマーのリセット
void SystemTimer_Reset(void);

// システムタイマーのスタート
void SystemTimer_Start(void);

// システムタイマーのストップ
void SystemTimer_Stop(void);

// システムタイマーを0.1秒進める
void SystemTimer_Advance(void);

// 計測時間の取得
double SystemTimer_GetTime(void);

// 現在の時間を取得
double SystemTimer_GetAbsoluteTime(void);

// 経過時間の取得
float SystemTimer_GetElapsedTime(void);

// システムタイマーが止まっているか？
bool SystemTimer_IsStoped(void);

// 現在のスレッドを1つのプロセッサ（現在のスレッド）に制限
void LimitThreadAffinityToCurrentProc(void);

#endif // SYSTEM_TIMER_H_
