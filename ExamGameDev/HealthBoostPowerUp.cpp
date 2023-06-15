#include "pch.h"
#include "HealthBoostPowerUp.h"
#include "Player.h"

HealthBoostPowerUp::HealthBoostPowerUp(const Point2f& position, const Texture* pTexture):
	BasePowerUp{PowerUps::HealthBoost, Chest::Rarity::common, position, Rectf{1.f ,70.f,34.f,34.f}, pTexture},
	m_HealthBoost{20},
	m_IsActivated{ false }
{

}

void HealthBoostPowerUp::Activate(Player* pPlayer, Level* pLevel)
{
	if (!m_IsActivated)
	{
		pPlayer->IncreaseMaxHealth(m_HealthBoost);
		m_IsActivated = true;
	}
}

void HealthBoostPowerUp::IncreasePowerUpAmount()
{
	m_Amount++;
	m_IsActivated = false;
}
