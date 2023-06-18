#include "pch.h"
#include "EnemySpawner.h"
#include "TextFileDataReader.h"
#include "InputStructs.h"
#include "utils.h"
#include "Parent.h"
#include "Suicider.h"
#include "EnemyManager.h"
#include "TimeObjectManager.h"
#include "StopwatchManager.h"
#include "Stopwatch.h"
#include <iostream>

EnemySpawner::EnemySpawner(TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager, EnemyManager* pEnemyManager) :
	m_pTimeObjectManager{ pTimeObjectManager },
	m_pSoundManager{ pSoundManager },
	m_pTextureManager{ pTextureManager },

	m_pEnemyManager{ pEnemyManager },

	m_pSpawnStopwatchManager{ m_pTimeObjectManager->CreateStopwatchManager()},
	m_pSpawnStopwatch { m_pSpawnStopwatchManager->CreateStopwatch(16.f) },

	m_SpawnableEnemies{},
	m_SpawnableRanges{}
{
}

EnemySpawner::~EnemySpawner()
{
	m_pTimeObjectManager->DeleteStopwatchManager(m_pSpawnStopwatchManager);
}

void EnemySpawner::Update(const Point2f& characterPos)
{
	const size_t mobCap{ m_pEnemyManager->GetCurrentMobCap() };
	const size_t amountOfEnemies{ m_pEnemyManager->GetEnemiesArr().size() };

	if (amountOfEnemies >= mobCap)
	{
		return;
	}


	if (m_pSpawnStopwatch->IsTimeReached())
	{
		HandleSpawnCalc(characterPos, mobCap, amountOfEnemies);
	}
}

bool EnemySpawner::SetSpawnableLocations(size_t stage, const Point2f& playerSpawnPos)
{
	TextFileDataReader<EnemySpawnAreaInput> spawnLocTextFileReader{ ("Level/EnemySpawnInfoStage" + std::to_string(stage) + ".txt") };
	const std::vector<EnemySpawnAreaInput> spawnAreaData{ spawnLocTextFileReader.ReadTextFile() };

	for (size_t dataIndex{0}; dataIndex < spawnAreaData.size(); ++dataIndex)
	{
		m_SpawnableRanges.emplace_back(spawnAreaData[dataIndex].rangePoint0, 
									   spawnAreaData[dataIndex].rangePoint1);

		std::vector<bool> canSpawn{};
		
		for (size_t canSpawnIndex{0}; canSpawnIndex < spawnAreaData[dataIndex].canSpawnEnemy.size(); ++canSpawnIndex)
		{
			canSpawn.emplace_back(bool(spawnAreaData[dataIndex].canSpawnEnemy[canSpawnIndex]));
		}

		m_SpawnableEnemies.emplace_back(canSpawn);
	}

	m_pSpawnStopwatch->Start();

	const size_t mobCap{ m_pEnemyManager->GetCurrentMobCap() };
	const size_t amountOfEnemies{ m_pEnemyManager->GetEnemiesArr().size() };

	//Spawn enemies once at start of game/stage
	HandleSpawnCalc(playerSpawnPos, mobCap, amountOfEnemies);

	return true;
}

bool EnemySpawner::ClearCurrentSpawnAbleLocations()
{
	m_SpawnableRanges.clear();
	m_SpawnableEnemies.clear();
	m_pSpawnStopwatch->Reset();

	return true;
}

