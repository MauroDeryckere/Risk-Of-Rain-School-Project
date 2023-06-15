#include "pch.h"
#include "MovementSpeedBoostPowerUp.h"
#include "Player.h"
MovementSpeedBoostPowerUp::MovementSpeedBoostPowerUp(const Point2f& position, const Texture* pTexture):
	BasePowerUp{PowerUps::MovementSpeed, Chest::Rarity::common ,position,Rectf{36.f,70.f,34.f,34.f}, pTexture},
	m_MovementSpeedBoost{10.f},
	m_IsActivated{ false }
{
}

void MovementSpeedBoostPowerUp::Activate(Player* pPlayer, Level* pLevel)
{
	if (!m_IsActivated)
	{
		pPlayer->IncreaseMovementSpeed(m_MovementSpeedBoost);
		m_IsActivated = true;
	}
}

void MovementSpeedBoostPowerUp::IncreasePowerUpAmount()
{
	m_Amount++;
	m_IsActivated = false;
}
