#include "pch.h"

#include "LevelManager.h"
#include "LevelObjectManager.h"
#include "EnemyManager.h"
#include "TextFileDataReader.h"
#include "InputStructs.h"

#include "Level.h"
#include "Player.h"
#include "Rope.h"
#include "LaunchPad.h"
#include "Chest.h"

#include <iostream>

LevelManager::LevelManager():
	m_pLevelObjectManager{ new LevelObjectManager{ } },
	m_pEnemyManager{ new EnemyManager{ } },

	m_pCurrentLevel { nullptr }
{
}

LevelManager::~LevelManager()
{
	delete m_pCurrentLevel;
	delete m_pEnemyManager;
	delete m_pLevelObjectManager;
}

void LevelManager::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	m_pLevelObjectManager->Update(elapsedSec, pPlayer, pInput);
	m_pEnemyManager->Update(elapsedSec, m_pCurrentLevel, pPlayer);
	m_pCurrentLevel->Update(elapsedSec, pPlayer, pInput);
}

void LevelManager::Draw(const Point2f& cameraPos) const
{
	m_pCurrentLevel->Draw(cameraPos);

	m_pLevelObjectManager->Draw();
	m_pEnemyManager->Draw();
}

bool LevelManager::LoadStage1LevelObjects(TextureManager* pTextureManager, TimeObjectManager* pTimeObjectManager)
{
	//Ropes
	TextFileDataReader<RopeInput> ropeTextFileReader{ "Level/RopeInfo.txt" };
	const std::vector<RopeInput> ropeData{ ropeTextFileReader.ReadTextFile() };

	for (size_t index{ 0 }; index < ropeData.size(); ++index)
	{
		if (!m_pLevelObjectManager->CreateLevelObject(new Rope{ ropeData[index].shape, pTextureManager }))
		{
			return false;
		}
	}

	//LaunchPads
	TextFileDataReader<LaunchPadInput> launchPadTextFileReader{ "Level/LaunchPadInfo.txt" };
	const std::vector<LaunchPadInput> launchPadData{ launchPadTextFileReader.ReadTextFile() };

	for (size_t index{ 0 }; index < launchPadData.size(); ++index)
	{
		m_pLevelObjectManager->CreateLevelObject(new LaunchPad{ launchPadData[index].shape,
												 launchPadData[index].yVelocity,
												 pTimeObjectManager,
												 pTextureManager });
	}

	//Chests
	TextFileDataReader<ChestInput> chestTextFileReader{ "Level/ChestInfo.txt" };
	const std::vector<ChestInput> chestData{ chestTextFileReader.ReadTextFile() };

	for (size_t index{ 0 }; index < chestData.size(); ++index)
	{
		//random = 0, 
		//money = 1,
		//common = 2,
		//rare = 3,
		//legendary = 4

		unsigned int rarity{ chestData[index].rarity };

		if (rarity == 0)
		{
			constexpr unsigned int max{ 3 };
			constexpr unsigned int min{ 1 };

			rarity = utils::GetRandomNumber(max, min);
		}

		m_pLevelObjectManager->CreateLevelObject(new Chest{ chestData[index].bottomLeft,
												 Chest::Rarity(rarity),
												 m_pCurrentLevel,
												 pTimeObjectManager,
												 pTextureManager });
	}
}

bool LevelManager::InitializeStage1(TextureManager* pTextureManager, SoundManager* pSoundManager, TimeObjectManager* pTimeObjectManager)
{
	if (m_pCurrentLevel != nullptr)
	{
		std::cerr << "previous level wasn't deleted \n";
		return false;
	}

	m_pCurrentLevel = new Level{ pTextureManager, pSoundManager, m_pLevelObjectManager, m_pEnemyManager, pTimeObjectManager };

	return LoadStage1LevelObjects(pTextureManager, pTimeObjectManager);
}

Level* LevelManager::GetCurrentLevel() const
{
	return m_pCurrentLevel;
}

bool LevelManager::DeleteCurrentLevel()
{
	if (m_pCurrentLevel != nullptr)
	{
		delete m_pCurrentLevel;
		m_pCurrentLevel = nullptr;

		m_pLevelObjectManager->ClearAllLevelObjects();
		m_pEnemyManager->ClearAllEnemies();

		return true;

	}
	return false;
}