void EnemySpawner::HandleSpawnCalc(const Point2f& characterPos, size_t mobCap, size_t amountOfEnemies)
{
	constexpr float MAXDISTANCE{ 5000.f };

	//Calculate the spawnarea for the enemy
	std::vector<float> spawnAreaDistances{};
	spawnAreaDistances.reserve(m_SpawnableRanges.size());

	for (size_t spawnableRangeIdx{ 0 }; spawnableRangeIdx < m_SpawnableRanges.size(); ++spawnableRangeIdx)
	{
		const float distance{ utils::GetDistance(characterPos, Point2f{ m_SpawnableRanges[spawnableRangeIdx].first.x + (m_SpawnableRanges[spawnableRangeIdx].second.x - m_SpawnableRanges[spawnableRangeIdx].first.x) / 2,
																		m_SpawnableRanges[spawnableRangeIdx].first.y }) };
		spawnAreaDistances.emplace_back(distance);
	}

	std::vector<size_t> threeClosestBySpawnAreaIdxs{};
	while (threeClosestBySpawnAreaIdxs.size() < 3)
	{
		const size_t closestBySpawnAreaIdx = std::distance(spawnAreaDistances.begin(),
			std::min_element(spawnAreaDistances.begin(), spawnAreaDistances.end()));

		threeClosestBySpawnAreaIdxs.emplace_back(closestBySpawnAreaIdx);

		spawnAreaDistances[closestBySpawnAreaIdx] = MAXDISTANCE;
	}

	//Spawn between 1-4 enemies at once
	const size_t enemiesToSpawn{ size_t(utils::GetRandomNumber(1, std::min(int(mobCap - amountOfEnemies), 4))) };

	for (size_t spawnedEnemies{}; spawnedEnemies < enemiesToSpawn; ++spawnedEnemies)
	{
		const size_t spawnAreaIdx{ threeClosestBySpawnAreaIdxs[utils::GetRandomNumber(0, threeClosestBySpawnAreaIdxs.size() - 1)] };

		//Which enemies can spawn in this area
		std::vector<size_t> spawnAbleEnemyIndexes{};
		for (size_t index{ 0 }; index < m_SpawnableEnemies[spawnAreaIdx].size(); ++index)
		{
			if (m_SpawnableEnemies[spawnAreaIdx][index] == true)
			{
				spawnAbleEnemyIndexes.emplace_back(index);
			}
		}

		const size_t indexEnemyToSpawn{ spawnAbleEnemyIndexes[utils::GetRandomNumber(0, spawnAbleEnemyIndexes.size() - 1)] };

		//Calculate a random position in the area
		const float randnxPos = utils::GetRandomNumber(int(m_SpawnableRanges[spawnAreaIdx].first.x), int(m_SpawnableRanges[spawnAreaIdx].second.x));

		const Point2f spawnPosition{ randnxPos, m_SpawnableRanges[spawnAreaIdx].first.y };

		SpawnEnemy(Enemies(indexEnemyToSpawn), spawnPosition);
	}
}

bool EnemySpawner::SpawnEnemy(Enemies enemyType, const Point2f& position)
{
	switch (enemyType)
	{
	case EnemySpawner::Enemies::parent:
		return SpawnParent(position);
	case EnemySpawner::Enemies::suicider:
		return SpawnSuicider(position);
	}

	return false;
}

bool EnemySpawner::SpawnParent(const Point2f& position)
{
	//Parent stats
	const float movementSpeed{ 25.f };
	const float jumpSpeed{ 100.f };
	constexpr size_t health{ 100 };
	constexpr size_t attackDamage{ 25 };

	BaseEnemy* pEnemy = new Parent{ position, movementSpeed, jumpSpeed, health, attackDamage, m_pTimeObjectManager, m_pSoundManager, m_pTextureManager };
	m_pEnemyManager->AddEnemy(pEnemy);
	return true;
}

bool EnemySpawner::SpawnSuicider(const Point2f& position)
{
	//Suicider stats
	const float movementSpeed{ 80.f };
	constexpr size_t health{ 50 };
	constexpr size_t attackDamage{ 25 };

	BaseEnemy* pEnemy = new Suicider{ position, movementSpeed, health, attackDamage, m_pTimeObjectManager, m_pSoundManager, m_pTextureManager };
	m_pEnemyManager->AddEnemy(pEnemy);
	return true;
}