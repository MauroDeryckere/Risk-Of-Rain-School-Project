#include "pch.h"
#include "TimeObjectManager.h"

#include "StopwatchManager.h"
#include "Timer.h"

#include <iostream>

TimeObjectManager::~TimeObjectManager()
{
	for (size_t index{ 0 }; index < m_pStopwatchManagers.size(); ++index)
	{
		delete m_pStopwatchManagers[index];
	}
	for (size_t index{ 0 }; index < m_pTimers.size(); ++index)
	{
		delete m_pTimers[index];
	}
}


void TimeObjectManager::Update(float elapsedSec)
{
	for (size_t index{0}; index < m_pStopwatchManagers.size(); ++index)
	{
		m_pStopwatchManagers[index]->Update(elapsedSec);
	}
	for (size_t index{ 0 }; index < m_pTimers.size(); ++index)
	{
		m_pTimers[index]->Update(elapsedSec);
	}
}

Timer* TimeObjectManager::CreateTimer()
{
	Timer* pTimer{ new Timer{ } };
	m_pTimers.emplace_back(pTimer);

	return pTimer;
}

StopwatchManager* TimeObjectManager::CreateStopwatchManager( )
{
	StopwatchManager* pStopwatchManager{ new StopwatchManager{} };
	m_pStopwatchManagers.emplace_back(pStopwatchManager);

	return pStopwatchManager;
}

bool TimeObjectManager::DeleteTimer(Timer* pTimer)
{
	for (size_t index{ 0 }; index < m_pTimers.size(); ++index)
	{
		if (m_pTimers[index] == pTimer)
		{
			delete m_pTimers[index];

			m_pTimers[index] = m_pTimers.back();
			m_pTimers.pop_back();

			return true;
		}
	}

	std::cerr << "Timer doesn't exist \n";
	return false;
}

bool TimeObjectManager::DeleteStopwatchManager(StopwatchManager* pStopwatchManager)
{
	for (size_t index{ 0 }; index < m_pStopwatchManagers.size(); ++index)
	{
		if (m_pStopwatchManagers[index] == pStopwatchManager)
		{
			delete m_pStopwatchManagers[index];

			m_pStopwatchManagers[index] = m_pStopwatchManagers.back();
			m_pStopwatchManagers.pop_back();

			return true;
		}
	}

	std::cerr << "StopwatchManager doesn't exist \n";
	return false;
}
