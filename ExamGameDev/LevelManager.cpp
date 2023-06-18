#include "pch.h"

#include "LevelManager.h"
#include "LevelObjectManager.h"
#include "EnemyManager.h"
#include "EnemySpawner.h"
#include "PowerUpManager.h"
#include "TextFileDataReader.h"
#include "InputStructs.h"

#include "Level.h"
#include "Player.h"
#include "Rope.h"
#include "LaunchPad.h"
#include "Chest.h"
#include "Teleporter.h"
#include "Game.h"

#include <iostream>

LevelManager::LevelManager(Game* pGame, PowerUpManager* pPowerUpManager, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager):
	m_pGame{pGame},
	m_pPowerUpManager{ pPowerUpManager },

	m_pLevelObjectManager{ new LevelObjectManager{ } },
	m_pEnemyManager{ new EnemyManager{pTimeObjectManager, pSoundManager, pTextureManager } },

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
	m_pPowerUpManager->Update(pPlayer, pInput);
}

void LevelManager::Draw(const Point2f& cameraPos) const
{
	m_pCurrentLevel->Draw(cameraPos);
	m_pLevelObjectManager->Draw();
	m_pEnemyManager->Draw();
	m_pPowerUpManager->Draw();
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

		size_t rarity{ chestData[index].rarity };

		if (rarity == 0)
		{
			constexpr size_t min{ 1 };
			constexpr size_t max{ 3 };

			rarity = utils::GetRandomNumber(min, max);
		}

		m_pLevelObjectManager->CreateLevelObject(new Chest{ chestData[index].bottomLeft,
												 Chest::Rarity(rarity),
												 m_pPowerUpManager,
												 m_pCurrentLevel,
												 pTimeObjectManager,
												 pTextureManager });
	}

	//Teleporter
	TextFileDataReader<TeleporterInput> teleporterTextFileReader{ "Level/TeleporterInfo.txt" };
	const std::vector<TeleporterInput> teleporterData{ teleporterTextFileReader.ReadTextFile() };

	for (size_t index{ 0 }; index < teleporterData.size(); ++index)
	{
		m_pLevelObjectManager->CreateLevelObject(new Teleporter{ teleporterData[index].shape, this, pTextureManager });
	}
	return true;
}

bool LevelManager::InitializeStage1(TextureManager* pTextureManager, SoundManager* pSoundManager, TimeObjectManager* pTimeObjectManager)
{
	if (m_pCurrentLevel != nullptr)
	{
		std::cerr << "previous level wasn't deleted \n";
		return false;
	}
	const Point2f playerSpawnPos{ 1272.f ,1877.f };

	m_pCurrentLevel = new Level {playerSpawnPos, m_pPowerUpManager, pTextureManager, pSoundManager, m_pLevelObjectManager, m_pEnemyManager, pTimeObjectManager };
	m_pEnemyManager->InitSpawnLocations(1, playerSpawnPos);

	return LoadStage1LevelObjects(pTextureManager, pTimeObjectManager);
}

Level* LevelManager::GetCurrentLevel() const
{
	return m_pCurrentLevel;
}

const Rectf& LevelManager::GetLevelBoundaries() const
{
	return m_pCurrentLevel->GetBoundaries();
}

const Point2f& LevelManager::GetSpawnPosition() const
{
	return m_pCurrentLevel->GetSpawnPosition();
}

bool LevelManager::DeleteCurrentLevel()
{
	if (m_pCurrentLevel != nullptr)
	{
		delete m_pCurrentLevel;
		m_pCurrentLevel = nullptr;

		m_pLevelObjectManager->ClearAllLevelObjects();
		m_pEnemyManager->ClearAllEnemies();
		m_pEnemyManager->ClearSpawnLocations();
		m_pPowerUpManager->ClearAllPowerUps();

		return true;

	}
	return false;
}

bool LevelManager::StartNextStage() const
{
	//Only one level right now so end game
	m_pGame->EndGame(true);
	return true;
}
