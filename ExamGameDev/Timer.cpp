#include "pch.h"
#include "Timer.h"

Timer::Timer():
    m_CurrentTime{0.f},
    m_IsPaused{true}
{
}

void Timer::Update(float elapsedSec)
{
    if (!m_IsPaused)
    {
        m_CurrentTime += elapsedSec;
    }
}

void Timer::Start()
{
    m_IsPaused = false;
}

void Timer::Pause()
{
    m_IsPaused = true;
}

void Timer::Reset()
{
    m_IsPaused = false;
    m_CurrentTime = 0.f;
}

float Timer::GetTime() const
{
    return m_CurrentTime;
}
