#include "pch.h"
#include "utils.h"
#include "EnemyManager.h"
#include "EnemySpawner.h"
#include "TimeObjectManager.h"
#include "BaseEnemy.h"
#include "Player.h"


#include <iostream>

EnemyManager::EnemyManager(TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager):
	m_pEnemies{},
	m_pEnemySpawner{ new EnemySpawner{pTimeObjectManager, pSoundManager, pTextureManager, this} },

	m_MobCap{ 25 }
{
}

EnemyManager::~EnemyManager()
{
	ClearAllEnemies();
	delete m_pEnemySpawner;
}

void EnemyManager::Update(float elapsedSec, Level* pLevel, Player* pPlayer)
{
	m_pEnemySpawner->Update(Point2f{pPlayer->GetShape().left + pPlayer->GetShape().width/2, pPlayer->GetShape().bottom});

	for (size_t index{ 0 }; index < m_pEnemies.size(); ++index)
	{
		m_pEnemies[index]->Update(elapsedSec, pLevel, pPlayer);
	}
}

void EnemyManager::Draw() const
{
	for (size_t index{ 0 }; index < m_pEnemies.size(); ++index)
	{
		m_pEnemies[index]->Draw();
	}
}

bool EnemyManager::DeleteEnemy(BaseEnemy* pEnemy)
{
	for (size_t index{ 0 }; index < m_pEnemies.size(); ++index)
	{
		if (m_pEnemies[index] == pEnemy)
		{
			delete m_pEnemies[index];

			m_pEnemies[index] = m_pEnemies.back();
			m_pEnemies.pop_back();

			return true;
		}
	}

	return false;
}

void EnemyManager::ClearAllEnemies()
{
	for (size_t index{ 0 }; index < m_pEnemies.size(); ++index)
	{
		delete m_pEnemies[index];
	}

	m_pEnemies.clear();
}

void EnemyManager::InitSpawnLocations(size_t stage, const Point2f& playerSpawnpos)
{
	m_pEnemySpawner->SetSpawnableLocations(stage, playerSpawnpos);
}

void EnemyManager::ClearSpawnLocations()
{
	m_pEnemySpawner->ClearCurrentSpawnAbleLocations();
}

const std::vector<BaseEnemy*>& EnemyManager::GetEnemiesArr() const
{
	return m_pEnemies;
}

BaseEnemy* EnemyManager::GetClosestByEnemyPtr(const Rectf& shape) const
{
	const Point2f center{ shape.left + shape.width / 2, shape.bottom + shape.height / 2 };
	BaseEnemy* closestEnemy{ nullptr };

	constexpr float MAXDISTANCE {5000.f};
	float distance{ MAXDISTANCE };

	for (size_t index{0}; index < m_pEnemies.size(); index++)
	{
		if (m_pEnemies[index]->IsAlive())
		{
			const Rectf& enemyShape{ m_pEnemies[index]->GetShape() };

			const Point2f enemyCenter{ enemyShape.left + enemyShape.width / 2, 
									  enemyShape.bottom + enemyShape.height / 2 };

			const float newDistance{ utils::GetDistance(center, enemyCenter) };

			if (newDistance < distance)
			{
				distance = newDistance;
				closestEnemy = m_pEnemies[index];
			}
		}
	}

	if (closestEnemy == nullptr)
	{
		//std::cout << "No Enemies alive to return \n";
	}

	return closestEnemy;
}

void EnemyManager::SetMobCap(size_t newMobCap)
{
	m_MobCap = newMobCap;
}

size_t EnemyManager::GetCurrentMobCap() const
{
	return m_MobCap;
}

void EnemyManager::AddEnemy(BaseEnemy* pEnemy)
{
	if (pEnemy)
	{
		m_pEnemies.emplace_back(pEnemy);
	}
	else
	{
		std::cerr << "can't add enemy, not a valid ptr" << std::endl;
	}
}