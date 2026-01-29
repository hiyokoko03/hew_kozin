// ==============================================================
// event.h	
// イベント呼び出し制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/26	
// 更新者:Kazuki Murakami		更新日付：2025/11/26
// ==============================================================
#ifndef EVENT_H
#define EVENT_H

#include <random>
#include <vector>
#include <algorithm>
#include <iostream>

std::vector<float> GenerateEventTimes(int totalTime, int excludeFirstSeconds,int min, int max);
float GetNextEventTime();

#endif