#include "stdafx.h"
#include "Timer.h"

void Timer::start()
{
    m_lastUpdate = std::chrono::steady_clock::now();
}

void Timer::reset()
{
    m_elapsedDeltaTime = 0.0;
}



void Timer::run()
{
   
   
    m_deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_lastUpdate).count() / 1000.0); //how much time in seconds between frames.
    m_lastUpdate = std::chrono::steady_clock::now();


    m_fps = 1.0f / m_deltaTime;
    if (isinf(m_fps))
    {
        m_fps = 0.0;
    }
    m_elapsedDeltaTime += m_deltaTime;
    m_lerpedDT = (m_deltaTime + m_fps - m_deltaTime) / m_fps;
    if (isnan(m_lerpedDT))
    {
        m_lerpedDT = 0.0;
    }
}


