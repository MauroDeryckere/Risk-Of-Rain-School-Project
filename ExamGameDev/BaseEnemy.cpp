#include "pch.h"
#include "BaseEnemy.h"

#include "TimeObjectManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "StopwatchManager.h"

#include "Level.h"
#include "Player.h"
#include "Stopwatch.h"
#include <iostream>


BaseEnemy::BaseEnemy(const Rectf& shape, float movementSpeed, float jumpSpeed, size_t MaxHealth, size_t attackDamage,
	TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager) :

	m_pTimeObjectManager{ pTimeObjectManager },
	m_pSoundManager{ pSoundManager },
	m_pTextureManager{ pTextureManager },

	m_pEnemyStopwatchManager{ m_pTimeObjectManager->CreateStopwatchManager() },

	m_Shape{ shape },

	m_IsFacingLeft{ false },
	m_HasAttacked{ false },

	m_MovementSpeed{ movementSpeed },
	m_JumpSpeed{ jumpSpeed },
	m_Velocity{ 0.f, 0.f },
	m_Acceleration{ 0.f, -981.f },

	m_MaxHealth{ MaxHealth },
	m_CurrentHealth{ MaxHealth },

	m_AttackDamage{ attackDamage },

	m_DroppedMoney{0},
	m_pMoneyAniStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(3.f)}
{

}

BaseEnemy::~BaseEnemy()
{
	m_pTimeObjectManager->DeleteStopwatchManager(m_pEnemyStopwatchManager);
}

void BaseEnemy::Update(float elapsedSec, Level* pLevel, Player* pPlayer)
{
	if (m_pMoneyAniStopwatch->IsTimeReached())
	{
		m_DroppedMoney = 0;
	}
}

bool BaseEnemy::IsAlive() const
{
	if (m_CurrentHealth == 0)
	{
		return false;
	}
	return true;
}

void BaseEnemy::DrawHealthBar() const 
{
	if (m_CurrentHealth == 0 || m_CurrentHealth == m_MaxHealth)
	{
		return;
	}

	constexpr float healthBarWidth{ 48.f };
	constexpr float healthbarHeight{ 10.f };

	constexpr float offset{ 1.f };

	const float leftHealthBarPos{ m_Shape.left + m_Shape.width / 2 - healthBarWidth / 2 };

	//background layer
	utils::SetColor(Color4f{ 0.f,0.f,0.f,.5f });
	utils::FillRect(Point2f{ leftHealthBarPos, m_Shape.bottom + m_Shape.height }, 
							 healthBarWidth, healthbarHeight);

	utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
	utils::FillRect(Point2f{ leftHealthBarPos + offset, m_Shape.bottom + m_Shape.height + offset }, 
							 (healthBarWidth - 2 * offset) * m_CurrentHealth / m_MaxHealth, healthbarHeight - 2 * offset);
}

void BaseEnemy::DrawDroppedMoney() const
{
	if (m_DroppedMoney != 0)
	{
		constexpr float moneyAniDistance{ 12.f };
		const Point2f bottomLeft{m_Shape.left + m_Shape.width/2,
								 m_Shape.bottom + m_Shape.height/2 + (m_pMoneyAniStopwatch->GetCurrSec() * moneyAniDistance)};

		float numWidth{};

		m_pTextureManager->DrawNumber(TextureManager::NumberTextures::_20PxGreenDigits, bottomLeft, m_DroppedMoney, 0.f, numWidth);
		m_pTextureManager->DrawSymbol(TextureManager::NumberTextures::_20PxGreenDigits, Point2f{bottomLeft.x + numWidth, bottomLeft.y}, TextureManager::Symbols::Dollar, 0.f);
	}
}

void BaseEnemy::TakeDamage(Player* pPlayer, size_t attackDamage)
{
	m_CurrentHealth = std::max(int(m_CurrentHealth) - int(attackDamage), 0);

	if (m_CurrentHealth == 0)
	{
		if (pPlayer)
		{
			m_DroppedMoney = utils::GetRandomNumber(22, 50);
			pPlayer->UpdateBalance(m_DroppedMoney);
			m_pMoneyAniStopwatch->Start();
		}
	}
}

const Rectf& BaseEnemy::GetShape() const
{ 
	return m_Shape;
}