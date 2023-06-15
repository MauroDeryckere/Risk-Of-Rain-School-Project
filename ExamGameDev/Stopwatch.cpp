#include "pch.h"
#include "Stopwatch.h"

Stopwatch::Stopwatch(float maxSeconds):
	m_MaxSeconds{ maxSeconds },
	m_CurrentSeconds{ 0.f },

	m_IsTimeReached{ false },	
	m_IsRunning{false}
{

}

void Stopwatch::Update(float elapsedSec)
{
	if (m_IsRunning)
	{
		m_IsTimeReached = false;
		m_CurrentSeconds += elapsedSec;

		if (m_CurrentSeconds >= m_MaxSeconds)
		{
			m_IsTimeReached = true;
			m_CurrentSeconds = 0.f;
		}
	}
}

void Stopwatch::Start()
{
	m_IsRunning = true;
}

void Stopwatch::Stop()
{
	m_IsRunning = false;
	m_IsTimeReached = false;
}

void Stopwatch::Reset()
{
	Stop();
	m_CurrentSeconds = 0.f;
}

bool Stopwatch::IsRunning() const
{
	return m_IsRunning;
}

bool Stopwatch::IsTimeReached() const
{
	return m_IsTimeReached;
}

const float Stopwatch::GetMaxSec() const
{
	return m_MaxSeconds;
}

const float Stopwatch::GetCurrSec() const
{
	return m_CurrentSeconds;
}
