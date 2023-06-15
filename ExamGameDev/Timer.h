#pragma once
class Timer
{
	public:
		Timer();
		~Timer() = default;

		void Update(float elapsedSec);
		void Start();
		void Pause();
		void Reset();

		float GetTime() const;

	private:
		float m_CurrentTime;
		bool m_IsPaused;
};

