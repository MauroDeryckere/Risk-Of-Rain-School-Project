#pragma once
class StopwatchManager;
class Timer;

#include <vector>

class TimeObjectManager final
{
	public:
		TimeObjectManager() = default;
		~TimeObjectManager();

		TimeObjectManager(const TimeObjectManager&) = delete;
		TimeObjectManager& operator=(const TimeObjectManager&) = delete;
		TimeObjectManager(TimeObjectManager&&) = delete;
		TimeObjectManager& operator=(TimeObjectManager&&) = delete;

		void Update(float elapsedSec);

		Timer* CreateTimer();
		StopwatchManager* CreateStopwatchManager( );

		bool DeleteTimer(Timer* pTimer);
		bool DeleteStopwatchManager(StopwatchManager* pStopwatchManager);

	private:
		std::vector<StopwatchManager*> m_pStopwatchManagers;
		std::vector<Timer*> m_pTimers;
};

