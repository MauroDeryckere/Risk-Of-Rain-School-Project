#include "pch.h"
#include "utils.h"
#include "EnemyManager.h"
#include "TimeObjectManager.h"
#include "BaseEnemy.h"

#include "Parent.h"
#include "Suicider.h"
#include "Lemurian.h"
#include <iostream>

EnemyManager::~EnemyManager()
{
	ClearAllEnemies();
}

void EnemyManager::Update(float elapsedSec, Level* pLevel, Player* pPlayer)
{
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

bool EnemyManager::SpawnParent(const Point2f& position, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager )
{
	//Parent stats
	const float movementSpeed{25.f};
	const float jumpSpeed{100.f};
	constexpr unsigned int health{ 100 };
	constexpr unsigned int attackDamage{ 25 };

	BaseEnemy* pEnemy = new Parent{position, movementSpeed, jumpSpeed, health, attackDamage, pTimeObjectManager, pSoundManager, pTextureManager };
	AddEnemy(pEnemy);
	return true;
}

bool EnemyManager::SpawnSuicider(const Point2f& position, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager)
{
	//Suicider stats
	const float movementSpeed{ 100.f };
	constexpr unsigned int health{ 50 };
	constexpr unsigned int attackDamage{ 25 };

	BaseEnemy* pEnemy = new Suicider{ position, movementSpeed, health, attackDamage, pTimeObjectManager, pSoundManager, pTextureManager };
	AddEnemy(pEnemy);
	return true;
}

bool EnemyManager::SpawnLemurian(const Point2f& position, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager)
{
	////Lemurian stats
	//const float movementSpeed{ 25.f };
	//const float jumpSpeed{ 100.f };
	//constexpr unsigned int health{ 100 };
	//constexpr unsigned int attackDamage{ 25 };

	//BaseEnemy* pEnemy = new Lemurian{ position, movementSpeed, jumpSpeed, health, attackDamage, pTimeObjectManager, pSoundManager, pTextureManager };
	//AddEnemy(pEnemy);
	//return true;
	std::cerr << "code doesnt exist yet for this enemy";
	return false;
}

bool EnemyManager::SetSpawnAreas(const std::string& filePath)
{
	return true;
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

const std::vector<BaseEnemy*>& EnemyManager::GetEnemiesArr() const
{
	return m_pEnemies;
}

BaseEnemy* EnemyManager::GetClosestByEnemyPtr(const Rectf& shape) const
{
	const Point2f center{ shape.left + shape.width / 2, shape.bottom + shape.height / 2 };
	BaseEnemy* closestEnemy{ nullptr };

	constexpr float MAXDISTANCE{5000.f};
	float distance{ MAXDISTANCE };

	for (size_t index{0}; index < m_pEnemies.size(); index++)
	{
		if (m_pEnemies[index]->IsAlive())
		{
			const Rectf& enemyShape{ m_pEnemies[index]->GetShape() };
			const Point2f enemyCenter{ enemyShape.left + enemyShape.width / 2, enemyShape.bottom + enemyShape.height / 2 };

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

void EnemyManager::AddEnemy(BaseEnemy* pEnemy)
{
	m_pEnemies.emplace_back(pEnemy);
}