// ==============================================================
// event.cpp	
// イベント呼び出し制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/26	
// 更新者:Kazuki Murakami		更新日付：2025/11/26
// ==============================================================
#include "event.h"

static std::vector<float> g_EventTimes;
static int g_CurrentIndex = 0;

std::vector<float> GenerateEventTimes(int totalTime, int excludeFirstSeconds, int min, int max)
{
    std::random_device rd;
    std::mt19937 mt(rd());

    // イベント回数  min～max
    std::uniform_int_distribution<int> dist_count(min, max);
    int eventCount = dist_count(mt);

    // 出力をクリア
    g_EventTimes.clear();
    g_EventTimes.reserve(eventCount);

    // ---- 重要 ----
    // 最初の数秒は絶対にイベントが起きない
    float t = static_cast<float>(excludeFirstSeconds);

    // 残り時間（ここが「間隔の合計として使える総容量」）
    float remain = static_cast<float>(totalTime) - t;

    // ランダムな間隔をためる
    std::vector<float> intervals;
    intervals.reserve(eventCount);

    const float minInterval = 1.0f;  // 1秒以上の間隔にしたい場合

    for (int i = 0; i < eventCount; i++)
    {
        // このイベント以降も間隔を確保するため：
        // 残りイベント数ぶんの最低値を差し引く
        float maxInterval = remain - (eventCount - i - 1) * minInterval;

        if (maxInterval < minInterval)
        {
            maxInterval = minInterval;
        }

        // 適度にバラけさせる
        std::uniform_real_distribution<float> dist(minInterval, maxInterval);

        float interval = dist(mt);

        intervals.push_back(interval);
        remain -= interval;
    }

    // 間隔→発生時刻に変換
    for (float dt : intervals)
    {
        t += dt;
        g_EventTimes.push_back(t);
    }

    // 念のため昇順（interval累積なのでほぼ昇順）
    std::sort(g_EventTimes.begin(), g_EventTimes.end());

    g_CurrentIndex = 0;

    return g_EventTimes;
}

float GetNextEventTime()
{
	if (g_CurrentIndex >= g_EventTimes.size())
		return -1.0f;

	return g_EventTimes[g_CurrentIndex++];
}