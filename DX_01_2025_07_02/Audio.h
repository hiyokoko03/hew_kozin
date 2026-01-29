// ==============================================================
// audio.cpp	
// オーディオ制御
// 
// 制作者:Ishijima	Momoka     	制作日付：2025/??/??	
// 更新者:Kazuki Murakami		更新日付：2025/10/29
// ==============================================================
#ifndef AUDIO_H
#define AUDIO_H

// ===============================================
// プロトタイプ宣言
// ===============================================
void Audio_Initialize();
void Audio_Finalize();

int LoadAudio(const char* FileName);
void UnloadAudio(int Index);
void PlayAudio(int Index, bool Loop = false);

#endif