#include "pch.h"
#include "LevelObjectManager.h"

#include <iostream>
#include "LevelObject.h"

LevelObjectManager::~LevelObjectManager()
{
	ClearAllLevelObjects();
}

void LevelObjectManager::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	for (size_t index{0}; index < m_pLevelObjects.size(); ++index)
	{
		m_pLevelObjects[index]->Update(elapsedSec, pPlayer, pInput);
	}
}

void LevelObjectManager::Draw() const
{
	for (size_t index{0}; index < m_pLevelObjects.size(); ++index)
	{
		m_pLevelObjects[index]->Draw();
	}
}

bool LevelObjectManager::CreateLevelObject(LevelObject* pLevelObject)
{
	if (pLevelObject == nullptr)
	{
		std::cerr << "can't add levelObj, is nullptr \n";
		return false;
	}

	m_pLevelObjects.emplace_back(pLevelObject);
	return true;
}

bool LevelObjectManager::DeleteLevelObject(LevelObject* pLevelObject)
{
	//Chests are at back of arr (created last) so start at back
	for (size_t index{ m_pLevelObjects.size() - 1 }; index > 0; --index)
	{
		if (m_pLevelObjects[index] == pLevelObject)
		{
			delete m_pLevelObjects[index];

			m_pLevelObjects[index] = m_pLevelObjects.back();
			m_pLevelObjects.pop_back();

			return true;
		}
	}

	std::cerr << "LevelObj doesn't exist \n";
	return false;
}

bool LevelObjectManager::ClearAllLevelObjects()
{
	for (size_t index{ 0 }; index < m_pLevelObjects.size(); ++index)
	{
		delete m_pLevelObjects[index];
	}

	m_pLevelObjects.clear();
	return true;
}
