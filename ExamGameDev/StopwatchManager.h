#pragma once
#include <vector>
class Stopwatch;

class StopwatchManager final
{
	public:
		StopwatchManager() = default;
		~StopwatchManager();

		StopwatchManager(const StopwatchManager&) = delete;
		StopwatchManager& operator=(const StopwatchManager&) = delete;
		StopwatchManager(StopwatchManager&&) = delete;
		StopwatchManager& operator=(StopwatchManager&&) = delete;
		
		void Update(float elapsedSec);

		Stopwatch* CreateStopwatch(const float maxSec);
		bool DeleteStopwatch(Stopwatch* pStopwatch);

	private:
		std::vector<Stopwatch*> m_pStopwatches;	
};


