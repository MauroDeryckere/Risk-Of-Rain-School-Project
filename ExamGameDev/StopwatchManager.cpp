#include "pch.h"

#include "StopwatchManager.h"
#include "Stopwatch.h"

#include <iostream>

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

bool StopwatchManager::DeleteStopwatch(Stopwatch* pStopwatch)
{
	for (size_t index{ 0 }; index < m_pStopwatches.size(); ++index)
	{
		if (m_pStopwatches[index] == pStopwatch)
		{
			delete m_pStopwatches[index];

			m_pStopwatches[index] = m_pStopwatches.back();
			m_pStopwatches.pop_back();

			return true;
		}
	}

	std::cerr << "Stopwatch doesn't exist" << std::endl;
	return false;
}
