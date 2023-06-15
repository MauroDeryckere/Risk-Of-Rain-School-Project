#pragma once
class Stopwatch final
{
public:
	Stopwatch(float maxSeconds);

	void Update(float elapsedSec);
	void Start();
	void Stop();
	void Reset();

	bool IsRunning() const;
	bool IsTimeReached() const;

	const float GetMaxSec() const;
	const float GetCurrSec() const;
	
private: 
	const float m_MaxSeconds;
	float m_CurrentSeconds;

	bool m_IsTimeReached;
	bool m_IsRunning;
};

