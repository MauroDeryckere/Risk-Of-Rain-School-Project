#include "pch.h"

#include "StopwatchManager.h"
#include "Stopwatch.h"

StopwatchManager::~StopwatchManager()
{
	for (size_t index{ 0 }; index < m_pStopwatches.size(); ++index)
	{
		delete m_pStopwatches[index];
	}
}

void StopwatchManager::Update(float elapsedSec)
{
	for (size_t index{0}; index < m_pStopwatches.size(); ++index)
	{
		m_pStopwatches[index]->Update(elapsedSec);
	}
}

Stopwatch* StopwatchManager::CreateStopwatch(const float maxSec)
{
	Stopwatch* pStopwatch{ new Stopwatch{maxSec} };
	m_pStopwatches.emplace_back(pStopwatch);

	return pStopwatch;
}
