// ==============================================================
// timer.cpp	
// タイマー制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/26	
// 更新者:Kazuki Murakami		更新日付：2025/11/26
// ==============================================================
#include "timer.h"

Timer::Timer()
    : m_time(0.0f)
    , m_duration(0.0f)
    , m_running(false)
    , m_paused(false)
{
}

void Timer::Start(float timeSec)
{
    if (timeSec < 0.0f) timeSec = 0.0f;

    m_duration = timeSec;
    m_time = timeSec;
    m_running = true;
    m_paused = false;
}

void Timer::Update(float elapsed_time)
{
    if (!m_running || m_paused)
        return;

    if (m_time <= 0.0f) {
        m_running = false;
        return;
    }

    m_time -= elapsed_time;
    if (m_time <= 0.0f) {
        m_time = 0.0f;
        m_running = false;
    }
}

void Timer::Pause()
{
    if (m_running)
        m_paused = true;
}

void Timer::Resume()
{
    if (m_running)
        m_paused = false;
}

void Timer::Stop()
{
    m_running = false;
    m_paused = false;
    m_time = 0.0f;
}