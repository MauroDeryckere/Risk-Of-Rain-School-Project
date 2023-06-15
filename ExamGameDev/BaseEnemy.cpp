#include "pch.h"
#include "BaseEnemy.h"

#include "TimeObjectManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "StopwatchManager.h"

#include "Level.h"
#include "Player.h"

BaseEnemy::BaseEnemy(const Rectf& shape, float movementSpeed, float jumpSpeed, unsigned int MaxHealth, unsigned int attackDamage, 
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

	m_AttackDamage{attackDamage}
{

}

BaseEnemy::~BaseEnemy()
{
	m_pTimeObjectManager->DeleteStopwatchManager(m_pEnemyStopwatchManager);
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

void BaseEnemy::TakeDamage(unsigned int attackDamage)
{
	if (m_CurrentHealth == 0)
	{
		return;
	}

	m_CurrentHealth = std::max(int(m_CurrentHealth - attackDamage), 0);
}

const Rectf& BaseEnemy::GetShape() const
{ 
	return m_Shape;
}