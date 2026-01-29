// ==============================================================
// timer.h	
// タイマー制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/26	
// 更新者:Kazuki Murakami		更新日付：2025/11/26
// ==============================================================
#ifndef TIMER_H
#define TIMER_H

// ===============================================
// クラス宣言
// ===============================================
class Timer
{
private:
    float m_time;      // 残り時間
    float m_duration;  // 開始時の時間
    bool  m_running;   // 稼働中か
    bool  m_paused;    // 一時停止中か

public:
    Timer();
    ~Timer() = default;

    // timeSec 秒のカウントダウンタイマーを開始
    void Start(float timeSec);

    // 経過時間の加算（毎フレーム呼ぶ）
    void Update(float elapsed_time);

    // 一時停止
    void Pause();

    // 再開
    void Resume();

    // 完全停止（タイマー終了扱い）
    void Stop();

    // タイマーが動いているか？
    bool IsRunning() const { return m_running && !m_paused; }

    // タイマーが終了したか？
    bool IsFinished() const { return (!m_running && m_time <= 0.0f); }

    // 残り時間
    float GetRemaining() const { return m_time; }

    // セットされた元の時間
    float GetDuration() const { return m_duration; }

    // 経過時間（Stopwatch 的用途にも）
    float GetElapsed() const { return m_duration - m_time; }
};

#endif // TIMER_H_